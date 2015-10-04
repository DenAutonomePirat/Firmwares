/*
  SailServo class
*/

#include <Arduino.h>
#include <SailServo.h>


SailServo::SailServo(int inAPin,int inBPin,int pwmPin,int diagAPin,int diagBPin,int dataPin,int clockPin,int chipSelect)
	: pid(&_currentPos, &_output, &_targetPos, _Kp, _Ki, _Kd, DIRECT), 
	motor(inAPin,inBPin,pwmPin,diagAPin,diagBPin),
	encoder(dataPin,clockPin,chipSelect)
{
	SailServo::init();
}
SailServo::SailServo(int inAPin,int inBPin,int pwmPin,int dataPin,int clockPin,int chipSelect)
	: pid(&_currentPos, &_output, &_targetPos, _Kp, _Ki, _Kd, DIRECT), 
	motor(inAPin,inBPin,pwmPin),
	encoder(dataPin,clockPin,chipSelect)
{
	SailServo::init();
}

int SailServo::init(void){
	pid.SetOutputLimits(-256, 255);
	pid.SetMode(MANUAL);
	_offset = encoder.getAbsolute();
	
}
int SailServo::setPosition(int value){
	_targetPos = (double)value;
	_trimming = true;
	pid.SetMode(AUTOMATIC);
	Serial.println(_targetPos);
}

int SailServo::getPosition(){
	return _currentPos;
}

int SailServo::setDeadband(int value){
	_deadband = value;
} 

int SailServo::update(void){
	_currentPos = (double)encoder.getAbsolute();
	if(_trimming){
		if(pid.Compute()){
			Serial.print ("Target  = ");
			Serial.println(_targetPos);
			Serial.print ("Current = ");
			Serial.println(_currentPos);
			Serial.print ("Output  = ");
			Serial.println(_output);
			motor.go((int)_output);
		}	
	}
}
void SailServo::stop(void){
	_trimming = false;
	motor.brake();
	pid.SetMode(MANUAL);
}
