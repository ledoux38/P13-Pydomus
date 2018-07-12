
#include <Arduino.h>
#include "Url.h"

Element::Element(): m_param(""), m_value("")
{

}


Element::Element(String param, String value): m_param(param), m_value(value)
{

}

String Element::get_param(void) const
{
  return this -> m_param;
}

String Element::get_value(void) const
{
  return this -> m_value;
}





Parameters::Parameters():m_v_param(""), m_v_valeur(""), m_recording(false), m_w_param(false), m_cpt(0)
{

}

Parameters::~Parameters()
{

}


void Parameters::filter(char& c)
{
  if(this -> m_recording)
  {

    if(c == END)
    {

      if(this -> m_v_param.length() != 0 && this -> m_v_valeur.length() != 0)
      {
        this -> m_table_el[this ->m_cpt] = Element(this -> m_v_param, this -> m_v_valeur);
        this -> m_cpt++;
        this -> m_v_valeur = "";
        this -> m_v_param = "";
        this -> m_recording = false;
      }
      else{
        this -> m_recording = false;
      }
    }
    else if(c == NEW_PARAM)
    {
      this -> m_table_el[this ->m_cpt] = Element(this -> m_v_param, this -> m_v_valeur);
      this -> m_cpt++;
      this -> m_v_valeur = "";
      this -> m_v_param = "";
    }

    else if(c == EQUAL)
    {
      this -> m_w_param = false;
    }

    else if(this -> m_w_param)
    {
      this -> m_v_param += c;
    }

    else
    {
      this -> m_v_valeur += c;
    }
  }

  else
  {
    if(c == BEGIN_PARAM_LIST)
    {
      this -> m_recording = true;
      this -> m_w_param = true;
    }

  }
}


Element Parameters::get_element(int index) const
{
  return this -> m_table_el[index];
}

int Parameters::length(void)
{
  return this -> m_cpt;
}
