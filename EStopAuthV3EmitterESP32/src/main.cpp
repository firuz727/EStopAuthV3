#include <Arduino.h>
#include <RH_ASK.h>
#include "Packet.h"
#include "BLAKE.h"
#include <SPI.h>

#define TX_PIN 11  // Connect to Transmitter Data
#define DUMMY_PIN 12

// Prototype Functions
void clearPacket(Packet* pkt);

// put function declarations here:
Packet packet;          // Create a packet
Packet* pkt = &packet;  // Create a pointer to it

// 2000 bps, RX = -1 (none), TX = 27, PTT = -1
RH_ASK radioDriver(2000, DUMMY_PIN, TX_PIN, DUMMY_PIN);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  // while (!Serial) {
  //   // wait for serial port to connect. Needed for Leonardo and Due
  // }
  if (!radioDriver.init()) Serial.println("init failed");
  delay(1000);
  resetNonces();
}

void loop() {
  // put your main code here, to run repeatedly:
  enum MessageType command;

  // Regular Packet
  if (digitalRead(2) == HIGH) {
    command = ESTOP;
    buildPacket(pkt, command);

    Serial.print(pkt->command);
    Serial.print(", ");
    Serial.print(pkt->seqNum);
    Serial.print(", ");
    Serial.print(pkt->nonce);
    Serial.print(", ");
    for (int i = 0; i < 32; i++) {
      Serial.print(pkt->hash[i]);
      Serial.print(".");
    }
    Serial.println();

    radioDriver.send((uint8_t*)pkt, sizeof(Packet));
    radioDriver.waitPacketSent();

    clearPacket(pkt);
    delay(1000);
  }

  // Incorrect Nonce
  // if (digitalRead(7) == HIGH) {
  //   command = ESTOP;
  //   buildPacket(pkt, command);

  //   pkt->nonce = 4;

  //   Serial.print(pkt->command);
  //   Serial.print(", ");
  //   Serial.print(pkt->seqNum);
  //   Serial.print(", ");
  //   Serial.print(pkt->nonce);
  //   Serial.print(", ");
  //   for (int i = 0; i < 32; i++) {
  //     Serial.print(pkt->hash[i]);
  //     Serial.print(".");
  //   }
  //   Serial.println();

  //   radioDriver.send((uint8_t*)pkt, sizeof(Packet));
  //   radioDriver.waitPacketSent();

  //   clearPacket(pkt);
  //   delay(1000);
  // }

  // // Tampered Hash
  // if (digitalRead(6) == HIGH) {
  //   command = ESTOP;
  //   buildPacket(pkt, command);

  //   pkt->hash[4] = 20;

  //   Serial.print(pkt->command);
  //   Serial.print(", ");
  //   Serial.print(pkt->seqNum);
  //   Serial.print(", ");
  //   Serial.print(pkt->nonce);
  //   Serial.print(", ");
  //   for (int i = 0; i < 32; i++) {
  //     Serial.print(pkt->hash[i]);
  //     Serial.print(".");
  //   }
  //   Serial.println();

  //   radioDriver.send((uint8_t*)pkt, sizeof(Packet));
  //   radioDriver.waitPacketSent();

  //   clearPacket(pkt);
  //   delay(1000);
  // }
}


void clearPacket(Packet* pkt) {
  memset(pkt, 0, sizeof(pkt));
}
