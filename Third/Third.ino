#include <DallasTemperature.h>
#include <OneWire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>


#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>
#include <AzureIoTUtility.h>

#define D_SSID "BORWL 34"
#define D_PASSWORD "123school"
#define MQTT_MAX_PACKET_SIZE 256 

#define M_SSID "E34"
#define M_PASSWORD "AlmostEasy"

#define DEVICE_CONNECTION_STRING "HostName=IoTSchoolHub.azure-devices.net;DeviceId=iotdevice;SharedAccessKey=Jc96azpOgk3re2fe1sl6JhsQbCk07M5vWKKu8ceJ8FQ="

#define DEVICE_ID "iotdevice"

#define INTERVAL 1000
#define EEPROM_SIZE 512
#define SSID_LEN 32
#define PASS_LEN 32
#define CONNECTION_STRING_LEN 256
#define MESSAGE_MAX_LEN 768

static char *connectionString;
static char *ssidString;
static char *passString;


static bool messagePending = false;
static bool messageSending = true;
   
const long interval_dots = 50;  
float Temperature_Array[interval_dots];

unsigned long Ticks_Array[interval_dots];

static int increment;

static int messageCount = 1;
static int interval = INTERVAL;

WiFiUDP ntpUdp;
NTPClient timeClient(ntpUdp, "europe.pool.ntp.org", 0, 60000);

IPAddress apIP(192, 168, 4, 1);

#define TEMPERATURE_PIN D4

OneWire oneWire(TEMPERATURE_PIN);
DallasTemperature DS18B20(&oneWire);


static IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    readCredentials();
    
    Connection(ssidString, passString);

    iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol);
    bool traceOn = true;
    IoTHubClient_LL_SetOption(iotHubClientHandle, "logtrace", &traceOn);
    
    if (iotHubClientHandle == NULL) { // if hub client handle is failed to create
          Serial.println("Failed on IoTHubClient_CreateFromConnectionString.");
          while (1);
    }
    //Init standart message call and callback functions for IoT Hub client
    IoTHubClient_LL_SetOption(iotHubClientHandle, "product_info", "NodeMCU_ESP8266");
    IoTHubClient_LL_SetMessageCallback(iotHubClientHandle, receiveMessageCallback, NULL);
    IoTHubClient_LL_SetDeviceMethodCallback(iotHubClientHandle, deviceMethodCallback, NULL);
    IoTHubClient_LL_SetDeviceTwinCallback(iotHubClientHandle, twinCallback, NULL);
    
    timeClient.begin();
    DS18B20.begin();
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
      Connection_Non_Pass();
    } else {
        float temp = getTemperature();
        Serial.println(temp);
        timeClient.update();
        Serial.println(timeClient.getEpochTime());
        Temperature_Array[increment] = temp;
        Ticks_Array[increment] = timeClient.getEpochTime();
        increment++;
        if (!messagePending && messageSending) {
          char messagePayload[MESSAGE_MAX_LEN];
          readMessage(messageCount, messagePayload, Temperature_Array, Ticks_Array, increment);
          sendMessage(iotHubClientHandle, messagePayload);
          memset(Temperature_Array, 0.0, interval_dots);
          memset(Ticks_Array, 0, interval_dots);
          messageCount++;
          increment = 0;
          delay(interval);
        } 
        IoTHubClient_LL_DoWork(iotHubClientHandle);
        delay(10);
    }
    delay(20);
}
