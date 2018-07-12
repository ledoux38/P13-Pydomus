#ifndef Url_h
#define Url_h

#include <Arduino.h>


#define BEGIN_PARAM_LIST '?'
#define NEW_PARAM '&'
#define VALUE '='
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

  void parameter_separation(char&);
  void parameter_separation(String&);
  void detachment(void);
  int length(void);

  String get_data(void) const;
  Element get_element(int) const;

  private:
  String m_data;
  Element m_table_el[8];
  boolean m_flag;
  int m_cpt;
};

#endif
