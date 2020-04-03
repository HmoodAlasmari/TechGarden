  /* 
      This is a Code for the ESP (NODEMUC) for the TechGarden Project for FCIT CPIT-499
                            Hmood Aiyd Alasmari - 1639865
                            Waleed Salem BinSaad - 1648154
*/
  
  #include "DHT.h"
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266mDNS.h>
  #include <SPI.h>
  #include <MFRC522.h>
  
  #define DHTPIN D6
  int waterLevelLed_0 = D0;
  int waterLevelLed_1 = D1;
  int waterLevelLed_2 = D2;
  const int waterPin_B = D3;
  const int waterPin_T = D4;
  int soilPin = A0; 
  int waterPin = D5; 
  
  #define DHTTYPE DHT11
  
  DHT dht(DHTPIN, DHTTYPE);
  
  int water_LB;
  int water_LT;
  float humidityData;
  float temperatureData;
  double soilValue;
  double soilMoistPersent;
  
  const char* ssid = "HmoodAlasmari";//
  const char* password = "25592559";
  //WiFiClient client;
  char server[] = "192.168.8.154";
  
  WiFiClient client;
  
  
  void setup()
  {
    Serial.begin(115200);
    delay(10);
    dht.begin();
    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
  
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
  
    // Start the server
    //  server.begin();
    Serial.println("Server started");
    Serial.print(WiFi.localIP());
    delay(1000);
    Serial.println("connecting...");
  
  
    // ####################### WATER #######################
    // LED Level
    pinMode(waterLevelLed_0, OUTPUT);
    pinMode(waterLevelLed_1, OUTPUT);
    pinMode(waterLevelLed_2, OUTPUT);
    pinMode(waterPin, OUTPUT);
  
  }
  
  

  
  void loop()
  {
    // ####################### DHT11 #######################
    humidityData = dht.readHumidity();
    temperatureData = dht.readTemperature();
    Sending_To_phpmyadmindatabase();
    delay(1500); // interval
    // ####################### DHT11 #######################
  
    //-----------------------------------------------------------
  
    // ####################### WATER #######################
    water_LB = digitalRead(waterPin_B);
    water_LT = digitalRead(waterPin_T);
  
    if (water_LB == 0 && water_LT == 0) {
      Serial.println("Water Tank is Empty");
      digitalWrite(waterLevelLed_0, HIGH);
      digitalWrite(waterLevelLed_1, LOW);
      digitalWrite(waterLevelLed_2, LOW);
  
    } else if (water_LB == 1 && water_LT == 0) {
      Serial.println("Water Tank is Half");
      digitalWrite(waterLevelLed_0, HIGH);
      digitalWrite(waterLevelLed_1, HIGH);
      digitalWrite(waterLevelLed_2, LOW);
  
    } else if (water_LB == 1 && water_LT == 1) {
      Serial.println("Water Tank is Full");
      digitalWrite(waterLevelLed_0, HIGH);
      digitalWrite(waterLevelLed_1, HIGH);
      digitalWrite(waterLevelLed_2, HIGH);
    }
    // ####################### WATER #######################
  
   //-----------------------------------------------------------
  
    // ####################### SOIL #######################
    soilValue = analogRead(soilPin);
    // Serial.println("Analog Value : ");
    Serial.println("---------------------");
    Serial.println(soilValue);
    double soildryPersent = ((soilValue / 1023) * 100);
    soilMoistPersent = 100 - soildryPersent;
    Serial.print(soilMoistPersent);
    Serial.println("%");
    Serial.println("");
    // ####################### SOIL #######################
  
   //-----------------------------------------------------------
  
    // ####################### Water Pump #######################
    if (soilMoistPersent<=40){
        digitalWrite(waterPin, HIGH);
      delay(2000);
      digitalWrite(waterPin, LOW);
          delay(2000);
      }
    // ####################### Water Pump #######################
  }
  
  
  
    // ####################### Send To SQL (DTH11 Only for now) #######################
  void Sending_To_phpmyadmindatabase()   //CONNECTING WITH MYSQL
  {
    if (client.connect(server, 80)) {
      Serial.println("connected");
      // Make a HTTP request:
      Serial.print("GET /esp.php?humidity=");
      client.print("GET /esp.php?humidity=");
      Serial.println(humidityData);
      client.print(humidityData);
      client.print("&temperature=");
      Serial.println("&temperature=");
      client.print(temperatureData);
      Serial.println(temperatureData);
      client.print(" ");    
      client.print("HTTP/1.1");
      client.println();
      client.println("Host: 192.168.8.154");
      client.println("Connection: close");
      client.println();
    } else {
      // Didn't get a connection to the server:
      Serial.println("connection failed");
    }
  }
