
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

// ------------------------ ateşleme sistemi
#define led 23
#define buzzer 22
// ------------------------ ateşleme sistemi

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "your_wifi_id"
#define WIFI_PASSWORD "your_wifi_password"

#define API_KEY "your_firebase_api_key"

#define DATABASE_URL "your_firebase_url" 

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

int atesleme = 0;
float floatValue;


void setup(){
  
  // ------------------------ ateşleme sistemi
  pinMode(led,OUTPUT);
  pinMode(buzzer,OUTPUT);
  // ------------------------ ateşleme sistemi
  
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback; 
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    if (Firebase.RTDB.getInt(&fbdo, "/gelenData/veri")) {
      if (fbdo.dataType() == "int") {
        atesleme = fbdo.intData();
        // mobil uygulamadan eğer ateşleme komutu geldiyse :
        if(atesleme == 1){
          // önce geri sayım yapılacak buzzer ile.
          delay(2000);
          digitalWrite(buzzer,HIGH);
          delay(675);
          digitalWrite(buzzer,LOW);
          delay(675);
          digitalWrite(buzzer,HIGH);
          delay(675);
          digitalWrite(buzzer,LOW);
          delay(675);
          digitalWrite(buzzer,HIGH);
          delay(675);
          digitalWrite(buzzer,LOW);
          delay(675);
          digitalWrite(buzzer,HIGH);
          delay(675);
          digitalWrite(buzzer,LOW);
          delay(675);
          // geri sayım bittiği anda ateşleme komutu veriliyor.
          digitalWrite(buzzer,HIGH);
          //ateşleme :
          digitalWrite(led,HIGH);
        }
        else{
          // geri sayım bittiği anda ateşleme komutu veriliyor.
          digitalWrite(buzzer,LOW);
          //ateşleme :
          digitalWrite(led,LOW);
        }
      }
    }   
    else {
      Serial.println(fbdo.errorReason());
    }    
  }
}
