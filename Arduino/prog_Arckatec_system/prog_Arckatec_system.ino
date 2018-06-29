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


#include <ArduinoJson.h>
#include <Ethernet2.h>
#include <SPI.h>

//Adress IP
byte ip[] = {192, 168, 1, 22};
// Adress MAC
byte mac[] = {0x90, 0xA2, 0xDA, 0x11, 0x23, 0xCF};


EthernetServer server(80);

void setup() {

  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);


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

void loop() {
  // Wait for an incomming connection
  EthernetClient client = server.available();

  // If no client connects I start again at the beginning of the loop
  if (!client) return;


  ///////////////////////////////////////////////////////////////////
  // DECODING VARIABLES GET URL
  ///////////////////////////////////////////////////////////////////

  // decoding steps
  int lecture = 0;
  // initialize the response string
  String resultat = "";
  // string to store the reading of the data
  String donnee = "";

  // read loop of the data
  while (client.available())
  {
    // reading a character
    char c = client.read();

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
      resultat += donnee + " : ";
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


  ///////////////////////////////////////////////////////////////////
  // UPDATED I / O
  ///////////////////////////////////////////////////////////////////

  if(resultat == "7 : 0")
  {
      digitalWrite(7, LOW);
  }
  if(resultat == "7 : 1")
  {
      digitalWrite(7, HIGH);
  }



  ///////////////////////////////////////////////////////////////////
  // PREPARING RESPONSE JSON
  ///////////////////////////////////////////////////////////////////

  // Allocate the JSON document
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
  for (int pin = 0; pin < 14; pin++) {
    // Read the digital input
    int value = digitalRead(pin);

    // Add the value at the end of the array
    digitalValues.add(value);
  }

  ////////////////////////////////////////////////////////////////
  // SEND THE JSON FILE TO THE CLIENT
  ////////////////////////////////////////////////////////////////

  // Write response headers
  client.println("HTTP/1.0 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();

  // Write JSON document
  serializeJsonPretty(root, client);

  // Disconnect
  client.stop();
}

// See also
//  https://arduinojson.org/
//  https://www.arduino.cc/en/Reference/Ethernet
