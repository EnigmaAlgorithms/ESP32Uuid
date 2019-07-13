/*
 * TrueRandom - A true random number generator for Arduino.
 * This is variant of original work originally implemented as:
 * https://code.google.com/archive/p/tinkerit/
 * https://github.com/Cathedrow/TrueRandom Copyright (c) 2010 Peter Knight,
 * Tinker.it! All rights reserved. Now modified for the ESP32
 */

#include "ESP32Uuid.h"
#include "esp_system.h"
#include "mbedtls/sha1.h"
#include "mbedtls/md5.h"

ESP32UuidClass::ESP32UuidClass() {
  useRNG = true;
}

char ESP32UuidClass::randomByte(void) {
  char result;
  esp_fill_random(&result, 1);
  return result;
}

void ESP32UuidClass::memfill(char* location, int size) {
  // for (; size--;) *location++ = randomByte();
  esp_fill_random(location, size);
}

void ESP32UuidClass::mac(uint8_t* macLocation) {
  memfill((char*)macLocation, 6);
}

void ESP32UuidClass::uuid3(uint8_t* uuidLocation, String input_string) {
  // Generate a Version 5 UUID according to RFC4122
  mbedtls_md5_context ctx;
  const size_t payloadLength = input_string.length();
  byte md5Result[16];

  mbedtls_md5_init(&ctx);
  mbedtls_md5_starts_ret(&ctx);
  mbedtls_md5_update_ret(&ctx, (const unsigned char*)input_string.c_str(),
                          payloadLength);
  mbedtls_md5_finish_ret(&ctx, md5Result);
  mbedtls_md5_free(&ctx);
  for (int i = 0; i < 16; i++) {
    uuidLocation[i] = md5Result[i];
  }
  // Although the UUID contains 128 bits, only 122 of those are random.
  // The other 6 bits are fixed, to indicate a version number.
  uuidLocation[6] = 0x30 | (0x0F & uuidLocation[6]);
  uuidLocation[8] = 0x80 | (0x3F & uuidLocation[8]);
}

void ESP32UuidClass::uuid4(uint8_t* uuidLocation) {
  // Generate a Version 4 UUID according to RFC4122
  memfill((char*)uuidLocation, 16);
  // Although the UUID contains 128 bits, only 122 of those are random.
  // The other 6 bits are fixed, to indicate a version number.
  uuidLocation[6] = 0x40 | (0x0F & uuidLocation[6]);
  uuidLocation[8] = 0x80 | (0x3F & uuidLocation[8]);
}

void ESP32UuidClass::uuid5(uint8_t* uuidLocation, String input_string) {
  // Generate a Version 5 UUID according to RFC4122
  mbedtls_sha1_context ctx;
  const size_t payloadLength = input_string.length();
  byte shaResult[20];

  mbedtls_sha1_init(&ctx);
  mbedtls_sha1_starts_ret(&ctx);
  mbedtls_sha1_update_ret(&ctx, (const unsigned char*)input_string.c_str(),
                          payloadLength);
  mbedtls_sha1_finish_ret(&ctx, shaResult);
  mbedtls_sha1_free(&ctx);
  for (int i = 0; i < 16; i++) {
    uuidLocation[i] = shaResult[i];
  }
  // Although the UUID contains 128 bits, only 122 of those are random.
  // The other 6 bits are fixed, to indicate a version number.
  uuidLocation[6] = 0x50 | (0x0F & uuidLocation[6]);
  uuidLocation[8] = 0x80 | (0x3F & uuidLocation[8]);
}

void ESP32UuidClass::stringToUuid(uint8_t* uuidLocation,
                                       String uuid_string) {
  // Write a UUID from a passed string input.
  // First lets strip out the dashe
  uuid_string.replace("-", "");
  uuid_string.toLowerCase();
  for (int i = 0; i < 16; i++) {
    uint8_t extract;
    char a = uuid_string.charAt(2 * i);
    char b = uuid_string.charAt(2 * i + 1);
    extract = convertCharToHex(a) << 4 | convertCharToHex(b);
    uuidLocation[i] = extract;
  }
}

String ESP32UuidClass::uuidToString(uint8_t* uuidLocation) {
  String string = "";
  int i;
  for (i = 0; i < 16; i++) {
    if (i == 4) string += "-";
    if (i == 6) string += "-";
    if (i == 8) string += "-";
    if (i == 10) string += "-";
    int topDigit = uuidLocation[i] >> 4;
    int bottomDigit = uuidLocation[i] & 0x0f;
    // High hex digit
    string += "0123456789abcdef"[topDigit];
    // Low hex digit
    string += "0123456789abcdef"[bottomDigit];
  }

  return string;
}

char convertCharToHex(char ch) {
  char returnType;
  switch (ch) {
    case '0':
      returnType = 0;
      break;
    case '1':
      returnType = 1;
      break;
    case '2':
      returnType = 2;
      break;
    case '3':
      returnType = 3;
      break;
    case '4':
      returnType = 4;
      break;
    case '5':
      returnType = 5;
      break;
    case '6':
      returnType = 6;
      break;
    case '7':
      returnType = 7;
      break;
    case '8':
      returnType = 8;
      break;
    case '9':
      returnType = 9;
      break;
    case 'A':
    case 'a':
      returnType = 10;
      break;
    case 'B':
    case 'b':
      returnType = 11;
      break;
    case 'C':
    case 'c':
      returnType = 12;
      break;
    case 'D':
    case 'd':
      returnType = 13;
      break;
    case 'E':
    case 'e':
      returnType = 14;
      break;
    case 'F':
    case 'f':
      returnType = 15;
      break;
    default:
      returnType = 0;
      break;
  }
  return returnType;
}

ESP32UuidClass ESP32Uuid;