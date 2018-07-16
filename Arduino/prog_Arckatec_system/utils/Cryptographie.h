#ifndef Cryptographie_h
#define Cryptographie_h
#include <Arduino.h>
class Pair
{
  public:
  Pair(void);
  Pair(String, String);
  ~Pair(void);

   String get_value(void) const;
   String get_convert(void) const;
   
   void set_all(String, String);
   
   char value_to_convert(char);
   char convert_to_value(char);

  protected:
  
  private:
  
  String m_value;
  String m_convert;
};


class Cryptographie
{
  public:
  Cryptographie();

  String cryptage(String, int);
  String decryptage(String, int);

  void set_param(Pair);
  void set_value(Pair);
  
  protected:
  
  private:
  String m_liste_param[];
  Pair m_param;
  Pair m_value;

};

#endif
