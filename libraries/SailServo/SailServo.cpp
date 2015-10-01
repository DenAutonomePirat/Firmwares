/*
  SailServo class
*/

#include <Arduino.h>
#include <SailServo.h>


SailServo::SailServo(int inAPin,int inBPin,int pwmPin,int diagAPin,int diagBPin,int dataPin,int clockPin,int chipSelect)
{
	Motor motor(inAPin,inBPin,pwmPin,diagAPin,diagBPin);
	Encoder encoder(dataPin,clockPin,chipSelect);
	PID pid(&_currentPos, &_Output, &_targetPos, _Kp, _Ki, _Kd, DIRECT);
}

int SailServo::init(void){	
	_offset = encoder.getAbsolute();
}
int SailServo::set(int pos){
	_targetPos = pos;
}
int SailServo::get(){
	return _currentPos;
}
int SailServo::update(void){
	_currentPos = encoder.getAbsolute();
	pid.compute();

}
