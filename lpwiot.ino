#include<ESP8266WiFi.h>
#include <DHT.h>
#define DHTPIN D3
#define DHTTYPE DHT11   
DHT dht(DHTPIN,DHTTYPE);
const char* ssid="V2s";
const char* password="sridevsu";
unsigned long lastHT=0;
unsigned long lastSE=0;
WiFiServer server(80);
float h;
float t;
char c;
//void printHT();
//int threshold=20;
//int volume;
//int led=13;
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
           //printHT();
           float h = dht.readHumidity();
  
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true) 
  Serial.println("Humidity: % ");
  Serial.println(h);
  
  
  Serial.println("Temperature: celsius");
  
  Serial.println(t);
   
  delay(3000);
  if(t>=30)
  
  {
    Serial.println("Sunny");
  }
   else
  {
    Serial.println("Cloudy");
  }
  if(h>=70)
  {
    Serial.println("rainy");
  }
  else
  {
    Serial.println("less chances of rain");
  }
  

   /*     if(millis()-(2*60*1000UL)>lastHT)
  {
    lastHT=millis();
    printHT();
     
  }
  if(millis()-(4*60*1000UL)>lastSE)
  {
    lastSE=millis();
    volume=analogRead(A0);
 if(volume>=threshold)
 {
  digitalWrite(led,HIGH);
  Serial.println("loud");
 }
 else
 {
  digitalWrite(led,LOW);
  Serial.println("quiet");
 }
 Serial.println(volume,DEC);

 delay(5000);
  
}  */


            
       
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("connection : close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head></head><body><h1>ESP8266 -Tempearature and Humidity</h1><h3>Temperature in celsius: ");
          client.println(t);
          client.println("*C</h3><h3>Humidity: ");
          client.println(h);
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
     
   
   }
   
}
/*void printHT()
{
  float h = dht.readHumidity();
  
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true) 
  Serial.println("Humidity: % ");
  Serial.println(h);
  
  
  Serial.println("Temperature: celsius");
  
  Serial.println(t);
   
  delay(3000);
  if(t>=30)
  
  {
    Serial.println("Sunny");
  }
   else
  {
    Serial.println("Cloudy");
  }
  if(h>=70)
  {
    Serial.println("rainy");
  }
  else
  {
    Serial.println("less chances of rain");
  }

}*/
  
    
  


