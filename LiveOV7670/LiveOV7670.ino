

// change setup.h to switch between buffered and pixel-by-pixel processing
#include<DHT11.h>
DHT11 dht(13);
#include <LiquidCrystal.h>
  LiquidCrystal lcd(0,11,10,9,8,1); 
  #include "setup.h"
void setup() {
 lcd.begin(16,2);
  // This is not necessary and has no effect for ATMEGA based Arduinos.
  // WAVGAT Nano has slower clock rate by default. We want to reset it to maximum speed
  CLKPR = 0x30; // enter clock rate change mode
  CLKPR = 0; // set prescaler to 0. WAVGAT MCU has it 3 by default.
   lcd.setCursor(0,0);
 float temp=dht.readTemperature();
 if(temp>27){
 lcd.print("Temperature: ");
 lcd.println(temp);
  lcd.setCursor(0, 1);
 lcd.print("Avoid the sun");
 }
 else lcd.print("have a nice day");
  initializeScreenAndCamera();
}
void loop() {
   processFrame();
   

}
