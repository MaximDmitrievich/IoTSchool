#include <DallasTemperature.h>
#include <OneWire.h>

#define TEMPERATURE_PIN D4

OneWire oneWire(TEMPERATURE_PIN);
DallasTemperature DS18B20(&oneWire);

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    DS18B20.begin();
}

void loop() {
    Serial.println(getTemperature());
    delay(20);
}
