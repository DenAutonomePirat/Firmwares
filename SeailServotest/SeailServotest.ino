#include <PID_v1.h>
#include <MotorLib.h>
#include <AEAT6010.h>
#include <SailServo.h>

SailServo mainSail(23,25,2,28,26,24);// SailServo(int inAPin,int inBPin,int pwmPin,int dataPin,int clockPin,int chipSelect);

SailServo jibSail(27,29,3,28,26,22);// SailServo(int inAPin,int inBPin,int pwmPin,int dataPin,int clockPin,int chipSelect);

  


 
void setup() {

	Serial.begin(115200);
  while(!Serial){}

	mainSail.init();
  mainSail.pid.SetTunings(7,7,.3);
  mainSail.pid.SetSampleTime(5);

  jibSail.init();
  jibSail.pid.SetTunings(7,7,.3);
  jibSail.pid.SetSampleTime(5);

}

void loop() {


	while (Serial.available() > 0) {
    char test = Serial.read();
    switch (test){
      case 'm':
        mainSail.setPosition(Serial.parseInt());
        Serial.flush();
        break;
      case 'j':
        jibSail.setPosition(Serial.parseInt());
        Serial.flush();
        break;
      default:
        Serial.flush();
        break;
    }
  }

  if(jibSail.update()){
    Serial.println(jibSail.getPosition());
    //Serial.println(micros()-timer);
  }
  if(mainSail.update()){
    Serial.println(mainSail.getPosition());
    //Serial.println(micros()-timer);
  }
}
