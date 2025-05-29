#include <DHT.h>

// Pin konfigurasi
#define DHTPIN 2              
#define DHTTYPE DHT22         
#define LED_PIN 3             
#define BUZZER_PIN 4          
#define MQ2_PIN A0            
#define RELAY_PIN 5           
#define FLAME_PIN 6 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(MQ2_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT); 

  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float suhu = dht.readTemperature();
  int mq2Value = analogRead(MQ2_PIN);       
  int flameDetected = digitalRead(FLAME_PIN);

  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" *C | MQ-2: ");
  Serial.print(mq2Value);
  Serial.print(" | Flame: ");
  Serial.println(flameDetected == LOW ? "API TERDETEKSI" : "AMAN");

  // Kondisi bahaya jika suhu tinggi, gas, atau api terdeteksi
  if (suhu >= 60.0 || mq2Value >= 400 || flameDetected == LOW) {
    tone(BUZZER_PIN, 1000);
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(RELAY_PIN, HIGH); 
    delay(300);

    tone(BUZZER_PIN, 1500);
    digitalWrite(LED_PIN, LOW);
    delay(300);
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(RELAY_PIN, LOW); 
  }

  delay(300);
}
