
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


Parameters::Parameters():m_data(""), m_flag(false)
{
  
}

Parameters::~Parameters()
{

}

void Parameters::parameter_separation(char& character)
{
  
  if(character == BEGIN_PARAM_LIST)
  {
    this->m_flag = true;
  }
  
  else if(this->m_flag)
  {
    this->m_data += character;
  }

  else if(this->m_flag && character == END)
  {
    this->m_flag = false;
  }
  
  else
  {
    
  }
}


void Parameters::parameter_separation(String& str)
{
  for(int i(0); i < str.length(); i++)
  {
    if(str[i] == BEGIN_PARAM_LIST)
    {
      this->m_flag = true;
    }
    
    else if(this->m_flag)
    {
      this->m_data += str[i];
    }
  
    else if(this->m_flag && str[i] == END)
    {
      this->m_flag = false;
    }
    
    else
    {
      
    }
  }
}

String Parameters::get_data(void) const
{
  return this->m_data;
}


void Parameters::detachment(void)
{
  int cpt = 0;
  boolean flag_param = true;
  String param = "";
  String value = "";
  
  for(int index(0); index < this -> m_data.length(); index++)
  {
    char c = this-> m_data[index];
    
    if(c == NEW_PARAM)
    {
      this -> m_table_el[cpt] = Element(param, value);
      param = "";
      value = "";
      cpt++;
      flag_param = true;
    }

    else if(c == VALUE)
    {
      flag_param = false;
    }

    else
    {
      if(flag_param)
      {
        param += c;
      }

      else
      {
        value += c;
      }
    }
  }
  this -> m_table_el[cpt] = Element(param, value);
}


Element Parameters::get_element(int index) const
{
  return this -> m_table_el[index];
}
