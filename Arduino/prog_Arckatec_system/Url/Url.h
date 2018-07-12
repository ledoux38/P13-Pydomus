#ifndef Url_h
#define Url_h

#include <Arduino.h>


#define BEGIN_PARAM_LIST '?'
#define NEW_PARAM '&'
#define EQUAL '='
#define END ' '

class Element
{
  public:
  Element();
  Element(String, String);

  String get_param(void) const;
  String get_value(void) const;

  private:
  String m_param;
  String m_value;
};

class Parameters
{
  public:
  Parameters();
  ~Parameters();

  void filter(char&);
  int length(void);

  Element get_element(int) const;

  private:

  Element m_table_el[8];
  boolean m_recording;
  boolean m_w_param;
  String  m_v_param;
  String  m_v_valeur;
  int m_cpt;
};

#endif
