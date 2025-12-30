#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

// ---------- RFID ----------
#define SS_PIN   5
#define RST_PIN  22
MFRC522 rfid(SS_PIN, RST_PIN);

// ---------- OUTPUTS ----------
#define GREEN_LED   2
#define SERVO_PIN  13
#define BUZZER_PIN 15   // ACTIVE BUZZER

Servo lockServo;

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  SPI.begin();
  rfid.PCD_Init();

  lockServo.attach(SERVO_PIN);
  lockServo.write(0);   // Locked position
}

void loop() {
  // Wait for card
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  // ---- FEEDBACK ----
  digitalWrite(GREEN_LED, HIGH);

  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);

  lockServo.write(90);     // Unlock
  delay(800);
  lockServo.write(0);      // Lock back

  digitalWrite(GREEN_LED, LOW);

  rfid.PICC_HaltA();
}
