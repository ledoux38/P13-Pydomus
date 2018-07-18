#ifndef Utils_h
#define Utils_h
#include <Arduino.h>

void update(int& var, int value, int maxi, int mini);

void update_output_piloted(const int& pin, const bool& state, const int& fixture, const int& value_sensor);

void output_piloted_pwm(const int& param, const int& value, const int& fixture);

void output_piloted(const int& param, const int& value, const int& fixture, const int& value_sensor);

#endif

