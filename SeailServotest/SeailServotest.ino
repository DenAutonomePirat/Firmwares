#include <PID_v1.h>
#include <MotorLib.h>
#include <AEAT6010.h>
#include <SailServo.h>




SailServo mainSail(23,25,2,5,6,28,26,24);//	SailServo(int inAPin,int inBPin,int pwmPin,int dataPin,int clockPin,int chipSelect);


void setup() {
	Serial.begin(115200);
	delay(250);
  	mainSail.init();

}

void loop() {
	while (Serial.available() > 0) {
    	int target = Serial.parseInt();
    	Serial.println(target);
  		mainSail.setPosition(target);
  	}
  	mainSail.update();
  	//Serial.println(mainSail.getPosition());

}
