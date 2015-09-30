//http://www.arduino.cc/playground/uploads/Code/FSM_1-6.zip
#include <FiniteStateMachine.h>
#include <MotorLib.h>

#include <PID_v1.h>
String inString = "";    // string to hold input


int max = 1024;
int min  = 0;
int center =max-min/2+min;
int pPot = 5;


/** this is the definitions of the states that our program uses */
State idle = State(idleUpdate);
State adjust = State(adjustEnter, adjustUpdate, NULL);
State maximum = State(maximumUpdate);

/** the state machine controls which of the states get attention and execution time */
FSM stateMachine = FSM(idle); //initialize state machine, start in state: noop

//Variables for the PID
double Setpoint, Input, Output;
//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

Motor rudder(4,5,6,2,3);


void setup(){
  Serial.begin(115200);
  
  inString.reserve(20);


  attachInterrupt(0, maximumSB, FALLING);
  attachInterrupt(1, maximumBB, FALLING);

  Input = analogRead(pPot);
  Setpoint = 100;

  myPID.SetOutputLimits(-255,256 );

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  // Set sample time to 10 ms
  myPID.SetSampleTime(10); 

}

void loop(){
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char 
      // and add it to the string:
      inString += (char)inChar; 
    }
    // if you get a newline, print the string,
    // then the string's value:
    if (inChar == '\n') {
      Setpoint = (inString.toInt());
      Serial.println(Setpoint);
      // clear the string for new input:
      inString = ""; 
    }
  }
  stateMachine.update();
}

void idleUpdate() {
  if (abs(Setpoint-analogRead(pPot))>2){
    stateMachine.transitionTo(adjust);
  } 
}
void adjustEnter() {
}
void adjustUpdate() {
  Setpoint = constrain(Setpoint,min,max);
  Input = analogRead(pPot);
  if (abs(Input - Setpoint)>2){
  myPID.Compute();
  rudder.go(Output);
  }else {
    rudder.go(0);
    stateMachine.transitionTo(idle);  
  }
}

void maximumUpdate() {
}
void maximumBB()
{
  rudder.go(0);
  max = analogRead(pPot);
  stateMachine.immediateTransitionTo(idle);

}
void maximumSB()
{
  rudder.go(0);
  min = analogRead(pPot);
  stateMachine.immediateTransitionTo(idle);

}



