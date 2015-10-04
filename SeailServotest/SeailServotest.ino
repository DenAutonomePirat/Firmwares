#include <PID_v1.h>
#include <MotorLib.h>
#include <AEAT6010.h>
#include <SailServo.h>




SailServo mainSail(23,25,2,28,26,24);//	SailServo(int inAPin,int inBPin,int pwmPin,int dataPin,int clockPin,int chipSelect);
int count;

void setup() {
	Serial.begin(115200);
  	mainSail.init();

}

void loop() {
	while (Serial.available() > 0) {
    	int target = Serial.parseInt();
    	Serial.println(target);
  		mainSail.setPosition(target);
  	}
    
  	if(mainSail.update()){
      
      Serial.print(count);
      Serial.println(" Updated");
      count = 0;
    }else count++;
  	//Serial.println(mainSail.getPosition());

}
