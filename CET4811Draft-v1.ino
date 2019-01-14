/*
Date: 11/13/2018
Completed:
	Serial Control Drafted. Only Automatic Control set. 
	Most functions completed. Will need to create turnLeft and turnRight, it spin versions should be fine for now. 
	Need to buy Bluetooth module and test at home with Smartphone to save time for next Draft. 
Next to do:
Find out maximum and minimum motor speed
	Possibly (0-200)?
	Current max sent to 179; search for MAX_MOTOR_SPEED
Add control system
	Take out Ultrasonic Drive Forward code
	First do Serial Monitor
	Then do Bluetooth
Eventually add speaker
*/

const int POTENTIOMETER = 0; //potentiometer pin number (Analog)
const int POWER_SWITCH = 1; //power switch pin number
const int TRIGGER_PIN = 2; //trigger pin for Ultrasonic Sensor
const int ECHO_PIN = 3; //trigger pin for Ultrasonic Sensor
const int SPEAKER_PIN = 4; //speaker pin number
const int FRONT_LEDS = 6;
const int BACK_LEDS = 7;

const int MOTOR_A = 8; //Motor chain A (left side);
const int PWM_A = A0; //Analog speed pin for chain A
const int MOTOR_B = 9; //Motor chain B (right side);
const int PWM_B = A1; //Analog speed pin for chain B

//const int speedFactor = 1; //multiplier for potentiometer speed
const int MAX_MOTOR_SPEED = 179; //Servo value
const int DISTANCE_BEFORE_STOP = 5; //inches

void driveForward(int motorA, int motorB);
void driveBackwards(int motorA, int motorB);
void spinLeft(int motorA, int motorB);
void spinRight(int motorA, int motorB);
void setMotorSpeed(int pwmA, int pwmB, int speed);

long pingUltrasonic(int trigger, int echo);
long microsecondsToInches(long microseconds);
int getSpeed(int potPin, int maxSpeed);

void turnOnLEDs(int front, int back);
void lightLEDsForward(int front, int back);
void lightLEDsBackward(int front, int back);
void turnOnAllLEDs(int front, int back);
void turnOffAllLEDs(int front, int back);


void setup() {
	Serial.begin(9600);
	
	//pins set to input and output
	
	pinMode(TRIGGER_PIN, OUTPUT);
	pinMode(ECHO_PIN,  INPUT);	
	pinMode(MOTOR_A,  OUTPUT);	
	pinMode(PWM_A,  OUTPUT);	
	pinMode(MOTOR_B,  OUTPUT);	
	pinMode(PWM_B,  OUTPUT);	
}

void loop() {
	bool isOn = digitalRead(POWER_SWITCH);
	if (isOn) {
		switch(Serial.read()) {	
			case('1'): //Potentiometer driving mode

			long inches;
			// convert the time into a distance
			inches = microsecondsToInches(pingUltrasonic(TRIGGER_PIN, ECHO_PIN));
			Serial.print("Distance = ");
			Serial.print(inches);
			Serial.println("in.");
			if (inches < DISTANCE_BEFORE_STOP) {
				lightLEDsForward(FRONT_LEDS, BACK_LEDS);
			}
			else {
				lightLEDsBackward(FRONT_LEDS, BACK_LEDS);
			}	
			
			int speed;
			speed = getSpeed(POTENTIOMETER, MAX_MOTOR_SPEED);
			setMotorSpeed(PWM_A, PWM_B, speed);
			driveForward(MOTOR_A, MOTOR_B);
			Serial.print("Speed = ");
			Serial.print(speed);
			Serial.println(".");
			break;
			
			//Manual driving mode
			case('2'):
			case('3'):
			case('4'):
			case('5'):
			case('6'):
			case('0'):
			default:
				break;
				
		}
	}
	else {
		setMotorSpeed(PWM_A, PWM_B, 0);
		turnOffAllLEDs(FRONT_LEDS, BACK_LEDS);
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

//for turning On and Off individual LEDs
void turnOnLED(int LED) { 		
	digitalWrite(LED, HIGH);
}
void turnOffLED(int LED) {
	digitalWrite(LED, LOW);
}
//LED lighting configuration
void lightLEDsForward(int front, int back) {
	turnOnLED(front);
	turnOffLED(back);
}
void lightLEDsBackward(int front, int back) {
	turnOffLED(front);
	turnOnLED(back);
}
void turnOnAllLEDs(int front, int back){
	turnOnLED(front);
	turnOnLED(back);
}
void turnOffAllLEDs(int front, int back){
	turnOffLED(front);
	turnOffLED(back);
}
void driveForward(int A, int B) {
	digitalWrite(A, HIGH);
	digitalWrite(B, HIGH);
}
void setMotorSpeed(int pwmA, int pwmB, int speed) {
	analogWrite(pwmA, speed);
	analogWrite(pwmB, speed);
}
void driveBackwards(int A, int B) {
	digitalWrite(A, LOW);
	digitalWrite(B, LOW);
}
void spinLeft(int A, int B) {
	digitalWrite(A, LOW);
	digitalWrite(B, HIGH);
}
void spinRight(int A, int B){
	digitalWrite(A, HIGH);
	digitalWrite(B, LOW);
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
