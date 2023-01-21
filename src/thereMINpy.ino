#include <math.h>
#include <MozziGuts.h>
#include <Oscil.h> // oscillator
#include <tables/cos2048_int8.h> // table for Oscils to play
#include <AutoMap.h> // maps unpredictable inputs to a range





/// Constant min & max values for the carrier frequency when inputting to the AutoMap
const int MIN_CARRIER_FREQ = 22;
const int MAX_CARRIER_FREQ = 440;

AutoMap kMapCarrierFreq(0,1023,MIN_CARRIER_FREQ,MAX_CARRIER_FREQ);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);

/// Constant min & max values for the dynamic intensity when inputting to the AutoMap
const int MIN_INTENSITY = 700;
const int MAX_INTENSITY = 10;

AutoMap kMapIntensity(0,1023,MIN_INTENSITY,MAX_INTENSITY);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);

/// Default Pin for the LDR Component 
const int LDR_PIN = 0;



/// Modifier for Harmonics
int mod_ratio = 3;


/// Global controller values 
long fm_intensity; // calculated value of the intensity value
int ldr_value; // value from the LDR Analog Read

void setup()
{
  // Initialize Communication Serial
  Serial.begin(115200);

  startMozzi();
}

/**
 * @brief Observes the controller's actions
 * 
 */
void updateControl(){
  

  /// 
  ldr_value = mozziAnalogRead(LDR_PIN);
    
  /// map the knob to carrier frequency
  int carrier_freq = kMapCarrierFreq(ldr_value);

  /// calculate the modulation frequency to stay in ratio
  int mod_freq = carrier_freq * mod_ratio;

  /// set the FM oscillator frequencies to the calculated values
  aCarrier.setFreq(carrier_freq);
  aModulator.setFreq(mod_freq);

  
  /// print the value to the Serial monitor for debugging
  Serial.print("light_level: ");
  Serial.print(ldr_value);

  fm_intensity = kMapIntensity(ldr_value);

  Serial.print(", fm_intensity: ");
  Serial.println(fm_intensity);


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
  return aCarrier.phMod(modulation);
}

void loop() {
  audioHook();

}
