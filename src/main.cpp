#include <Arduino.h>
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Hash.h"

const String mac = WiFi.macAddress();
const String serialNumber = sha1(mac);

const String mqtt_server = "plantacle.com";

const String topic = "/plantacle/" + serialNumber;

WiFiClient espClient;
PubSubClient mqttClient(espClient);
long lastMsg = 0;
char msg[50];


#define ONE_WIRE_BUS 4
int humidityPin = A0;
int humidityValue = 0;
int humidityPercent = 0;

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
WiFiManager wifiManager;

void setup()
{
  Serial.begin(9600);

  String shortenedSerialNumber = serialNumber;
  shortenedSerialNumber.remove(0,20);
  const String APName = "Plantacle-" + shortenedSerialNumber;

  wifiManager.setConfigPortalTimeout(30);
  wifiManager.autoConnect(APName.c_str());
  if (WiFi.status() != WL_CONNECTED)
  {
    ESP.restart();
  }

  mqttClient.setServer(mqtt_server.c_str(), 1883);
  mqttClient.connect(serialNumber.c_str(), "plantacle", "123456789");
  Serial.print("MQQT connection state: ");
  Serial.println(mqttClient.state());
  sensors.begin(); // initialize dallas sensors
}

void loop()
{
  delay(10000); /* Delay of amount equal to sampling period */
  humidityValue = analogRead(humidityPin);
  humidityPercent = ((float)humidityValue / (float)950) * 100; // 950 is the max value of the sensor
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0); /* Get temperature value */
  const String json = "{\"temperature\": " + String(temperature, 2) + ", \"humidity\": " + String(humidityPercent) + " }";
  Serial.println(json);
  mqttClient.publish(topic.c_str(), json.c_str());
  Serial.print("MQTT connection state: ");
  Serial.println(mqttClient.state());
  Serial.print("WiFi status: ");
  Serial.println(WiFi.status());
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("WiFi disconnected! Reconnecting.");
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
  }
  if (!mqttClient.connected())
  {
    Serial.println("MQTT disconnected! Trying reconnect.");
    mqttClient.connect("whatever", "plantacle", "123456789");
  }
  
  
}