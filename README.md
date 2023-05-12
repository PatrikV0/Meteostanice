
# Meteostanice
Meteostanice využívá openweathermap API z kterého pomocí požadavku GET získá JSON informace o lokaci, teplotě, vlhkosti atd... Data jsou následně pomocí knihovny ArduinoJson uloženy do proměnných. Současně se zapisujou informace o vlhkosti a teploty místnosti z DHT11 senzoru. Hodnoty proměnných jsou následně zapsány na Blynk a HTTP server. 


## Zapojení
![image](https://github.com/PatrikV0/Meteostanice/assets/94992801/d93beb6f-f9e1-4934-98e5-51064e9291ba)

## Schéma Zapojení
![schemaZapojeni](https://github.com/PatrikV0/Meteostanice/assets/94992801/f5c120cb-6582-444a-a280-2fbc184ac92f)

## Sériový monitor
ESP32 se připojí na WIFI, vypíše adresu HTTP serveru a připojí se na Blynk. Následně se do sériového monitoru začnou vypisovat data ze senzoru a API.
![seriovyMonitor](https://github.com/PatrikV0/Meteostanice/assets/94992801/ce3c26b3-cf32-40bb-b6a2-ba593af1732a)

## Webové rozhraní Blynk
![webBlynk](https://github.com/PatrikV0/Meteostanice/assets/94992801/e62e266e-8b65-4585-a273-02fc97b82d06)
## Mobilní rozhraní Blynk
![mobileBlynk](https://github.com/PatrikV0/Meteostanice/assets/94992801/cfa81fe2-3e29-4c0b-bdd4-e05a5daf25e5)

## HTTP server
![httpServer](https://github.com/PatrikV0/Meteostanice/assets/94992801/f23e8d97-29c9-49da-abf9-1e1134d95471)
## Návod na openweathermap API
Nejdříve je nutné zaregistrovat se na https://openweathermap.org/. Po registraci dostaneme přístup k API klíči na https://home.openweathermap.org/api_keys. 
![image](https://github.com/PatrikV0/Meteostanice/assets/94992801/c336af02-dfeb-4a95-a84e-938e763e2c85)

API klíč zkopírujeme a vložíme do proměnné apiKey v kódu.
Do proměnné cityID vložíme ID města u kterého chceme znát počasí.
![image](https://github.com/PatrikV0/Meteostanice/assets/94992801/8bc4b340-0aea-4b85-8045-bbd854b18462)
### CityID
ID města zjistíme z url adresy 
![image](https://github.com/PatrikV0/Meteostanice/assets/94992801/b3727f49-fb10-4fd2-9b50-470c536a4d1a)






