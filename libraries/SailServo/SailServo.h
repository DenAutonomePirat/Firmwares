/*
  SailServo class
*/

#ifndef SailServo_h
#define SailServo_h
#include "../MotorLib/MotorLib.h"
#include "../AEAT6010/AEAT6010.h"
#include "../PID_v1/PID_v1.h"


class SailServo
{
public:
	
	SailServo(int inAPin,int inBPin,int pwmPin,int diagAPin,int diagBPin,int dataPin,int clockPin,int chipSelect);
	SailServo(int inAPin,int inBPin,int pwmPin,int dataPin,int clockPin,int chipSelect);
	int init(void);
	int setPosition(int pos);
	int getPosition();
	int setDeadband(int value);
	int update(void);
	void stop(void);
private:
	PID pid;
	Motor motor;
	Encoder encoder;
	double _targetPos, _currentPos, _output;
	bool _homing;
	int _offset,_deadband=5;
	bool _trimming;
	double _Kp=2, _Ki=5, _Kd=1;
};

#endif
