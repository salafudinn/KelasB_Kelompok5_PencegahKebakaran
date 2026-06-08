#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo servoFlame;
Servo servoPump;

const int pinFlame    = 4;
const int pinMQ2      = 34;
const int pinINA      = 25;
const int pinINB      = 26;
const int pinRelay    = 27;
const int pinBuzzer   = 14;
const int pinLedHijau = 18;
const int pinLedMerah = 19;
const int pinServo1   = 5;
const int pinServo2   = 23;

int servoPos            = 90;
int servoArah           = 1;
unsigned long lastServo = 0;
const int SERVO_STEP    = 5;
const int SERVO_SPEED   = 30;

void updateServoScanning(bool apiKetemu) {
  if (millis() - lastServo < SERVO_SPEED) return;
  lastServo = millis();
  if (apiKetemu) {
    servoFlame.write(servoPos);
    servoPump.write(servoPos);
  } else {
    servoPos += (servoArah * SERVO_STEP);
    if (servoPos >= 180) { servoPos = 180; servoArah = -1; }
    if (servoPos <= 0)   { servoPos = 0;   servoArah =  1; }
    servoFlame.write(servoPos);
    servoPump.write(servoPos);
  }
}

unsigned long lastBuzzer = 0;
bool buzzerState         = false;

void updateBuzzer(bool aktif) {
  if (!aktif) { digitalWrite(pinBuzzer, LOW); return; }
  if (millis() - lastBuzzer >= 150) {
    lastBuzzer  = millis();
    buzzerState = !buzzerState;
    digitalWrite(pinBuzzer, buzzerState ? HIGH : LOW);
  }
}

unsigned long lastLCD = 0;
String lastBaris1     = "";

void updateLCD(String baris1, String baris2) {
  if (millis() - lastLCD < 300) return;
  lastLCD = millis();
  if (baris1 != lastBaris1) {
    lastBaris1 = baris1;
    lcd.setCursor(0, 0); lcd.print(baris1);
    lcd.setCursor(0, 1); lcd.print(baris2);
  }
}

void setKondisiAman() {
  digitalWrite(pinINA,      LOW);
  digitalWrite(pinINB,      LOW);
  digitalWrite(pinRelay,    LOW);
  digitalWrite(pinBuzzer,   LOW);
  digitalWrite(pinLedHijau, HIGH);
  digitalWrite(pinLedMerah, LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(pinFlame,    INPUT_PULLUP);
  pinMode(pinMQ2,      INPUT);
  pinMode(pinINA,      OUTPUT);
  pinMode(pinINB,      OUTPUT);
  pinMode(pinRelay,    OUTPUT);
  pinMode(pinBuzzer,   OUTPUT);
  pinMode(pinLedHijau, OUTPUT);
  pinMode(pinLedMerah, OUTPUT);

  servoFlame.setPeriodHertz(50);
  servoFlame.attach(pinServo1, 500, 2400);
  servoPump.setPeriodHertz(50);
  servoPump.attach(pinServo2, 500, 2400);

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("  SYSTEM READY  ");
  lcd.setCursor(0, 1); lcd.print("Initializing... ");
  delay(2000);
  lcd.clear();

  setKondisiAman();
  servoFlame.write(90);
  delay(200);
  servoPump.write(90);
  delay(500);

  Serial.println("Setup selesai!");
}

void loop() {
  int statusApi = digitalRead(pinFlame);
  int statusGas = digitalRead(pinMQ2);

  bool adaApi = (statusApi == LOW);
  bool adaGas = (statusGas == LOW);

  Serial.print("Api: "); Serial.print(statusApi);
  Serial.print(" | Gas: "); Serial.print(statusGas);
  Serial.print(" | Servo: "); Serial.println(servoPos);

  if (adaApi && adaGas) {
    digitalWrite(pinLedHijau, LOW);
    digitalWrite(pinLedMerah, HIGH);
    digitalWrite(pinRelay,    HIGH);
    digitalWrite(pinINA,      HIGH);
    digitalWrite(pinINB,      LOW);
    updateBuzzer(true);
    updateServoScanning(true);
    updateLCD("FIRE & GAS!     ", "POS:" + String(servoPos) + "        ");

  } else if (adaApi) {
    digitalWrite(pinLedHijau, LOW);
    digitalWrite(pinLedMerah, HIGH);
    digitalWrite(pinRelay,    HIGH);
    digitalWrite(pinINA,      LOW);
    digitalWrite(pinINB,      LOW);
    updateBuzzer(true);
    updateServoScanning(true);
    updateLCD("ADA API / FIRE! ", "POS:" + String(servoPos) + "        ");

  } else if (adaGas) {
    digitalWrite(pinLedHijau, LOW);
    digitalWrite(pinLedMerah, HIGH);
    digitalWrite(pinRelay,    LOW);
    digitalWrite(pinINA,      HIGH);
    digitalWrite(pinINB,      LOW);
    updateBuzzer(true);
    updateServoScanning(false);
    updateLCD("GAS DETECTED!   ", "FAN ON          ");

  } else {
    setKondisiAman();
    updateBuzzer(false);
    updateServoScanning(false);
    updateLCD("Status: AMAN    ", "Scanning...     ");
  }
}