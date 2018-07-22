#include"Utils.h"

void update(int& var, int value, int maxi, int mini)
{
  if(value < mini || value > maxi)
     var = maxi;
  else
    var = value;
}


void update_output_piloted(const int& pin, const bool& state, const int& fixture, const int& value_sensor)
{
  if(state)
  {

    if(value_sensor - 3 > fixture)
    {
      digitalWrite(pin, LOW);
    }
    else
    {
      digitalWrite(pin, HIGH);
    }
  }
  else
  {
    digitalWrite(pin, LOW);
  }
}

void output_piloted(const int& param, const int& value, const int& fixture, const int& value_sensor)
{
  if(value)
  {
    if(value_sensor > fixture)
    {
      digitalWrite(param, value);
    }

    else
    {
      digitalWrite(param, 0);
    }
  }

  else
  {
    digitalWrite(param, value);
  }
}


void output_piloted_pwm(const int& param, const int& value, const int& fixture)
{
  if(value)
  {
    analogWrite(param, fixture);
  }

  else
  {
    analogWrite(param, 0);
  }
}






