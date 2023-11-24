#define BLYNK_TEMPLATE_ID "TMPL6OmCtSn95"
#define BLYNK_TEMPLATE_NAME "HeThongBaoChay"
#define BLYNK_AUTH_TOKEN "SjdMAHWAm8VwtpG9HE3_GtCcTAZLDvv_"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <string.h>

#include "DHT.h"

// DHT define
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 2
#define BUZZER_PIN 23
DHT dht(DHTPIN, DHTTYPE);

char auth[] = BLYNK_AUTH_TOKEN;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Tang2_Luan";
char pass[] = "luantrungpro";

bool temperatureConditionMet = false;
bool humidityConditionMet = false;

void setup()
{
  // Debug console
  Serial.begin(115200);
  dht.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  // Read Temp
  float t = dht.readTemperature();
  // Read Humi
  float h = dht.readHumidity();
  // Check isRead ?
  if (isnan(h) || isnan(t)) {
    delay(500);
    Serial.println("Failed to read from DHT sensor!\n");
    return;
  }

  // Check temperature condition
  if (t > 55) {
    temperatureConditionMet = true;
  } else {
    temperatureConditionMet = false;
  }

  // Check humidity condition
  if (h >= 85) {
    humidityConditionMet = true;
  } else {
    humidityConditionMet = false;
  }

  // Handle LED and buzzer based on conditions
  if (temperatureConditionMet || humidityConditionMet) {
    digitalWrite(BUZZER_PIN, HIGH);
    //WidgetLED LED(V0);
    //LED.on();
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    //WidgetLED LED(V0);
    //LED.off();
  }

  Blynk.virtualWrite(V0, t); //Nhiêt độ
  Blynk.virtualWrite(V1, h); //Độ ẩm

  Serial.print("\n");
  Serial.print("Humidity: " + String(h) + "%");
  Serial.print("\t");
  Serial.print("Temperature:" + String(t) + " C");
  delay(2000);
}
