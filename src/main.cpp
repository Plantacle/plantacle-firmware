#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char *ssid =  "Stads-Lab";     // replace with your wifi ssid and wpa2 key
const char *pass =  "initialkey4iot";
const char *mqtt_server = "plantacle.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];

int humidityPin = A0;
int humidityValue = 0;
int humidityPercent = 0;

void setup()
{
  Serial.begin(9600);
  delay(10);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println();
  client.setServer(mqtt_server, 1883);
  client.connect("whatever", "plantacle", "123456789");
  Serial.println(client.state());  
}

void loop()
{
  delay(2000); /* Delay of amount equal to sampling period */
  humidityValue = analogRead(humidityPin);
  humidityPercent = ((float)humidityValue / (float)950) * 100; // 950 is the max value of the sensor
  float temperature = 20; /* Get temperature value */
  // Serial.print(dht.getStatusString());/* Print status of communication */
  // Serial.println("Humidity: ");
  // Serial.println(humidityPercent);
  // Serial.println("Temperature: ");
  // Serial.println(temperature);
  const char* topic = "/plantacle/example";
  const String json = "{\"temperature\": " + String(temperature, 2) + ", \"humidity\": " + String(humidityPercent) + " }";
  Serial.println(json);
  client.publish(topic, json.c_str());
}