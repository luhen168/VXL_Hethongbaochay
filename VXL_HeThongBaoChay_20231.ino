#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

#define WIFI_SSID "Tang2_Luan"
#define WIFI_PASSWORD "luantrungpro"
#define MQTT_SERVER "test.mosquitto.org"
#define MQTT_PORT 1883

unsigned long previousMillis = 0; 
const long interval = 5000;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

const int i2c_addr = 0x27; // Địa chỉ mặc định của màn hình LCD I2C 16x2
LiquidCrystal_I2C lcd(i2c_addr, 16, 2); 

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
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void setup_LCD(){
  lcd.init();
  lcd.backlight();
}
 
void connect_to_broker() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "lu1";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish("lu1","Hello everyone");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}
 
void callback(char* topic, byte *payload, unsigned int length) {
  char status[20];
  Serial.println("-------new message from broker-----");
  Serial.println("topic: ");
  Serial.println(topic);
  Serial.println("message: ");
  Serial.write(payload, length);
  Serial.println();
  for(int i = 0; i<length; i++)
  {
    status[i] = payload[i];
  }
}
 
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(500);
  setup_LCD();
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT );
  client.setCallback(callback);
  connect_to_broker();
  Serial.println("Start transfer");
}
 
  
void loop() {
  client.loop();
  if (!client.connected()) {
    connect_to_broker();
  }
}