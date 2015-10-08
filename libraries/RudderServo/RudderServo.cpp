/*
  RudderServo class
*/

#include <Arduino.h>
#include <RudderServo.h>


RudderServo::RudderServo(int inAPin,int inBPin,int pwmPin,int maxStarboard,int maxPort,int analogPin)
	: pid(&_currentPos, &_output, &_targetPos, _Kp, _Ki, _Kd, DIRECT), 
	motor(inAPin,inBPin,pwmPin)
{
	RudderServo::init();
	_analogPin = analogPin;
	RudderServo::setPosition(0);
}


bool RudderServo::init(void){
	pid.SetOutputLimits(-256, 255);
	pid.SetMode(MANUAL);
	pid.SetTunings( _Kp, _Ki, _Kd);
	pid.SetSampleTime(5);
	analogReadResolution(12);
	return true;
	
}
bool RudderServo::setPosition(int value){
	_targetPos = (double)value;
	_manouvering = true;
	_manTimestamp = millis()+3000;
	pid.SetMode(AUTOMATIC);
	return true;
}

int RudderServo::getPosition(){
	long int _sum = 0;
	for (int i = 0; i < 10;i++){
		_sum += analogRead(_analogPin);
	}
	return (_sum/10)-_offset ;
}

bool RudderServo::setDeadband(int value){
	_deadband = value;
	return true;
} 

bool RudderServo::update(void){
	if(_manouvering){
		if(millis()<_manTimestamp){
			_currentPos = (double)RudderServo::getPosition();
			if (abs(_currentPos - _targetPos) < _deadband){
				RudderServo::stop();
				return true;
			}else if(pid.Compute()){
				motor.go((int)_output);
				return true;
			}
		} else {
			RudderServo::stop();
			return false;
		}
	}else return false;
}
bool RudderServo::stop(void){
	_manouvering = false;
	motor.brake();
	pid.SetMode(MANUAL);
	return true;
}
