#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
uint8_t broadcastAddress[] = {0xC4, 0xDD, 0x57, 0xC9, 0x05, 0x28};
#define DHTPIN 25
#define DHTTYPE DHT11

typedef struct struct_message
{
  int a;
  float b;
  float c;
} struct_message;
struct_message myDataSen;
esp_now_peer_info_t peerInfo;
// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  
}

const int sensor=33;
const int bomba=27;
const int doneTpl=32;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  dht.begin();
  pinMode(sensor, INPUT_PULLDOWN);
  pinMode(bomba, OUTPUT);
  pinMode(doneTpl, OUTPUT);
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  digitalWrite(doneTpl, LOW);
  int sensorValue = analogRead(sensor);
  delay(3000);
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit 
  float f = dht.readTemperature(true);
  // Compute heat index in Fahrenheit 
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius 
  float hic = dht.computeHeatIndex(t, h, false);
  myDataSen.b=h;
  myDataSen.c=hic;
  myDataSen.a=sensorValue;
  Serial.println(sensorValue);
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myDataSen, sizeof(myDataSen));
  delay(1000);
  if(sensorValue>=730){
    digitalWrite(bomba, HIGH);
    delay(3000);
    digitalWrite(bomba, LOW);
  }
  digitalWrite(doneTpl, HIGH);
  delay(2000);
}
