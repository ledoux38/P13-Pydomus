#include "Cryptographie.h"

Pair::Pair(void)
:m_value(""), m_convert(""), m_id("")
{
  
}

Pair::Pair(String str, String convert, String key)
:m_id(key)
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

String Pair::get_id(void) const
{
  return this -> m_id;
}

void Pair::set_all(String str, String convert) 
{
  if (str.length() == convert.length())
  {
    this -> m_value = str;
    this -> m_convert = convert;
  }
}

void Pair::set_id(String str)
{
 this -> m_id = str;
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
:m_size(8), m_occupied(0)
{
  
}

String Cryptographie::cryptage(String str, String key)
{
	String r ="";
	Pair p = this -> get(key);
	for(int index(0); index < str.length(); index++)
	{
		r += p.value_to_convert(str[index]);
	}
	return r;
}

String Cryptographie::decryptage(String str, String key)
{
	String r ="";
	Pair p = this -> get(key);
	for(int index(0); index < str.length(); index++)
	{
		r += p.convert_to_value(str[index]);
	}

	return r;
}

Pair& Cryptographie::get(String key)
{
	for(int index(0); index < this -> m_size; index ++)
	{
		if(this -> m_list[index].get_id() == key)
		{
			return this -> m_list[index];
		}
	} 

}

void Cryptographie::add_pair(Pair element)
{
	this -> m_list[this -> m_occupied] = element;
	this -> m_occupied ++;
}



