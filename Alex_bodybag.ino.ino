// easing servo movements with low pass filter
// the servo signal must be connected directly to the arduino pin 2
 
// Time interval, this executes one piece of code from time to time
// Download Metro lybrary and instructions:
// http://www.arduino.cc/playground/Code/Metro
//#include <Metro.h>
int duration = 1000; // duration of the interval in miliseconds
//Metro intervaller = Metro (duration);
 
// MegaServo library
// download and instructions:
// http://www.arduino.cc/playground/Code/MegaServo
//#include <MegaServo.h>
//MegaServo servos;
 
#include <Servo.h>
 
// servos minimum and maximum position
#define MIN_POS 800 // the minuimum pulse width for your servos
#define MAX_POS 2200 // maximum pulse width for your servos
#define MIN_TIM 8000 // minimum time to wait between loops
#define MAX_TIM 12000 // maximum time to wait between loops
 
Servo backServo;
Servo frontServo;
//const int soundPin =  13;
 
// servo pin
//#define s0_pin 9
//#define s1_pin 10
 
// variables to hold new destination positions
int d0; // destination0
int d1;
int d0_sh; // destination0 to be smoothed
int d1_sh;
int delayBetweenStruggles;
String msg = "";
int movementsBetweenStruggles = 10;

bool testExtremes = false;

// the filter to be aplied
// this value will be multiplied by "d0" and added to "d0_sh"
float filtro = 0.1; // 0.01 to 1.0

// Moves the frontServo and the backServo to the target position
void moveToPositionSmoothly(int posTo0, int posTo1) {  // , int posFrom0, int posFrom1 
  int newPos0 = frontServo.read(); //posFrom0;
  int newPos1 = backServo.read(); //posFrom1;
  float stepInterval = 0.1;
  int numberOfSteps = 15;
  // 15 steps of 0.1 gets you 80% to the target position

  for (int n=0; n<15; n++) {
    newPos0 = newPos0 * (1.0-stepInterval) + posTo0 * stepInterval;
    newPos1 = newPos1 * (1.0-stepInterval) + posTo1 * stepInterval;

    // assign new position to the servo
    frontServo.write(newPos0);
    backServo.write(newPos1);

    // delay to make the servo move
    delay(20);
  }
}

// Moves the frontServo and the backServo to the "center" position
void moveToCenterPosition() {
  d0_sh = (1.0*MIN_POS + 1.0*MAX_POS)/2.0;
  d1_sh = (1.0*MIN_POS + 1.0*MAX_POS)/2.0;  
  frontServo.write(d0_sh);
  backServo.write(d1_sh);
}


// setup runs once, when the sketch starts
void setup() {
  // set sound pin
  //pinMode(soundPin, OUTPUT);
  // set servo pin
  Serial.begin(9600);
  frontServo.attach(9);
  backServo.attach(10);
  
  // Initialize the starting position to be centered
  moveToCenterPosition();
  Serial.println("Init done! Show begins in 5, 4, 3, 2, 1");
  delay(5000);
}
 
// main program loop, executes forever after setup()
void loop() {
  

  if (testExtremes) {
    d0 = MIN_POS;
    d1 = MIN_POS;
    frontServo.write(MIN_POS);
    backServo.write(MIN_POS);
    delay(2000);
    frontServo.write(MAX_POS);
    backServo.write(MAX_POS);
    delay(5000);
    // the same thing but with smoothening, maybe that's the issue
    moveToPositionSmoothly(MIN_POS,MIN_POS);
    delay(2000);
    moveToPositionSmoothly(MAX_POS,MAX_POS);
    delay(5000);
  }
  else {
    for (int i=0; i<movementsBetweenStruggles; i++) {
      // calculate a new random position between max and min values
      d0 = random(MIN_POS, MAX_POS);
      d1 = random(MIN_POS, MAX_POS);
      moveToPositionSmoothly(d0,d1);
      delay(300);
    }     
    delayBetweenStruggles = random(MIN_TIM, MAX_TIM);
    Serial.println("A new struggle begins");
    delay(delayBetweenStruggles);  
  }
}
