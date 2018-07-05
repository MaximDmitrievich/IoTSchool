#include <ArduinoJson.h>

void readMessage(int messageId, char *payload, float *temperature, unsigned long *ticks, int inc) {
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    
    root["DeviceId"] = DEVICE_ID;
    root["MessageId"] = messageId;
    
    JsonArray &tempjson = root.createNestedArray("Temperature");
    JsonArray &ticksjson = root.createNestedArray("Ticks");
    
    for (int i = 0; i < inc; i++) {
        tempjson.add(temperature[i]);
        ticksjson.add(ticks[i]);
    }
    
    root.printTo(payload, MESSAGE_MAX_LEN);
}

void parseTwinMessage(char *message) {
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(message);
    
    if (!root.success()) {
        Serial.printf("Parse %s failed.\r\n", message);
        return;
    }

    if (root["desired"]["interval"].success()) {
        interval = root["desired"]["interval"];
    } else if (root.containsKey("interval")) {
        interval = root["interval"];
    }
}

