#ifndef Cryptographie_h
#define Cryptographie_h
#include <Arduino.h>

#define A 97
#define Z 122

#define NUMBER_0 48
#define NUMBER_9 57

#define PARAMETRE 0
#define VALEUR 1

#define ALPHABET 0
#define NUMBER 1
		      //abcdefghijklmnopqrstuvwxyz
const char PARAM_C[] = "NBVCXWMLKJHGFDSQPOIUYTREZA";
		      //0123456789
const char VALUE_C[] = "AZERTYUIOP";

String cryptage(String &str, char *tabl_crypt, byte type = ALPHABET);

String decryptage(String &str, char *tabl_crypt, byte type = ALPHABET);

#endif
