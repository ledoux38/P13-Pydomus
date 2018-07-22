#ifndef DTH11_H
#define DTH11_H

#include <Arduino.h>

#define DHT_SUCCESS 0
#define DHT_TIMEOUT_ERROR 1
#define DHT_CHECKSUM_ERROR 2

byte readDHT11(byte pin, float* temperature, float* humidity);

byte readDHT(byte pin, byte* data, unsigned long start_time, unsigned long timeout);
#endif

