#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

typedef struct struct_message {
    int a;
    float b;
    float c;
} struct_message;

// Create a struct_message called myData
struct_message myDataRec;

esp_now_peer_info_t peerInfo;

String array[3];

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myDataRec, incomingData, sizeof(myDataRec));
  array[0]=myDataRec.a;
  array[1]=myDataRec.b;
  array[2]=myDataRec.c;
  Serial.print("Humedad tierra: ");
  Serial.print(array[0]);
  Serial.print(", humedad ambiente: ");
  Serial.print(array[1]);
  Serial.print(", temperatura ambiente: ");
  Serial.print(array[2]);
  Serial.println("");
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  // Add peer    
     
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
}
 
void loop() {
  
}
