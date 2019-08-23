#include <Arduino.h>

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

// my biggest screen is 320x480

#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
//#include <Adafruit_TFTLCD.h>
//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

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
void printmsg(int row, const char *msg)
{
    tft.setTextColor(YELLOW, BLACK);
    tft.setCursor(0, row);
    tft.println(msg);
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
    delay(1000);
}

void show(const char *msg){
 tft.setFont(&FreeSevenSegNumFont);
 tft.setCursor(0, 50);
 tft.setTextSize(1);
 tft.setTextColor(GREEN);
 tft.print(msg);
}

void myfunc3 (void){
 //showmsgXY(0, 50, 1, &FreeSevenSegNumFont, "0:1-2+3=4ABC567890");
 //showmsgXY(0, 40, 2, &FreeSevenSegNumFont, "01234567890");
}

void myfunc2(void)
{
    //showmsgXY(20, 10, 1, NULL, "System x1");
    //showmsgXY(20, 24, 2, NULL, "System x2");
    //showmsgXY(20, 60, 1, &FreeSans9pt7b, "FreeSans9pt7b");
    //showmsgXY(20, 80, 1, &FreeSans12pt7b, "FreeSans12pt7b");
    //showmsgXY(20, 100, 1, &FreeSerif12pt7b, "FreeSerif12pt7b");
    //showmsgXY(20, 120, 1, &FreeSmallFont, "FreeSmallFont");
    //showmsgXY(5, 180, 1, &FreeSevenSegNumFont, "01234"); 

}

void myfunc1(void)
{
 //tft.setTextColor(GREEN);
 //tft.setCursor(0, 0);
 //tft.println(123.45);

 tft.setRotation(LANDSCAPE);
 tft.setTextSize(2);  
 tft.fillScreen(BLACK);
 printmsg(1, "HELLO!! its a vrvbncvbnbvbnvbnvnv ncv bn cvbn cvb n cvbn cvbn cvbn cvbn cvbncvb n cvbn cv bn cvbn cvb ncv bn cv bnc vb");
}



int i=0;
void setup(void) {
    Serial.begin(9600);
    //    tft.reset();                 //hardware reset
    uint16_t ID = tft.readID(); //
    Serial.print("ID = 0x");
    Serial.println(ID, HEX);
    tft.begin(ID);  // my is 9327
    tft.setRotation(LANDSCAPE);
    tft.fillScreen(BLACK);
    //show("5");
}
char cstr[16];


void loop(void) {
    //myfunc1();
    //delay(1000);
    //tft.fillScreen(BLACK);delay(100);
    //tft.fillRect(0,0, 400, 50, RED);delay(100);
    //delay(1000);
    //myfunc3();
    //delay(10000);
  
  //sprintf(cstr, "%04d", 0);

  for (int i=0;i<1000; i++){
   tft.fillRect(0,0, 95, 50, BLACK);
   sprintf(cstr, "%03d", i);
   showmsgXY(0, 50, 1, &FreeSevenSegNumFont,GREEN, cstr);
   tft.setFont();
   tft.setTextSize(1);
   tft.print(utf8rus("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЪЫЭЮЯ")); 
   if (i==0){
   showmsgXY(0, 100, 1, &FreeSans12pt7b,GREEN, "abcdefghijklmnopqrstuvwxyz");
   showmsgXY(0, 125, 1, &FreeSans12pt7b,GREEN, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
   //delay(50);
   }
  }

}

