// #include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <PubSubClient.h>
// #include <OneWire.h>
// #include <DallasTemperature.h>

// //Temperature sensor setup
// #define ONE_WIRE_BUS 2
// OneWire oneWire(ONE_WIRE_BUS);
// DallasTemperature sensors(&oneWire);

// const char *ssid =  "Stads-Lab";     // replace with your wifi ssid and wpa2 key
// const char *pass =  "initialkey4iot";
// const char *mqtt_server = "plantacle.com";

// WiFiClient espClient;
// PubSubClient client(espClient);
// long lastMsg = 0;
// char msg[50];
// int value = 0;

// void setup()
// {
//   // pinMode(2, INPUT);
//   Serial.begin(9600);
//   delay(10);

//   Serial.println("Connecting to ");
//   Serial.println(ssid);

//   WiFi.begin(ssid, pass);
//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.println();
//   client.setServer(mqtt_server, 1883);
//   client.connect("whatever", "plantacle", "123456789");
//   Serial.println(client.state());  

//   sensors.begin();
// }

// void loop()
// {
//   delay(2000); /* Delay of amount equal to sampling period */
//   float humidity = 40;/* Get humidity value */
//   /* Get temperature value */
//   // Serial.print(dht.getStatusString());/* Print status of communication */
//   Serial.println("Humidity: ");
//   Serial.println(humidity);
//   Serial.println("Temperature: ");

//   sensors.requestTemperatures(); 
//   float temperature = sensors.getDeviceCount();
 
//   Serial.println(temperature);
//   const char* topic = "/plantacle/example";
//   const String json = "{\"temperature\": " + String(temperature, 2) + ", \"humidity\": " + String(humidity, 2) + " }";
//   client.publish(topic, json.c_str());
// }


/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com  
*********/

#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 2;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the Serial Monitor
  Serial.begin(9600);
  // Start the DS18B20 sensor
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
  delay(5000);
}