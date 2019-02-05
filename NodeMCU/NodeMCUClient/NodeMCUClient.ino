#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
extern "C" {
#include "user_interface.h"
  uint16 readvdd33(void);
  bool wifi_set_sleep_type(sleep_type_t);
  sleep_type_t wifi_get_sleep_type(void);
}
#include <ESP8266HTTPClient.h>
// Имя и пароль вашей сети WiFi
const char* ssid = "ZTE MF920";
const char* password = "c3KLPasa";
const char* host = "mumuka.000webhostapp.com";
WiFiServer server(80);
IPAddress ser(192, 168, 0, 12);
WiFiClient client;
HTTPClient http;
void setup() {
  pinMode(2, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  Serial.begin(9600);
  printWifiStatus();
}

void loop() {
  String line = "";
  delay(500);
  digitalWrite(16, LOW);
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  digitalWrite(2, LOW);
  delay(500);
  digitalWrite(2, HIGH);
  wifi_set_sleep_type(NONE_SLEEP_T);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(5, HIGH);
    printWifiStatus();
  }
  // Проверка подключения
  client = server.available();
  if (client) {
    while (client.connected()) {
        digitalWrite(16, HIGH);

      if (client.available()) {
        digitalWrite(4, HIGH);    
        String line = client.readStringUntil('\n');
        digitalWrite(5, HIGH);
        // Чтение первой строки запроса
        Serial.println(line);
        if (line.indexOf("/4") != -1) {
          http.begin("http://mumuka.000webhostapp.com/insert.php?tok=f87sdf97sd97wer&id=2&x=1&y=2");
          int httpCode = http.GET();
          http.end();
          Serial.println("Данные переданы");
          String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><meta charset='UTF-8'><meta http-equiv='Cache-Control' content='no-cache'><title>freetense.ru</title></head><body>Данные отправлены";
          s += "</body></html>\n";
          client.print(s);
          delay(4000);
        } else if (line.indexOf("/5") != -1) {
          Serial.println("Тест");
          String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nSet-cookie: ARDUINOSESSIONID=1\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><meta charset='UTF-8'><meta http-equiv='Cache-Control' content='no-cache'><title>freetense.ru</title></head><body><div ></div></div><div ><div ><a href='/javascript'>JavaScript</a></div><div ></div></div></div></div><div ><div ><br>www.freetense.ru &copy;<br><div ></div><br><div >При копировании материала ссылка на первоисточник обязательна.</div>ВСЕ ПРАВА ЗАЩИЩЕНЫ.</div></div></div></div></body></html>";
          client.print(s);
        } else {
          // Если неверный запрос написать об ошибке
          Serial.println("Неверный запрос");
          String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><meta charset='UTF-8'><meta http-equiv='Cache-Control' content='no-cache'><title>freetense.ru</title></head><body>Неверный запрос</body>";
          s += "</html>\n";
          client.print(s);
        }
      }
    digitalWrite(4, LOW);
    delay(300);
    client.stop();
    Serial.println("клиент отключен");
  }
 }
}

void printWifiStatus() {
  Serial.print("Соединяюсь с WIFI ");
  Serial.println(ssid);
  WiFi.disconnect();
  delay(500);
  WiFi.config(IPAddress (192, 168, 0, 12), IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0), IPAddress(192, 168, 0, 1));
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WIFI соединение установлено");
  long rssi = WiFi.RSSI();
  Serial.print("Уровень сигнала (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  server.begin();
  Serial.println("Сервер запущен");
  // Вывод полученного IP адреса
  Serial.println(WiFi.localIP());
  wifi_set_sleep_type(NONE_SLEEP_T);
}
