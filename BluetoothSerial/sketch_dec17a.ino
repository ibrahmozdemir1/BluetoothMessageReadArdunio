#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include <SoftwareSerial.h>
#include "Arial_black_16.h"
#include "Arial_Black_16_ISO_8859_1.h"
#include "Arial14.h"
#include "SystemFont5x7.h"

#define DISPLAYS_ACROSS 1 //-> Number of P10 panels used, side to side.
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

SoftwareSerial btSerial(0,1);
String message = "";

void ScanDMD() {
  dmd.scanDisplayBySPI();
}

void setup(void) {
  Timer1.initialize(5000);
  Timer1.attachInterrupt(ScanDMD);
  dmd.clearScreen(true);
  Serial.begin(19200);
  btSerial.begin(9600);
}

void loop(void) {
    // setup dmd font
    dmd.selectFont(Arial_Black_16);

    // read message from bt or get global value as default
    message = readMessageIfThereIsNewMessageOrGetDefault();
    Serial.println(message);
    // display message
    drawStringToLCD(message.c_str());
}

// draw given c-style string to LCD
void drawStringToLCD(char* text) {
    dmd.drawMarquee(text, strlen(text), (32 * DISPLAYS_ACROSS) - 1, 0);
    long timer = millis();
    boolean ret = false;
    int interval = 50;
    while(!ret) {
        if ((timer + interval) < millis()) {
            ret = dmd.stepMarquee(-1, 0);
            timer = millis();
        }
    }
}

// if bt is available, read message from it
// else, return global message variable because it is not changed
// and it should display the last message read
String readMessageIfThereIsNewMessageOrGetDefault() {
    if (btSerial.available() > 0) {
        return btSerial.readString();
    }
    return message;
}
