#include <SoftwareSerial.h>
SoftwareSerial sim(10, 11);
int _timeout;
String _buffer;
String numberb = "+639750236694"; //-> change with your number
String numberc = "+639513080377"; //-> change with your number

#include "DHT.h"
#define DHTPIN 4    
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const int ledpinred=13; 
const int ledpingreen=12;// ledpin,flamepin and buzpin are not changed throughout the process
const int flamepin=A2;
const int buzpin=6;
const int smoke = A0;
const int threshold=200;// sets threshold value for flame sensor
const int smokethres=700;
//int flamesensvalue=0; // initialize flamesensor reading
int analogInput = A1;
float vout = 0.0;
float vin = 0.0;
float R1 = 30000.0; //  
float R2 = 7500.0; // 
int value = 0;



void setup() {
 Serial.println(F("DHTxx test!"));
 dht.begin();
 delay(3000);
 pinMode(smoke, INPUT);
 pinMode(ledpinred,OUTPUT);
 pinMode(ledpingreen,OUTPUT);
 pinMode(flamepin,INPUT);
 pinMode(buzpin,OUTPUT);
 pinMode(analogInput, INPUT);
 _buffer.reserve(50);
 sim.begin(9600);
 Serial.begin(9600);



}

void loop() {

int analogSensor = analogRead(smoke);
int flamesensvalue=analogRead(flamepin); // reads analog data from flame sensor
 value = analogRead(analogInput);
   vout = (value * 5.0) / 1024.0; 
   vin = vout / (R2/(R1+R2)); 


if(flamesensvalue <= threshold){
  digitalWrite(ledpinred,HIGH);
  tone(buzpin,3000);
  SendMessage();
}

  
 if(analogSensor > smokethres){
  digitalWrite(ledpinred,HIGH);
  tone(buzpin,3000);
  SendMessagea();
}
else{
 noTone(buzpin);
 digitalWrite(ledpinred,LOW);
 digitalWrite(ledpingreen,HIGH);

 
}

 
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  int t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  int f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }


 
 
  Serial.print(h);
  Serial.println(";");
  Serial.print(t);
  Serial.println(";");
  Serial.print(f);
  Serial.println(";");
  Serial.print(flamesensvalue);
  Serial.println(";");
  Serial.print(analogSensor);
  Serial.println(";");
  Serial.print(vin,2);
  Serial.println(";");
  delay(3000);

 
}
void SendMessage()
{
  //Serial.println ("Sending Message");]]
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
 sim.println("AT+CMGS=\"" + numberb + "\"\r"); //Mobile phone number to send message
 delay(200);
 String SMS = "fire has been detected at 1234 san vicente west urdaneta city owner brian kenneth";
 sim.println(SMS);
 delay(100);
 sim.println((char)26);// ASCII code of CTRL+Z
 delay(200);
 _buffer = _readSerial();
 
}
 void SendMessagea()
{
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200); 
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + numberc + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "smoke has been detected at living room ";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = _readSerial();
  
}
String _readSerial() {
 _timeout = 0;
 while  (!sim.available() && _timeout < 12000  )
 {
 delay(13);
 _timeout++;
 }
 if (sim.available()) {
  return sim.readString();
}
}
