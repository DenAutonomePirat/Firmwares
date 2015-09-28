//http://www.arduino.cc/playground/uploads/Code/FSM_1-6.zip
#include <FiniteStateMachine.h>

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


//MonsterMoto Shield
/*  VNH2SP30 pin definitions
 xxx[0] controls '1' outputs
 xxx[1] controls '2' outputs */
int inApin[2] = {
  7, 4};  // INA: Clockwise input
int inBpin[2] = {
  8, 9}; // INB: Counter-clockwise input
int pwmpin[2] = {
  5, 6}; // PWM input
int cspin[2] = {
  2, 3}; // CS: Current sense ANALOG input
int enpin[2] = {
  0, 1}; // EN: Status of switches output (Analog pin)


void setup(){
  Serial.begin(9600);
  Serial.println("Helmsmann 01");
  inString.reserve(20);


  //attachInterrupt(0, maximumSB, HIGH);
  //attachInterrupt(1, maximumBB, HIGH);


  //initialize the variables we're linked to
  Input = analogRead(5);
  Setpoint = 100;

  myPID.SetOutputLimits(-255,256 );

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  // Set sample time to 10 ms
  myPID.SetSampleTime(10); 

  // Initialize digital pins as outputs
  for (int i=0; i<2; i++)
  {
    pinMode(inApin[i], OUTPUT);
    pinMode(inBpin[i], OUTPUT);
    pinMode(pwmpin[i], OUTPUT);
  }
  // Initialize braked
  for (int i=0; i<2; i++)
  {
    digitalWrite(inApin[i], LOW);
    digitalWrite(inBpin[i], LOW);
  }
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
  delay(100);
}

void idleUpdate() {
  if (abs(Setpoint-analogRead(pPot))>2){
    stateMachine.transitionTo(adjust);
  } 
}
///[fade state:enter] the statemachine is just told to enter the fade state, we need to 
void adjustEnter() {
  Serial.println ("adjust");
}
///[fade state:update] we need to keep updating the tlc in order to see the effects
void adjustUpdate() {
  int current = analogRead(cspin[0]);
  Setpoint = constrain(Setpoint,min,max);
  Input = analogRead(pPot);
  if (abs(Input - Setpoint)>2){
  myPID.Compute();
  motorGo(0,Output);
  }else {
    motorOff(0);
    stateMachine.transitionTo(idle);  
  }
}
/* motorGo() will set a motor going in a specific direction
 the motor will continue going in that direction, at that speed
 until told to do otherwise.
 
 motor: this should be either 0 or 1, will selet which of the two
 motors to be controlled
 
 direct: Should be between 0 and 3, with the following result
 0: Brake to VCC
 1: Clockwise
 2: CounterClockwise
 3: Brake to GND
 
 pwm: should be a value between ? and 1023, higher the number, the faster
 it'll go
 */
void motorGo(uint8_t motor, int pwm)
{
  if (motor <= 1)
  {
    // Set inA[motor]
    if (pwm > 0)
      digitalWrite(inApin[motor], HIGH);
    else
      digitalWrite(inApin[motor], LOW);

    // Set inB[motor]
    if (pwm < 0)
      digitalWrite(inBpin[motor], HIGH);
    else
      digitalWrite(inBpin[motor], LOW);

    analogWrite(pwmpin[motor], abs(pwm));
  }
}
void motorOff(int motor)
{
  // Initialize braked
  for (int i=0; i<2; i++)
  {
    digitalWrite(inApin[i], LOW);
    digitalWrite(inBpin[i], LOW);
  }
  analogWrite(pwmpin[motor], 255);
}

void maximumUpdate() {
}
void maximumBB()
{
  motorOff(0);
  max = analogRead(pPot);
  stateMachine.immediateTransitionTo(idle);

}
void maximumSB()
{
  motorOff(0);
  min = analogRead(pPot);
  stateMachine.immediateTransitionTo(idle);

}



