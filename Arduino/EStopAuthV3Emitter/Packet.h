#ifndef PACKET_H
#define PACKET_H
#include <Arduino.h>

typedef struct __attribute__((__packed__)) {
  uint8_t command;   // ESTOP, etc.
  uint16_t seqNum;   // The raw sequence count
  uint32_t nonce;    // Your 10*x logic value
  uint8_t hash[32];  // BLAKE2s output of (command + seq + nonce)
} Packet;

enum MessageType {
  ESTOP,
  CHECK
  // ...
};

void buildPacket(Packet* pkt, MessageType type);
void resetNonces();
uint32_t generateNonce();


#endif