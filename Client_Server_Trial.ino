#include <DNSServer.h>

#include <dht.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
dht DHT; 
#define DHT11_PIN D3 
const char* ssid = "V2s"; 
const char* password = "sridevsu"; 
const char* mqtt_server = "broker.mqtt-dashboard.com"; 
WiFiClient espClient; 
PubSubClient client(espClient); 
long lastMsg = 0;
char msg[50];
int value = 0;
void setup_wifi() 
{    
  delay(100);   // We start by connecting to a WiFi network     
  Serial.print("Connecting to ");    
  Serial.println(ssid);     
  WiFi.begin(ssid, password);    
  while (WiFi.status() != WL_CONNECTED)     
  {
    delay(500);      
    Serial.print("."); 
  }    
    randomSeed(micros());  
  Serial.println("");   
  Serial.println("WiFi connected");   
  Serial.println("IP address: ");   
  Serial.println(WiFi.localIP()); 
  }

void callback(char* topic, byte* payload, unsigned int length)
{   
  Serial.print("Command is : [");   
  Serial.print(topic);  
  int p =(char)payload[0]-'0';   
  int chk = DHT.read11(DHT11_PIN);   // if MQTT comes a 0 message, show humidity   
  if(p==0)   
  { 
    Serial.println("to show humidity!]");     
    Serial.print(" Humidity is: " );     
    Serial.print(DHT.humidity, 1);     Serial.println('%');   
    }   // if MQTT comes a 1 message, show temperature  
    if(p==1)   
    {     
      Serial.println(" is to show temperature!] ");   
      int chk = DHT.read11(DHT11_PIN);   
      Serial.print(" Temp is: " );    
      Serial.print(DHT.temperature, 1);    
      Serial.println(' C');  
      }   
      Serial.println(); 
      } 

void reconnect() 
{   
  while (!client.connected())  
  {    
    Serial.print("Attempting MQTT connection...");     
    String clientId = "ESP8266Client-";    
    clientId += String(random(0xffff), HEX);       
    if (client.connect(clientId.c_str()))     
    {       
      Serial.println("connected");
     client.subscribe("OsoyooCommand");    
     } 
     else
     {       
      Serial.print("failed, rc=");      
      Serial.print(client.state());       
      Serial.println(" try again in 5 seconds");            
      delay(6000);     
      }   
      } 
      } //end reconnect()
     
void setup() 
{   
  Serial.begin(115200);   
  setup_wifi();   
  client.setServer(mqtt_server, 1883);   
  client.setCallback(callback);   
  int chk = DHT.read11(DHT11_PIN);   
  Serial.print(" Starting Humidity: " );   
  Serial.print(DHT.humidity, 1);   
  Serial.println('%');   
  Serial.print(" Starting Temparature ");   
  Serial.print(DHT.temperature, 1);   
  Serial.println('C'); 
  }
void loop() 
{   if (!client.connected()) 
{     
  reconnect();   
  }   
  client.loop();   
  long now = millis();     
  if (now - lastMsg > 6000) 
  {      
    lastMsg = now;      
    int chk = DHT.read11(DHT11_PIN);      
    String msg="real time temperature: ";      
    msg= msg+ DHT.temperature;      
    msg = msg+" C ;real time Humidity: " ;      
    msg=msg+DHT.humidity ;      
    msg=msg+"%";      
    char message[58];      
    msg.toCharArray(message,58);      
    Serial.println(message);           
    client.publish("OsoyooData", message);
}
}
