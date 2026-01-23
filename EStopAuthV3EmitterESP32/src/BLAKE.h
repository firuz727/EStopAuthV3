#ifndef BLAKE_H
#define BLAKE_H
#include <Arduino.h>
#include "Packet.h"

struct SecretKey {
  char* key;
  uint16_t length;
};

void generateBLAKE(Packet* pkt);

#endif