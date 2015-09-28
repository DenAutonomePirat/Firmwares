/*
  MototLib.h is forBB-VNH3SP30
*/

#include "Arduino.h"
#include "MotorLib.h"
Motor::Motor(int inAPin,int inBPin,int pwmPin,int diagAPin,int diagBPin)
{
  pinMode(inAPin, OUTPUT);
  pinMode(inBPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);
  pinMode(diagAPin, INPUT);
  pinMode(diagBPin, INPUT);
  _inAPin= inAPin;
  _inBPin= inBPin;
  _pwmPin = pwmPin;
  _diagAPin = diagAPin;
  _diagBPin = diagBPin;
}

int Motor::diagnostic(void)
{
  return 1;
}

int Motor::go(int value)
{
	_value = constrain(abs(value),0,255);
	 if (value > 0){
		digitalWrite(_inAPin,LOW);
		digitalWrite(_inBPin,HIGH);
		
	}

	if (value < 0)
		{
		digitalWrite(_inAPin,HIGH);
		digitalWrite(_inBPin,LOW);
	}
	
	analogWrite(_pwmPin,_value);
}

