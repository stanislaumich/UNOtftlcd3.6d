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
/*

10 ss
11 di
12 do
13 sck

*/
// my biggest screen is 320x480

#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>
#include <SD.h>
MCUFRIEND_kbv tft;
//#include <Adafruit_TFTLCD.h>
//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

File myFile;

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

void printDirectory(File dir, int numTabs) {
  // Begin at the start of the directory
  dir.rewindDirectory();
  
  while(true) {
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');   // we'll have a nice indentation
     }
     // Print the 8.3 name
     Serial.print(entry.name());
     // Recurse for directories, otherwise print the file size
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

int i=0;
void setup(void) {
    Serial.begin(74880);
    //    tft.reset();                 //hardware reset
    uint16_t ID = tft.readID(); //
    Serial.print("ID = 0x");
    Serial.println(ID, HEX);
    tft.begin(ID);  // my is 9327
    //tft.cp437(true);
    tft.setRotation(LANDSCAPE);
    tft.fillScreen(BLACK);
    tft.setFont();
    tft.setTextSize(2);
    i=0;
    if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    //while (1);
  }
  Serial.println("initialization done.");
/**/
File root;
root = SD.open("/");
  
  printDirectory(root, 0);
  
  Serial.println("done!");

 myFile = SD.open("test.txt", FILE_WRITE);
 if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }


/**/

}


char cstr[16];
String s="";

void loop(void) {
    //myfunc1();
    //delay(1000);
    //tft.fillScreen(BLACK);delay(100);
    //tft.fillRect(0,0, 400, 50, RED);delay(100);
    //delay(1000);
    //myfunc3();
    //delay(10000);
  /*  int maxscroll=15;*/
  if (Serial.available() > 0) {  //если есть доступные данные
        // считываем байт
        s=Serial.readString();     
    }
   
  //sprintf(cstr, "%04d", 0);
   //хороший рабочий кусок
  
   tft.fillRect(0,0, 95, 50, BLACK);
   sprintf(cstr, "%03d", i);
   showmsgXY(0, 50, 1, &FreeSevenSegNumFont,GREEN, cstr);
   tft.setFont();
   tft.setTextSize(2);
   //tft.print(utf8rus("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЪЫЭЮЯ"));
   tft.println();  
   tft.fillRect(0,51, 400, 25, BLACK);
   tft.println(s);
   i++;
   //tft.println(utf8rus("ДОКТОР БОРОДАТЫЙ ПО ЛЕСУ ИДЁТ,"));
   //tft.println();
  // tft.println(utf8rus("ШИШКИ СОБИРАЕТ, объяснительную пишет"));
   //if (i==0){
   //showmsgXY(0, 100, 1, &FreeSans12pt7b,GREEN, "abcdefghijklmnopqrstuvwxyz");
   //showmsgXY(0, 125, 1, &FreeSans12pt7b,GREEN, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
   //delay(50);
   //}
   if(i==1000) i=0;
   

}

