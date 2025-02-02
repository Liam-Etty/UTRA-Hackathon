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


// for sumaid level 1
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
bool timing = false;  // Track whether the timer is running
int colourDetected = 0;
int colourCheck = 0;
int colour = 0;
int prev_colour = 0;


bool UltraSonic(){
  digitalWrite(trig, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trig, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trig, LOW); 

  duration = pulseIn(echo, HIGH);  
  distance = (duration*.0343)/2; 
  delay(300); 
  if(distance < 19){
    return true;
  }
  return false;
}

void forward() {
  // Set both motors to move forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 100);  // Adjust speed as needed
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 100);  // Adjust speed as needed
}

void leftturn() {
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
  analogWrite(ENA, 100);  // Adjust speed as needed
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 100);  // Adjust speed as needed
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
	int redPW = getRedPW();
	// Map to value from 0-255
	redValue = map(redPW, redMin,redMax,255,0);
	// Delay to stabilize sensor
	delay(200);

	// Read Green value
	int greenPW = getGreenPW();
	// Map to value from 0-255
	greenValue = map(greenPW, greenMin,greenMax,255,0);
	// Delay to stabilize sensor
	delay(200);

	// Read Blue value
	int bluePW = getBluePW();
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
  if(redValue < 60 && greenValue < 60 && blueValue < 60){
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

int count_rings = 0;


void setup() {
  // put your setup code here, to run once:

  //Servo
  Servo1.attach(servo);   // Set servo to fully closed position (0 degrees) check again if this servo
  Serial.println("Servo set to 0 degrees (Fully closed)");

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
  count_rings = 0;
}


void loop() {
  // put your main code here, to run repeatedly:
  delay(200);
  if (timing == false){
    for (int pos = 180; pos >= 0; pos -= 5) { 
      Servo1.write(pos);
      Serial.print("Servo Position: ");
      Serial.println(pos);
      delay(70);
    }
    timing = true;
  }

  int colour = ColourCheck();

  Serial.print("\nMoving forward");
  forward();
    // int colour_check = ColourCheck();
    // if(colour_check == 0){
    //   stopMotors();
    //   delay(500) // slowing down the process
    //   backward();
    //   delay(500);
    //   int colour = ColourCheck();
    //   if (colour == 1){ // Red
    //     stopMotors();
    //     delay(500);
    //     leftturn();
    //     delay(1000);
    //     forward();
    //   }
    //   else if (colour == 2){ // Green
    //     rightturn();
    //     delay(500);
    //     forward();
    //   }
    //   else if (colour == 3){ // Blue
    //     leftturn();
    //     delay(500);
    //     forward();
    //   }

    // }


    // THE CODE AFTER THIS WORKS WELL, HAVENT TESTED THE CODE BEFORE THIS:

    // if(UltraSonic()){
    //   Serial.print("\nObstacle ");
    //   Serial.print(distance);
    //   Serial.print("cm away! \nStopping! \n");
    //   stopMotors();
    //   delay(1000);
    //   int colour = ColourCheck();
    //   if (colour == 0){
    //     Serial.print("On a black square, We win! \n");
    //     stopMotors();
    //     delay(2000);
    //     return; // this should work because when there is an obstacle and we are on a black square, we win.
    //   }
    

    if ((colour == 1 || colour == 2 || colour == 3) && colourDetected == 0){ //starter code only , 
      stopMotors();
      Serial.print("I see new ring, starting timer\n");
      startTime = millis();
      prev_colour = colourDetected;
      colourDetected = colour;
      count_rings ++;
      forward();
    }

    else if ((colour == 1 || colour == 2 || colour == 3) && colourDetected != colour) { //starter code only , 
      stopMotors();
      Serial.print("I see new ring, starting timer\n");
      startTime = millis();
      prev_colour = colourDetected;
      colourDetected = colour;
      count_rings ++;
      forward();
    }


    if ((colour != colourDetected) && (prev_colour != 0)){ //any other colour
      stopMotors();
      unsigned long elapsedTime = millis() - startTime;
      Serial.print("Time elapsed: ");
      Serial.print(elapsedTime);
      startTime = millis();
      count_rings ++;
      forward();
    }



      // delay(500);
      // leftturn();
      // delay(1405);
      // stopMotors();
      // delay(300); // commenting out forwards for now to test out turning
      // // forward();

    if ((colour != colourDetected) && (colour == 0 || colour == prev_colour)){
      unsigned long elapsedTime = millis() - startTime;
      unsigned long cheatTime = elapsedTime / 2;
      backward();
      delay(cheatTime);
      stopMotors();
      leftturn();
      delay(720);
      forward();
      
      if (colour == 0){
        leftturn();
        delay(1500);
        forward();
        
      }
    }

   
      


      // Serial.print("I see Green, turning Right \n");
      // rightturn();
      // delay(720); // DONT CHANGE THIS DELAY
      // stopMotors();
      // delay(300); // commenting out forwards for now to test out turning
      // // forward();
      //
    if (count_rings == 5){
      Servo1.write(50);
      for (int pos = 50; pos <= 130; pos += 5) { 
        Servo1.write(pos);  // Move servo to 'pos' degrees
        Serial.print("Servo Position: ");
        Serial.println(pos);
        delay(100);  // Wait 50ms
        }
    }

}


