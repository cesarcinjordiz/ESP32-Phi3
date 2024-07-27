#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress1[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcastAddress2[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct struct_message
{
  char a[200];
} struct_message;
struct_message myDataRec;
struct_message myDataSen;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
}
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&myDataRec, incomingData, sizeof(myDataRec));
  Serial.println(myDataRec.a);
}
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  // Register peer
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  //Add peer mac1
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  //Add peer mac2
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
   if (Serial.available()>0) {
    String data = Serial.readString();
    data.toCharArray(myDataSen.a, 200);
    esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *)&myDataSen, sizeof(myDataSen));
  }
}
