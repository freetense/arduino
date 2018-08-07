#include <SoftwareSerial.h>
#include <TinyGPS.h>
TinyGPS gps;
String _response = "";
String result = "";
String result1 = "i";
float flat, flon;
unsigned long age;
int year;
int pin = 5;
int res = 0;
byte month, day, hour, minute, second, hundredths;
SoftwareSerial portOne(10, 11);
SoftwareSerial portToo(7, 8); // RX, TX
void setup()
{
  Serial.begin(9600);
  portOne.begin(9600);
  portToo.begin(9600);
  
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delay(1000);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
    delay(100);
  portToo.listen();
  portToo.println("AT+CSCLK=0");
  delay(1000);
}
void loop()
{  

   if(result1 == "i"){
   delay(5);
   portToo.println("AT+CREG?");
   if (portToo.available()) {                     // Если есть, что считывать...
     _response = ReadGSM();                // ... считываем и запоминаем
   }
   Serial.println(_response);
   delay(100);
   res =  _response.indexOf("+CREG: 0,1");
   if(res > 0){
   }else{
    in_it();
   }
    digitalWrite(5, LOW);
     result1 = "n";
     portOne.listen();
     delay(100);
     gps_count();
     delay(100);
     portToo.listen();
     gprs();
     delay(100);
     while(result != "OK"){
     gprs();
     delay(5000);
     }
     portToo.listen();
     portToo.println("AT+SAPBR=0,1");
     delay(10000);
     portToo.println("AT+CUSD=1,\"*105#\"");                // ...и отправляем полученную команду модему
   }
    balance();
    digitalWrite(5, HIGH);


}
void in_it(){
    while(1){
      char value1 = 'e';
      int c;
      String v = "";
  while (portToo.available()) {  //сохраняем входную строку в переменную v
    c = portToo.read();
    v += char(c);
    delay(10);
    value1 = 'i';
  }
  if(value1 == 'i'){
  Serial.println(v);
  }
       res =  v.indexOf("Tele2");
    if (res > 0) break;    // и если он в "готовности", выходим из цикла
 }     
  }
void balance(){
  if (portToo.available())   {// Если модем, что-то отправил...
            long _timeout = millis() + 10000;             // Переменная для отслеживания таймаута (10 секунд)
            while (!portToo.available() && millis() < _timeout)  {}; // Ждем ответа 10 секунд, если пришел ответ или наступил таймаут, то...
            if (portToo.available()) {                     // Если есть, что считывать...
               _response = ReadGSM();                // ... считываем и запоминаем
            }
            _response.trim();                           // Убираем лишние пробелы в начале и конце
            if (_response.startsWith("+CUSD:")) {       // Пришло уведомление о USSD-ответе
                _response = _response.substring(_response.indexOf("\"") + 1);  // Получаем непосредственно содержимое ответа
                _response = _response.substring(0, _response.indexOf("\""));
                _response = UCS2ToString(_response); // Декодируем ответ
                Serial.println(_response);
            }else{
                Serial.println(_response);
            }
  }
}
void gprs(){
 gprs_init();
 result = "AT+HTTPPARA=\"URL\",\"http://mumuka.000webhostapp.com/?a=8\"";
    //отправка данных на сайт
  int d = 400;
 Serial.println("Send start");
 Serial.println("setup url");
  portToo.println(result);
  delay(d * 2);
  _response = ReadGSM();
    _response.trim();
      if ( _response.startsWith(result)) {                // Убираем из ответа дублирующуюся команду
       result =  _response.substring( _response.indexOf("\r\n", result.length()) + 2);
    }
  Serial.println(_response);
  delay(d);
  result = "AT+HTTPPARA=\"URL\",\"http://mumuka.000webhostapp.com/?a=2\"";
  portToo.println(result);
  delay(d * 2);
  _response = ReadGSM();
  _response.trim();
      if ( _response.startsWith(result)) {                // Убираем из ответа дублирующуюся команду
       result =  _response.substring( _response.indexOf("\r\n", result.length()) + 2);
    }
  Serial.println(_response);
  delay(d);
  Serial.println("GET url");
  portToo.println("AT+HTTPACTION=0");
  delay(d * 2);
  Serial.println(ReadGSM());
  delay(d);
  //Serial.println("Send done");
  }
void gps_count(){
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  gps.f_get_position(&flat, &flon, &age);
  smartdelay(1000);
  }
void gprs_init() {  //Процедура начальной инициализации GSM модуля
  int d = 500;
  int ATsCount = 7;
  String ATs[] = {  //массив АТ команд
    "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"",  //Установка настроек подключения
    "AT+SAPBR=3,1,\"APN\",\"internet.tele2.ru\"",
    "AT+SAPBR=3,1,\"USER\",\"tele2\"",
    "AT+SAPBR=3,1,\"PWD\",\"tele2\"",
    "AT+SAPBR=1,1",  //Устанавливаем GPRS соединение
    "AT+HTTPINIT",  //Инициализация http сервиса
    "AT+HTTPPARA=\"CID\",1"  //Установка CID параметра для http сессии
  };
  int ATsDelays[] = {6, 1, 1, 1, 3, 3, 1}; //массив задержек
  //Serial.println("GPRG init start");
  for (int i = 0; i < ATsCount; i++) {
    Serial.println(ATs[i]);  //посылаем в монитор порта
    portToo.println(ATs[i]);  //посылаем в GSM модуль
    delay(d * ATsDelays[i]);
    Serial.println(ReadGSM());  //показываем ответ от GSM модуля
    delay(d);
  }
  Serial.println("GPRG init complete");
}
String ReadGSM() {  //функция чтения данных от GSM модуля
  int c;
  String v;
  while (portToo.available()) {  //сохраняем входную строку в переменную v
    c = portToo.read();
    v += char(c);
    delay(10);
  }
  return v;
}
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (portOne.available())
      gps.encode(portOne.read());
  } while (millis() - start < ms);
}
String UCS2ToString(String result) {                       // Функция декодирования UCS2 строки
_response = "";
  unsigned char c[5] = "";                            // Массив для хранения результата
  for (int i = 0; i < result.length() - 3; i += 4) {       // Перебираем по 4 символа кодировки
    unsigned long code = (((unsigned int)HexSymbolToChar(result[i])) << 12) +    // Получаем UNICODE-код символа из HEX представления
                         (((unsigned int)HexSymbolToChar(result[i + 1])) << 8) +
                         (((unsigned int)HexSymbolToChar(result[i + 2])) << 4) +
                         ((unsigned int)HexSymbolToChar(result[i + 3]));
    if (code <= 0x7F) {                               // Теперь в соответствии с количеством байт формируем символ
      c[0] = (char)code;                              
      c[1] = 0;                                       // Не забываем про завершающий ноль
    } else if (code <= 0x7FF) {
      c[0] = (char)(0xC0 | (code >> 6));
      c[1] = (char)(0x80 | (code & 0x3F));
      c[2] = 0;
    } else if (code <= 0xFFFF) {
      c[0] = (char)(0xE0 | (code >> 12));
      c[1] = (char)(0x80 | ((code >> 6) & 0x3F));
      c[2] = (char)(0x80 | (code & 0x3F));
      c[3] = 0;
    } else if (code <= 0x1FFFFF) {
      c[0] = (char)(0xE0 | (code >> 18));
      c[1] = (char)(0xE0 | ((code >> 12) & 0x3F));
      c[2] = (char)(0x80 | ((code >> 6) & 0x3F));
      c[3] = (char)(0x80 | (code & 0x3F));
      c[4] = 0;
    }
    _response += String((char*)c);                       // Добавляем полученный символ к результату
  }
  return (_response);
}
unsigned char HexSymbolToChar(char c) {
  if      ((c >= 0x30) && (c <= 0x39)) return (c - 0x30);
  else if ((c >= 'A') && (c <= 'F'))   return (c - 'A' + 10);
  else                                 return (0);
}

