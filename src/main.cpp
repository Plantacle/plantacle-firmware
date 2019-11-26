#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char *ssid =  "Stads-Lab";     // replace with your wifi ssid and wpa2 key
const char *pass =  "initialkey4iot";
const char *mqtt_server = "plantacle.com";

//Thermistor settings
const double VCC = 3.3;             // NodeMCU on board 3.3v vcc
const double R2 = 10000;            // 10k ohm series resistor
const double adc_resolution = 1023; // 10-bit adc

//Thermistor Equition Parameters
const double A = 0.001129148;   
const double B = 0.000234125;
const double C = 0.0000000876741; 


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

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
  float humidity = 40;/* Get humidity value */
  /* Get temperature value */
  // Serial.print(dht.getStatusString());/* Print status of communication */
  Serial.println("Humidity: ");
  Serial.println(humidity);
  Serial.println("Temperature: ");


    //Thermistor
  double Vout, Rth, temperature, adc_value; 
  adc_value = analogRead(A0);
  Vout = (adc_value * VCC) / adc_resolution;
  Rth = (VCC * R2 / Vout) - R2;
  temperature = (1 / (A + (B * log(Rth)) + (C * pow((log(Rth)),3))));   // Temperature in kelvin
  temperature = temperature - 273.15;  // Temperature in degree celsius

  Serial.println(temperature);
  const char* topic = "/plantacle/example";
  const String json = "{\"temperature\": " + String(temperature, 2) + ", \"humidity\": " + String(humidity, 2) + " }";
  client.publish(topic, json.c_str());
}