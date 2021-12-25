/* Send the current temperature via sms
   Made by Umutcan Genç            */
#include<avr/io.h>  
#include<stdio.h>
#include<string.h>
#include<SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

float value;
int temperature;
char message[120];

void Gsm_init();
void ADC_init();
unsigned int ADC_read();
void Gsm_Sendmsg(char*);

void setup() {
 DDRC=0x00;    //Set PORTC for input
 DDRB=0xFF;    //Set PORTB for output
 mySerial.begin(9600); //İnitialize uart communication
 delay(100);

 Gsm_init();   //Start Gsm
 delay(200);
 ADC_init();   //Start ADC
 delay(200);
   
}
 void loop(){
   value=ADC_read();
   temperature = ((value/1023)*5000)/10;   //Calculate the temperature
   sprintf(message,"Current temperature = %d celsius,made by Umutcan Genc ", temperature);  //Strings transfer to array
   Gsm_Sendmsg(message);
   PORTB=0x04;   //Turn on blue led
   delay(2000);
   PORTB=0x00;   //Turn off all leds
   delay(3600000);  //delay 1 hour
   
 }  
 
 void Gsm_init(){
  mySerial.println("AT");  // Module check for initialization
  PORTB=0x02;             // Open red led
  delay(1000);
}

 void ADC_init()
{   
    DDRC=0x0;         //Set PORTC for input
    ADMUX =0x40;     //Avcc=Aref
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);    // ADC Enable and prescaler of 128
}

 unsigned int ADC_read()
{
   ADMUX = 0x40;        //channel A0 selected
   ADCSRA|=(1<<ADSC);   // start conversion
   while(!(ADCSRA & (1<<ADIF)));   // wait for ADIF conversion complete return
    ADCSRA|=(1<<ADIF);       // clear ADIF when conversion complete by writing 1
   return (ADC); //return calculated ADC value
}

 void Gsm_Sendmsg(char *message){
   mySerial.println("AT+CMGF=1");   // Set message format to text mode
   delay(1000);
   mySerial.println("AT+CMGS=\"905468088515\"\r");  //Send telephone number
   delay(500);
   mySerial.println(message);         //Send the message via uart communication
   delay(500);
   mySerial.println((char)26);     //Finish the message
   delay(1000);   
}

 
