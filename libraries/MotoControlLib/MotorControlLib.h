#ifndef MotorControlLib_h
#define MotorControlLib_h
#define BRAKEVCC 0
#define CW   1
#define CCW  2
#define BRAKEGND 3
#define CS_THRESHOLD 100

class MotorControl
{
public:
MotorControl(int inApin,int inBpin,int pwmpin,int cspin,int enpin);
int motorGo(int);
private:
int _inApin;
int _inBpin;
int _pwmpin;
int _cspin;
int _enpin;
int _statpin;
int _value;
};

#endif