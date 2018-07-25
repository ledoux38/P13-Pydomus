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
#include <Utils.h>
#include <Dth11.h>
#include <Cryptographie.h>



///////////////////////////////////////////////////////////////////
// DEFINES
///////////////////////////////////////////////////////////////////
#define MINI_PIN  5
#define MAXI_PIN  9

#define MINI_LIGHTING  0
#define MAXI_LIGHTING  100

#define MINI_HEATING  0
#define MAXI_HEATING  30
#define KEY 1234

#define REPONSE_403  "HTTP/1.0 403 Forbidden"
#define REPONSE_400  "HTTP/1.0 400 Bad Request"
#define REPONSE_200  "HTTP/1.0 200 OK"
#define CONTENT_TYPE "Content-Type: application/json"
#define CONNECTION   "Connection: close"

///////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
///////////////////////////////////////////////////////////////////
//Adress IP
byte ip[] = {192, 168, 1, 15};
// Adress MAC
byte mac[] = {0x90, 0xA2, 0xDA, 0x11, 0x23, 0xCF};

EthernetServer server(80);

int LIGHTING_FIXTURE_MAIN = 100;
int LIGHTING_FIXTURE_HEATING = 100;
int HEATING_FIXTURE = 20;
int VALUE_SENSOR_HEATING = 0;

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
  Ethernet.begin(mac);

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
  Serial.println(F("client."));
  Parameters parameters;

  // DECODING VARIABLES GET URL
  while (client.available())
  {
    char c = client.read();
    // reading a character
    parameters.filter(c);
  }
  

  for (int i(0); i < parameters.length(); i++)
  {
   String mp = "";
   mp = parameters.get_to_index(i).get_param();
   parameters.get_to_index(i).set_param(decryptage(mp, PARAM_C));
   
   mp = parameters.get_to_index(i).get_value();
   parameters.get_to_index(i).set_value(decryptage(mp, VALUE_C, NUMBER));
  }
  
  if(parameters["key"].toInt() == KEY)
  {
  Serial.println(F("securité ok."));
  
     if(parameters.length() > 1)
     {
        Serial.println(F("reception d'un ordre"));
        int e; 
        e = parameters["element"].toInt();
        Serial.println(e);
        int v; 
        v = parameters["valeur"].toInt();
        Serial.println(v);
        switch(parameters["type"].toInt())
        {

          case 1:
            Serial.println(F("tor"));
            digitalWrite(e, v);
            break;
  
          case 2:
            switch(e)
            {
             Serial.println(F("mettre a jour valeur"));
             // update fixture lighting main
             case 100:
               update(LIGHTING_FIXTURE_MAIN, parameters["valeur"].toInt(), MAXI_LIGHTING, MINI_LIGHTING);
               break;
               
             // update fixture lighting bath
             case 110:
               update(LIGHTING_FIXTURE_HEATING, parameters["valeur"].toInt(), MAXI_LIGHTING, MINI_LIGHTING);
               break;
               
             // update fixture heating bath
             case 120:
               update(HEATING_FIXTURE, parameters["valeur"].toInt(), MAXI_HEATING, MINI_HEATING);
               break;
  
             default:
               Serial.println(F("conneexion ok ordre mais erreur "));
               client.println(REPONSE_400);
               client.println(CONTENT_TYPE);
               client.println(CONNECTION);
               client.stop();
               break;
            }
            
            break;
            
          case 3:
            switch(parameters["element"].toInt())
            {
              // update variable heating
              case 7:
                HEATING = parameters["valeur"].toInt();
                break;
              
              // deactivate all output
              case 130:
                for( int index = MINI_PIN; index <= MAXI_PIN; index ++)
                {
                  digitalWrite(index, LOW);
                }
                HEATING = false;
                break;
            
              default:
                for(int i(0); i<3; i++)
                {
                 Serial.println(F("conneexion ok mais erreur element"));
                 client.println(REPONSE_400);
                 client.println(CONTENT_TYPE);
                 client.println(CONNECTION);
                 client.stop();
                }
                break;
            }
            
          default:
             Serial.println(F("conneexion ok mais erreur type"));
             client.println(REPONSE_400);
             client.println(CONTENT_TYPE);
             client.println(CONNECTION);
             client.stop();
             break;
        }
         Serial.println(F("ok deco"));
         client.println(REPONSE_200);
         client.println(CONTENT_TYPE);
         client.println(CONNECTION);
         client.stop();
     }
     else
     {
      Serial.println(F("reception d'un GET"));
        // PREPARING RESPONSE JSON (Allocate the JSON document)
        StaticJsonDocument<500> doc;
      
        // Make our document represent an object
        JsonObject& root = doc.to<JsonObject>();
      
        // Create the "analog" array
        String crypt = "analog";
        crypt = cryptage(crypt, PARAM_C);
        JsonArray& analogValues = root.createNestedArray(crypt);
        for (int pin = 0; pin < 6; pin++) {
          
          // Read the analog input
          crypt = String(analogRead(pin));
          crypt = cryptage(crypt, VALUE_C, NUMBER);
          
          // Add the value at the end of the array
          analogValues.add(crypt);
        }
      
        // Create the "digital" array
        crypt = "digital";
        crypt = cryptage(crypt, PARAM_C);
        JsonArray& digitalValues = root.createNestedArray(crypt);
        for (int pin = 0; pin < 14; pin++)
        {
          // Read the digital input
          crypt = String(digitalRead(pin));
          crypt = cryptage(crypt, VALUE_C, NUMBER);  
              
          // Add the value at the end of the array
          digitalValues.add(crypt);
        }
        
        crypt = "capteurs";
        crypt = cryptage(crypt, PARAM_C);
        JsonArray& cpt_values = root.createNestedArray(crypt);
        float temperature, humidity;
      
        /* Reading of temperature and humidity, with error management */
        switch (readDHT11(PIN_CPT, &temperature, &humidity))
        {
        case DHT_SUCCESS:
          VALUE_SENSOR_HEATING = temperature;
          
          crypt = String(temperature);
          crypt = cryptage(crypt, VALUE_C, NUMBER);  
          cpt_values.add(crypt);
          
          crypt = String(humidity);
          crypt = cryptage(crypt, VALUE_C, NUMBER);  
          cpt_values.add(crypt);
          break;
      
        default:
          crypt = "0";
          crypt = cryptage(crypt, VALUE_C, NUMBER);  
          cpt_values.add(crypt);
          cpt_values.add(crypt);
          break;
        }
      
        // SEND THE JSON FILE TO THE CLIENT (Write response headers)
         client.println(REPONSE_200);
         client.println(CONTENT_TYPE);
         client.println(CONNECTION);
         client.println();
      
        // Write JSON document
        serializeJsonPretty(root, client);
      
        // Disconnect
        client.stop();
        Serial.println(F("get ok deco"));
     }
  }
  else
  {

   client.println(REPONSE_403);
   client.println(CONTENT_TYPE);
   client.println(CONNECTION);
   client.stop();
   Serial.println(F("erreur acces refusée"));
  }

}



// See also
//  https://arduinojson.org/
//  https://www.arduino.cc/en/Reference/Ethernet

