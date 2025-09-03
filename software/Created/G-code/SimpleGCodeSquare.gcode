G21 ; millimeters
G90 ; absolute coordinate
G17 ; XY plane
G94 ; units per minute feed rate mode
M3 S1000 ; Turning on spindle

; Go to safety height
G0 Z5

; Go to zero location
G0 X7 Y0
G0 Z0

; Create rectangle
G1 X30 Y20 F200
G1 X10 Y40
G1 X40 Y50
G1 X70 Y10
G1 X7 Y0


; Turning off spindle
M5
