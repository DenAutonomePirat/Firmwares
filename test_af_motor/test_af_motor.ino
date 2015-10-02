

#include <MotorLib.h>
Motor motorMain(23,25,2,4,5);//Motor(int inAPin,int inBPin,int pwmPin);
Motor motorJib(27,29,3,6,7);//Motor(int inAPin,int inBPin,int pwmPin);

void setup(){
  
}

void loop(){
//  motorMain.go(0);
  motorJib.go(-100);
  motorMain.go(-100);
  delay(1000);
  motorJib.go(100);
  motorMain.go(100);
  delay(1000);
}
