#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>


#define D_SSID "BORWL 34"
#define D_PASSWORD "123school"

#define M_SSID "E34"
#define M_PASSWORD "AlmostEasy"

static char *ssidString;
static char *passString;

WiFiUDP ntpUdp;
NTPClient timeClient(ntpUdp, "europe.pool.ntp.org", 0, 60000);

IPAddress apIP(192, 168, 4, 1);

void setup() {
  // put your setup code here, to run once:

    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    
    //Connection to Wi-Fi
    Connection(D_SSID, D_PASSWORD);
    
    timeClient.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
if (WiFi.status() != WL_CONNECTED) {
      Connection_Non_Pass();
    } else {
        Serial.println(timeClient.getEpochTime());
    }
}
