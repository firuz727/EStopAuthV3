#include <BLAKE2s.h>
#include "BLAKE.h"
#include "Packet.h"

static const SecretKey key = {
  .key = "SECRET_KEY_HERE",
  .length = strlen("SECRET_KEY_HERE")
};

BLAKE2s blake;

void generateBLAKE(Packet* pkt) {
  blake.resetHMAC(key.key, key.length);
  // Offsetof makes sure blake doesnt hash an empty array of 0's
  blake.update((uint8_t*)pkt, offsetof(Packet, hash));
  blake.finalizeHMAC(key.key,key.length, pkt->hash, 32);  // 32-byte output
}
