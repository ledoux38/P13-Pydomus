///////////////////////////////////////////////////////
// this program must be transferred to an Arduino
// card equipped with an Ethernet shield or
// integrated into the card.
//
// COMPONENTS:
// https://store.arduino.cc/arduino-uno-rev3
// https://store.arduino.cc/arduino-ethernet-shield-2
//
// WARNING:
// install the Ethernet2 library
// https://www.arduino.cc/en/Reference/Ethernet
//
// install the ArduinoJson library
// https://arduinojson.org/
//
// The program is in version V1.1
//
///////////////////////////////////////////////////////






///////////////////////////////////////////////////////////////////
// INCLUDES
///////////////////////////////////////////////////////////////////
#include <ArduinoJson.h>
#include <Ethernet2.h>
#include <SPI.h>
#include <Url.h>





///////////////////////////////////////////////////////////////////
// DEFINES
///////////////////////////////////////////////////////////////////
#define MINI_PIN  5
#define MAXI_PIN  9

#define MINI_LIGHTING  0
#define MAXI_LIGHTING  100

#define MINI_HEATING  0
#define MAXI_HEATING  30



///////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////

//the url_item_recovery function retrieves the parameters of the url
String url_item_recovery(const EthernetClient& cl);

// the function cuts the parameter and returns the parameter and value by reference
void url_parameters_recovery(const String&, int& return_p, int& return_v, char del=':');

void output_piloted_pwm(const int& param, const int& value, const int& fixture);

void output_piloted(const int& param, const int& value, const int& fixture, const int& value_sensor);

void update_output_piloted(const int& pin, const bool& state, const int& fixture, const int& value_sensor);

///////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
///////////////////////////////////////////////////////////////////
//Adress IP
byte ip[] = {192, 168, 1, 22};
// Adress MAC
byte mac[] = {0x90, 0xA2, 0xDA, 0x11, 0x23, 0xCF};

EthernetServer server(80);

int LIGHTING_FIXTURE_MAIN = 100;
int LIGHTING_FIXTURE_HEATING = 100;
int HEATING_FIXTURE = 20;
int VALUE_SENSOR_HEATING = 0;

const byte DHT_SUCCESS = 0;        // Pas d'erreur
const byte DHT_TIMEOUT_ERROR = 1;  // Temps d'attente dépassé
const byte DHT_CHECKSUM_ERROR = 2; // Données reçues erronées

int PIN_CPT = 3;

int PIN_HEATING = 7;
bool HEATING = false;

///////////////////////////////////////////////////////////////////
// SETUP
///////////////////////////////////////////////////////////////////

void setup()
{

  //  initialization of I / O variables
  for( int index = MINI_PIN; index <= MAXI_PIN; index ++)
  {
    pinMode(index, OUTPUT);
    digitalWrite(index, LOW);
  }
  pinMode(PIN_CPT, INPUT_PULLUP);
  // serial port initialization
  Serial.begin(9600);
  while (!Serial) continue;

  // Initialize Ethernet libary
  Ethernet.begin(mac, ip);

  // Start to listen
  server.begin();

  Serial.println(F("Server is ready."));
  Serial.print(F("Please connect to http://"));
  Serial.println(Ethernet.localIP());
}





///////////////////////////////////////////////////////////////////
// LOOP
///////////////////////////////////////////////////////////////////
void loop()
{
  update_output_piloted(PIN_HEATING, HEATING, HEATING_FIXTURE, VALUE_SENSOR_HEATING);

  // Wait for an incomming connection
  EthernetClient client = server.available();

  // If no client connects I start again at the beginning of the loop
  if (!client) return;

  Parameters parameters;

  // DECODING VARIABLES GET URL
  while (client.available())
  {
    char c = client.read();
    // reading a character
    parameters.filter(c);
  }

    // Serial.println("# DEBUT #");
    // Serial.println(parameters.get_element(0).get_param());
    // Serial.println(parameters.get_element(0).get_value());
    // Serial.println("# FIN #");

 if(parameters.length() != 0)
 {

     int param = parameters.get_element(0).get_param().toInt();
     int val = parameters.get_element(0).get_value().toInt();

     // UPDATED I / O
       switch(param)
       {
         // lighting bath on/off
         case 5:
           digitalWrite(param, val);
           break;

         // plug bath on/off
         case 6:
           digitalWrite(param, val);
           break;

         // heating bath on/off
         case 7:
           HEATING = val;
           break;

         // plug main on/off
         case 8:
           digitalWrite(param, val);
           break;

         // lighting main on/off
         case 9:
           digitalWrite(param, val);
           break;

         // update fixture lighting main
         case 100:
           if(val < MINI_LIGHTING || val > MAXI_LIGHTING)
           {
              LIGHTING_FIXTURE_MAIN = MAXI_LIGHTING;
           }
           else
           {
             LIGHTING_FIXTURE_MAIN = val;
           }

           break;

         // update fixture lighting bath
         case 110:
           if(val < MINI_LIGHTING || val > MAXI_LIGHTING)
           {

              LIGHTING_FIXTURE_HEATING = MAXI_LIGHTING;

           }
           else
           {

             LIGHTING_FIXTURE_HEATING = val;

           }

           break;

         // update fixture heating bath
         case 120:
         if(val < MINI_HEATING || val > MAXI_HEATING)
         {
            HEATING_FIXTURE = MAXI_HEATING;
         }
         else
         {

           HEATING_FIXTURE = val;

         }
         break;

         // deactivate all output
         case 130:
           for( int index = MINI_PIN; index <= MAXI_PIN; index ++)
           {
             pinMode(index, OUTPUT);
             digitalWrite(index, LOW);
           }
         break;
       }
       }


  // PREPARING RESPONSE JSON (Allocate the JSON document)
  StaticJsonDocument<500> doc;

  // Make our document represent an object
  JsonObject& root = doc.to<JsonObject>();

  // Create the "analog" array
  JsonArray& analogValues = root.createNestedArray("analog");
  for (int pin = 0; pin < 6; pin++) {
    // Read the analog input
    int value = analogRead(pin);

    // Add the value at the end of the array
    analogValues.add(value);
  }

  // Create the "digital" array
  JsonArray& digitalValues = root.createNestedArray("digital");
  for (int pin = 0; pin < 14; pin++)
  {
    // Read the digital input
    int value = digitalRead(pin);

    // Add the value at the end of the array
    digitalValues.add(value);
  }

  JsonArray& cpt_values = root.createNestedArray("capteurs");
  float temperature, humidity;

  /* Reading of temperature and humidity, with error management */
  switch (readDHT11(PIN_CPT, &temperature, &humidity))
  {
  case DHT_SUCCESS:
    VALUE_SENSOR_HEATING = temperature;
    cpt_values.add(temperature);
    cpt_values.add(humidity);
    break;

    default:
      cpt_values.add(0);
      cpt_values.add(0);
      break;
  }

  // SEND THE JSON FILE TO THE CLIENT (Write response headers)
  client.println("HTTP/1.0 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();

  // Write JSON document
  serializeJsonPretty(root, client);

  // Disconnect
  client.stop();
}




void output_piloted_pwm(const int& param, const int& value, const int& fixture)
{
  if(value)
  {
    analogWrite(param, fixture);
  }

  else
  {
    analogWrite(param, 0);
  }
}





void output_piloted(const int& param, const int& value, const int& fixture, const int& value_sensor)
{
  if(value)
  {
    if(value_sensor > fixture)
    {
      digitalWrite(param, value);
    }

    else
    {
      digitalWrite(param, 0);
    }
  }

  else
  {
    digitalWrite(param, value);
  }
}





void update_output_piloted(const int& pin, const bool& state, const int& fixture, const int& value_sensor)
{
  if(state)
  {

    if(value_sensor - 3 > fixture)
    {
      digitalWrite(pin, LOW);
    }
    else
    {
      digitalWrite(pin, HIGH);
    }
  }
  else
  {
    digitalWrite(pin, LOW);
  }
}





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





// See also
//  https://arduinojson.org/
//  https://www.arduino.cc/en/Reference/Ethernet
