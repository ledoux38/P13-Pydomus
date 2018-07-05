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
void loop() {
  // Wait for an incomming connection
  EthernetClient client = server.available();

  // If no client connects I start again at the beginning of the loop
  if (!client) return;


  // DECODING VARIABLES GET URL
  String result = url_item_recovery(client);

  if(result.length() != 0)
  {
      int param;
      int val;
      url_parameters_recovery(result, param, val);
      Serial.println(param);
      Serial.println(val);
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
            output_piloted(param, val, HEATING_FIXTURE, 30 );
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
          //function
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





String url_item_recovery(const EthernetClient& cl)
{
  // DECODING VARIABLES GET URL

  // decoding steps
  int lecture = 0;
  // initialize the response string
  String resultat = "";
  // string to store the reading of the data
  String donnee = "";

  // read loop of the data
  while (cl.available())
  {
    // reading a character
    char c = cl.read();

    // start reading name data
    if (lecture == 0 && c == '?')
    {
      lecture = 1;
      donnee = "";
    }

    // start reading a value
    else if (lecture == 1 && c == '=')
    {
      lecture = 2;
      resultat += donnee + ":";
      donnee = "";
    }

    // new variables
    else if (lecture == 2 && c == '&')
    {
      lecture = 1;
      // build the response string
      resultat += donnee;
      donnee = "";
    }

    // end of reading
    else if ((lecture == 2 || lecture == 1) && c == ' ')
    {
      lecture = 3;
      resultat += donnee;
    }

    // retrieve name or value data
    else if (lecture == 1 || lecture == 2)
    {
      donnee += c;
    }
    delay(1);
  }
  return resultat;
}





void url_parameters_recovery(const String& value_p, int& return_p, int& return_v, char del)
{

  String v = "";
  String p = "";

  // manages the switch between the parameter and the value
  bool toggle = false;

  for(int i=0; i<=value_p.length()-1; i++)
  {

    // if the value is equal to the delimiter i recover the values
    if(value_p[i] == del)
    {
      toggle = true;
    }

    // otherwise I consider that the parameter scanning is finished and
    // I start the scanning of the values passing the toggle variable to true.
    else
    {
      // if toggle equal to false i consider
      // that i am still in the scan of the parameter.
      if(toggle)
      {
        v += value_p[i];
      }
      else
      {
        p += value_p[i];
      }
    }
  }

  return_p = p.toInt();
  return_v = v.toInt();

}




void output_piloted_pwm(const int& param, const int& value, const int& fixture)
{
  Serial.println("process dans fonction");
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





// See also
//  https://arduinojson.org/
//  https://www.arduino.cc/en/Reference/Ethernet
