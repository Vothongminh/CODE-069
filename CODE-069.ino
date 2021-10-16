//#VTM https://www.youtube.com/c/VTMVlogVoThongMinh
#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>     //Thư viện LiquidCrystal_I2C: https://github.com/ArduinoKIT/Luc-Nguyen/raw/master/LiquidCrystal_I2C-1.1.2.zip
LiquidCrystal_I2C lcd(0x27, 20, 4);
#define stepPin1 7
#define stepPin2 9
#define dirPin1 8
#define dirPin2 10
#define up A0
#define down A1
#define left A2
#define right A3
#define runn 2
#define CTHT 11
#define relay 3
#define outputA 4
#define outputB 5
#define sw 6
int counter = 0;
int aState;
int aLastState;
int a = EEPROM.read(0); //distance
double b = EEPROM.read(1); //speed
double c = EEPROM.read(2); //timer
int demSW = 0;
//...............................................
void setup() {
  Serial.begin(9600);
  pinMode(outputA , INPUT);
  pinMode(outputB , INPUT);
  pinMode(stepPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(up , INPUT_PULLUP);
  pinMode(down , INPUT_PULLUP);
  pinMode(left , INPUT_PULLUP);
  pinMode(right , INPUT_PULLUP);
  pinMode(runn , INPUT_PULLUP);
  pinMode(CTHT , INPUT_PULLUP);
  pinMode(sw , INPUT_PULLUP);
  digitalWrite(relay, HIGH);
  aLastState = digitalRead(outputA);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Speed   :");
  lcd.setCursor(15, 0);
  lcd.print("(v/s)");
  lcd.setCursor(0, 1);
  lcd.print("Distance:");
  lcd.setCursor(15, 1);
  lcd.print("(v)");
  lcd.setCursor(0, 2);
  lcd.print("Delay   :");
  lcd.setCursor(15, 2);
  lcd.print("(s)");
}
//...............................................
void loop() {
ok:
  lcd.setCursor(10, 0);
  lcd.print(b / 20);
  lcd.setCursor(10, 1);
  lcd.print(a / 2);
  lcd.setCursor(10, 2);
  lcd.print(c / 20);
  lcd.setCursor(0, 3);
  lcd.print("     ");
  if ((a < 0) || (b < 0) || (c < 0))
  {
    lcd.setCursor(15, 3);
    lcd.print("ERROR");
  }
  while (digitalRead(sw) == 0)
  {
    unsigned long time1 = millis();
    while (1) {
      if ((millis() - time1) > 3000) {
        a = 0;
        b = 0;
        c = 0;
        lcd.setCursor(15, 3);
        lcd.print("     ");
        goto ok;
      }
      if (digitalRead(sw) == 1)
      {
        readVR();
        goto ok;
      }
    }
  }
  if (digitalRead(left) == 0) {
    digitalWrite(dirPin2, HIGH);
    lcd.setCursor(0, 3);
    lcd.print("LEFT");
    trucY();
  }
  if (digitalRead(right) == 0) {
    digitalWrite(dirPin2, LOW);
    lcd.setCursor(0, 3);
    lcd.print("RIGHT");
    trucY();
  }
  if (digitalRead(up) == 0) {
    digitalWrite(dirPin1, LOW);
    lcd.setCursor(0, 3);
    lcd.print("UP  ");
    trucZ();
  }
  if (digitalRead(down) == 0) {
    digitalWrite(dirPin1, HIGH);
    lcd.setCursor(0, 3);
    lcd.print("DOWN");
    trucZ();
  }
  if (digitalRead(runn) == 0) {
    lcd.setCursor(0, 3);
    lcd.print("START");
    Serial.println("start");
    runZ();
  }
}
//...............................................
void readVR() {
  while (1) {
    lcd.setCursor(9, 3);
    lcd.print("  ");
    lcd.setCursor(8, 0);
    lcd.print(">");
    counter = b;
    delay(500);
    while (1) {
      aState = digitalRead(outputA);
      if (aState != aLastState)
      {
        if (digitalRead(outputB) != aState) {
          counter++;
        }
        else {
          counter--;
        }
        //Serial.println(counter);
        if (counter > 198 ) {
          counter = 198;
        }
        if (counter < 2 ) {
          counter = 2;
        }
        b = counter;
        lcd.setCursor(10, 0);
        lcd.print(b / 20);
      }
      aLastState = aState;
      if (digitalRead(sw) == 0) {
        goto a;
      }
    }
  }
a:
  while (1) {
    lcd.setCursor(8, 0);
    lcd.print(":");
    lcd.setCursor(8, 1);
    lcd.print(">");
    counter = a;
    delay(500);
    while (1) {
      aState = digitalRead(outputA);
      if (aState != aLastState)
      {
        if (digitalRead(outputB) != aState) {
          counter++;
        }
        else {
          counter--;
        }
        //Serial.println(counter);
        if (counter > 198 ) {
          counter = 198;
        }
        if (counter < 2 ) {
          counter = 2;
        }
        a = counter;
        lcd.setCursor(10, 1);
        lcd.print(a / 2);
      }
      aLastState = aState;
      if (a < 20) {
        lcd.setCursor(11, 1);
        lcd.print(" ");
      }
      if (digitalRead(sw) == 0) {
        goto b;
      }
    }
  }
b:
  while (1) {
    lcd.setCursor(8, 1);
    lcd.print(":");
    lcd.setCursor(8, 2);
    lcd.print(">");
    counter = c;
    delay(500);
    while (1)
    {
      aState = digitalRead(outputA);
      if (aState != aLastState)
      {
        if (digitalRead(outputB) != aState) {
          counter++;
        }
        else {
          counter--;
        }
        //Serial.println(counter);
        if (counter > 100 ) {
          counter = 100;
        }
        if (counter < 0 ) {
          counter = 0;
        }
        c = counter;
        lcd.setCursor(10, 2);
        lcd.print(c / 20);
      }
      aLastState = aState;
      if (digitalRead(sw) == 0) {
        goto c;
      }
    }
  }
c:;
  EEPROM.write(0, a);
  EEPROM.write(1, b);
  EEPROM.write(2, c);
  lcd.setCursor(8, 2);
  lcd.print(":");
  lcd.setCursor(9, 3);
  lcd.print("OK");
  delay(500);
}
//...............................................
void trucY() {
  while ((digitalRead(left) == 0) || (digitalRead(right) == 0)) {
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(500);
  }
}
//...............................................
void trucZ() {
  while ((digitalRead(up) == 0) || (digitalRead(down) == 0)) {
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(500);
  }
}
//...............................................
void runZ() {
  digitalWrite(dirPin1, HIGH);
  for (int x = 0; x < a / 2; x++) {
    for (int y = 0; y < 1600; y++) {
      digitalWrite(stepPin1, HIGH);
      delayMicroseconds(6250 / b);
      digitalWrite(stepPin1, LOW);
      delayMicroseconds(6250 / b);
      if (digitalRead(CTHT) == 0) {
        int x1 = x;
        int y1 = y;
        digitalWrite(relay, LOW);
        delay(c * 50);
        digitalWrite(relay, HIGH);
        digitalWrite(dirPin1, LOW);
        for (int y = 0 ; y < y1; y++) {
          digitalWrite(stepPin1, HIGH);
          delayMicroseconds(6250 / b);
          digitalWrite(stepPin1, LOW);
          delayMicroseconds(6250 / b);
        }
        for (int x = 0; x < x1; x++) {
          for (int y = 0 ; y < 1600; y++) {
            digitalWrite(stepPin1, HIGH);
            delayMicroseconds(6250 / b);
            digitalWrite(stepPin1, LOW);
            delayMicroseconds(6250 / b);
          }
        }
        goto endd;
      }
    }
  }
  digitalWrite(relay, LOW);
  delay(c * 50);
  digitalWrite(relay, HIGH);
  digitalWrite(dirPin1, LOW);
  for (int x = 0; x < a / 2; x++) {
    for (int y = 0 ; y <= 1600; y++) {
      digitalWrite(stepPin1, HIGH);
      delayMicroseconds(6250 / b);
      digitalWrite(stepPin1, LOW);
      delayMicroseconds(6250 / b);
    }
  }
endd:;
}
