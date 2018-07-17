#ifndef Cryptographie_h
#define Cryptographie_h
#include <Arduino.h>
class Pair
{
  public:
  Pair(void);
  Pair(String, String, String);
  ~Pair(void);

   String get_value(void) const;
   String get_convert(void) const;
   String get_id(void) const;

   void set_all(String, String);
   void set_id(String);

   char value_to_convert(char);
   char convert_to_value(char);

  protected:
  
  private:
  String m_id;
  String m_value;
  String m_convert;
};


class Cryptographie
{
  public:
  Cryptographie();

  String cryptage(String, String);
  String decryptage(String, String);

  void add_pair(Pair);
  //void remove_pair(String);


  protected:
  Pair& get(String key);

  private:
  int m_size;
  Pair m_list[8];
  int m_occupied;
};

#endif
