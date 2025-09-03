https://github.com/gnea/grbl

to setup:
Download zip from git.
unzip the code and zip the grbl folder in ./grbl-master/

Go to arduinoIDE and go to 'sketch'->'include library'->'add .zip library' and select the ziped grbl code.

Go to file->examples->grbl->grblUpload and run the code without changing it.
Open the serial monitor on 115200 Baud and you will see the start message.




Change it to compare the sensor input with the counted steps to validate. If it is not correct change the planning
or
Make closed loop with motor drivers that converte the position to the right location.


## Commands
- $ -> help
- $I -> info
- ? -> gives location and positions.


Motor driver: https://wiki.keyestudio.com/Ks0160_keyestudio_CNC_Shield_V3
https://www.amazon.nl/-/en/STEPPERONLINE-Stepper-Pancake-Degrees-Printer/dp/B0B93PNYCP

Zet de motor op 12 Volt en 2-3 amp
Snelheid naar 200 -> F200