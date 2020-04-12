  /*
   * 
   *       King Abdulaziz University - Faculty of Computing and Information Technology
   *     This is a Code for the ESP (NODEMUC) for the TechGarden Project for FCIT CPIT-499
   *     
   *
   *                                   TechGarden 1.0v
   *
   *                                          __/)
   *                                        .-(__(=:
   *                                        |    \)
   *                                  (\__  |
   *                                 :=)__)-|  __/)
   *                                  (/    |-(__(=:
   *                                ______  |  _ \)
   *                               /      \ | / \
   *                                    ___\|/___\
   *                                   [         ]\
   *                                    \       /  \
   *                                     \     /
   *                                      \___/
   *
   *                      #######################################
   *                      ############ Supervised By ############
   *                      #********-#(Dr. Ahmed Sabban)#********#
   *                      #######################################
   *                       ------------------------------------
   *                       |   Hmood Aiyd Alasmari - 1639865   |
   *                       |   Waleed Salem BinSaad - 1648154  |
   *                       ------------------------------------
   *
  */
  
  // Libraries
  #include "DHT.h"
  #include <ESP8266WiFi.h>
  
  // ESP GPIO Pins
  const int waterPin_B = D3;
  const int waterPin_T = D4;
  #define DHTPIN D6
  int waterLevelLed_0 = D0;
  int waterLevelLed_1 = D1;
  int waterLevelLed_2 = D2;
  int soilPin = A0;
  int waterPin = D5;
  #define DHTTYPE DHT11
  DHT dht(DHTPIN, DHTTYPE);
  
  // Variables
  const int GARDEN_ID = 1;
  String sendAllData;
  float humidityData;
  float temperatureData;
  int waterLevel;
  int water_LB;
  int water_LT;
  double soilValue;
  int waterPumpOn;
  double soilMoistPersent;
  int minMoist;
  int maxMoist;
  
  // Wifi Connection
  const char* ssid = "HmoodAlasmari";//
  const char* password = "25592559";
  
  // Server IP (HERE IT IS LOCAL AND CONECTED TO RASPBERRY PI)
  char server[] = "192.168.8.154";
  
  // Creating client object of WiFiClient
  WiFiClient client;
  
  
  void setup()
  {
    // Set all Variables to 0s 
    humidityData = 0.0;
    temperatureData = 0.0;
    waterLevel = 0;
    water_LB = 0;
    water_LT = 0;
    soilValue = 0.0;
    waterPumpOn = 0;
    soilMoistPersent = 0;
    minMoist = 0;
    maxMoist = 0;
  
    // Start Serial Monitering for debuging 
    Serial.begin(115200);
    delay(10);
    dht.begin();
    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
  
    // Waiting for the ESP to connect 
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
  
    // Starting
    Serial.println("Server started");
    Serial.print(WiFi.localIP());
    delay(1000);
    Serial.println();
    Serial.println("connecting...");
  
  
    // Set water's level LEDs
    pinMode(waterLevelLed_0, OUTPUT);
    pinMode(waterLevelLed_1, OUTPUT);
    pinMode(waterLevelLed_2, OUTPUT);
    pinMode(waterPin, OUTPUT);
    // END OF SETUP
  }


  
  void loop()
  {
    // ####################### DHT11 #######################
      humidityData = dht.readHumidity();
      temperatureData = dht.readTemperature();
      delay(1500);
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  
    // ####################### WATER #######################
      water_LB = digitalRead(waterPin_B);
      water_LT = digitalRead(waterPin_T);
      Serial.print("water_LB:  ");
      Serial.println(water_LB);
      Serial.print("water_LT:  ");
      Serial.println(water_LT);
      
      if (water_LB == 0 && water_LT == 0) {
        Serial.println("Water Tank is Empty");
        digitalWrite(waterLevelLed_0, HIGH);
        digitalWrite(waterLevelLed_1, LOW);
        digitalWrite(waterLevelLed_2, LOW);
        waterLevel = 0;
    
      } else if (water_LB == 1 && water_LT == 0) {
        Serial.println("Water Tank is Half");
        digitalWrite(waterLevelLed_0, HIGH);
        digitalWrite(waterLevelLed_1, HIGH);
        digitalWrite(waterLevelLed_2, LOW);
        waterLevel = 1;
    
      } else if (water_LB == 1 && water_LT == 1) {
        Serial.println("Water Tank is Full");
        digitalWrite(waterLevelLed_0, HIGH);
        digitalWrite(waterLevelLed_1, HIGH);
        digitalWrite(waterLevelLed_2, HIGH);
        waterLevel = 2;
      }
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  
    // ####################### SOIL #######################
      soilValue = analogRead(soilPin);
      double soildryPersent = ((soilValue / 950) * 100);
      soilMoistPersent = 100 - soildryPersent;
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
    //-----------------------------------------------------------
  
    // ####################### Water Pump #######################
    // MIZ + Min Get from DATABASE
      if (soilMoistPersent <= 40) {
        digitalWrite(waterPin, HIGH);
        delay(2000);
        digitalWrite(waterPin, LOW);
        delay(2000);
      }
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  

    // ####################### POST #######################
      sendAllData = "garden_id=" + String(GARDEN_ID) + "&" +
                    "humidity=" + String(humidityData) + "&" +
                    "temperature=" + String(temperatureData) + "&" +
                    "water_level=" + String(waterLevel) + "&" +
                    "water_pump=" + String(waterPumpOn) + "&" +
                    "soil_moist=" + String(soilMoistPersent + 25);
                    post();
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  
    // Send every 1 min
      delay(60000);
    // END OF LOOP
  }
  
  
   // HTTP POST
  void post()
  {
    if (client.connect(server, 80)) {
      Serial.println("");
      Serial.println("ooooooooooo)");
      Serial.println("POSTING...");
      Serial.println("ooooooooooo)");
      Serial.println("");
      client.println("POST /espgarden.php? HTTP/1.1");
      client.print("Host: ");
      client.println(server);
      client.println("User-Agent: ESP/1.0");
      client.println("Connection: close");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(sendAllData.length());
      client.println();
      client.println(sendAllData);
  
      // Monitering
      Serial.println("* * * * * * * * * * * * * * * * * * * * *");
      Serial.println("-------------------Data-----------------)");
      Serial.print("Garden ID: ");
      Serial.println(GARDEN_ID);
      Serial.print("Humidity: ");
      Serial.println(humidityData);
      Serial.print("Temperature: ");
      Serial.println(temperatureData);
      Serial.print("Water Level: ");
      Serial.println(waterLevel);
      Serial.print("Water PumpOn: ");
      Serial.println(waterPumpOn);
      Serial.print("Soil Moist: %");
      Serial.println(soilMoistPersent);
      Serial.println("*******$~~~> Posted OK <~~~$*******");
      Serial.println("*");
      Serial.println("*");
      Serial.println("*");
      Serial.println("V");
    } else {
      Serial.println("Failed to Connect to server...");
    }
    client.stop();
    // END OF POST
  }