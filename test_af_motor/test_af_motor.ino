#include <MotorLib.h>
Motor motorMain(4,5,6,2,3);
Motor motorJib(9,10,11,7,8);
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
