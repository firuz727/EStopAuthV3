//#include <MemoryFree.h>
#include <pgmStrToRAM.h>
//#include <Crypto.h>
#include <String.h>
#include <Serial.h>
#include "BLAKE.h"
#include "Packet.h"
#include <RH_ASK.h>
#include <SPI.h>

Packet packet;               // Create a packet
Packet* incoming = &packet;  // Create a pointer to it

const uint16_t MAX_GAP = 100;

RH_ASK radioDriver;

uint16_t lastValidSeq = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(7, INPUT);
  pinMode(6, INPUT);
  pinMode(8, OUTPUT);
  while (!Serial) {
    // wait for serial port to connect. Needed for Leonardo and Due
  }
  if (!radioDriver.init()) Serial.println("init failed");
  resetNonces();
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
  memset(buf, 0, buflen);

  // If packet received was valid
  if (radioDriver.available() && radioDriver.recv(buf, &buflen)) {

    // Stenciling the recieved data into the struct
    incoming = (Packet*)buf;

    Serial.print(incoming->command);
    Serial.print(", ");
    Serial.print(incoming->seqNum);
    Serial.print(", ");
    Serial.print(incoming->nonce);
    Serial.print(", ");
    for (int i = 0; i < 32; i++) {
      Serial.print(incoming->hash[i]);
      Serial.print(".");
    }
    Serial.println();

    uint8_t receivedHash[32];
    memcpy(receivedHash, incoming->hash, 32);

    // Reset hash field
    memset(incoming->hash, 0, 32);

    // Regenerate the BLAKE hash on receiving end to compare.
    generateBLAKE(incoming);

    uint8_t calculatedHash[32];
    memcpy(calculatedHash, incoming->hash, 32);

    // Reset hash field
    memset(incoming->hash, 0, 32);

    if (incoming->seqNum > lastValidSeq) {
      // SUCCESS: The sequence is correct
      if (incoming->seqNum - lastValidSeq > MAX_GAP) {
        // This jump in seq is too big! It might be a glitch
        // Handle it. Idk maybe an alarm or something
      }
      if (checkNonce(incoming->seqNum, incoming->nonce)) {
        // SUCCESS: The nonce is also valid!
        if (memcmp(calculatedHash, receivedHash, 32) == 0) {
          // SUCCESS: The key is correct and the data hasn't been tampered with
          lastValidSeq = incoming->seqNum;

          Serial.println("PACKET VALID!");
        } else {
          Serial.println("SECURITY ALERT: Invalid Hash Received!");
        }
      } else {
        Serial.println("INVALID NONCE!");
      }
    } else {
      Serial.println("INVALID SEQ!");
    }
  }
}

uint16_t usedMask = 0;
uint32_t prevMinNonce = 0;

int checkNonce(uint16_t seq, uint32_t nonce) {

  // 1. Determine the base of the current 10-block range
  uint32_t currentMinNonce = (seq / 10) * 10;

  // 2. If we moved to a new block, reset the tracking mask
  if (currentMinNonce > prevMinNonce) {
    usedMask = 0;
    prevMinNonce = currentMinNonce;
  }
  // Safety: If for some godforsaken reason we get an old block, reject it
  else if (currentMinNonce < prevMinNonce) {
    return 0;
  }

  // 3. Calculate index (should be 0-9)
  int32_t index = (int32_t)nonce - (int32_t)currentMinNonce;

  // 4. Bounds Check: Ensure the nonce is actually within the 10-block range
  if (index < 0 || index >= 10) {
    return 0;
  }

  // 5. Bitmask Check: (1 << index) creates the "stencil" for this specific nonce
  if (!(usedMask & (1 << index))) {
    // If the bit was 0 (NOT used), mark it as used and return 1 (Valid)
    usedMask |= (1 << index);
    return 1;
  } else {
    // Bit was already 1, this is a replay
    return 0;
  }
}

void clearPacket(Packet* pkt) {
  memset(pkt, 0, sizeof(pkt));
}
