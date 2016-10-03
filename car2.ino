#include <QTRSensors.h>

// This example is designed for use with eight QTR-1RC sensors or the eight sensors of a
// QTR-8RC module.  These reflectance sensors should be connected to digital inputs 3 to 10.
// The QTR-8RC's emitter control pin (LEDON) can optionally be connected to digital pin 2, 
// or you can leave it disconnected and change the EMITTER_PIN #define below from 2 to 
// QTR_NO_EMITTER_PIN.

// The main loop of the example reads the raw sensor values (uncalibrated).
// You can test this by taping a piece of 3/4" black electrical tape to a piece of white 
// paper and sliding the sensor across it.  It prints the sensor values to the serial 
// monitor as numbers from 0 (maximum reflectance) to 2500 (minimum reflectance).


#define NUM_SENSORS   8     // number of sensors u
#define TIMEOUT       1200  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   1     // emitter is controlled by digital pin 13
#define RIGHTPIN      10
#define LEFTPIN       11


//#define WHITE          300           // general value read for pure white from the sensor ( experiment )

// sensors 0 through 7 are conected to digital pins 2 through 9, respectively
QTRSensorsRC qtrrc((unsigned char[]) {2,3,4,5,6,7,8,9},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];




void setup()
{
  pinMode( LEFTPIN, OUTPUT );
  pinMode( RIGHTPIN, OUTPUT );
  delay(500);
  Serial.begin(9600); // set the data rate in bits per second for serial data transmission
  delay(1000);
  
  //analogWrite( LEFTPIN, 30 );
  //analogWrite( RIGHTPIN, 30 );
}


void loop()
{
  analogWrite( LEFTPIN, 60 );
  analogWrite( RIGHTPIN, 60 );
  // read raw sensor values
  qtrrc.read(sensorValues);
  /*sensorValues[0] -= 244;
  sensorValues[1] -= 120;
  sensorValues[3] -= 60;
  sensorValues[6] -= 60;
  sensorValues[7] -= 120;*/
  int angle = computeAngle( sensorValues );

  directionControl( angle );
  
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
  }
  
  
  Serial.println();
  
  //delay(50);
}


// name: computerAngle
// parameter: array of int
// return type int
// anaylysing the data read by the sensor, return a rough range of turnning angles noted by 1 to 18

int computeAngle( unsigned int sensorValues[NUM_SENSORS] ) {
  
  /* turnning right for 90
  if( sensorValues[0] > 300 && sensorValues[1] > 300 && sensorValues[2] > 300 && sensorValues[5] < 300 && sensorValues[6] < 300 && sensorValues[7] < 300 ) {
    Serial.print( "right right angle\n" ); 
    return 18;
  }
  // turnning left for 90
  if( sensorValues[0] < 300 && sensorValues[1] < 300 && sensorValues[2] < 300 && sensorValues[5] > 300 && sensorValues[6] > 300 && sensorValues[7] > 300 ) {
    Serial.print( "left right angle\n" ); 
    return 0;
  }*/
  
  // case 1: intersections between white tapes, determined by sensor values as both end, should go straight
  if( ( sensorValues[0] > sensorValues[3] ) && ( sensorValues[1] > sensorValues[3] ) && 
       ( sensorValues[6] > sensorValues[4] ) && ( sensorValues[7] > sensorValues[4] ) ) {
    // 9 means go straight
    Serial.print( "straight \n" );    
    return 9; 
  }

  // case 2: turning left, need experiments to get different situation's sensor data
  // general case: middle 4 sensor should be good( close to WHITE ),  left 2 sensors should change according to how sharp
  // the angle is

 
  // going back to track if positioning left of the track
  // right sensors to be white, middle and left sensors to be dark
  if(  ( sensorValues[7] + sensorValues[6] + sensorValues[5] + sensorValues[4] ) < ( sensorValues[0] + sensorValues[1] + sensorValues[2] + sensorValues[3] ) ) {
    Serial.print( "turning right\n" );
    return 14;
  }
   
  // going back to track if positioning right of the track
  // left sensors to be white, middle and right sensors to be dark
  if( ( sensorValues[7] + sensorValues[6] + sensorValues[5] + sensorValues[4] ) > ( sensorValues[0] + sensorValues[1] + sensorValues[2] + sensorValues[3] ) ) { 
     Serial.print( "turning left\n" );
    return 5;
  }
 
  Serial.print( "straight\n" );
  return 9;
}
void directionControl( int angle ) {
  // we quite need a function that mapping the angle onto the analogPin from 0 to 255

  //analogWrite( LEFTPIN, leftWheel( angle ) );
  //analogWrite( RIGHTPIN, rightWheel( angle ) );
  
  // how long should we delay, determined by the angle or just a fixed value??????
  //delay( );
  
  
  if( angle == 9 ) {
    analogWrite( LEFTPIN, 80 );
    analogWrite( RIGHTPIN, 80 );
    //delay( 1000 );
  }
  
  if( angle == 5 ) {
    analogWrite( LEFTPIN, 10 );
    analogWrite( RIGHTPIN, 80 );
  }
  
  if( angle == 14 ) {
    analogWrite( LEFTPIN, 80 );
    analogWrite( RIGHTPIN, 10 );
  }
  
  if( angle == 18 ) {    
    delay( 30 );
    qtrrc.read(sensorValues);
    if(  sensorValues[3] > 700 && sensorValues[4] > 700  ) {
      Serial.print( "turn right hard!!!!!!!!!!!!\n");
      analogWrite( LEFTPIN, 80 );
      analogWrite( RIGHTPIN, 10 );
      delay( 200 );
    }
    else {
      analogWrite( LEFTPIN, 80 );
      analogWrite( RIGHTPIN, 80 );
    }
  }
  
  if( angle == 0 ) {
    delay( 30 );
    qtrrc.read(sensorValues);
    if( sensorValues[3] > 700 && sensorValues[4] > 700 ) {
      Serial.print( "turn left hard!!!!!!!!!!!!\n");
      analogWrite( LEFTPIN, 10 );
      analogWrite( RIGHTPIN, 80 );
      delay( 200 );
    } 
    else {
       analogWrite( LEFTPIN, 80 );
       analogWrite( RIGHTPIN, 80 );
    }
  }
}



  


