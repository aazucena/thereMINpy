# thereMINpy


## Description

The theremin, an already-existing instrument that adjusts pitch depending on the user's hand location, served as the inspiration for this project. If the distance is close to its sensors, the LED lights on either side will shine brightly; otherwise, they will dim as far as the sensors can detect the distance. The instrument's volume dial, which may be used to change the volume, and its LED indicators are also present. The device's goal is to slowly move or remain constant on the hand movement sensed by the instrument while maintaining the proper balance of acceptable LED brightness and pitch.

## Requirements
- ### Hardware
  - #### Parts
    - Arduino UNO Board
    - Jump Wires
    - Solderable Wires 
    - External Audio Speaker
    - ~~Small Speaker~~
    - Auxiliary Cable
    - 3.5mm Audio Jack
    - ~~Alligator Clips~~
    - ~~LDR Sensor~~
    - Ultrasonic Sensor (x2)
    - Potentiometer
    - Cardboard
    - 2 Large Fabric
    - Resistors
      - 220 Ohms (x13)
    - LEDs
      - Green (x1)
      - Yellow (x1)
      - Red (x1)
      - White (x10)

- ### Software
  - Framework:
    - Arduino IDE
    - PlatformIO:
      - Dependencies
        - Python 3
        - C++
  - IDE:
    - VSCode
  - Language:
    - C++

## Iterations
- ### First
  - Include the handle that you can wave around the device
  - Planned to add the pulsing LED lights when the pitch changes
  - Planned to implement the white noise that increase/decreases when the pitch changes
  - Add the LDR Sensor to change the pitch of the sound
  - Have the device be in a small form factor
  - Attach the device with a small speaker

![iteration-1](designs\iteration_1.png)

- ### Second
  - Cancel the plan to add the pulsing lights 
  - Replace the main sensor which is the LDR with an ultrasonic sensor
  - Make the pitch sound limit to the lower frequency from around 100Hz to 180Hz
  - Upgrade the small speakers to bigger speakers
  - Change the device form factor to the larger size
  
![iteration-2](designs\iteration_2.png)
- ### Third (final)
  - Add the volume dial to change the gain based on the potentiometer's value
  - Reintegrate 10 pulsing white LEDs
  - Include an extra Ultrasonic Sensor
  - Replace the built-in speaker with a 3.5mm audio jack for the purpose of dynamically outputting sound from either speaker or headphones
  - Include a 2 arc structure as it can function either as the table legs or a leg rests for the instrument
  - Add 3 indicator lights for the volume dial potentiometer

![iteration-3](designs\iteration_3.png)

<br>
<br>
<br>
<br>
<br>

## Pseudocode

```c++
/// Plug the audio output to the audio jack
///
/// Power the thereMINpy device
///
/// While the device is turned on:
///   Device initializes the the two pins from each ultrasonic sensors
///
///   While the turning the potentiometer to the desired volume:
///     If the volume value is in range 1/3 of the max volume or equal to 0:
///       light the red LED
///     Else if the volume value is in range of 2/3 of the max volume:
///       light the yellow LED
///     Else:
///       light the green LED
///     End
///
///     If the designated volume has been set:
///       Set it to the volume value  
///       Stop turning the potentiometer knob
///     End
///   End
///   
///   While the ultrasonic sensors are observing
///     If both sensors don't detect any movement within the certain distance:
///       Volume is set to 0
///     Else:
///       If the left sensor detects movement:
///         Pulse the left LED lights with variable brightness based on the distance of the movement
///       End
///
///       If the right sensor detects movement:
///         Pulse the right LED lights with variable brightness based on the distance of the movement
///       End
///
///       Play the sound and pitch based on the distance of the detected movement
///     End
///   End
///
/// End
```

