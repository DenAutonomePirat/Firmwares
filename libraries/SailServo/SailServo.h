/*
  SailServo class
*/

#ifndef SailServo_h
#define SailServo_h
#include <MotorLib.h>
#include <AEAT6010.h>
#include <PID_v1.h>


class SailServo
{
public:
	SailServo(int inAPin,int inBPin,int pwmPin,int diagAPin,int diagBPin,int dataPin,int clockPin,int chipSelect);
	int init(void);
	int set(int pos);
	int get();
	int update(void);
private:
	double _targetPos;
	double _currentPos;
	bool _homing;
	bool _last;
	int _offset;
	double _Setpoint, _Input, _Output;
	double _Kp=2, _Ki=5, _Kd=1;
};

#endif
