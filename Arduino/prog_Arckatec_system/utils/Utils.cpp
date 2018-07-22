#include"Utils.h"

void update(int& var, int value, int maxi, int mini)
{
  if(value < mini || value > maxi)
     var = maxi;
  else
    var = value;
}
