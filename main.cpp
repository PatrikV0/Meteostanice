#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <WebServer.h>


const char *apiKey = ""; // Proměnná do které se uloží API klíč, který nalezneme po registraci na stránce https://home.openweathermap.org/
String CityID = "3065607"; // ID města které nalezneme na https://home.openweathermap.org/
const char *ssid = "";
const char *password = "";
const char *auth = BLYNK_AUTH_TOKEN;

float t, h;
String CityID = "3065607"; // ID města které nalezneme na https://home.openweathermap.org/
String response = "";

float temperature;
float humidity;
float pressure;
float windSpeed;
float windDegree;
String country;
String location;

#define DHTPIN 33
DHT dht(DHTPIN, DHT11);
WebServer server(80);

void loop()
{
  // Připojení na Openweathermap server
  WiFiClientSecure client;
  client.setInsecure();
  if (!client.connect("api.openweathermap.org", 443))
  {
    Serial.println("Connection failed");
    return;
  }

  // Poslání GET požadavku,
  client.println("GET /data/2.5/weather?id=" + CityID + "&units=metric&APPID=" + apiKey);
  client.println("Host: api.openweathermap.org");
  client.println("User-Agent: ArduinoWiFi/1.1");
  client.println("Connection: close");
  client.println();

  // Vyčkání na odpověď serveru
  while (!client.available())
  {
    delay(10);
  }

  // Přečtení odpovědi serveru

  while (client.available())
  {
    response += char(client.read());
  }

  // Odpojení od serveru
  client.stop();

  t = dht.readTemperature();   //čtení hodnot ze senzoru
  h = dht.readHumidity();

  // Ukládání hodnot z JSON souboru
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, response);
  JsonObject main = doc["main"];
  float temperature = main["temp"];
  float humidity = main["humidity"];
  float pressure = main["pressure"];
  float windSpeed = doc["wind"]["speed"];
  float windDegree = doc["wind"]["deg"];
  String country = doc["sys"]["country"];
  String location = doc["name"];

  // Vypsání informací
  Serial.print("Country: ");
  Serial.print(country);
  Serial.print(", Location: ");
  Serial.print(location);
  Serial.print(", Outside Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C,Outside Humidity: ");
  Serial.print(humidity);
  Serial.print(" %,Inside Temperature: ");
  Serial.print(t);
  Serial.print(" °C,Inside Humidity: ");
  Serial.print(h);
  Serial.print("%, Wind speed: ");
  Serial.print(windSpeed);
  Serial.print("m/s, Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  delay(1000);
  server.handleClient();
  Blynk.run();
}

void error()
{
  server.send(404, "text/plain", "Not found");   //Funkce error se vykoná při chybě http serveru
}
//Kompletní HTML soubor
String SendHTML(float Temperaturestat, float Humiditystat, String locationStat, String countryStat, float pressureStat, float windSpeedStat, float outsideTempStat, float outsideHumidityStat)
{

  String web = "<!DOCTYPE html> <html>\n";
  web += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  web += "<title>DHT11 Senzor</title>\n";
  web += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  web += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  web += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  web += "</style>\n";
  web += "</head>\n";
  web += "<body>\n";
  web += "<div id=\"webpage\">\n";

  web += "<h1>Country: ";
  web += (String)countryStat;
  web += "</h1>";

  web += "<h2>Location: ";
  web += (String)locationStat;
  web += "</h2>";

  web += "<p>Pressure: ";
  web += (float)pressureStat;
  web += "hPa</p>";

  web += "<p>Wind speed: ";
  web += (float)windSpeedStat;
  web += "m/s</p>";

  web += "<p>Outside temperature: ";
  web += (float)outsideTempStat;
  web += "C</p>";

  web += "<p>Outside humidity: ";
  web += (float)outsideHumidityStat;
  web += "%</p>";

  web += "<p>Room temperature: ";
  web += (float)Temperaturestat;
  web += "C</p>";

  web += "<p>Room humidity: ";
  web += (int)Humiditystat;
  web += "%</p>";

  web += "</div>\n";
  web += "</body>\n";
  web += "</html>\n";
  return web;
}
//funkce která pošle HTML soubor + informace ze senzoru a API na HTTP server a Blynk
void pripojeniUzivatele()
{
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, response);
  JsonObject main = doc["main"];
  float temperature = main["temp"];
  float humidity = main["humidity"];
  float pressure = main["pressure"];
  float windSpeed = doc["wind"]["speed"];
  float windDegree = doc["wind"]["deg"];
  String country = doc["sys"]["country"];
  String location = doc["name"];
  t = dht.readTemperature();
  h = dht.readHumidity();
  float insideTemp, insideHumid;
  insideTemp = t;
  insideHumid = h;

  Blynk.virtualWrite(V0, location);
  Blynk.virtualWrite(V1, country);
  Blynk.virtualWrite(V2, insideTemp);
  Blynk.virtualWrite(V3, insideHumid);
  Blynk.virtualWrite(V4, pressure);
  Blynk.virtualWrite(V5, windSpeed);
  Blynk.virtualWrite(V6, temperature);
  Blynk.virtualWrite(V7, humidity);
  server.send(200, "text/html", SendHTML(t, h, location, country, pressure, windSpeed, temperature, humidity));
}

void setup()
{
  Serial.begin(9600);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");          //Pokud se ESP32 snaží připojit k WIFI tak každou sekundu vypíše "Connecting to Wi-Fi"
  }
  

  Serial.println("Connected to Wi-Fi");              //Po připojení na Wifi se vypíše "Connected to Wi-Fi" a IP adresa HTTP serveru
  Serial.println(WiFi.localIP());
  server.on("/", pripojeniUzivatele);
  server.onNotFound(error);
  server.begin();

  Serial.println("HTTP server zapnut");
  // Connect to Blynk
  Blynk.begin(auth, ssid, password);
  Serial.println("Connected to Blynk");
}
