#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "SFE_BMP180.h"
#include "pressureCalculate.h"
#include "dht.h"
#define dht_apin A0 // Analog Pin sensor is connected to

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial gprsSerial(8, 13);

//SFE_BMP180 pressureSensor;
dht DHT;
// Ultrasonic   
const int trigPin = 9;
const int echoPin = 10;

int moisturePin = A1;


// defines variables
long duration;

// Relay
#define ledPin 7
int state = 0;



// Sensor Readings
double humidity;
double temperature;
double pressure;
double moisture;
int distance;


void setup(){
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Team Data Geeks");
  delay(2000);
  Serial.println("Connecting to Network...");
  gprsSerial.begin(9600);
  Serial.begin(9600);
 
  delay(2000);//Delay to let system boot
  
  Serial.println("Connected!...");
  gprsSerial.flush();
  Serial.flush();
  
  // attach or detach from GPRS service 
  gprsSerial.println("AT+CGATT?");
  delay(100);
  toSerial();


  // bearer settings
  gprsSerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(2000);
  toSerial();

  // bearer settings
  gprsSerial.println("AT+SAPBR=3,1,\"APN\",\"bsnlnet\"");
  delay(2000);
  toSerial();

  // bearer settings
  gprsSerial.println("AT+SAPBR=1,1");
  delay(2000);
  toSerial();

  
  pinMode(ledPin, OUTPUT);
  // Sets the trigPin as an Output
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  
  digitalWrite(ledPin, LOW);
  
//  if (pressureSensor.begin())
//    Serial.println("BMP180 init success");
//  else {
//    lcd.clear();
//    lcd.print("Pressure Sensor");
//    lcd.setCursor(0,1);
//    lcd.print("Failed, Restart Pls");
//    Serial.println("BMP180 init fail\n\n");
//    while(1); // Pause forever.
//  } 
}

void loop(){
 initHttp();
    DHT.read11(dht_apin);
    humidity = DHT.humidity;
    temperature = DHT.temperature;  
//    delay(1000);  //Wait 1 seconds before accessing sensor again.
    moisture = analogRead(moisturePin);
    moisture = ( 100 - ( (moisture/1023.00) * 100 ) );
    // print out the value you read:
    //Ultrasonic
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration*0.343/2;
    pressure = 998.0;
//    pressure = calculatePressure(pressureSensor);
    
  lcd.clear();
  lcd.print("Team Data Geeks");
  delay(2000);
  lcd.clear();
  lcd.print("Humidity : "); lcd.print(humidity);
  lcd.setCursor(0, 1);
  lcd.print("temperature : ");  lcd.print(temperature); lcd.print("C  ");
  delay(2000);
  lcd.clear();
  lcd.print("Moisture : ");  lcd.print(moisture);
  lcd.setCursor(0, 1);
  lcd.print("Rainfall : ");  lcd.print(distance);
  delay(2000);
  lcd.clear();
  lcd.print("Pressure : ");  lcd.print(pressure);
  delay(2000);

    Serial.print("Current humidity = ");
    Serial.print(humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(temperature); 
    Serial.println("C  ");
    Serial.println("MOISTURE");
    Serial.println(moisture);
    Serial.print("Distance: ");
    Serial.println(distance);
    Serial.print("Pressure: ");
    Serial.println(pressure);

apiCall();
    
    
    ///////////////////////////////////////////////////////
    if(Serial.available() > 0){ // Checks whether data is comming from the serial port
        state=Serial.read(); // Reads the data from the serial port
     }
     if (state == '0') {
      digitalWrite(ledPin, LOW); // Turn LED OFF
      Serial.println("LED: OFF"); // Send back, to the phone, the String "LED: ON"
      state = 0;
     }
     else if (state == '1') {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED: ON");
      state = 0;
     } 
    
}

void initHttp(){
   gprsSerial.println("AT+HTTPINIT");
   delay(2000); 
   toSerial();
}

void apiCall(){

  char url[250];
  char hum[10];
  char tem[10];
  char pres[10];
  char mois[10];
  dtostrf( humidity,5, 2, hum);
  dtostrf( temperature,5, 2,tem );
  dtostrf( pressure,6, 2,pres );
  dtostrf( moisture,5, 2,mois );
  
  sprintf(url,"AT+HTTPPARA=\"URL\",\"http://irrigation-syytem.000webhostapp.com/write_file.php?humidity=%s&temperature=%s&pressure=%s&moisture=%s&distance=%d\"", hum,tem,pres,mois,distance);
  Serial.println(url);
//  Serial.println("AT+HTTPPARA=\"URL\",\"http://irrigation-syytem.000webhostapp.com/write_file.php?humidity=48.00&temperature=24.00&pressure=945.42&moisture=986&distance=248\"");
   // set http param value
//   gprsSerial.println("AT+HTTPPARA=\"URL\",\"http://irrigation-syytem.000webhostapp.com/write_file.php?humidity=48.00&temperature=24.00&pressure=945.42&moisture=986&distance=248\"");
   gprsSerial.println(url);
   delay(2000);
   toSerial();

   // set http action type 0 = GET, 1 = POST, 2 = HEAD
   gprsSerial.println("AT+HTTPACTION=0");
   delay(6000);
   toSerial();

   // read server response
   gprsSerial.println("AT+HTTPREAD"); 
   delay(1000);
   toSerial();

   gprsSerial.println("");
   gprsSerial.println("AT+HTTPTERM");
   toSerial();
   delay(300);

   gprsSerial.println("");
   delay(10000);
}

void toSerial(){
  while(gprsSerial.available()!=0){
    Serial.write(gprsSerial.read());
  }
}

