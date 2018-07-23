#include <avr/sleep.h>
#include <DHT.h>
#define DHTPIN 3
#define DHTTYPE DHT11   
DHT dht(DHTPIN,DHTTYPE);
unsigned long lastHT=0;
unsigned long lastSE=0;
void printHT();
int led=13;
int threshold=20;
int volume;
// This library contains functions to set various low-power 
// states for the ATmega328

// This variable is made volatile because it is changed inside
// an interrupt function
volatile int sleep_count = 0; // Keep track of how many sleep
// cycles have been completed.
const int interval = 5; // Interval in minutes between waking
// and doing tasks.
const int sleep_total = (interval*60)/8; // Approximate number 
// of sleep cycles needed before the interval defined above 
// elapses. Not that this does integer math.

void setup(void) {
watchdogOn(); // Turn on the watch dog timer.

// The following saves some extra power by disabling some 
// peripherals I am not using.

// Disable the ADC by setting the ADEN bit (bit 7) to zero.
ADCSRA = ADCSRA & B01111111;

// Disable the analog comparator by setting the ACD bit
// (bit 7) to one.
ACSR = B10000000;

// Disable digital input buffers on all analog input pins
// by setting bits 0-5 to one.
DIDR0 = DIDR0 | B00111111;
}

void loop(void) {
goToSleep(); // ATmega328 goes to sleep for about 8 seconds
// and continues to execute code when it wakes up

if (sleep_count == sleep_total) {

// CODE TO BE EXECUTED PERIODICALLY
if(millis()-(10*60*1000UL)>lastHT)
  {
    lastHT=millis();
    printHT();
  }
  if(millis()-(5*60*1000UL)>lastSE)
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
 
}
     
    
}

}

void goToSleep()   
{
// The ATmega328 has five different sleep states.
// See the ATmega 328 datasheet for more information.
// SLEEP_MODE_IDLE -the least power savings 
// SLEEP_MODE_ADC
// SLEEP_MODE_PWR_SAVE
// SLEEP_MODE_STANDBY
// SLEEP_MODE_PWR_DOWN -the most power savings
// I am using the deepest sleep mode from which a
// watchdog timer interrupt can wake the ATMega328

set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode.
sleep_enable(); // Enable sleep mode.
sleep_mode(); // Enter sleep mode.
// After waking from watchdog interrupt the code continues
// to execute from this point.

sleep_disable(); // Disable sleep mode after waking.
                     
}

void watchdogOn() {
  
// Clear the reset flag, the WDRF bit (bit 3) of MCUSR.
MCUSR = MCUSR & B11110111;
  
// Set the WDCE bit (bit 4) and the WDE bit (bit 3) 
// of WDTCSR. The WDCE bit must be set in order to 
// change WDE or the watchdog prescalers. Setting the 
// WDCE bit will allow updtaes to the prescalers and 
// WDE for 4 clock cycles then it will be reset by 
// hardware.
WDTCSR = WDTCSR | B00011000; 

// Set the watchdog timeout prescaler value to 1024 K 
// which will yeild a time-out interval of about 8.0 s.
WDTCSR = B00100001;

// Enable the watchdog timer interupt.
WDTCSR = WDTCSR | B01000000;
MCUSR = MCUSR & B11110111;

}

ISR(WDT_vect)
{
sleep_count ++; // keep track of how many sleep cycles
// have been completed.
}
void printHT()
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

}

