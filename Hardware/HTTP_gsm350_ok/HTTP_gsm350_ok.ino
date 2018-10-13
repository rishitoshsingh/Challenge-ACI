#include <SoftwareSerial.h>
SoftwareSerial myGsm(7,8);
float temp;
int tempPin = 0;
void setup()
{
 myGsm.begin(9600);  
 Serial.begin(9600);  
 delay(500);

myGsm.println("AT+CIPSHUT");
 delay(1000);
 printSerialData();

 myGsm.println("AT+CIPMUX=0");
 delay(2000);
 printSerialData();
 
 myGsm.println("AT+CGATT=1");
 delay(1000);
 printSerialData();
 
  
 myGsm.println("AT+CSTT=\"bsnlnet\",\"\",\"\"");//setting the APN,2nd parameter empty works for all networks 
 delay(5000);
 printSerialData();
 
 myGsm.println();
 myGsm.println("AT+CIICR");
 delay(6000);
 printSerialData();
 
 myGsm.println("AT+CIFSR"); //init the HTTP request
 delay(2000); 
 printSerialData();
 
 myGsm.println("AT+CIPSTART=\"TCP\",\"122.178.80.228\",\"350\"");
 delay(5000);
 printSerialData();
 delay(5000);
 
 myGsm.println("AT+CIPSEND");
 delay(2000);
 printSerialData();
 
sendtemp();
delay(3000);
myGsm.println("AT+CIPCLOSE");
printSerialData();

myGsm.println("AT+CIPSHUT");
delay(1000);
printSerialData();
}

void loop()
{
}


void printSerialData()
{
 while(myGsm.available()!=0)
 Serial.write(myGsm.read());
}

void sendtemp()
{
  temp = analogRead(tempPin);
  temp = temp * 0.48828125;
  Serial.print("TEMPERATURE = ");
  Serial.print(temp);
  Serial.print("*C");
  Serial.println();
  delay(5000);
 myGsm.println(temp);
 delay(3000);
 printSerialData();
 myGsm.write(0x1A);
 delay(3000);
   printSerialData();
   
   
}




