#include <avr/sleep.h>
#include <avr/power.h>
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

#define LED_PIN (13)

volatile int f_timer=0;



/***************************************************
 *  Name:        ISR(TIMER1_OVF_vect)
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Timer1 Overflow interrupt.
 *
 ***************************************************/
ISR(TIMER1_OVF_vect)
{
  /* set the flag. */
   if(f_timer == 0)
   {
     f_timer = 1;
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
  set_sleep_mode(SLEEP_MODE_IDLE);
  
  sleep_enable();


  /* Disable all of the unused peripherals. This will reduce power
   * consumption further and, more importantly, some of these
   * peripherals may generate interrupts that will wake our Arduino from
   * sleep!
   */
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer2_disable();
  power_twi_disable();  

  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the timer timeout*/
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
 *                timer. 
 *
 ***************************************************/
void setup()
{
  Serial.begin(9600);
  
  /* Don't forget to configure the pin! */
  pinMode(LED_PIN, OUTPUT);

  /*** Configure the timer.***/
  
  /* Normal timer operation.*/
  TCCR1A = 0x00; 
  
  /* Clear the timer counter register.
   * You can pre-load this register with a value in order to 
   * reduce the timeout period, say if you wanted to wake up
   * ever 4.0 seconds exactly.
   */
  TCNT1=0x0000; 
  
  /* Configure the prescaler for 1:1024, giving us a 
   * timeout of 4.09 seconds.
   */
  TCCR1B = 0x05;
  
  /* Enable the timer overlow interrupt. */
  TIMSK1=0x01;
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
  if(f_timer==1)
  {
    f_timer = 0;
    /* Toggle the LED */
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
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
     
    
    /* Re-enter sleep mode. */
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

