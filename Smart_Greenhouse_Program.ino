/*Libraries: 
https://github.com/adafruit/DHT-sensor-library
https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
*/

#include <stdio.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*
Takes values from Photoresistor - 
if hits certain threshold - send signal to led light strip to turn it on. 
When above the threshold - turns led lights off ---------THIS FUNCTIONALITY REMOVED----------

Takes values from photoresistor, moisture-, humidity- and temperature sensors and convert them 
to be easily understandable by humans and display them on a LCD screen.*/

//Define PINs
#define ledPin A0      //Led strip PIN
#define moistPin A1    //Moisture Sensor PIN
#define dhtPin A2      //Temp&Humidity PIN
#define lightPin 3    //Ambient Light Sensor PIN

//A4 and A5 for the LCD display

//For Temp&Humidity sensor
#define DHTTYPE DHT11        
DHT dht(dhtPin, DHTTYPE);

//Define LCD Pins(Correct HEX address, cols, rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);       

int fadeSpeed = 1000;                      //Led light fading speed when turned ON/OFF
int lBright = 0;                           //led brightness 



//Setup PINs & Components
void setup() {
    lcd.init();                //Start the LCD
    lcd.backlight();           //Turns on the LCD Screen backlight
//    lcd.clear();             // Clears the LCD Screen
    dht.begin();               //Start Temp&Humid sensor
    pinMode(ledPin, OUTPUT);
//  pinMode(fanPin, OUTPUT);    Enable, if fan is added
    pinMode(moistPin, OUTPUT);
    pinMode(lightPin, OUTPUT);

    Serial.begin(9600);
}

//Function to turn leds ON
void LedOn() {
    for (int i=0;i<256; i++) {
        analogWrite(ledPin, lBright);
        lBright +=1;                      
        delay(fadeSpeed);
    }
}

//Function to turn leds OFF
void LedOff() {
    for (int i=0;i<256; i++) {
        analogWrite(ledPin, lBright);
        lBright -= 1;  
        delay(fadeSpeed);
    }
}

void loop(){
  LCDLoop();
  lightLoop();
  delay(5000);
}

//LCD Screen Loop
void LCDLoop() {
    float humidValuef = dht.readHumidity();         //Reads Temp&Humidity sensor values
    float tempValuef = dht.readTemperature();       //& Turns them into decimal
    int humidValue;
    humidValue = (int)humidValuef;
    int tempValue;
    tempValue = (int)tempValuef;

    int moistValue = analogRead(moistPin);    //Sensor value ==> 0-100 Percent value                    
    float moistValuef = moistValue / 10;      //0-30% DRY
    int moistValuep;                          //30-70% MOIST
    moistValuep = (int)moistValuef;           //70+% WATERY

    lcd.setCursor(0, 0);             //Prints values/errors to LCD screen
    if (isnan(tempValuef)) {
        lcd.print("Failed");
    }
    else {
        lcd.print("T: ");
        lcd.print(tempValue);
        lcd.print((char)223);
        lcd.print("C");
    }                 


    lcd.setCursor(8, 0);
    if (isnan(humidValuef)) {
        lcd.print("Failed");
    }
    else {
        lcd.print("H: ");
        lcd.print(humidValue);
        lcd.print("%");
    }


    lcd.setCursor(0, 1);
    lcd.print("M: ");
    lcd.print(moistValuep);
    lcd.print(" %");
}

//Led light loop        ***NEED TO FIGURE OUT PHOTORESISTOR VALUES MANUALLY***
void lightLoop() {                                       
    int lightValue = analogRead(lightPin);

    Serial.println(lightValue);

    lcd.setCursor(8, 1);
    if (isnan(lightValue)) {
        lcd.print("Failed");
    }
    else {
        lcd.print("L: ");
        if(lightValue < 250){
          lcd.print("Low  ");
        }else if(lightValue >= 250 && lightValue < 400){
          lcd.print("Ideal");
        }else{
          lcd.print("High ");
        }

    }

/*                                      
    if(lightValue<200 && lBright==0)                //***Change Light threshold accordingly***
    {                                               //Turn led on if light threshold hit and
        LedOn();                                    //leds are off
        delay(3000);                               
    }
    if(lightValue>200 && lBright==255)      //***Change Light threshold accordingly***
    {                                       //Turns led off if light threshold is hit and                                      
        LedOff();                           //led brightness is max
        delay(3000);
    }
*/
}
  
