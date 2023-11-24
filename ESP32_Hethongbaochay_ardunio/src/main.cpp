#define BLYNK_TEMPLATE_ID "TMPL6OmCtSn95"
#define BLYNK_TEMPLATE_NAME "HeThongBaoChay"
#define BLYNK_AUTH_TOKEN "SjdMAHWAm8VwtpG9HE3_GtCcTAZLDvv_"
#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <MQ135.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define BUZZER_PIN 23
#define MQ135_PIN 32    
#define DHTTYPE DHT11   
#define DHTPIN 33
#define WIFI_SSID "Tang2_Luan"
#define WIFI_PASSWORD "luantrungpro"

const int i2c_addr = 0x27; // Địa chỉ mặc định của màn hình LCD I2C 16x2
LiquidCrystal_I2C lcd(i2c_addr, 16, 2); 

MQ135 mq135(MQ135_PIN); 
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
bool airQualityConditionMet = false;
bool temperatureConditionMet = false;
bool humidityConditionMet = false;

void setup_wifi();
void setup_LCD();
void getData_MQ135();
void getData_DHT11();
void compare_Data();



void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1000);
  setup_LCD();        // Khởi tạo LCD
  setup_wifi();       // Khởi tạo kết nối WiFi      
  pinMode(BUZZER_PIN, OUTPUT);    // Khởi tạo chân còi
  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);    // Kết nối với Blynk
}

void loop() {
    Blynk.run();
    timer.run();  
    getData_MQ135();
    getData_DHT11();
    delay(2000);
}

void setup_wifi() {
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD );
  lcd.setCursor(0, 0);
  lcd.print("Connecting to ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    for (int i=0;i<=12;i++){
      delay(500);
      lcd.setCursor(i, 1);
      lcd.print(".");
    }
    lcd.setCursor(0, 1);
    lcd.print("                 ");
  }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connected");
    Serial.println("");
    Serial.println("WiFi connected");
}

void setup_LCD(){
  lcd.init();
  lcd.backlight();
}

void getData_MQ135() {
    float gas = mq135.getPPM();
    Serial.print("Air Quality (PPM): "); // In ra màn hình Serial Monitor
    Serial.println(gas); // In giá trị PPM
    Blynk.virtualWrite(V2, gas); // Gửi dữ liệu MQ135 lên biểu đồ trong Blynk

}

void getData_DHT11(){
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

  Blynk.virtualWrite(V0, t); //Nhiêt độ
  Blynk.virtualWrite(V1, h); //Độ ẩm

  Serial.print("\n");
  Serial.print("Humidity: " + String(h) + "%");
  Serial.print("\t");
  Serial.print("Temperature:" + String(t) + " C");

}

