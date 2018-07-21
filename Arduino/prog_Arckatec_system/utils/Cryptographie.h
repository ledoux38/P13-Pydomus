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
const char PARAM_C[] = "LC#TRE_IXA$+NJ-*/%H<>!FDOU";
const char VALUE_C[] = "L*$I|<_DXA";

String cryptage(String &str, char *tabl_crypt, byte type = ALPHABET);

String decryptage(String &str, char *tabl_crypt, byte type = ALPHABET);

#endif
