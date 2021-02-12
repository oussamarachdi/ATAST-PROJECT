#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTPIN D0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

ESP8266WebServer server(80);

const char* ssid = "Orange-A4D5" ;
const char* pass = "M71LHT1G936";

WiFiClient  client;

unsigned long myChannelNumber = 1270126;
const char * myWriteAPIKey = "PKMME6U16VK5SA3M";

int Ground_H;

void setup() {
  pinMode(A0, INPUT);
  Serial.begin(115200);
  Serial.println("Serial started  ");
  dht.begin();
  Serial.println("DHT11  started");
  WiFi.softAP(ssid, pass);
  Serial.printf("\n SSID : %s \n",ssid);
  Serial.printf("\n PASS : %s \n",pass);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Ip adress : ");
  Serial.println(IP);
  server.begin();
  Serial.println("Server Started");
  ThingSpeak.begin(client);
}

void loop() {
  // read the input on analog pin 0 
  //Soil
  server.handleClient();
  Ground_H = analogRead(A0);  // the ground humidty value 
  Serial.println(Ground_H);
  //Temperature
  float temp = dht.readTemperature();
  Serial.println(temp);
  //Atmosphere humidity
  delay(100);
  float Atmosphere_H = dht.readHumidity();
  Serial.println(Atmosphere_H);
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network.
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  // ****** write data on Thingspeak *******
  ThingSpeak.setField(1, Ground_H);
  ThingSpeak.setField(2, temp);
  ThingSpeak.setField(3, Atmosphere_H);

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(10000); // delay of 10 seconds beetween each value

}
