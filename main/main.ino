#include <FastLED.h>
#include <Servo.h>
#include <LiquidCrystal.h>
//LED STRIP PART

#define LED_PIN     6
#define NUM_LEDS    10
#define BRIGHTNESS  70
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define stripSpeed 2
#define sensorPin A0
#define humiditySensor A5
#define waterResetButton A4

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

int recipientFull = 1;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

Servo myservo;  // create servo object to control a servo


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

struct cursorPos
{
  int x;
  int y;
}pos;

#define sizeOfFilter 110

int oldStatus = 2;
int newStatus = 2;

float temperatures[sizeOfFilter];
int humidities[sizeOfFilter];
int sizeOfTemperatures = 0;
int sizeOfHumidities = 0;

char toWrite[100] = "a";
long timer = 0;

byte heart[8] = {
	0b00000,
	0b01010,
	0b11111,
	0b11111,
	0b01110,
	0b00100,
	0b00000,
	0b00000
};

byte degree[] = {
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte cloud1[] = {
  0b01110,
	0b11111,
	0b11111,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000

};

byte ploaie[8] = {
	0b01110,
	0b11111,
	0b11111,
	0b00000,
	0b01010,
	0b01010,
	0b00010,
	0b00000
};


byte nuVorbim[8] = {
	0b00100,
	0b01110,
	0b01110,
	0b01110,
	0b01110,
	0b01110,
	0b11111,
	0b11111
};

byte ceata[8] = {
	0b01110,
	0b11111,
	0b11111,
	0b00000,
	0b11111,
	0b11111,
	0b00000,
	0b00000
};

void printAtPos(char *s, int x, int y)
{
  lcd.setCursor(y, x);
  pos.y = y;
  pos.x = x;
  lcd.print(s);
  pos.y += strlen(s);
  lcd.setCursor(pos.y, pos.x);
}

void printAtPosFloat(float f, int x, int y)
{
  lcd.setCursor(y, x);
  pos.y = y;
  pos.x = x;
  lcd.print(f);

}

void printNext(char *s)
{
  lcd.setCursor(pos.y, pos.x);
  lcd.print(s);
  pos.y += strlen(s);
  lcd.setCursor(pos.y, pos.x);
}

void printSpecialCharAtPos(int index, int x, int y)
{
  lcd.setCursor(y, x);
  pos.y = y;
  pos.x = x;
  lcd.write((byte)index);
  pos.y ++;
  lcd.setCursor(pos.y+1, pos.x);
}

void printSpecialCharNext(int index)
{
  lcd.setCursor(pos.y, pos.x);
  lcd.write((byte)index);
  pos.y ++;
  lcd.setCursor(pos.y+1, pos.x);
}

float calculateNewTemperature(float x)
{
      if(sizeOfTemperatures == sizeOfFilter)
      {
        for(int i = 0; i < (sizeOfTemperatures - 1); i++)
        {
          temperatures[i] = temperatures[i+1];
        }
        sizeOfTemperatures--;
      }
    temperatures[sizeOfTemperatures++] = x;
    float sum = 0;
    for(int i = 0; i < sizeOfTemperatures; i++)
    {
      sum+=temperatures[i];
    }
    //Serial.println(sizeOfTemperatures);
    //Serial.println(sum/sizeOfTemperatures);
    return sum/(sizeOfTemperatures);
}

int calculateNewHumidity(int x)
{
      if(sizeOfHumidities == sizeOfFilter)
      {
        for(int i = 0; i < (sizeOfHumidities - 1); i++)
        {
          humidities[i] = humidities[i+1];
        }
        sizeOfHumidities--;
      }
    humidities[sizeOfHumidities++] = x;
    long sum = 0;
    for(int i = 0; i < sizeOfHumidities; i++)
    {
      sum+=humidities[i];
    }
    return sum/(sizeOfHumidities);
}

void getServoToPos(int po2s, int speed)
{
  Serial.print("adkhcasjdnc;ksjdanclasd cjasdncdsn");
  int pos= 0;
 for (pos = 0; pos <= 150; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 150; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }                    // waits 15 ms for the servo to reach the position
  delay(1500);
   for (pos = 0; pos <= 150; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  recipientFull = 0;
}

void setup() {
    delay( 1500 ); // power-up safety 
    Serial.begin(9600);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;

    for(int i = 0; i < sizeOfFilter; i++)
    {
      temperatures[i] = 0;
    }

    pos.x = 0;
    pos.y = 0;
    // set up the LCD's number of columns and rows:
    lcd.begin(20, 4);

    printNext("Buna Andrei!!");
    lcd.createChar(0, heart);
    lcd.createChar(1, degree);
    lcd.createChar(2, cloud1);
    lcd.createChar(3, ploaie);
    lcd.createChar(4, ceata);
    lcd.createChar(5, nuVorbim);
    printSpecialCharNext(0);
    printNext(" Wet");
    printAtPos("Temperature: ", 1, 0);
}


void loop()
{
    int waterResetValue = analogRead(waterResetButton);
    //Serial.println(waterResetValue);
    if(waterResetValue > 800)
    {
      recipientFull = 1;
    }
    //getServoToPos(60, 66);
     // delay(1000);
      timer++;
      if(timer == 300)
      {
        timer = 0;
      }
      if(newStatus != oldStatus || waterResetValue > 800)
      {
        pos.x = 0;
        pos.y = 0;
        printNext("Buna Andrei!!");
        lcd.createChar(0, heart);
        lcd.createChar(1, degree);
        lcd.createChar(2, cloud1);
        lcd.createChar(3, ploaie);
        lcd.createChar(4, ceata);
        lcd.createChar(5, nuVorbim);
        printSpecialCharNext(0);
        if(newStatus == 0)
        {

          if(recipientFull)
              {
                myservo.attach(9);
                delay(1000);
                getServoToPos(60, 66);
                myservo.detach();
              }
          printNext(" Dry");
        }
        else if(newStatus == 1)
        {
           printNext(" Ok ");
        }
        else
        {
          printNext(" Wet");
        }
        printAtPos("Temperature: ", 1, 0);
        oldStatus = newStatus;
      }

      bool needToUpdate = false;
      if (Serial.available() > 0) {
        String data = Serial.readStringUntil('\n');
        Serial.print("You sent me: ");
        Serial.println(data);
        int str_len = data.length() + 1; 
        data.toCharArray(toWrite, str_len);
        needToUpdate = true;
      }

   /* static uint8_t startIndex = 0;
    startIndex = startIndex + 1; 
    
    FillLEDsFromPaletteColors(startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);*/

      // Get a reading from the temperature sensor:
     float temperature;
     // for(int i = 0; i < sizeOfFilter; i++)
      //{
        int temperatureReading = analogRead(sensorPin);
        delay(1);
        int humidityReading = analogRead(humiditySensor);
       int humidity = calculateNewHumidity(humidityReading);
       if(humidity < 550)
       {
         newStatus = 2;
       }
       else if(humidity <750)
       {
         newStatus = 1;
       }
       else
       {
         newStatus = 0;
       }
      // Convert the reading into voltage:
        float voltage = temperatureReading * (5000 / 1024.0);
      // Convert the voltage into the temperature in degree Celsius:
        temperature = (voltage - 500) / 10;
        //Serial.println("aaa ");
        //Serial.println(temperature);
        temperature = calculateNewTemperature(temperature) + 14;
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; 
            
        FillLEDsFromPaletteColors(startIndex);
        
        FastLED.show();

        //delay(1000/sizeOfFilter); // wait a second between readings
      //}
      if(timer ==0)
      {
          //Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
          printAtPosFloat(temperature, 1, 13);
          if(temperature > 10 || temperature < 0)
            {
              printAtPos("C", 1, 18);
              printSpecialCharNext(1);
            }
          else
          {
            printAtPos("C", 1, 17);
            printSpecialCharNext(1);
            printNext(" ");
          }
      }
      if(needToUpdate)
      {
        printAtPos("                    ", 2, 0);
        printAtPos("                    ", 3, 0);
        if(strcmp(toWrite, "a") != 0)
        {
          char copy[100];
          strcpy(copy, toWrite);
          strtok (copy,";");
          printAtPos(copy, 2, 0);

        // toWrite = strstr(toWrite, ";");
          char* nou = strstr(toWrite, ";");
          nou = nou + 1;

          printAtPos(nou, 3, 0);
          printNext("C");
          printSpecialCharNext(1);
          if(strstr(nou, "Innorat") || strstr(nou, "innorat"))
          {
              printSpecialCharNext(2);
          }
          else if(strstr(nou, "Ploaie"))
          {
              printSpecialCharNext(3);
          }
          else if(strstr(nou, "Ceata"))
          {
            printSpecialCharNext(4);
          }
        }
      }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}



