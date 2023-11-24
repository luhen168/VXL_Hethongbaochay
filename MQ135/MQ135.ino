#define BLYNK_TEMPLATE_ID "TMPL6OmCtSn95"
#define BLYNK_TEMPLATE_NAME "HeThongBaoChay"
#define BLYNK_AUTH_TOKEN "SjdMAHWAm8VwtpG9HE3_GtCcTAZLDvv_"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <MQ135.h>

#define BUZZER_PIN 23
#define MQ135_PIN A4    

char ssid[] = "Tang2_Luan";
char pass[] = "luantrungpro";
MQ135 mq135(MQ135_PIN); 

BlynkTimer timer;
bool airQualityConditionMet = false;

void sendSensorData() {
    float airQuality = mq135.getPPM();
    Serial.print("Air Quality (PPM): "); // In ra màn hình Serial Monitor
    Serial.println(airQuality); // In giá trị PPM
    Blynk.virtualWrite(V2, airQuality); // Gửi dữ liệu MQ135 lên biểu đồ trong Blynk

    // Kiểm tra nồng độ khói
    if (airQuality > 1000) {
        airQualityConditionMet = true;
    } else {
        airQualityConditionMet = false;
    }

    if (airQualityConditionMet) {
        Blynk.virtualWrite(BUZZER_PIN, HIGH);
        // WidgetLED LED(V0);
        // LED.on();
    } else {
        Blynk.virtualWrite(BUZZER_PIN, LOW);
        // WidgetLED LED(V0);
        // LED.off();
    }
}

void setup() {
  Serial.begin(9600);

  // Khởi tạo kết nối WiFi
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Kết nối với Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Khởi tạo cảm biến MQ135

  // Gọi hàm sendSensorData mỗi 2 s (300000ms)
  timer.setInterval(2000, sendSensorData);
}

void loop() {
    Blynk.run();
    timer.run();
}
