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
//  Serial.begin(9600);
//  while (!Serial) continue;

  
  //  initialization of I / O variables
  for( int index = MINI_PIN; index <= MAXI_PIN; index ++)
  {
    pinMode(index, OUTPUT);
    digitalWrite(index, LOW);
  }
  pinMode(PIN_CPT, INPUT_PULLUP);
  // serial port initialization



  // Initialize Ethernet libary
  Ethernet.begin(mac);

  // Start to listen
  server.begin();

//  Serial.println(F("Server is ready."));
//  Serial.print(F("Please connect to http://"));
//  Serial.println(Ethernet.localIP());
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

  
     if(parameters.length() > 1)
     {

        int e; 
        e = parameters["element"].toInt();

        int v; 
        v = parameters["valeur"].toInt();
        
        switch(parameters["type"].toInt())
        {

          case 1:
            digitalWrite(e, v);
            break;
  
          case 2:
            switch(e)
            {
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
                 client.println(REPONSE_400);
                 client.println(CONTENT_TYPE);
                 client.println(CONNECTION);
                 client.stop();
                }
                break;
            }
            break;
            
          default:
             client.println(REPONSE_400);
             client.println(CONTENT_TYPE);
             client.println(CONNECTION);
             client.stop();
             break;
        }
         client.println(REPONSE_200);
         client.println(CONTENT_TYPE);
         client.println(CONNECTION);
         client.stop();
     }
     else
     {

        // PREPARING RESPONSE JSON (Allocate the JSON document)
        StaticJsonDocument<300> doc;
      
        // Make our document represent an object
        JsonObject& root = doc.to<JsonObject>();
      
        // Create the "digital" array

        String crypt = "digital";
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



//
        crypt = "capteurs";
        crypt = cryptage(crypt, PARAM_C);
        JsonArray& cpt_values = root.createNestedArray(crypt);
        float temperature, humidity;

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
          crypt = cryptage(crypt, VALUE_C, NUMBER);  
          cpt_values.add("0");
          cpt_values.add("0");
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

     }
  }
  else
  {

   client.println(REPONSE_403);
   client.println(CONTENT_TYPE);
   client.println(CONNECTION);
   client.stop();
  }

}



// See also
//  https://arduinojson.org/
//  https://www.arduino.cc/en/Reference/Ethernet

