
#include <ArduinoJson.h>
#include <SPI.h> //bibliotheque pour SPI
#include <SD.h> //bibliotheque pour carte SD
#include <Ethernet2.h> //bibliothèque pour Ethernet


byte mac[] = {0x90, 0xA2, 0xDA, 0x11, 0x23, 0xCF}; // Adresse MAC
byte ip[] = {192, 168, 1, 22}; //Adresse IP
EthernetServer serveur(80);

//affication des GPIO
int led = 5;


void setup() 
{
  //initialisation de communication série
  Serial.begin (9600); 

  // initiailisation des pins
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  // test et démarrage connexion carte SD
  if (!SD.begin(4)) 
  { 
    Serial.println("Pb avec la carte");
    return;
  };

  //initialisation de la communication Ethernet
  Ethernet.begin (mac, ip); 
  Serial.print("\nLe serveur est sur l'adresse : ");
  Serial.println(Ethernet.localIP()); 
  serveur.begin();
}

void loop() 
{
  //Attendre la connection sur le port défini
  EthernetClient client = serveur.available();

  //si un client se connecte
  if(client) 
  {
    Serial.println("connection d'un client");

    // si le client est connecté
    if(client.connected()) 
    {

      //appel de la fonction de décodage
      String affichage = GET(client); 
      
      if(affichage == "5 : 0")
      {
          digitalWrite(led, LOW);
      }
      if(affichage == "5 : 1")
      {
          digitalWrite(led, HIGH);
      }
    }

    //transmission d'une reponse au client.
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<meta charset='utf-8'>");
    client.println("<title> Analogique </title>");
    client.println("</head>");
    client.println("<body>");
    client.println("</body>");
    client.println("</html>");

    client.stop();
    Serial.println("Fin de la communication avec le client");
  }

}

//fonctin décodage GET
String GET(EthernetClient cl) {
  int lecture = 0; //étapes de décodage
  String resultat = ""; //initialisation de la chaine de réponse
  String donnee = ""; //chaine pour stocker la lecture des données
  while (cl.available()) { // tant qu'il a des infos à transmettre
    char c = cl.read(); // on lit le caractère
    if (lecture == 0 && c == '?') { //début de lecture des données donc d'un nom
      lecture = 1;
      donnee = "";
    }
    else if (lecture == 1 && c == '=') { //début de lecture d'une valeur
      lecture = 2;
      resultat += donnee + " : "; //on construit la chaîne de réponse
      donnee = "";
    }
    else if (lecture == 2 && c == '&') { //nouveau nom
      lecture = 1;
      resultat += donnee; //on construit la chaîne de réponse
      donnee = "";
    }
    else if ((lecture == 2 || lecture == 1) && c == ' ') { //fin de lecture
      lecture = 3;
      resultat += donnee; // on finit la chaîne réponse.
    }
    else if (lecture == 1 || lecture == 2) {//récupération des données de nom ou de valeur
      donnee += c;
    }
    delay(1); //delai de lecture
  }
  return resultat; // retour le la chaîne de réponse
}

