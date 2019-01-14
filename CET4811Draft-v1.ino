/*
Date: 10/23/2018
As many initial functions as I can think of, done.

*/

#include <Servo.h> 

const int POTENTIOMETER = 0; //potentiometer pin number (Analog)
const int POWER_SWITCH = 1; //power switch pin number
const int TRIGGER_PIN = 2; //trigger pin for Ultrasonic Sensor
const int ECHO_PIN = 3; //trigger pin for Ultrasonic Sensor
const int SPEAKER_PIN = 4; //speaker pin number
const int FRONT_LEDS = 6;
const int BACK_LEDS = 7;

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;
const Servo MOTOR_ARRAY[] ={ motor1, motor2, motor3, motor4 };
const int MOTOR_ARRAY_LENGTH = 4;
const int MOTOR_PIN1 = 8;
const int MOTOR_PIN2 = 9;
const int MOTOR_PIN3 = 10;
const int MOTOR_PIN4 = 11;

//const int speedFactor = 1; //multiplier for potentiometer speed
const int MAX_MOTOR_SPEED = 179; //Servo value
const int DISTANCE_BEFORE_STOP = 5; //inches

void driveForward(Servo motorArray[], int speed);
void driveBackwards(Servo motorArray[], int speed);
void turnRight(Servo motorArray[], int speed);
void turnLeft(Servo motorArray[], int speed);
void turnOffAllMotors(Servo motorArray[], int motorArrayLength);
long pingUltrasonic(int trigger, int echo);
long microsecondsToInches(long microseconds);
int getSpeed(int potPin, int maxSpeed);
void lightLEDs(int front, int back, long inches, int stopLength);


void setup() {
	Serial.begin(9600);
	motor1.attach(MOTOR_PIN1);
	motor2.attach(MOTOR_PIN2);
	motor3.attach(MOTOR_PIN3);
	motor4.attach(MOTOR_PIN4);
	
	//pins set to input and output
	
	pinMode(TRIGGER_PIN, OUTPUT);
	pinMode(ECHO_PIN,  INPUT);	
}

void loop() {
	bool isOn = digitalRead(POWER_SWITCH);
	if (!isOn) {
	long inches;
	// convert the time into a distance
	inches = microsecondsToInches(pingUltrasonic(TRIGGER_PIN, ECHO_PIN));
	Serial.println(inches);
	lightLEDs(FRONT_LEDS, BACK_LEDS, inches, DISTANCE_BEFORE_STOP);
	
	int speed;
	speed = getSpeed(POTENTIOMETER, MAX_MOTOR_SPEED);
	driveForward(MOTOR_ARRAY,speed);
	}
	else {
		turnOffAllMotors(MOTOR_ARRAY, MOTOR_ARRAY_LENGTH);
	}
}
/* 
--------- Function Definitions ----------
*/

int getSpeed(int potPin, int maxSpeed) {
	int speed;
	speed = analogRead(potPin); //* speedFactor; 
	speed = map(speed, 0, 1023, 0, maxSpeed);
	return speed;
}

void turnOnFrontLEDs(int front) {
	digitalWrite(front, HIGH);
}
void turnOnBackLEDs(int back) {
	digitalWrite(back, HIGH);
}
void turnOffFrontLEDs(int front) {
	digitalWrite(front, LOW);
}
void turnOffBackLEDs(int back) {
	digitalWrite(back, LOW);	
}

void lightLEDs(int front, int back, long inches, int stopLength) {
	if (inches < stopLength) {
		turnOnFrontLEDs(front);
		turnOffBackLEDs(back);
	}
	else {
		turnOffFrontLEDs(front);
		turnOffBackLEDs(back);
	}
}
void driveForward(Servo motorArray[], int speed) {
	motorArray[0].write(speed);
	motorArray[1].write(speed);
	motorArray[2].write(0);
	motorArray[3].write(0);
}
void turnOffAllMotors(Servo motorArray[], int motorArrayLength) {
	for(int i = 0; i < motorArrayLength; i++) {
		motorArray[i].write(0);
	}
}

void driveBackwards(Servo motorArray[], int speed){
	motorArray[0].write(0);
	motorArray[1].write(0);
	motorArray[2].write(speed);
	motorArray[3].write(speed);	
}

void turnRight(Servo motorArray[], int speed){
	
}
void turnLeft(Servo motorArray[], int speed){
	
}


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

