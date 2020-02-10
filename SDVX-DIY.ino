/*
This code is written for the SDVX DIY tutorial at https://sdvx-diy.github.io/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "./src/Encoder/Encoder.h"
#include <Keyboard.h>
#include <Mouse.h>

/* Button Input Pins */
#define BTA_PIN 9
#define BTB_PIN 7
#define BTC_PIN 6
#define BTD_PIN 3
#define FxL_PIN 8
#define FxR_PIN 5
#define START_PIN 4

/* Software Debounce Interval */
#define DEBOUNCE 10

/* Encoder */
#define ENC_1_PIN_A 15
#define ENC_1_PIN_B 14
#define ENC_2_PIN_A 16
#define ENC_2_PIN_B 10

/* Encoder */
Encoder encLeft(ENC_1_PIN_A, ENC_1_PIN_B);
Encoder encRight(ENC_2_PIN_A, ENC_2_PIN_B);

/* Button */
unsigned int buttonPin[7] = {BTA_PIN, BTB_PIN, BTC_PIN, BTD_PIN, FxL_PIN, FxR_PIN, START_PIN};
unsigned long keyTimer[7] = {0, 0, 0, 0, 0, 0, 0};
bool buttonState[7];
bool switchType[7] = {true, true, true, true, true, true, true};
char asciiKey[7] = {0x73, 0x64, 0x6B, 0x6C, 0x63, 0x6D, 0x0a};
/* Startup Loop */
void setup() {
  Keyboard.begin();
  Mouse.begin();
  initPin(BTA_PIN);
  initPin(BTB_PIN);
  initPin(BTC_PIN);
  initPin(BTD_PIN);
  initPin(FxL_PIN);
  initPin(FxR_PIN);
  initPin(START_PIN);
}

/* Main Loop */
void loop() {
  checkAllKeyEvents();
  encFuncLeft();
  encFuncRight();
}

void initPin(int pin) {
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH);
}

void checkAllKeyEvents(){
  for(int i = 0; i < sizeof(buttonPin) / 2; i++){
    if(switchType[i] == true){
      if(digitalRead(buttonPin[i]) == LOW && buttonState[i] == false){
        Keyboard.press(asciiKey[i]);
        buttonState[i] = true;
        keyTimer[i] = millis();
      }
      else if(digitalRead(buttonPin[i]) == HIGH && buttonState[i] == true && millis() - keyTimer[i] > DEBOUNCE){
        Keyboard.release(asciiKey[i]);
        buttonState[i] = false;
      }
    }
    else{
      if(digitalRead(buttonPin[i]) == HIGH && buttonState[i] == false){
        Keyboard.press(asciiKey[i]);
        buttonState[i] = true;
        keyTimer[i] = millis();
      }
      else if(digitalRead(buttonPin[i]) == LOW && buttonState[i] == true && millis() - keyTimer[i] > DEBOUNCE){
        Keyboard.release(asciiKey[i]);
        buttonState[i] = false;
      }
    }
  }
}

void encFuncLeft(){
  updateMousePositionLeft();
  encLeft.write(0);
}

void encFuncRight(){
  updateMousePositionRight();
  encRight.write(0);
}

void updateMousePositionLeft(){
  Mouse.move(encLeft.read(), 0, 0);
}

void updateMousePositionRight(){
  Mouse.move(0, encRight.read(), 0);
}
