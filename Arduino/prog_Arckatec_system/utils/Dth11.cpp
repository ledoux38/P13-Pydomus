#include"Dth11.h"


byte readDHT11(byte pin, float* temperature, float* humidity)
{

  /* read sensor */
  byte data[5];
  byte ret = readDHT(pin, data, 18, 1000);

  /* detection error communication */
  if (ret != DHT_SUCCESS)
    return ret;

  /* calculate the humidity and temp */
  *humidity = data[0];
  *temperature = data[2];

  /* Ok */
  return DHT_SUCCESS;
}


byte readDHT(byte pin, byte* data, unsigned long start_time, unsigned long timeout)
{
  data[0] = data[1] = data[2] = data[3] = data[4] = 0;
  // start_time millisecondes
  // timeout microsecondes
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *ddr = portModeRegister(port);   // Registre MODE (INPUT / OUTPUT)
  volatile uint8_t *out = portOutputRegister(port); // Registre OUT (écriture)
  volatile uint8_t *in = portInputRegister(port);   // Registre IN (lecture)

  unsigned long max_cycles = microsecondsToClockCycles(timeout);

  *out |= bit;  // PULLUP
  *ddr &= ~bit; // INPUT
  delay(100);

  /* wake up the sensor */
  *ddr |= bit;  // OUTPUT
  *out &= ~bit; // LOW
  delay(start_time);
  noInterrupts();

  /* listening to the sensor*/
  *out |= bit;  // PULLUP
  delayMicroseconds(40);
  *ddr &= ~bit; // INPUT

  /* Waiting for the sensor response */
  timeout = 0;
  while(!(*in & bit)) {
    if (++timeout == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
  }

  timeout = 0;
  while(*in & bit) {
    if (++timeout == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
  }

  /* Reading sensor data (40 bits) */
  for (byte i = 0; i < 40; ++i) {

    /* état LOW */
    unsigned long cycles_low = 0;
    while(!(*in & bit)) {
      if (++cycles_low == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
    }

    /* état HIGH */
    unsigned long cycles_high = 0;
    while(*in & bit) {
      if (++cycles_high == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
    }

    data[i / 8] <<= 1;
    if (cycles_high > cycles_low) {
      data[i / 8] |= 1;
    }
  }

  interrupts();

  byte checksum = (data[0] + data[1] + data[2] + data[3]) & 0xff;
  if (data[4] != checksum)
    return DHT_CHECKSUM_ERROR; /* error checksum */
  else
    return DHT_SUCCESS; /* not error */

}
