#include<ESP8266WiFi.h>
#include "DHT.h"
#define DHTTYPE DHT11
const char* ssid="V2s";
const char* password="sridevsu";
WiFiServer server(80);
const int DHTPin=5;
DHT dht(DHTPin,DHTTYPE);
static char celsiusTemp[7];
static char humidityTemp[7];
void setup()
{
  Serial.begin(115200);
  delay(10);
  dht.begin();
  Serial.println();
  Serial.print("connecting to");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while(WiFi.status() !=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
Serial.println(" ");
Serial.println("WiFi connected");
server.begin();
Serial.println("web server running.Waiting for the ESP IP...");
delay(10000);
Serial.println(WiFi.localIP());
}
void loop()
{
  WiFiClient client=server.available();
  if(client) {
    Serial.println("New client");
    boolean blank_line=true;
    while(client.connected()){
      if(client.available()){
        char c=client.read();
        if(c=='\n' && blank_line){
          float h=dht.readHumidity();
          float t=dht.readTemperature();
          if(isnan(h)||isnan(t)){
            Serial.println("Failed to read from dht sensor");
            strcpy(celsiusTemp,"Failed");
            strcpy(humidityTemp,"Failed");
          }
          else{
            float hic=dht.computeHeatIndex(t,h,false);
            dtostrf(hic,6,2,celsiusTemp);
            dtostrf(h,6,2,humidityTemp);
            Serial.print("Humidity: ");
            Serial.print(h);
            Serial.print("%\t Temperature: ");
            Serial.print(t);
            Serial.print(" *C");
            Serial.print("Humidity: ");
            Serial.print(h);
            Serial.print("%\t Temperature: ");
            Serial.print(t);
            Serial.print(" *C");
            Serial.print(hic);
            Serial.print(" *C ");
          
            
          }
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("connection : close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head></head><body><h1>ESP8266 -Tempearature and Humidity</h1><h3>Temperature in celsius: ");
          client.println(celsiusTemp);
          client.println("*C</h3><h3>Humidity: ");
          client.println(humidityTemp);
          client.println("%</h3><h3>");
          client.println("</body></html>");
          break;
        }
        if(c== '\n'){
          blank_line=true;
        }
        else if(c!= '\r'){
          blank_line=false;
        }
        }
        
      }
      delay(1);
      client.stop();
      Serial.println("Client disconnected");
    }
    
  }


