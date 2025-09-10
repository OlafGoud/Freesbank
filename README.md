# Internship Sens2Sea 3D Printer

**Sens2Sea is a small business that develops radar technology. The business is run by Geert Mosterdijk and a few partners.**(wie) 

## Radar
Radar is a system that emits radio waves from a transmitter. Those waves travel through the air until they contact an object. Then they are reflected or scatterd in many directions. Some waves will be absorbed by or penetrate the target. Materials with good electrical conductivity like metals, seawater and wet ground reflect the radar waves especially well. The receivers are usually in the same location as the transmitter. They capture the radar waves but the reflected waves are usually very weak so they need to be amplified.

Radio waves are weakly absorbed by the medium they go through. That makes them sutable for relatively long range detection of objects. Other wavelengths like visable light, infrared light and ultra violet light are to much weakend by most mediums. Wheather like fog, clouds, snow and rain that block those other wavelengths are usually transparent  to radiowaves. Certain radiowaves are absorbed by those mediums but they are avoided for radar technology.

Radar beams that are emited by a transmitter will go in all directions and that is not optimal for long range detection. That's why antennas are used for radars. They redirect the waves to the desired direction. A lot of antennes are shaped like a parabola because that shape directes the waves always in the same direction regardless of where they hit the parabola. **To make those antennes in a different size than a 'standard' size is considerable more expensive so Sens2Sea want to print them with a 3D printer. 3D printers are usually small compared to the antennes so you print them in pieces.**(waarom, wat, wanneer) But if there are minor mistakes in the process of attaching all pieces together the antenne does not work as intended. The 3d printer that needs to be build must be able to print those antennes accurately in one piece.

## 3D printer.
**The printer mus be able to print in one piece. That's why the work area is 4 meter long, 1 meter wide and 0.40 meter high. The position of the head of the machine needs to be known to print without impacting the shape. That needs to be done with sensors. The base of the program is the opensource project grbl. It does not include the capabilities of determin the exact location so the grbl code needs to be modified. The commands are given via the serial monitor by a tool named 'Universal Gcode Sender'** (hoe)

Probleemstelling:
- voor wie is het een probleem
- wat is het probleem
- waarom is het een probleem
- wanneer is het een probleem
- hoe los je het op

analyse:
- product: bestaande oplossing, product
- mensen: stakeholders
- processen: welke processen

functioneel = wat
non functioneel = hoe



zijn er deelproducten bij het bedrijf?
er staan

scope:
lengte en breedte
realitstisch
grenzen van de scope





closed loop stepper driver control board???


## Info
### Project owner
Geert Mosterdijk  
[Website](https://sens2sea.com/)

### Contributors
- Olaf Goudriaan (1071349)

## sources
https://github.com/gnea/grbl/wiki | https://github.com/gnea/grbl
https://en.wikipedia.org/wiki/Radar#Radar_signal