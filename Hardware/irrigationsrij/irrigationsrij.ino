#include <LiquidCrystal.h>
#include "SFE_BMP180.h"
#include "pressureCalculate.h"
#include "dht.h"
#define dht_apin A0 // Analog Pin sensor is connected to

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

SFE_BMP180 pressureSensor;
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
int moisture;
int distance;


void setup(){
  
  pinMode(ledPin, OUTPUT);
  // Sets the trigPin as an Output
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  
  digitalWrite(ledPin, LOW);
  
  Serial.begin(9600);
  delay(500);//Delay to let system boot
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(500);//Wait before accessing Sensor


  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  
  if (pressureSensor.begin())
    Serial.println("BMP180 init success");
  else {
    lcd.clear();
    lcd.print("Pressure Sensor");
    lcd.setCursor(0,1);
    lcd.print("Failed, Restart Pls");
    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  } 
}

void loop(){
 
    DHT.read11(dht_apin);
    humidity = DHT.humidity;
    temperature = DHT.temperature;  
//    delay(1000);  //Wait 1 seconds before accessing sensor again.
    moisture = analogRead(moisturePin);
    // print out the value you read:
    //Ultrasonic
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration*0.034/2;
    pressure = calculatePressure(pressureSensor);
    
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
  lcd.print("Distance : ");  lcd.print(distance);
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
