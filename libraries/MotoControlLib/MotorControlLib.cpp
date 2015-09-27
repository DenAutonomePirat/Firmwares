#include "Arduino.h"
#include "MotorControlLib.h"

MotorControl::MotorControl(int inApin,int inBpin,int pwmpin,int cspin,int enpin)
{
pinMode(inApin, OUTPUT);
pinMode(inBpin, OUTPUT);
pinMode(pwmpin, OUTPUT);
_inApin = inApin;
_inBpin = inBpin;
_pwmpin = pwmpin;
_cspin  = cspin;
_enpin  = enpin;
}
int MotorControl::motorGo(int value)
{
//here goes the logic for deciding rotation  from whiteboard

return analogRead(_cspin); 
}
