
//================================================================
// esp8266_bmp180_server
//
// An ESP8266 with BMP180 attached to I2C to serve up temperature
// and pressure.
//
// * pin 4 = SDA
// * pin 5 = SCL
//
// Carter Nelson
// 2016-03-15
//================================================================
#include <ESP8266WiFi.h>
#include <Adafruit_BMP085.h>
#include "network_config.h"    // your network config here

WiFiServer server(MY_PORT);
IPAddress IP_ADDRESS(MY_IP_ADDRESS);
IPAddress IP_GATEWAY(MY_IP_GATEWAY);
IPAddress IP_SUBNET(MY_IP_SUBNET);

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(115200);
  Serial.println("ESP8266 BMP180 Server");

  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(MY_SSID);
  WiFi.config(IP_ADDRESS, IP_GATEWAY, IP_SUBNET);
  WiFi.begin(MY_SSID, MY_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected. [ip addr ");  
  Serial.print(WiFi.localIP());
  Serial.println("]");

  server.begin();
  Serial.println("Server started.");

  // Initialize BMP180 sensor
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1) {}
  }
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("New client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Process the request
  req.toUpperCase();
  String resp;
  if (req.indexOf("GCT") != -1)
    resp = String(32.0+1.8*bmp.readTemperature(), 2);
  else if (req.indexOf("GCP") != -1)
    resp = String(0.000145038*bmp.readPressure(), 2);
  else
    resp = "-999";

  // Send the response to the client
  Serial.println(resp);
  client.print(resp);
  delay(1);
  Serial.println("Client disonnected");
}
