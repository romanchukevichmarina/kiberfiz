#include <ESP8266WiFi.h>
#include <WiFiClient.h>
 
#define RADIO_DATA 13
#define RADIO_BUF_SIZE 10
#define G433_SPEED 1000
 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Gyver433.h>
#include <GyverNTC.h>
 
// Общие настройки
// WiFi-settings
const char* ssid = "mgts339";
const char* password = "4953881574";
 
// MQTT-settings
const char* mqtt_server = "188.0.19.63";
const char* mqtt_login = "someonef";
const char* mqtt_password = "someonef";
const char* mqtt_clientId = "alarm_sys_03459";
 
char msg[50]; // Буфер сообщениия MQTT
String motion_message;
bool flag;
 
// Создаем объект-киента WiFiи MQTT
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  client.setServer(mqtt_server, 1883);
  pinMode(D2, INPUT);
  pinMode(D5, OUTPUT);
  pinMode(D7, OUTPUT);

  digitalWrite (D5, LOW);
  digitalWrite (D7, LOW);
 
  setup_wifi();
  mqtt_reconnnect();
  client.publish("home/hall/alarm_system/motion_captured", String("OFF").c_str());
}
 
void loop() {
   if (!client.connected()) {
    mqtt_reconnnect();
  }
  client.loop();
  byte motion_sensor_status = digitalRead(D2);
  Serial.println(motion_sensor_status);
  if(motion_sensor_status == 1 && flag == false)
 {
    client.publish("home/hall/alarm_system/motion_captured", String("ON").c_str());
    flag = true;
    digitalWrite (D7, HIGH);
  }
  if(motion_sensor_status == 0 && flag == true)
  {
    client.publish("home/hall/alarm_system/motion_captured", String("OFF").c_str());
    flag = false;
    digitalWrite (D7, LOW);
  }
  delay(50);
}
 
void mqtt_reconnnect() {
  // Пока не подулючится, будем пытаться это сделть
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
 
    if (client.connect(mqtt_clientId, mqtt_login, mqtt_password)) {
      Serial.println("Mqtt connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
 
 
void setup_wifi() {
  delay(10);
 
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //WiFi.begin(ssid); // Меняем строчку выше на эту, если пароля нет
 
  // Ждем пока не подключится
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  randomSeed(micros()); // Что это - без понятия, но наверное лучше оставить)))
 
  // Выводим инфу о подключении
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
