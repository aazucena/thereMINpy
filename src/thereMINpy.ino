/**
 * @file thereMINpy.ino
 * @author Aldrin Azucena (a.azucena@uleth.ca)
 * @brief The program that run a synth for an arduino-based theremin with 2 sensors and a volume pot.
 * @version 0.1
 * @date 2023-02-01
 * @ref https://sensorium.github.io/Mozzi/examples
 * 
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <math.h>                // C+ Math Library 
#include <MozziGuts.h>           // Main mozzi object
#include <Oscil.h>               // oscillator
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <Smooth.h>              // smoothen the frequency 
#include <AutoMap.h>             // maps unpredictable inputs to a range
#define CONTROL_RATE  64         // The available number of bits it can store the sound


/// Constant values based on the distance the ultrasonic sensor's detection ranged distance
const int MIN_DISTANCE = 0;       // cm
const int MAX_DISTANCE = 80;      // cm

/// Constant min & max values for the carrier frequency when inputting to the AutoMap
const int MIN_CARRIER_FREQ = 440;
const int MAX_CARRIER_FREQ = 50;


/// Map Object that specializes the unpredictability of the sensor values to retrieve the frequency values
AutoMap kMapSine(0, 1023, MIN_CARRIER_FREQ, MAX_CARRIER_FREQ);

/// Oscillator object that acts a sine sound template that includes the wave tables
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> sineSound(SIN2048_DATA);


/// Smoother helper function that enables seamless sine sounds against changes in frequency
Smooth <int> kSmoothFreq(0.975f);


/// Default Analog Pin for the LDR Component
const int POT_PIN = 0;

/// Default pins for the indicator lights for the volume pot.
const int RED_INDICATOR_PIN = 2;
const int YELLOW_INDICATOR_PIN = 3;
const int GREEN_INDICATOR_PIN = 4;

/// Default pins for the parallel pulsing pins
const int RIGHT_LEDS_PIN = 5;
const int LEFT_LEDS_PIN = 6;

/// Default minmax values for the volume pot,
const int POT_MIN = 0;
const int POT_MAX = 1023;

/// Default Digital Pins for the Ultrasonic Sensor
const int SENSOR_MIN = 326; 
const int SENSOR_MAX = 4;

const int SOUND_MIN = 0;
const int SOUND_MAX = 255;

const int RIGHT_TRIGGER_PIN = 10; // Right Sensor Trigger Pin
const int RIGHT_ECHO_PIN = 11; // Right Sensor Echo Pin
const int LEFT_TRIGGER_PIN = 12; // Left Sensor Trigger Pin
const int LEFT_ECHO_PIN = 13; // Left Sensor Echo Pin


/// default minmax & 1/3-steps volume values
const int VOLUME_MIN = 0;
const int VOLUME_THIRDS = 85;
const int VOLUME_TWO_THIRDS = 170;
const int VOLUME_MAX = 255;


byte volume; // volume variable based on byt data type


/// The values for the sensors
unsigned long left_duration; // left sensor recorded duration
int left_distance; // converted and calculation distance (cm) based on the left sensor's duration
unsigned long right_duration; // right sensor recorded duration
int right_distance; // converted and calculation distance (cm) based on the right sensor's duration

void setup() {
  /// Initialize the pins for the indicator lights
  pinMode(RED_INDICATOR_PIN, OUTPUT);
  pinMode(YELLOW_INDICATOR_PIN, OUTPUT);
  pinMode(GREEN_INDICATOR_PIN, OUTPUT);

  /// Initialize the pins for the pulsing parallel lights for each sides 
  pinMode(RIGHT_LEDS_PIN, OUTPUT);
  pinMode(LEFT_LEDS_PIN, OUTPUT);


  /// Initializes the trigger and echo pins for ultrasonic sensors
  pinMode(LEFT_TRIGGER_PIN, OUTPUT);  // Initialize the pin withe ultrasonic sensor's trigger pin
  pinMode(LEFT_ECHO_PIN, INPUT);      // Initialize the pin withe ultrasonic sensor's echo output  pin
  pinMode(RIGHT_TRIGGER_PIN, OUTPUT); // Initialize the pin withe ultrasonic sensor's trigger pin
  pinMode(RIGHT_ECHO_PIN, INPUT);     // Initialize the pin withe ultrasonic sensor's echo output  pin
  
  /// Initializes the constructor starting the synth 
  startMozzi(CONTROL_RATE);
}

/**
 * @brief Observes the controller's actions
 *
 */
void updateControl() {

  /// Indicate the volume status through using the indicator lights
  indicateVolume(POT_PIN);

  /// Retrieve the sensors' detection distance based on duration
  detectSensors();

  // ultrasonic_value = ultrasonic_value <= 0 && ultrasonic_value > 100 ? 0 : ultrasonic_value;
  detectSensorsByDistance(POT_PIN);
}

/**
 * @brief Updates the audio frequency code that should return the integer between -244 to 243
 *
 * @return int
 */
int updateAudio() {

  int sine = sineSound.next(); // Get the real-time value of the sine frequency
  return volume > 0 ? (sine * volume) >> 8 : 0; // if the volume is not set to 0, play the sound through the volume value in 8 bits
}


/**
 * @brief Runs the loop of the program
 * 
 */
void loop() {
  audioHook(); // Run the audio hook to run the synth
}


/**
 * @brief Helper function that is used for sensor to set values when detected in certain distance
 * 
 * @param pot_pin 
 */
void detectSensorsByDistance(int pot_pin) {

  /// Retrieve the volume value from the pot. analog value
  byte pot_value = getVolume(pot_pin);

  /// boolean value to validate the volume value, and the sensors' distance
  bool hasSound = pot_value > 0 && (left_distance > 0 && left_distance < MAX_DISTANCE) || (right_distance > 0 && right_distance < MAX_DISTANCE);


  /// If it detect sound from sensor and has volume
  if (hasSound == true) {

    /// Set the volume value based on the potentionmeter's value
    volume = pot_value;

    /// If the left sensor is detected
    if (left_distance > 0 && left_distance < MAX_DISTANCE) {

      /// Map the sensor value to carrier frequency
      int sine_freq = kMapSine(left_distance);
      
      /// Smoothen the carrier frequency
      int smoothed_freq = kSmoothFreq.next(sine_freq);

      /// Set the sin sound based on the smoothen carrier frequency
      sineSound.setFreq(smoothed_freq);
    } 

    /// If the right sensor is detected
    if (right_distance > 0 && right_distance < MAX_DISTANCE) {

      /// Map the sensor value to carrier frequency
      int sine_freq = kMapSine(right_distance);
      
      /// Smoothen the carrier frequency
      int smoothed_freq = kSmoothFreq.next(sine_freq);

      /// Set the sin sound based on the smoothen carrier frequency
      sineSound.setFreq(smoothed_freq);
    } 
  } else {
        
    /// don't play the audio
    volume = 0;
  }
}


/**
 * @brief Function that lights up the LEDs bases on the analog pot value
 * 
 * @param pot_pin 
 */
void indicateVolume(int pot_pin) {

  /// Retrieve the pot value
  int pot_value = mozziAnalogRead(pot_pin);

  /// Map the brightness value based on the pot value
  int brightness = map(pot_value, POT_MIN, POT_MAX, VOLUME_MIN, VOLUME_MAX);


  /// If the volume is around the 1/3 of the max pot. value
  if (brightness >= VOLUME_MIN && brightness <= VOLUME_THIRDS) {

    /// Light red LED only
    analogWrite(RED_INDICATOR_PIN, 255);
    analogWrite(YELLOW_INDICATOR_PIN, 0);
    analogWrite(GREEN_INDICATOR_PIN, 0);

  /// Else if the volume is around the 2/3 of the max pot. value
  } else if (brightness > VOLUME_THIRDS && brightness <= VOLUME_TWO_THIRDS) {
    
    /// Light yellow LED only
    analogWrite(RED_INDICATOR_PIN, 0);
    analogWrite(YELLOW_INDICATOR_PIN, 255);
    analogWrite(GREEN_INDICATOR_PIN, 0);

  /// Else if the volume is greater than 2/3 of the max pot. value and less than or equal to the max pot. value
  } else if (brightness > VOLUME_TWO_THIRDS && brightness <= VOLUME_MAX) {
    
    /// Light green LED only
    analogWrite(RED_INDICATOR_PIN, 0);
    analogWrite(YELLOW_INDICATOR_PIN, 0);
    analogWrite(GREEN_INDICATOR_PIN, 255);

  } else {
    /// Otherwise turn all lights off
    analogWrite(RED_INDICATOR_PIN, 0);
    analogWrite(YELLOW_INDICATOR_PIN, 0);
    analogWrite(GREEN_INDICATOR_PIN, 0);
  }
}

/**
 * @brief Get the Volume value from the analog object
 * 
 * @param pot_pin 
 * @return byte 
 */
byte getVolume(int pot_pin) {

  /// Retrieve pot value
  int pot_value = mozziAnalogRead(pot_pin);

  /// Map the value value based on the pot value
  return map(pot_value, POT_MIN, POT_MAX, VOLUME_MIN, VOLUME_MAX);
}

/**
 * @brief helper function that sets the values and light the LED lights based on the sensors
 * 
 */
void detectSensors() {

  /// Retrieve the left sensor's duration & convert it to the distance(cm)
  left_duration = getSensorDuration(LEFT_TRIGGER_PIN, LEFT_ECHO_PIN);
  left_distance = left_duration * 0.034 / 2;

  /// light the LED based on the left sensor's distance
  pulseLights(LEFT_LEDS_PIN, left_distance);


  /// Retrieve the right sensor's duration & convert it to the distance(cm)
  right_duration = getSensorDuration(RIGHT_TRIGGER_PIN, RIGHT_ECHO_PIN);
  right_distance= right_duration * 0.034 / 2;
  
  /// light the LED based on the right sensor's distance
  pulseLights(RIGHT_LEDS_PIN, right_distance);

}

/**
 * @brief Pulse the parallel lights based on the sensor's distance
 * 
 * @param led_pin 
 * @param distance 
 */
void pulseLights(int led_pin, int distance) {

  /// map the ambience value based on the distance of the sensor
  int ambience = map(distance, SENSOR_MIN, SENSOR_MAX, SOUND_MIN, SOUND_MAX);

  /// if the less than the max distance, light up the LED based on the ambient value
  if (distance < MAX_DISTANCE) {    
      analogWrite(led_pin, SOUND_MAX - ambience);
  
  /// Otherwise turn off the LED
  } else {
    analogWrite(led_pin, 0);
  }
} 

/**
 * @brief Get the Sensor Duration value
 * 
 * @param trig_pin 
 * @param echo_pin 
 * @return unsigned long 
 */
unsigned long getSensorDuration(int trig_pin, int echo_pin) {
  /// Clear the trigger pin value & retrieve the instance value for the echo pin to record
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig_pin, LOW);

  /// Retrieve the the recorded duration value
  return pulseIn(echo_pin, HIGH);
}
