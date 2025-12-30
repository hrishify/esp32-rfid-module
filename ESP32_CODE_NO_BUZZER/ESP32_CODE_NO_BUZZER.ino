#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

#define SS_PIN 5
#define RST_PIN 22
#define GREEN_LED 2
#define SERVO_PIN 13

MFRC522 rfid(SS_PIN, RST_PIN);
Servo lockServo;

// Store up to 2 cards
byte allowed[2][4];
int cardCount = 0;

bool matchUID(byte *a, byte *b) {
  for (int i = 0; i < 4; i++)
    if (a[i] != b[i]) return false;
  return true;
}

bool isAllowed(byte *uid) {
  for (int i = 0; i < cardCount; i++)
    if (matchUID(uid, allowed[i])) return true;
  return false;
}

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  SPI.begin();
  rfid.PCD_Init();

  lockServo.attach(SERVO_PIN);
  lockServo.write(0);
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  byte *uid = rfid.uid.uidByte;

  // Auto-register first 2 cards
  if (cardCount < 2) {
    memcpy(allowed[cardCount], uid, 4);
    cardCount++;

    digitalWrite(GREEN_LED, HIGH);
    delay(300);
    digitalWrite(GREEN_LED, LOW);
  }
  else if (isAllowed(uid)) {
    digitalWrite(GREEN_LED, HIGH);
    lockServo.write(90);
    delay(800);
    lockServo.write(0);
    digitalWrite(GREEN_LED, LOW);
  }

  rfid.PICC_HaltA();
}
