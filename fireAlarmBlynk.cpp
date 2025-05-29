#define BLYNK_TEMPLATE_ID "TMPLxxx" 
#define BLYNK_TEMPLATE_NAME "ESP8266xxx"
#define BLYNK_AUTH_TOKEN "YourAuthToken" 

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char ssid[] = " ";
char pass[] = " ";

#define DHTPIN D2
#define DHTTYPE DHT22
#define LED_PIN D3
#define BUZZER_PIN D4
#define MQ2_PIN A0
#define RELAY_PIN D5
#define FLAME_PIN D6

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void bacaSensor() {
  float suhu = dht.readTemperature();
  int mq2Value = analogRead(MQ2_PIN);
  int flameDetected = digitalRead(FLAME_PIN);

  Blynk.virtualWrite(V0, suhu);
  Blynk.virtualWrite(V1, mq2Value);
  Blynk.virtualWrite(V2, flameDetected == LOW ? 1 : 0);

  Serial.print("Suhu: "); Serial.print(suhu);
  Serial.print(" *C | MQ-2: "); Serial.print(mq2Value);
  Serial.print(" | Flame: ");
  Serial.println(flameDetected == LOW ? "API TERDETEKSI" : "AMAN");

  // Kondisi
  if (suhu >= 60.0 || mq2Value >= 400 || flameDetected == LOW) {
    tone(BUZZER_PIN, 1000);
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(RELAY_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(MQ2_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);

  dht.begin();

  timer.setInterval(2000L, bacaSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}
