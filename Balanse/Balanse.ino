#include <SoftwareSerial.h>
String _response = "";
String result = "";
String result1 = "i";
SoftwareSerial ss(7, 8); // RX, TX
void setup()
{
  Serial.begin(9600);
  ss.begin(9600);
}

void loop()
{
   if(result1 == "i"){
    result1 = "n";
    ss.println("AT+CUSD=1,\"*105#\"");                // ...и отправляем полученную команду модему
   }
if (ss.available())   {// Если модем, что-то отправил...
long _timeout = millis() + 10000;             // Переменная для отслеживания таймаута (10 секунд)
  while (!ss.available() && millis() < _timeout)  {}; // Ждем ответа 10 секунд, если пришел ответ или наступил таймаут, то...
  if (ss.available()) {                     // Если есть, что считывать...
    _response = ss.readString();                // ... считываем и запоминаем
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

