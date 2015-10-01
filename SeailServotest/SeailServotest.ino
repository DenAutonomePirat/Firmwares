#include <PID_v1.h>
#include <MotorLib.h>
#include <AEAT6010.h>
#include <SailServo.h>




SailServo mainSail(1, 2, 3, 6, 7, 8);

void setup() {
  mainSail.init();
}

void loop() {
  mainSail.setPosition(20);
  mainSail.update();
}
