#include <SoftwareSerial.h>
SoftwareSerial gprsSerial(8, 13);
int humidity = 99;
void setup()
{
  gprsSerial.begin(9600);
  Serial.begin(9600);

  Serial.println("Config SIM900...");
  delay(2000);
  Serial.println("Done!...");
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
}


void loop()
{
   // initialize http service
   gprsSerial.println("AT+HTTPINIT");
   delay(2000); 
   toSerial();
   
   // set http param value
   String one = String("\"http://irrigation-syytem.000webhostapp.com/write_file.php?humidity=" + String(humidity) );
   String apiQuery = String(one + "\"");
   String AtQuery = String("AT+HTTPPARA=\"URL\"," + apiQuery);
//   Serial.println(url);
//   Serial.println(AtQuery);

  char url[180];
  sprintf(url,"AT+HTTPPARA=\"URL\",\"http://irrigation-syytem.000webhostapp.com/write_file.php?humidity=%d\"",humidity);
  Serial.println(url);

   
//   gprsSerial.println("AT+HTTPPARA=\"URL\",\"http://irrigation-syytem.000webhostapp.com/write_file.php?humidity=555\"");
   gprsSerial.println(url);
//   gprsSerial.print(humidity);
//   gprsSerial.print("\"");
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

    Serial.print("Api called");
   gprsSerial.println("");
   delay(10000);
   
}

void toSerial(){
  while(gprsSerial.available()!=0){
    Serial.write(gprsSerial.read());
  }
}



//
//###correction according to stackoverflow:::::####
//int readpin = 4;
//byte pinValue;
//
//void loop()
//{
//   // initialize http service
//   gprsSerial.println("AT+HTTPINIT");
//   delay(2000); 
//   toSerial();
//
//   pinValue = digitalread(readpin);
//
//   // set http param value
//   gprsSerial.println("AT+HTTPPARA=\"URL\",\"http://www.oniee.com/new.php?sensor1="" + pinValue + ""\"");
//   delay(2000);
//   toSerial();
//
//   ...
