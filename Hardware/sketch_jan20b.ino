#include <LiquidCrystal.h>               //Load Liquid Crystal Library
LiquidCrystal LCD(12, 11, 5, 4, 3, 2);      //Create Liquid Crystal Object called LCD

void setup() {
 
LCD.begin(16,2);             //Tell Arduino to start your 16 column 2 row LCD
LCD.setCursor(0,0);                 //Set LCD cursor to upper left corner, column 0, row 0
LCD.print("INNOVACION");                   //Print Message on First Row
}
void loop() 
{
}  

