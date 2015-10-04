#include <PID_v1.h>
#include <MotorLib.h>
#include <AEAT6010.h>
#include <SailServo.h>




SailServo mainSail(23,25,2,28,26,24);//	SailServo(int inAPin,int inBPin,int pwmPin,int dataPin,int clockPin,int chipSelect);
long unsigned time;

void setup() {
	Serial.begin(115200);
  	mainSail.init();
    mainSail.pid.SetTunings(5,3,.5);
    mainSail.pid.SetSampleTime(5);

}

void loop() {
	while (Serial.available() > 0) {
    	int target = Serial.parseInt();
    	Serial.println(target);
  		mainSail.setPosition(target);
  	}
    time = micros();
  	if(mainSail.update()){
      Serial.println(micros()-time);
    }
  	//Serial.println(mainSail.getPosition());

}
