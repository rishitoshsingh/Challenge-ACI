#include "dht.h"
#define dht_apin A0 // Analog Pin sensor is connected to
 
dht DHT;
// Ultrasonic   
const int trigPin = 9;
const int echoPin = 10;

// defines variables
long duration;
int distance;
 
// Relay
#define ledPin 7
int state = 0;



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
 
}

void loop(){
 
    DHT.read11(dht_apin);
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
    
    delay(1000);  //Wait 1 seconds before accessing sensor again.
     // read the input on analog pin 0:
    int sensorValue = analogRead(A1);
    // print out the value you read:
    Serial.println("MOISTURE");
    Serial.println(sensorValue);
    delay(1000); 
 
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distance= duration*0.034/2;

    Serial.print("Distance: ");
    Serial.println(distance);
    
    
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
