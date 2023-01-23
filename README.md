# thereMINpy


## Description

Based on an existing instrument Thermine that reacts to the pitch based on the hand position between the instrument and the user. If the rate of the distances changes quickly, it becomes more chaotic. It also includes the volume dial that changes the volume of the instrument. The goal in mind for this device is to slowly move or be still onf the hand movement detected by the instrument

## Requirements
- ### Hardware
  - #### Parts
    - Arduino UNO Board
    - Jump Wires
    - Solderable Wires 
    - External Audio Speaker
    - Auxilary Cable
    - Alligator Clips
    - ~~LDR Sensor~~
    - Ultrasonic Sensor
    - Potentionmeter
    - ~~Ohm Resistors~~
      - ~~1K~~

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

## Changelog
- ### First Iteration
   - Planned to add the pulsing lights when the pitch changes
   - Planned to implement the white noise that increase/decreases when the pitch changes
   - Add the LDR Sensor to change the pitch of the sound


![iteration-1](docs\diagrams\thereMINpy_p1.png)

- ### Second Iteration
  - Cancel the plan to add the pulsing lights 
  - Add the volume dial to change the gain based on the potentionmeter's value
  - Replace the main sensor which is the LDR with Ultrasonic sensors
  - Make the pitch sound limit to the lower frequency from around 100Hz to 180Hz
  
  
![iteration-2](docs\diagrams\thereMINpy_p2.png)
- ### Third Iteration
  - 

## Contributions
- Aldrin Azucena
  - Arduino Developer
  - Videographer
  - Photographer
  - Musician

