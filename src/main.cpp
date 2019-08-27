#include <Arduino.h>
#include "LowPower.h"

// All the mcufriend.com UNO shields have the same pinout.
// i.e. control pins A0-A4.  Data D2-D9.  microSD D10-D13.
// Touchscreens are normally A1, A2, D7, D6 but the order varies
//
// This demo should work with most Adafruit TFT libraries
// If you are not using a shield,  use a full Adafruit constructor()
// e.g. Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

//https://alexgyver.ru/lessons/arduino-libraries/

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define PORTRAIT 0
#define LANDSCAPE 1
#define PORTRAIT_REV 2
#define LANDSCAPE_REV 3
/*

10 ss
11 di
12 do
13 sck

*/
// my biggest screen is 320x480

//#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>
#include "GyverUART.h";
//#include <SD.h>
MCUFRIEND_kbv tft;
//#include <Adafruit_TFTLCD.h>
//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

//File myFile;

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <FreeDefaultFonts.h>

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

//void setup(void);
//void loop(void);

String utf8rus(String source)
{
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };

  k = source.length(); i = 0;

  while (i < k) {
    n = source[i]; i++;

    if (n >= 0xBF){
      switch (n) {
        case 0xD0: {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x2F;
          break;
        }
        case 0xD1: {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB7; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x6F;
          break;
        }
      }
    }
    m[0] = n; target = target + String(m);
  }
return target;
}

void showmsgXY(int x, int y, int sz, const GFXfont *f, int col,  const char *msg)
{
    int16_t x1, y1;
    uint16_t wid, ht;
    //tft.drawFastHLine(0, y, tft.width(), WHITE);
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(col);
    tft.setTextSize(sz);
    tft.print(msg);
}

long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(75); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}


int i=0;
int tw;
int th;
void setup(void) {
    //uart.begin(74880);
    uartBegin(); 
    //    tft.reset();                 //hardware reset
    uint16_t ID = tft.readID(); //
    uartPrintln(ID, HEX);
    tft.begin(ID);  // my is 9327
    //tft.cp437(true);
    tft.setRotation(LANDSCAPE);
    tft.fillScreen(BLACK);
    tft.setFont();
    tft.setTextSize(2);
    i=0;
    
// tw = tft.width();
// th = tft.height();
 uartPrintln(tw);
 uartPrintln(th);
 tft.drawFastHLine(0,0,399,RED) ;
 tft.drawFastHLine(0,239,399,RED) ;
 tft.drawFastVLine(0,0,239,RED) ;
 tft.drawFastVLine(399,0,239,RED) ;
/**/
i=1;
}


char cstr[80];
//char cstrp[16];
String inString="";
String g="";
void loop(void) {
    
  

  while (uartAvailable() > 0) {
  char inChar = uartRead();
    inString += inChar;

  if (inChar == '\n') {
    //uartPrintln(inString);
    switch (inString.charAt(0))
{
case '1':
    g=inString.substring(2,16);
    g.toCharArray(cstr, g.length());
    tft.fillRect(1,1, 95, 50, BLACK);//190
    showmsgXY(1, 51, 1, &FreeSevenSegNumFont,GREEN, cstr);
  break;
case '2':
    g=inString.substring(2,16);
    g.toCharArray(cstr, g.length());
    tft.fillRect(96,1, 95, 50, BLACK);
    showmsgXY(96, 51, 1, &FreeSevenSegNumFont,YELLOW, cstr);
  break;
case '3':
    g=inString.substring(2,16);
    g.toCharArray(cstr, g.length());
    tft.fillRect(1,52, 95, 50, BLACK);
    showmsgXY(1, 101, 1, &FreeSevenSegNumFont,RED, cstr);
  break;
case '4':
    g=inString.substring(2,16);
    g.toCharArray(cstr, g.length());
    tft.fillRect(96,52, 95, 50, BLACK);
    showmsgXY(96, 101, 1, &FreeSevenSegNumFont,CYAN, cstr);
  break;
case '5':
    g=inString.substring(2,80);
    g.toCharArray(cstr, g.length());
    tft.fillRect(1,99, 398, 22, BLACK);
    showmsgXY(2, 119, 1,&FreeSans12pt7b,GREEN, cstr);
  break; 
case '6':
    g=inString.substring(2,80);
    g.toCharArray(cstr, g.length());
    tft.fillRect(1,119, 398, 22, BLACK);
    showmsgXY(2, 139, 1,&FreeSans12pt7b,GREEN, cstr);
  break;            
default:
  break;
}
 inString = "";
 //uartPrintln(readVcc());
}
}

//LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
}

