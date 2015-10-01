#ifndef MotorLib_h
#define MotorLib_h

class Motor
{
public:
Motor(int inAPin,int inBPin,int pwmPin,int diagAPin,int diagBPin);
Motor(int inAPin,int inBPin,int pwmPin);
int go(int);
int diagnostic(void);
void brake(void);
private:
int _inAPin;
int _inBPin;
int _pwmPin;
int _diagAPin;
int _diagBPin;
int _value;
};

#endif
