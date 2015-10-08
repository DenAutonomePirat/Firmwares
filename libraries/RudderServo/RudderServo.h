#ifndef RudderServo_h
#define RudderServo_h
#include "../MotorLib/MotorLib.h"
#include "../PID_v1/PID_v1.h"


class RudderServo
{
public:
	
	RudderServo(int starboard,int port,int pwmPin,int intPinStarboard,int intPinPort,int analogPin);
	bool init(void);
	bool setPosition(int pos);
	int getPosition();
	bool setDeadband(int value);
	bool update(void);
	bool stop(void);
	PID pid;
private:
	Motor motor;
	double _targetPos, _currentPos, _output;
	bool _homing,_initializing;
	int _deadband=0;
	int _offset = 2245;
	bool _manouvering;
	double _Kp=15, _Ki=9, _Kd=0.5;
	unsigned long _manTimestamp;
	int maxPort,maxStarboard;
	int _analogPin;
};

#endif
