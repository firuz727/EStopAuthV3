//#include <MemoryFree.h>
#include <pgmStrToRAM.h>
//#include <Crypto.h>
#include <String.h>
#include <Serial.h>
#include "BLAKE.h"
#include "Packet.h"
#include <RH_ASK.h>
#include <SPI.h>

Packet packet;          // Create a packet
Packet* pkt = &packet;  // Create a pointer to it

RH_ASK radioDriver;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(7, INPUT);
  pinMode(6, INPUT);
  while (!Serial) {
    // wait for serial port to connect. Needed for Leonardo and Due
  }
  if (!radioDriver.init()) Serial.println("init failed");
  resetNonces();
}

void loop() {
  // put your main code here, to run repeatedly:
  enum MessageType command;
  if (digitalRead(7) == HIGH) {
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

    radioDriver.send((uint8_t*)pkt, sizeof(pkt));
    radioDriver.waitPacketSent();

    clearPacket(pkt);
    delay(1000);
  }
}

void clearPacket(Packet* pkt) {
  memset(pkt, 0, sizeof(pkt));
}
