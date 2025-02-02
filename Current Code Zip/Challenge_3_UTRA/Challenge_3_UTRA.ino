#include <Servo.h> 
int servo = 12; 
Servo Servo1; 

const int trig = 10;  
const int echo = 11; 
float distance;
float duration;


int ENA = 7; 
int IN1 = 2; 
int IN2 = 3; 
int IN3 = 4; 
int IN4 = 5; 
int ENB = 6; 
int motor_speed;
int motor_speed1;

// define color sensor pins
#define S0 9
#define S1 8
#define S2 0
#define S3 13
#define sensorOut 1

// Calibration Values
// *Get these from Calibration Sketch
int redMin = 37; // Red minimum value
int redMax = 245; // Red maximum value
int greenMin = 20; // Green minimum value
int greenMax = 255; // Green maximum value
int blueMin = 32; // Blue minimum value
int blueMax = 190; // Blue maximum value

// Variables for Color Pulse Width Measurements
int redPW = 0;
int greenPW = 0;
int bluePW = 0;

// Variables for final Color values
int redValue;
int greenValue;
int blueValue;

int colour[5] = {1,2,3,2,3};




void setup() {
  // put your setup code here, to run once:

//Servo
Servo1.attach(servo); 

//Ultrasonic
pinMode(trig, OUTPUT);  
pinMode(echo, INPUT);  
Serial.begin(9600); 

//Driver Motors
pinMode(ENA, OUTPUT);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
pinMode(ENB, OUTPUT);

//Colour
	// Set S0 - S3 as outputs
	pinMode(S0, OUTPUT);
	pinMode(S1, OUTPUT);
	pinMode(S2, OUTPUT);
	pinMode(S3, OUTPUT);

	// Set Sensor output as input
	pinMode(sensorOut, INPUT);

	// Set Frequency scaling to 20%
	digitalWrite(S0,HIGH);
	digitalWrite(S1,LOW);

	// Setup Serial Monitor
	Serial.begin(9600);


}


}

void loop() {
  // put your main code here, to run repeatedly:
    for(int i = 0, i<5,i++){
    while(){
      forward();
      if(UltraSonic()){
      rightturn();
      delay(250);
      if(UltraSonic()){

      }
      }   
      forward();
      colour_check = ColourCheck();
          if(colour_check = colour[i]){
              pinMode(A1, OUTPUT);
              digitalWrite(A1,HIGH);
              delay(500);
              digitalWrite(A1,LOW);
              delay(500);
            break
          }
 
      delay(1000)
    }

    }
   

    forward();
    colour_check = ColourCheck();
    if(colour_check = 0){
      stopMotors();
      backward();
      delay(500);
      int colour = ColourCheck();
      if (colour = 1){
        leftturn();
        delay(1000);
        foward();
      }
      else if (colour = 2){
        rightturn();
        delay(500);
        foward();
      }
      else if (colour = 3){
        leftturn();
        delay(500);
        foward();
      }

    }
    if(UltraSonic()){
      stopMotors();
      int colour = ColourCheck();
      if (colour = 0){
        break;
      }
      else if (colour = 1){
        leftturn();
        delay(1000);
        foward();
      }
      else if (colour = 2){
        rightturn();
        delay(500);
        foward();
      }
      else if (colour = 3){
        leftturn();
        delay(500);
        foward();
      }
    }

}

bool UltraSonic(){
  digitalWrite(trig, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trig, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trig, LOW);  
  duration = pulseIn(echo, HIGH);  

  distance = (duration*.0343)/2;  
  if(distance < 20){
    return 1;
  }
  return 0;
}

void forward() {
  // Set both motors to move forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 200);  // Adjust speed as needed
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 200);  // Adjust speed as needed
}

void leftTurn() {
  // Set left motor to move backward, right motor to move forward for a left turn
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 200);  // Adjust speed as needed
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 200);  // Adjust speed as needed
}

void rightturn() {
  // Set left motor to move backward, right motor to move forward for a left turn
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 200);  // Adjust speed as needed
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 200);  // Adjust speed as needed
}

void backward() {
  // Set both motors to move backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 200);  // Adjust speed as needed
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 200);  // Adjust speed as needed
}

void stopMotors() {
  // Stop both motors
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}



int ColourCheck(){
	// Read Red value
	redPW = getRedPW();
	// Map to value from 0-255
	redValue = map(redPW, redMin,redMax,255,0);
	// Delay to stabilize sensor
	delay(200);

	// Read Green value
	greenPW = getGreenPW();
	// Map to value from 0-255
	greenValue = map(greenPW, greenMin,greenMax,255,0);
	// Delay to stabilize sensor
	delay(200);

	// Read Blue value
	bluePW = getBluePW();
	// Map to value from 0-255
	blueValue = map(bluePW, blueMin,blueMax,255,0);
	// Delay to stabilize sensor
	delay(200);

	// Print output to Serial Monitor
	Serial.print("Red = ");
	Serial.print(redValue);
	Serial.print(" - Green = ");
	Serial.print(greenValue);
	Serial.print(" - Blue = ");
	Serial.println(blueValue);
if((redValue && greenValue && blueValue) < 60){
  return 0;
}
if(redValue > greenValue && redValue > blueValue){
  return 1;
}
if(greenValue > redValue && greenValue > blueValue){
  return 2;
}
if(blueValue > redValue && blueValue > greenValue){
  return 3;
  
}


// Function to read Red Pulse Widths
int getRedPW() {
	// Set sensor to read Red only
	digitalWrite(S2,LOW);
	digitalWrite(S3,LOW);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}

// Function to read Green Pulse Widths
int getGreenPW() {
	// Set sensor to read Green only
	digitalWrite(S2,HIGH);
	digitalWrite(S3,HIGH);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}

// Function to read Blue Pulse Widths
int getBluePW() {
	// Set sensor to read Blue only
	digitalWrite(S2,LOW);
	digitalWrite(S3,HIGH);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}

}