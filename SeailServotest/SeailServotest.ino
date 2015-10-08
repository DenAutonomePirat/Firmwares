#include <PID_v1.h>
#include <MotorLib.h>
#include <AEAT6010.h>
#include <SailServo.h>
#include <RudderServo.h>

SailServo *mainSail = NULL;

SailServo *jibSail = NULL;
RudderServo *rudder = NULL;



  

int value2;
 
void setup() {
 

	Serial.begin(115200);
  while(!Serial){}

  
  // SailServo(int inAPin,int inBPin,int pwmPin,int dataPin,int clockPin,int chipSelect);
  mainSail = new SailServo(23,25,2,28,26,24);
  jibSail = new SailServo(27,29,3,28,26,22);

  //SailServo(int starboard,int port,int pwmPin,int intPinStarboard,int intPinPort,int analogPin);
  rudder = new RudderServo(33,31,4,30,32,0);

}

void loop() {


	while (Serial.available() > 0) {
    char test = Serial.read();
    switch (test){
      case 'm':
        mainSail->setPosition(Serial.parseInt());
        Serial.flush();
        break;
      case 'j':
        jibSail->setPosition(Serial.parseInt());
        Serial.flush();
        break;
      case 'r':
        rudder->setPosition(Serial.parseInt());
        Serial.flush();
        break;

      default:
        Serial.flush();
        break;
    }
  }

  if(jibSail->update()){
    Serial.println(jibSail->getPosition());
    //Serial.println(micros()-timer);
  }
  if(mainSail->update()){
    Serial.println(mainSail->getPosition());
    //Serial.println(micros()-timer);
  }
  if(rudder->update()){
    Serial.println(rudder->getPosition());
    //Serial.println(micros()-timer);
  }
}
