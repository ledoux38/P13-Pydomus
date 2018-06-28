// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License
//
// This example shows how to implement an HTTP server that sends JSON document
// in the responses.
// It uses the Ethernet library but can be easily adapted for Wifi.
//
// It sends the value of the analog and digital pins.
// The JSON document looks like the following:
// {
//   "analog": [ 0, 1, 2, 3, 4, 5 ],
//   "digital": [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 ]
// }

#include <ArduinoJson.h>
#include <Ethernet2.h>
#include <SPI.h>
//#include <SD.h> //bibliotheque pour carte SD

byte mac[] = {0x90, 0xA2, 0xDA, 0x11, 0x23, 0xCF}; // Adresse MAC
byte ip[] = {192, 168, 1, 22}; //Adresse IP
EthernetServer server(80);

void setup() {
  Serial.begin (9600); 
  // test et démarrage connexion carte SD
  

  
  Ethernet.begin (mac, ip); 
  Serial.print("\nLe serveur est sur l'adresse : ");
  Serial.println(Ethernet.localIP()); 
  server.begin();
}

void loop() {
  // Wait for an incomming connection
  EthernetClient client = server.available();

  // Do we have a client?
  if (!client) return;

  Serial.println(F("New client"));

  // Read the request (we ignore the content in this example)
  while (client.available()) client.read();

  // Allocate the JSON document
  // Use arduinojson.org/assistant to compute the capacity.
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

  Serial.print(F("Sending: "));
  serializeJson(root, Serial);
  Serial.println();

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
// --------
//
// The website arduinojson.org contains the documentation for all the functions
// used above. It also includes an FAQ that will help you solve any
// serialization problem.
// Please check it out at: https://arduinojson.org/
//
// The book "Mastering ArduinoJson" contains a tutorial on serialization.
// It begins with a simple example, then adds more features like serializing
// directly to a file or an HTTP client.
// Please check it out at: https://arduinojson.org/book/
