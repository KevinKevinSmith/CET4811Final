/*
Date: 12/13/2018 
Changes:
  Changed pins around
  Took out motor speed stuff
  Commented out Power Switch
  Added more Bluetooth controls along with Speaker
  Tied Potentiometer to LEDs (Now we can have flood lights)  
Next to do:
	Reuse getSpeed for Potentiometer to LEDs later.
	Check if Speaker needs to be an analog pin (pin A1 reserved just in case)

*/

const int POTENTIOMETER = A0; //potentiometer pin number (Analog)
//const int POWER_SWITCH = 1; //power switch pin number
const int TRIGGER_PIN = 2; //trigger pin for Ultrasonic Sensor
const int ECHO_PIN = 3; //trigger pin for Ultrasonic Sensor
const int SPEAKER_PIN = 4; //speaker pin number
const int FRONT_LEDS = 11;
const int BACK_LEDS = 12;

const int MOTOR_APOS = 9; //Motor chain A Positive power direction (right side);
const int MOTOR_ANEG = 8; //Motor chain A Negative power direction (right side);
const int MOTOR_A[2] = {MOTOR_APOS, MOTOR_ANEG};
//const int PWM_A = A2; //Analog speed pin for chain A
const int MOTOR_BPOS = 6; //Motor chain B Positive power direction (left side);
const int MOTOR_BNEG = 7; //Motor chain B Negative power direction (left side);
const int MOTOR_B[2] = {MOTOR_BPOS, MOTOR_BNEG};
//const int PWM_B = A3; //Analog speed pin for chain B

const int DISTANCE_BEFORE_STOP = 5; //inches

void driveForward(int motorChainA[], int motorChainB[]);
void driveBackwards(int motorChainA[], int motorChainB[]);
void spinForwardLeft(int motorChainA[], int motorChainB[]); 
void spinForwardRight(int motorChainA[], int motorChainB[]);
void spinBackwardLeft(int motorChainA[], int motorChainB[]); 
void spinBackwardRight(int motorChainA[], int motorChainB[]);
void setMotorSpeed(int pwmA, int pwmB, int speed);
void turnOffAllMotors(int motorChainA[], int motorChainB[]);

long pingUltrasonic(int trigger, int echo);
long microsecondsToInches(long microseconds);
int getSpeed(int potPin, int maxSpeed);

int getBrightness(int potPin);
void turnOnLED(int LED, int brightness);
void turnOffLED(int LED);
void lightLEDsForward(int front, int back, int brightness);
void lightLEDsBackward(int front, int back, int brightness);
void turnOnAllLEDs(int front, int back, int brightness);
void turnOffAllLEDs(int front, int back);

/* 
--------- Setup Pin Modes ----------
*/
void setup() {
  Serial.begin(9600);
  
  //pins set to input and output
  pinMode(POTENTIOMETER, INPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(FRONT_LEDS, OUTPUT);
  pinMode(BACK_LEDS, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN,  INPUT);  
  pinMode(MOTOR_APOS,  OUTPUT); 
  pinMode(MOTOR_ANEG,  OUTPUT); 
  //pinMode(PWM_A,  OUTPUT);  
  pinMode(MOTOR_BPOS,  OUTPUT); 
  pinMode(MOTOR_BNEG,  OUTPUT); 
  //pinMode(PWM_B,  OUTPUT);  
}

/* 
--------- Main Loop ----------
*/
void loop() {
	int brightness = analogRead(POTENTIOMETER);
	if(!brightness) { //if no potentiometer is read, assume it's not connected
		brightness = 256;
	}
  switch(Serial.read()) { 
    case('1'): //Stop the car
    	turnOffAllLEDs(FRONT_LEDS, BACK_LEDS);
      turnOffAllMotors(MOTOR_A, MOTOR_B);
      //delayMicroseconds(1000);
      break;
    //Manual driving mode
    case('2'): //Drive Forward
    	driveForward(MOTOR_A, MOTOR_B);
    	lightLEDsForward(FRONT_LEDS, BACK_LEDS, brightness);
      //delayMicroseconds(1000);
      break;
    case('3'): //Drive Backwards
    	driveBackwards(MOTOR_A, MOTOR_B);
    	lightLEDsBackward(FRONT_LEDS, BACK_LEDS, brightness);
      //delayMicroseconds(1000);
      break;
    case('4'): //Spin Forward Left
    	spinForwardLeft(MOTOR_A, MOTOR_B);
    	lightLEDsForward(FRONT_LEDS, BACK_LEDS, brightness);
      //delayMicroseconds(1000);
      break;
    case('5'): //Spin Forward Right
    	spinForwardRight(MOTOR_A, MOTOR_B);
    	lightLEDsForward(FRONT_LEDS, BACK_LEDS, brightness);
      //delayMicroseconds(1000);
      break;
    case('6'): //Spin Backward Left
    	spinBackwardLeft(MOTOR_A, MOTOR_B);
    	lightLEDsBackward(FRONT_LEDS, BACK_LEDS, brightness);
      //delayMicroseconds(1000);
      break;
    case('7'): //Spin Backward Right
	 		spinBackwardRight(MOTOR_A, MOTOR_B);
    	lightLEDsBackward(FRONT_LEDS, BACK_LEDS, brightness);
      //delayMicroseconds(1000);
    	break;
		case('7'): //Honk Horn
			honk(SPEAKER_PIN);
			break;
    case('0'): 
    default: //Potentiometer driving mode
      int duration = pingUltrasonic(TRIGGER_PIN, ECHO_PIN);
      long inches;
      // convert the time into a distance
      inches = microsecondsToInches(duration);
      Serial.println(duration);
      Serial.print("Distance = ");
      Serial.print(inches);
      Serial.println("in.");
      if (inches < DISTANCE_BEFORE_STOP) {
        lightLEDsForward(FRONT_LEDS, BACK_LEDS, brightness);
      }
      else {
        lightLEDsBackward(FRONT_LEDS, BACK_LEDS, brightness);
      } 

      if (inches < DISTANCE_BEFORE_STOP) {
        turnOffAllMotors(MOTOR_A, MOTOR_B);
      }
      else {
        driveForward(MOTOR_A, MOTOR_B);
      }
      delay(500);
      break;   
  } //end switch statement  
} //end loop

/* 
--------- Function Definitions ----------
*/


int getSpeed(int potPin, int maxSpeed) {
  int speed;
  speed = analogRead(potPin); //* speedFactor; 
  speed = map(speed, 0, 1023, 0, maxSpeed);
  return speed;
}

void honk(int speaker) {
	digitalWrite(speaker, HIGH);
	delay(1000);
	digitalWrite(speaker, LOW);
}

//for turning On and Off individual LEDs
void turnOnLED(int LED, int brightness) {     
  digitalWrite(LED, brightness);
}
void turnOffLED(int LED) {
  digitalWrite(LED, LOW);
}

//LED lighting configuration
void lightLEDsForward(int front, int back, int brightness) {
  turnOnLED(front, brightness);
  turnOffLED(back);
}
void lightLEDsBackward(int front, int back, int brightness) {
  turnOffLED(front);
  turnOnLED(back, brightness);
}
void turnOnAllLEDs(int front, int back, int brightness){
  turnOnLED(front, brightness);
  turnOnLED(back, brightness);
}
void turnOffAllLEDs(int front, int back){
  turnOffLED(front);
  turnOffLED(back);
}

//Driving Functions
void driveForward(int A[], int B[]) {
  digitalWrite(A[0], HIGH);
  digitalWrite(A[1], LOW);
  digitalWrite(B[0], HIGH);
  digitalWrite(B[1], LOW);
}
void setMotorSpeed(int pwmA, int pwmB, int speed) {
  analogWrite(pwmA, speed);
  analogWrite(pwmB, speed);
}
void driveBackwards(int A[], int B[]){ 
  digitalWrite(A[1], HIGH);
  digitalWrite(A[0], LOW);
  digitalWrite(B[1], HIGH);
  digitalWrite(B[0], LOW);
}
void spinForwardLeft(int A[], int B[]){
  digitalWrite(A[0], LOW);
  digitalWrite(A[1], HIGH);
  digitalWrite(B[0], HIGH);
  digitalWrite(B[1], LOW);
}
void spinForwardRight(int A[], int B[]) {
  digitalWrite(B[0], LOW);
  digitalWrite(B[1], HIGH);
  digitalWrite(A[0], HIGH);
  digitalWrite(A[1], LOW);
}
void spinBackwardLeft(int A[], int B[]) {
  digitalWrite(A[0], HIGH);
  digitalWrite(A[1], LOW);
  digitalWrite(B[0], LOW);
  digitalWrite(B[1], HIGH);
}
void spinBackwardRight(int A[], int B[]) {
  digitalWrite(B[0], HIGH);
  digitalWrite(B[1], LOW);
  digitalWrite(A[0], LOW);
  digitalWrite(A[1], HIGH);
}
void turnOffAllMotors(int A[], int B[]) {
  digitalWrite(B[0], LOW);
  digitalWrite(B[1], LOW);
  digitalWrite(A[0], LOW);
  digitalWrite(A[1], LOW);
}

//Ultrasonic Control
long pingUltrasonic(int trigger, int echo){
  // The HC-SR04 is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse
  digitalWrite(trigger,  LOW);
  delayMicroseconds( 5);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,  LOW);
  
  // The Echo pin is used to read the signal from HC-SR04; a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  return pulseIn(echo, HIGH);
  
}
long microsecondsToInches(long microseconds)
{
  // According to datasheet for the Ping sensor, there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second). Dividing by 74 gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.

  return microseconds / 74 / 2;
}
