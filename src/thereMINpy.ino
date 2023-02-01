#include <math.h>
#include <MozziGuts.h>
#include <Oscil.h>               // oscillator
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <EventDelay.h>
#include <Smooth.h>
#include <AutoMap.h>             // maps unpredictable inputs to a range
#define CONTROL_RATE 64



/// Constant values based on the distance the ultrasonic sensor's detection ranged distance

const int MIN_DISTANCE = 0;       // cm
const int MAX_DISTANCE = 80;      // cm
const int LIMITED_DISTANCE = 100; // cm
const int MOD_DISTANCE = 1;

/// Constant min & max values for the carrier frequency when inputting to the AutoMap
const int MIN_CARRIER_FREQ = 440;
const int MAX_CARRIER_FREQ = 50;

// AutoMap kMapCarrierFreq(0,1023,MIN_CARRIER_FREQ,MAX_CARRIER_FREQ);
AutoMap kMapSine(0, 1023, MIN_CARRIER_FREQ, MAX_CARRIER_FREQ);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> bSin(SIN2048_DATA);



Smooth <int> kSmoothFreq(0.975f);


/// Default Analog Pin for the LDR Component
const int POT_PIN = 0;

/// Default Digital Pins for the Ultrasonic Sensor

const int RED_INDICATOR_PIN = 2;
const int YELLOW_INDICATOR_PIN = 3;
const int GREEN_INDICATOR_PIN = 4;

const int RIGHT_LEDS_PIN = 5;
const int LEFT_LEDS_PIN = 6;

const int POT_MIN = 1023;
const int POT_MAX = 0;

const int SENSOR_MIN = 326;
const int SENSOR_MAX = 4;

const int SOUND_MIN = 0;
const int SOUND_QUARTER = 64;
const int SOUND_HALF = 127;
const int SOUND_THREE_QUARTERS = 191;
const int SOUND_MAX = 255;

const int RIGHT_TRIGGER_PIN = 10;
const int RIGHT_ECHO_PIN = 11;
const int LEFT_TRIGGER_PIN = 12;
const int LEFT_ECHO_PIN = 13;

const int VOLUME_MIN = 0;
const int VOLUME_THIRDS = 85;
const int VOLUME_TWO_THIRDS = 170;
const int VOLUME_MAX = 255;

/// Modifier for Harmonics
int mod_ratio = 3;

/// Global controller values
long fm_intensity; // calculated value of the intensity value

byte volume; // volume variable based on byt data type

unsigned long left_duration;
int left_distance;
unsigned long right_duration;
int right_distance;

void setup()
{
  // Initialize Communication Serial
  Serial.begin(9600);
  // Serial.begin(115200);

  pinMode(RED_INDICATOR_PIN, OUTPUT);
  pinMode(YELLOW_INDICATOR_PIN, OUTPUT);
  pinMode(GREEN_INDICATOR_PIN, OUTPUT);
  pinMode(RIGHT_LEDS_PIN, OUTPUT);
  pinMode(LEFT_LEDS_PIN, OUTPUT);

  pinMode(LEFT_TRIGGER_PIN, OUTPUT);  // Initialize the pin withe ultrasonic sensor's trigger pin
  pinMode(LEFT_ECHO_PIN, INPUT);      // Initialize the pin withe ultrasonic sensor's echo output  pin
  pinMode(RIGHT_TRIGGER_PIN, OUTPUT); // Initialize the pin withe ultrasonic sensor's trigger pin
  pinMode(RIGHT_ECHO_PIN, INPUT);     // Initialize the pin withe ultrasonic sensor's echo output  pin
  startMozzi(CONTROL_RATE);
}

/**
 * @brief Observes the controller's actions
 *
 */
void updateControl()
{
  indicateVolume(POT_PIN);

  /// Retrieve the sensor's detection distance based on duration
  detectLeftSensor(LEFT_TRIGGER_PIN, LEFT_ECHO_PIN, LEFT_LEDS_PIN);
  detectRightSensor(RIGHT_TRIGGER_PIN, RIGHT_ECHO_PIN, RIGHT_LEDS_PIN);

  // ultrasonic_value = ultrasonic_value <= 0 && ultrasonic_value > 100 ? 0 : ultrasonic_value;
  // detectSensorByDistance(left_sensor_value, POT_PIN);
  detectSensorByDistance(POT_PIN);
}

/**
 * @brief Updates the audio frequency code that should return the integer between -244 to 243
 *
 * @return int
 */
int updateAudio() {
  return ((aSin.next() + bSin.next())* volume) >> 8;
}

void loop()
{
  audioHook();
}

void detectSensorByDistance(int pot_pin)
{


  bool hasSound = (left_distance > 0 && left_distance < MAX_DISTANCE) || (right_distance > 0 && right_distance < MAX_DISTANCE);

  if (hasSound == true) {
    /// Set the volume value based on the potentionmeter's value
    volume = getVolume(pot_pin);
  } else {
    volume = (volume > 10) ? volume - 2 : 0;     
  }

  /// if the sensor detects at a certain distance, play the audio
  if (left_distance > 0 && left_distance < MAX_DISTANCE) {

    /// map the knob to carrier frequency
    int sine_freq = kMapSine(left_distance);
    
    int smoothed_freq = kSmoothFreq.next(sine_freq);
    aSin.setFreq(smoothed_freq);
  }
  
  if (right_distance > 0 && right_distance < MAX_DISTANCE) {
    /// map the knob to carrier frequency
    int sine_freq = kMapSine(left_distance);
    
    int smoothed_freq = kSmoothFreq.next(sine_freq);
    bSin.setFreq(smoothed_freq);
  } 
}

void indicateVolume(int pot_pin)
{
  int pot_value = mozziAnalogRead(pot_pin);
  int brightness = map(pot_value, POT_MIN, POT_MAX, VOLUME_MIN, VOLUME_MAX);

  if (brightness >= VOLUME_MIN && brightness <= VOLUME_THIRDS)
  {
    analogWrite(RED_INDICATOR_PIN, 255);
    analogWrite(YELLOW_INDICATOR_PIN, 0);
    analogWrite(GREEN_INDICATOR_PIN, 0);
  }
  else if (brightness > VOLUME_THIRDS && brightness <= VOLUME_TWO_THIRDS)
  {
    analogWrite(RED_INDICATOR_PIN, 0);
    analogWrite(YELLOW_INDICATOR_PIN, 255);
    analogWrite(GREEN_INDICATOR_PIN, 0);
  }
  else if (brightness > VOLUME_TWO_THIRDS && brightness <= VOLUME_MAX)
  {
    analogWrite(RED_INDICATOR_PIN, 0);
    analogWrite(YELLOW_INDICATOR_PIN, 0);
    analogWrite(GREEN_INDICATOR_PIN, 255);
  }
  else
  {
    analogWrite(RED_INDICATOR_PIN, 0);
    analogWrite(YELLOW_INDICATOR_PIN, 0);
    analogWrite(GREEN_INDICATOR_PIN, 0);
  }

}

byte getVolume(int pot_pin)
{
  int pot_value = mozziAnalogRead(pot_pin);

  return map(pot_value, POT_MIN, POT_MAX, VOLUME_MIN, VOLUME_MAX);
}

void detectLeftSensor(int trig_pin, int echo_pin, int led_pin)
{
  left_duration = getSensorDuration(trig_pin, echo_pin);
  left_distance = left_duration * 0.034 / 2;

  int ambience = map(left_distance, SENSOR_MIN, SENSOR_MAX, SOUND_MIN, SOUND_MAX);

  if (left_distance < MAX_DISTANCE)
  {
    {
      analogWrite(led_pin, SOUND_MAX - ambience);
    }
  }
  else
  {
    analogWrite(led_pin, 0);
  }
}
void detectRightSensor(int trig_pin, int echo_pin, int led_pin)
{
  right_duration = getSensorDuration(trig_pin, echo_pin);
  right_distance = right_duration * 0.034 / 2;

  int ambience = map(right_distance, SENSOR_MIN, SENSOR_MAX, SOUND_MIN, SOUND_MAX);

  if (right_distance < MAX_DISTANCE)
  {
    {
      analogWrite(led_pin, SOUND_MAX - ambience);
    }
  }
  else
  {
    analogWrite(led_pin, 0);
  }
}

unsigned long getSensorDuration(int trig_pin, int echo_pin)
{
  /// Clear the trigger pin value
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(1000);
  /// Set Trigger Pin value for 2 Microseconds

  digitalWrite(trig_pin, LOW);

  return pulseIn(echo_pin, HIGH);
}
