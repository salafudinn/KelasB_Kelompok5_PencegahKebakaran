#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pinFlame    = 4;
const int pinMQ2      = 34;
const int pinINA      = 25;
const int pinINB      = 26;
const int pinRelay    = 27;
const int pinBuzzer   = 14;
const int pinLedHijau = 18;
const int pinLedMerah = 19;

volatile bool adaApi = false;
volatile bool adaGas = false;

SemaphoreHandle_t mutexLCD;

void taskSensor(void *pvParameters) {
  for (;;) {
    adaApi = (digitalRead(pinFlame) == LOW);
    adaGas = (digitalRead(pinMQ2)   == LOW);
    Serial.print("Api: "); Serial.print(adaApi);
    Serial.print(" | Gas: "); Serial.println(adaGas);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void taskAktuator(void *pvParameters) {
  for (;;) {
    if (adaApi && adaGas) {
      digitalWrite(pinLedHijau, LOW);
      digitalWrite(pinLedMerah, HIGH);
      digitalWrite(pinRelay,    HIGH);
      digitalWrite(pinINA,      HIGH);
      digitalWrite(pinINB,      LOW);
    } else if (adaApi) {
      digitalWrite(pinLedHijau, LOW);
      digitalWrite(pinLedMerah, HIGH);
      digitalWrite(pinRelay,    HIGH);
      digitalWrite(pinINA,      LOW);
      digitalWrite(pinINB,      LOW);
    } else if (adaGas) {
      digitalWrite(pinLedHijau, LOW);
      digitalWrite(pinLedMerah, HIGH);
      digitalWrite(pinRelay,    LOW);
      digitalWrite(pinINA,      HIGH);
      digitalWrite(pinINB,      LOW);
    } else {
      digitalWrite(pinINA,      LOW);
      digitalWrite(pinINB,      LOW);
      digitalWrite(pinRelay,    LOW);
      digitalWrite(pinLedHijau, HIGH);
      digitalWrite(pinLedMerah, LOW);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void taskBuzzer(void *pvParameters) {
  for (;;) {
    if (adaApi || adaGas) {
      digitalWrite(pinBuzzer, HIGH);
      vTaskDelay(pdMS_TO_TICKS(150));
      digitalWrite(pinBuzzer, LOW);
      vTaskDelay(pdMS_TO_TICKS(150));
    } else {
      digitalWrite(pinBuzzer, LOW);
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}

void taskLCD(void *pvParameters) {
  String baris1Lama = "";
  for (;;) {
    String baris1, baris2;
    if (adaApi && adaGas) {
      baris1 = "FIRE & GAS!     ";
      baris2 = "PUMP & FAN ON   ";
    } else if (adaApi) {
      baris1 = "ADA API / FIRE! ";
      baris2 = "PUMP ON         ";
    } else if (adaGas) {
      baris1 = "GAS DETECTED!   ";
      baris2 = "FAN ON          ";
    } else {
      baris1 = "Status: AMAN    ";
      baris2 = "Monitoring...   ";
    }
    if (baris1 != baris1Lama) {
      baris1Lama = baris1;
      if (xSemaphoreTake(mutexLCD, pdMS_TO_TICKS(100)) == pdTRUE) {
        lcd.setCursor(0, 0); lcd.print(baris1);
        lcd.setCursor(0, 1); lcd.print(baris2);
        xSemaphoreGive(mutexLCD);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(300));
  }
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
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("  SYSTEM READY  ");
  lcd.setCursor(0, 1); lcd.print("Initializing... ");
  delay(2000);
  lcd.clear();
  digitalWrite(pinINA,      LOW);
  digitalWrite(pinINB,      LOW);
  digitalWrite(pinRelay,    LOW);
  digitalWrite(pinBuzzer,   LOW);
  digitalWrite(pinLedHijau, HIGH);
  digitalWrite(pinLedMerah, LOW);
  mutexLCD = xSemaphoreCreateMutex();
  xTaskCreate(taskSensor,   "Sensor",   2048, NULL, 3, NULL);
  xTaskCreate(taskAktuator, "Aktuator", 2048, NULL, 2, NULL);
  xTaskCreate(taskBuzzer,   "Buzzer",   1024, NULL, 2, NULL);
  xTaskCreate(taskLCD,      "LCD",      2048, NULL, 1, NULL);
  Serial.println("RTOS Tasks started!");
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}