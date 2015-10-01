#include <SailServo.h>
#include <MotorLib.h>
#include <AEAT6010.h>
#include <PID_v1.h>


SailServo mainSail(1,2,3,4,5,6,7,8);

void setup() {
  mainSail.init();
  }

void loop() {
  
}