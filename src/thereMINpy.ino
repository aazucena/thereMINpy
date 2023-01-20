const int ldrPin = A0;
const int speakerPin = 5;

int ldrSensorValue = 0;

int sensorMin = 42;
int sensorMax = 950;

int freqMin = 10;
int freqMax = 1500;

void setup()
{
  // Initialize Communication Serial
  Serial.begin(9600);
}

void loop()
{
  // read analog values from pin with LDR Sensor
  ldrSensorValue = analogRead(ldrPin);

  if (ldrSensorValue == 0)
  {
    // Stop playing the sound
    noTone(speakerPin);
  }
  else
  {
    // Map the values to the LDR
    int ldrMappedValue = map(ldrSensorValue, sensorMin, sensorMax, freqMin, freqMax);

    // Play the tone on LDR
    tone(speakerPin, ldrMappedValue);

    // print the results to the serial monitor:
    Serial.print("sensor = ");
    Serial.println(ldrSensorValue);
  }
}