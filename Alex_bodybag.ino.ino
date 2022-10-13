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
#define MIN_TIM 3000 // minimum time to wait between loops
#define MAX_TIM 7000 // maximum time to wait between loops
 
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
int loop_num;
bool reachedDestination = true;
String msg = "";
int movementsBetweenStruggles = 10;

bool testExtremes = true;

// the filter to be aplied
// this value will be multiplied by "d0" and added to "d0_sh"
float filtro = 0.1; // 0.01 to 1.0
 
// setup runs once, when the sketch starts
void setup() {
  // set sound pin
  //pinMode(soundPin, OUTPUT);
  // set servo pin
  Serial.begin(9600);
  frontServo.attach(9);
  backServo.attach(10);

  if (testExtremes == false) {
    d0_sh = random(MIN_POS, MAX_POS);
    d1_sh = random(MIN_POS, MAX_POS);
    d0 = random(MIN_POS, MAX_POS);
    d1 = random(MIN_POS, MAX_POS);
    frontServo.write(d0_sh);
    backServo.write(d1_sh);
    reachedDestination = false;
    Serial.println("Init done! :-)");
  }
}
 
// main program loop, executes forever after setup()
void loop() {
  delay(5000);

  if (testExtremes) {
    d0 = MIN_POS;
    d1 = MIN_POS;
    frontServo.write(MIN_POS);
    backServo.write(MIN_POS);
    delay(3000);
    frontServo.write(MAX_POS);
    backServo.write(MAX_POS);

    delay(3000);
    d0_sh = MAX_POS;
    d1_sh = MAX_POS;
    // the same thing but with smoothening, maybe that's the issue
    for (int n=0; n<15; n++) {
      d0_sh = d0_sh * (1.0-filtro) + d0 * filtro;
      d1_sh = d1_sh * (1.0-filtro) + d1 * filtro;
      // assign new position to the servo
      frontServo.write(d0_sh);
      backServo.write(d1_sh);
      // delay to make the servo move
      delay(20);
    }

  }
  else {
    //int sensorValue = analogRead(A2);
    for (int i=0; i<movementsBetweenStruggles; i++) {
        
        //digitalWrite(soundPin, LOW);
        //delay(1000);
        //digitalWrite(soundPin, HIGH);

        if (reachedDestination) {
          // calculate a new random position between max and min values
          d0 = random(MIN_POS, MAX_POS);
          d1 = random(MIN_POS, MAX_POS);
          reachedDestination = false;
        }
    
        // smooth the destination value
        for (int n=0; n<15; n++) {
          d0_sh = d0_sh * (1.0-filtro) + d0 * filtro;
          d1_sh = d1_sh * (1.0-filtro) + d1 * filtro;
      
          // assign new position to the servo
          frontServo.write(d0_sh);
          backServo.write(d1_sh);

          // delay to make the servo move
          delay(20);
        }
        reachedDestination = true;
        
    }     
    delayBetweenStruggles = random(MIN_TIM, MAX_TIM);
    Serial.println("A new struggle begins");
    delay(delayBetweenStruggles);  
  }
}
