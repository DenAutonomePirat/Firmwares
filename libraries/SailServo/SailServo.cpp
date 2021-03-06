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

bool SailServo::init(void){
	pid.SetOutputLimits(-256, 255);
	pid.SetMode(MANUAL);
	pid.SetTunings( _Kp, _Ki, _Kd);
	_offset = encoder.getAbsolute();
	pid.SetSampleTime(5);
	return true;
	
}
bool SailServo::setPosition(int value){
	_targetPos = (double)value;
	_trimming = true;
	_trimTimestamp = millis()+10000;
	pid.SetMode(AUTOMATIC);
	return true;
}

int SailServo::getPosition(){
	return encoder.getAbsolute()- _offset;
}

bool SailServo::setDeadband(int value){
	_deadband = value;
	return true;
} 

bool SailServo::update(void){
	if(_trimming){
		if(millis()<_trimTimestamp){
			_currentPos = (double)encoder.getAbsolute();
			_currentPos = _currentPos - _offset;
			if (abs(_currentPos - _targetPos) < _deadband){
				SailServo::stop();
				return true;
			}
			if(pid.Compute()){
				motor.go((int)_output);
				return true;
			}
		} else {
			SailServo::stop();
			return false;
		}
	}else return false;
}
bool SailServo::stop(void){
	_trimming = false;
	motor.brake();
	pid.SetMode(MANUAL);
	return true;
}
