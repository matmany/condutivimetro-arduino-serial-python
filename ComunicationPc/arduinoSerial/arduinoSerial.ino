#include <ArduinoJson.h>

void setup() {
  Serial.begin(9600);
  while (!Serial) continue;
}

void loop() {
  enviaJsonSerial(25.0, 99.9);
  delay(1000);
}

void enviaJsonSerial(float temperatura, float condutividade) {
  StaticJsonDocument<100> doc;
  doc["temp"] = temperatura;
  doc["condu"] = condutividade;
  serializeJson(doc, Serial);
  Serial.println();
}