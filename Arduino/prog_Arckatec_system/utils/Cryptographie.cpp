#include "Cryptographie.h"

String cryptage(String &str, char *tabl_crypt, byte type = ALPHABET)
{
  byte cpt = 0;
  String r_str = "";
  for(int u(0); u < str.length(); u++)
  {
    switch(type)
    {
      case ALPHABET:
      for(int i(A); i <= Z; i++)
      {
        if(str[u] == char(i))
        {
          r_str += tabl_crypt[cpt];
          cpt = 0;
          break;
        }
        cpt += 1;
      }
      break;
        
      case NUMBER:
      for(int i(NUMBER_0); i <= NUMBER_9; i++)
      {
        if(str[u] == char(i))
        {
          r_str += tabl_crypt[cpt];
          cpt = 0;
          break;
        }
        cpt += 1;
      }
      break;

      default:
        return r_str;
      break;
    }

  }

  return r_str;
}

String decryptage(String &str, char *tabl_crypt, byte type = ALPHABET)
{
  byte cpt = 0;
  String r_str = "";
  for(int u(0); u < str.length(); u++)
  {
    switch(type)
    {
      case ALPHABET:
      for(int i(0); i <= 26; i++)
      {        
        if(str[u] == tabl_crypt[i])
        {
          r_str += char(A + cpt);
          cpt = 0;
          break;
        }
        cpt += 1;
      }
      break;
        
      case NUMBER:
      for(int i(0); i <= 9; i++)
      {
        if(str[u] == tabl_crypt[i])
        {
          r_str += char(NUMBER_0 + cpt);
          cpt = 0;
          break;
        }
        cpt += 1;
      }
      break;

      default:
        return r_str;
      break;
    }

  }

  return r_str;
}

