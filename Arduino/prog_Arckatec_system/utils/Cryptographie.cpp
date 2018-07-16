#include "Cryptographie.h"

Pair::Pair(void)
:m_value(""), m_convert("")
{
  
}

Pair::Pair(String str, String convert)
{
  this -> set_all(str,convert);
}

Pair::~Pair()
{
  
}


String Pair::get_value(void) const
{
  return this -> m_value;
}

String Pair::get_convert(void) const
{
  return this -> m_convert;
}

void Pair::set_all(String str, String convert) 
{
  if (str.length() == convert.length())
  {
    this -> m_value = str;
    this -> m_convert = convert;
  }
}

char Pair::value_to_convert(char character)
{
  for(int index(0); index < this -> m_value.length(); index ++)
  {
    if(character == this -> m_value[index])
    {
      return this -> m_convert[index];
    }
  }
}

char Pair::convert_to_value(char character)
{
  for(int index(0); index < this -> m_convert.length(); index ++)
  {
    if(character == this -> m_convert[index])
    {
      return this -> m_value[index];
    }
  }
}





Cryptographie::Cryptographie()
{
  
}

String Cryptographie::cryptage(String str, int num)
{
  String r ="";
  if(num == 1)
  {
    for(int index(0); index < str.length(); index++)
    {
      r += this -> m_param.value_to_convert(str[index]);
    }
  }
  else if(num == 2)
  {
    for(int index(0); index < str.length(); index++)
    {
      r += this -> m_value.value_to_convert(str[index]);
    }
  }
  else
  {
    
  }
  return r;
}

String Cryptographie::decryptage(String str, int num)
{
  String r ="";
  if(num == 1)
  {
    for(int index(0); index < str.length(); index++)
    {
      r += this -> m_param.convert_to_value(str[index]);
    }
  }
  else if(num == 2)
  {
    for(int index(0); index < str.length(); index++)
    {
      r += this -> m_value.convert_to_value(str[index]);
    }
  }
  else
  {
    
  }
  return r;
}


void Cryptographie::set_param(Pair element)
{
  this -> m_param = element;
}

void Cryptographie::set_value(Pair element)
{
  this -> m_value = element;
}


