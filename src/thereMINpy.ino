#include <math.h>
#include <MozziGuts.h>
#include <Oscil.h> // oscillator
#include <tables/cos2048_int8.h> // table for Oscils to play
#include <AutoMap.h> // maps unpredictable inputs to a range

/// Constant values based on the distance the ultrasonic sensor's detection ranged distance

const int MIN_DISTANCE = 0; // cm
const int MAX_DISTANCE = 30; // cm
const int LIMITED_DISTANCE = 100; // cm
const int MOD_DISTANCE = 1;


/// Constant min & max values for the carrier frequency when inputting to the AutoMap
const int MIN_CARRIER_FREQ = 22;
const int MAX_CARRIER_FREQ = 440;

// AutoMap kMapCarrierFreq(0,1023,MIN_CARRIER_FREQ,MAX_CARRIER_FREQ);
AutoMap kMapCarrierFreq(MIN_DISTANCE,MAX_DISTANCE <= LIMITED_DISTANCE ? MAX_DISTANCE*MOD_DISTANCE : LIMITED_DISTANCE,MIN_CARRIER_FREQ,MAX_CARRIER_FREQ);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);

/// Constant min & max values for the dynamic intensity when inputting to the AutoMap
const int MIN_INTENSITY = 700;
const int MAX_INTENSITY = 10;

// AutoMap kMapIntensity(0,1023,MIN_INTENSITY,MAX_INTENSITY);
AutoMap kMapIntensity(MIN_DISTANCE,MAX_DISTANCE <= LIMITED_DISTANCE ? MAX_DISTANCE*MOD_DISTANCE : LIMITED_DISTANCE,MIN_INTENSITY,MAX_INTENSITY);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);

/// Default Analog Pin for the LDR Component 
const int LDR_PIN = 0;
const int POT_PIN = 1;


/// Default Digital Pins for the Ultrasonic Sensor
const int TRIGGER_PIN = 10;
const int ECHO_PIN = 11;


/// Modifier for Harmonics
int mod_ratio = 3;


/// Global controller values 
long fm_intensity; // calculated value of the intensity value
int ldr_value; // value from the LDR Analog Read

byte volume; // volume variable based on byt data type

unsigned long ultrasonic_duration; // duration value recorded from the sensor
int ultrasonic_value; // calculated value of the distance between the target and the sensor

void setup()
{
  // Initialize Communication Serial
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT); // Initialize the pin withe ultrasonic sensor's trigger pin
  pinMode(ECHO_PIN, INPUT);// Initialize the pin withe ultrasonic sensor's echo output  pin
  startMozzi();
}

/**
 * @brief Observes the controller's actions
 * 
 */
void updateControl(){
  
  /// Retrieve the duration of the ultrasonic sensor
  ultrasonic_duration = getSensorDuration();

  /// Calculate the distance based on duration
  ultrasonic_value = ultrasonic_duration*0.034/2;

  // ultrasonic_value = ultrasonic_value <= 0 && ultrasonic_value > 100 ? 0 : ultrasonic_value;

  // ldr_value = mozziAnalogRead(LDR_PIN);

  int pot_value = mozziAnalogRead(POT_PIN);
  

  /// if the sensor detects at a certain distance, play the audio
  if (ultrasonic_value < MAX_DISTANCE) {

    /// Set the volume value based on the potentionmeter's value
    volume = map(pot_value, 0, 1023, 255, 0);

    /// map the knob to carrier frequency
    int carrier_freq = kMapCarrierFreq(ultrasonic_value);

    /// calculate the modulation frequency to stay in ratio
    int mod_freq = carrier_freq * mod_ratio;

    /// set the FM oscillator frequencies to the calculated values
    aCarrier.setFreq(carrier_freq);
    aModulator.setFreq(mod_freq);

    fm_intensity = kMapIntensity(ldr_value);
    
    /// print the value to the Serial monitor for debugging
    Serial.print("duration: ");
    Serial.print(ultrasonic_duration);

    
    Serial.print(", distance: ");
    Serial.print(ultrasonic_value);


    Serial.print(", fm_intensity: ");
    Serial.println(fm_intensity);
  } else {

    /// don't play the audio
    volume = 0;
  }

}

/**
 * @brief Updates the audio frequency code that should return the integer between -244 to 243
 * 
 * @return int 
 */
int updateAudio(){
  /// calculate the modulation fn frequency
  long modulation = fm_intensity * aModulator.next();
  /// return the phMod of the modulation fm frequency
  int sine = aCarrier.phMod(modulation);
  
  return (sine * volume)>>8; 
}

void loop() {
  audioHook();
}

/**
 * @brief Get the Sensor Duration object from the Trigger and Echo Pin of the Ultrasonic Sensor
 * 
 * @return unsigned long 
 */
unsigned long getSensorDuration() {
  /// Clear the trigger pin value
  
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(1);
  
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(100);
  /// Set Trigger Pin value for 2 Microseconds

  digitalWrite(TRIGGER_PIN, HIGH);

  return pulseIn(ECHO_PIN, HIGH);
}