#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <DHT.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
#define DHTPIN 3
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);
#define LED_PIN (13)
unsigned long lastHT=0;
unsigned long lastSE=0;
void printHT();
int led=13;
int threshold=20;
int volume;

volatile int f_wdt=1;



/***************************************************
 *  Name:        ISR(WDT_vect)
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Watchdog Interrupt Service. This
 *               is executed when watchdog timed out.
 *
 ***************************************************/
ISR(WDT_vect)
{
  if(f_wdt == 0)
  {
    f_wdt=1;
  }
  
}


/***************************************************
 *  Name:        enterSleep
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Enters the arduino into sleep mode.
 *
 ***************************************************/
void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  sleep_enable();
  
  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  
  /* Re-enable the peripherals. */
  power_all_enable();
}



/***************************************************
 *  Name:        setup
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Setup for the serial comms and the
 *                Watch dog timeout. 
 *
 ***************************************************/
void setup()
{
  Serial.begin(9600);
  Wire.begin();
    RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  Serial.println("Initialising...");
  delay(100); //Allow for serial print to complete.

  pinMode(LED_PIN,OUTPUT);

  /*** Setup the WDT ***/
  
  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */
  
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
  
  Serial.println("Initialisation complete.");
  delay(100); //Allow for serial print to complete.
  Serial.println("Temperature,humidity and sound");
}



/***************************************************
 *  Name:        enterSleep
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Main application loop.
 *
 ***************************************************/
void loop()
{
  if(f_wdt == 1)
  {
    /* Toggle the LED */
     if(millis()-(2*60*1000UL)>lastHT)
  {
    lastHT=millis();
    DateTime now = RTC.now(); 
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println(); 
    delay(1000);
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
  }
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));

    
 
    /* Don't forget to clear the flag. */
    f_wdt = 0;
    
    // Re-enter sleep mode
    enterSleep();
  }
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


