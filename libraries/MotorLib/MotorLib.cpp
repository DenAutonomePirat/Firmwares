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
  _inAPin = inAPin;
  _inBPin = inBPin;
  _pwmPin = pwmPin;
  _diagAPin = diagAPin;
  _diagBPin = diagBPin;
}
Motor::Motor(int inAPin,int inBPin,int pwmPin)
{
  pinMode(inAPin, OUTPUT);
  pinMode(inBPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);
  _inAPin = inAPin;
  _inBPin = inBPin;
  _pwmPin = pwmPin;
}

int Motor::diagnostic(void)
{
  return 1;
}
void Motor::brake(void)
{
	digitalWrite(_inAPin,LOW);
	digitalWrite(_inBPin,LOW);
	digitalWrite(_pwmPin,LOW);

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
	if (_value<25)_value=25;
	analogWrite(_pwmPin,_value);
}

