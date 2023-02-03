# thereMINpy


## Description

A theremin, but with a nice touch. It is a portable music instrument that adjusts the pitch depending on the user's hand location, and it serves as the inspiration for this project. Whenver you play music instruments with the pitch, I always find it very calm and relaxing when playing them which is why I chose to do this project for the topic on meditation. If the distance is close to its sensors, the LED lights on either side will shine brightly; otherwise, they will dim as far as the sensors can detect them. The music instrument's dial changes the volume and indicates its level through the 3 designated LEDs. The intended goal for this device is to slowly move or remain constant on the hand movement sensed by the instrument while maintaining the proper balance of acceptable LED brightness and pitch from both hands. 

## Requirements
- ### Hardware
  - #### Parts
    - Terminals
    - Electric Tape
    - Zip Ties
    - Arduino UNO Board
    - Jump Wires
    - Solderable Wires 
    - External Audio Speaker
    - ~~Small Speaker~~
    - Auxiliary Cable
    - 3.5mm Audio Jack
    - Alligator Clips
    - ~~LDR Sensor~~
    - Ultrasonic Sensor (x2)
    - Potentiometer
    - Cardboard
    - Resistors
      - 220 Ohms (x13)
    - LEDs
      - Green (x3
      - Yellow (x3)
      - Red (x1)
      - Blue (x5)

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


## Instructions
1. Turn on the thereMINpy
2. Input the audio source to the 3.5mm audio jack socket
3. Set the volume to the desired level
4. Place the hands above the sensors
5. Freely play around by moving your hands up & down

## Diagram

![diagram](/docs/diagrams/thereMINpy.png)

## Iterations
- ### First
  - Include the handle that you can wave around the device
  - Planned to add the pulsing LED lights when the pitch changes
  - Planned to implement the white noise that increase/decreases when the pitch changes
  - Add the LDR Sensor to change the pitch of the sound
  - Have the device be in a small form factor
  - Attach the device with a small speaker

![iteration-1](/docs/designs/iteration_1.png)

- ### Second
  - Cancel the plan to add the pulsing lights 
  - Replace the main sensor which is the LDR with an ultrasonic sensor
  - Make the pitch sound limit to the lower frequency from around 100Hz to 180Hz
  - Upgrade the small speakers to bigger speakers
  - Change the device form factor to the larger size
  
![iteration-2](/docs/designs/iteration_2.png)
- ### Third (final)
  - Add the volume dial to change the gain based on the potentiometer's value
  - Reintegrate 10 pulsing white LEDs
  - Include an extra Ultrasonic Sensor
  - Replace the built-in speaker with a 3.5mm audio jack for the purpose of dynamically outputting sound from either speaker or headphones
  - Include a 2 arc structure as it can function either as the table legs or a leg rests for the instrument
  - Add 3 indicator lights for the volume dial potentiometer

![iteration-3](/docs/designs/iteration_3.png)

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

## Contributions
- Aldrin Azucena
  - Arduino Developer
  - Videographer
  - Photographer

