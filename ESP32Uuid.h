/*
 * TrueRandom - A true random number generator for Arduino.
 * This is variant of original work originally implemented as:
 * https://code.google.com/archive/p/tinkerit/
 * https://github.com/Cathedrow/TrueRandom Copyright (c) 2010 Peter Knight,
 * Tinker.it! All rights reserved. Now modified for the ESP32
 */

#ifndef ESP32Uuid_h
#define ESP32Uuid_h

#include <Arduino.h>
#include <inttypes.h>

char convertCharToHex(char ch);

class ESP32UuidClass {
 public:
  ESP32UuidClass();
  char randomByte(void);
  void memfill(char* location, int size);
  void mac(uint8_t* macLocation);
  void uuid3(uint8_t* uuidLocation, String input_string);
  void uuid4(uint8_t* uuidLocation);
  void uuid5(uint8_t* uuidLocation, String input_string);
  void stringToUuid(uint8_t* uuidLocation, String uuid_string);
  String uuidToString(uint8_t* uuidLocation);
  bool useRNG;
};
extern ESP32UuidClass ESP32Uuid;
#endif