#include <ESP8266WiFi.h>

void Connection(char *_ssid, char *_password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);
    
    while (WiFi.status() != WL_CONNECTED) {
        uint8_t mac[6];
        
        WiFi.macAddress(mac);
        Serial.printf("You device with MAC address %02x:%02x:%02x:%02x:%02x:%02x connects to %s failed! Waiting 10 seconds to retry.\r\n",
                    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], _ssid);
                    
        WiFi.begin(_ssid, _password);
  
        for (int i = 0; i < 10; i++) {
              digitalWrite(LED_BUILTIN, HIGH);
              delay(500);    
              digitalWrite(LED_BUILTIN, LOW);
              delay(500);
        }
    }
  
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Error: Connection failed");
    } else {
      Serial.println("Connection succeed");
    }
}

void Connection_Non_Pass() {
    Serial.println("Start to connect to free WiFi points");
    int networksQty = WiFi.scanNetworks();
    if (networksQty == 0) {
        Serial.println("There are no free WiFi points");
    } else {
        for (int i = 0; i < networksQty; i++) {
             if (WiFi.encryptionType(i + 1) == ENC_TYPE_NONE) {
                continue;
             } else {
                Connection((char *)WiFi.SSID(i + 1).c_str(), "");
                if (WiFi.status() == WL_CONNECTED) {
                    break;
                } else {
                    continue;
                }
             }
        }
    }
}
