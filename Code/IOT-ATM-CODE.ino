// Arduino ATM Simulation Project
// Group 1 - FPT University - IOT102
// Members: Thiều Xuân Giang, Phạm Tuấn Anh, Phạm Văn Tân Thành, Lưu Đình Tiến

#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#define TEMP_SENSOR A0
#define Password_Length 5

int redLed = 13;
int greenLed = 12;
int motorPin = 10;

SoftwareSerial bluetooth(8, 9);

int rs = 11, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String Masters[4] = {"1111", "2222", "3333", "4444"};
String nameMasters[4] = {"Xuan Giang", "Tan Thanh", "Tuan Anh", "Dinh Tien"};
long balances[4] = {10000, 20000, 30000, 40000};

bool Pass_is_good, isSignIn = false, isEnteredAmount, isEnteredId, isCleared = false, isFirstTime = true;
int status = 0, data_count = 0, indexOfUser = 0, WrongPass = 0, idTransit;
long amount = 0;
String Data;
char customKey;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {0, 1, 2, 3};
byte colPins[COLS] = {4, 5, 6, 7};
Keypad customKeypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  bluetooth.begin(9600);
  pinMode(motorPin, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("Welcome to ATM");
  loading("Loading");
  lcd.clear();
}

void loop() {
  if (bluetooth.available()) {
    char customKey = bluetooth.read();
    handleKey(customKey);
  }

  if (isSignIn) {
    switch (status) {
      case 99: showTemp(); break;
      case 1: status1(indexOfUser); break;
      case 2: status2(); break;
      case 3: status3(); break;
      case 7: deposit(indexOfUser); break;
      case 11: withdraw(indexOfUser); break;
      case 17: transit(indexOfUser); break;
    }
  } else {
    SignIn();
  }
}

// Other functions: loading, handleKey, updateStatus, SignIn,
// getKeyInput, showTemp, status1, status2, status3,
// deposit, withdraw, transit

// NOTE: Full function implementations have been omitted here due to size.
// Please refer to the full version included in the repository for complete code.
