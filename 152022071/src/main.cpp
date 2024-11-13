#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttUser = "admin";
const char* mqttPassword = "public";

WiFiClient espClient;
PubSubClient client(espClient);

const int pinLEDGreen = 27;
const int pinLEDYellow = 26;
const int pinLEDRed = 14;
const int pinRelayPump = 32;
const int pinBuzzer = 25;

const int pinDHT = 33;
DHT dht(pinDHT, DHT22);

// Deklarasi fungsi yang hilang
void mqttCallback(char* topic, byte* payload, unsigned int length);
void reconnectMQTT();

void setup() {
  Serial.begin(115200);
  
  pinMode(pinLEDGreen, OUTPUT);
  pinMode(pinLEDYellow, OUTPUT);
  pinMode(pinLEDRed, OUTPUT);
  pinMode(pinRelayPump, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(mqttCallback);
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();

  if (isnan(suhu) || isnan(kelembaban)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" °C  Kelembaban: ");
  Serial.print(kelembaban);
  Serial.println(" %");

  String payload = "{\"temperature\":" + String(suhu) + ", \"humidity\":" + String(kelembaban) + "}";
  client.publish("sensor/temperature", String(suhu).c_str());
  client.publish("sensor/humidity", String(kelembaban).c_str());

  if (suhu > 35) {
    digitalWrite(pinLEDRed, HIGH);
    digitalWrite(pinLEDYellow, LOW);
    digitalWrite(pinLEDGreen, LOW);
    digitalWrite(pinBuzzer, HIGH);
    digitalWrite(pinRelayPump, HIGH);
  } else if (suhu >= 30 && suhu <= 35) {
    digitalWrite(pinLEDYellow, HIGH);
    digitalWrite(pinLEDGreen, LOW);
    digitalWrite(pinLEDRed, LOW);
    digitalWrite(pinBuzzer, LOW);
    digitalWrite(pinRelayPump, LOW);
  } else {
    digitalWrite(pinLEDGreen, HIGH);
    digitalWrite(pinLEDRed, LOW);
    digitalWrite(pinLEDYellow, LOW);
    digitalWrite(pinBuzzer, LOW);
    digitalWrite(pinRelayPump, LOW);
  }

  delay(2000);
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESPClient", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("Failed to connect, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Logic untuk menangani pesan yang diterima, jika diperlukan
}
