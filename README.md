An inexpensive DDS Signal generator based on the AD9851 module to generate sign wave of up to 70MHz (20-30MHz realistically). The module also has a built-in reference square wave up to 1 MHz, this works independent of the sign wave and the duty cycle is controlled through a potentiometer built in the module itself.

The project utilizes an Arduino Nano, I am using a clone version that can be purchased for just a couple of dollars. I am also using a rotary encoder to control the signal frequency for the sign-wave. I am using the KTM-S1201 7 Segment LCD display only because it's the only display I had at hand (It's not the best option but works). The LCD display requires a variable resistor to control the display contrast.

Bill of material

* AD9851 DDS module $12
* Arduino Nano $2
* KTM-S1201 7 Segment LCD $1.20
* Rotary encoder $0.51
* potentiometer (10K)

Total cost approximately $16-$20

Code requires KTMS1201 library by MCUdude (I am using version 1.0.1 that was installed through the Arduino IDE)

Detailed instruction on Instructable :  http://www.instructables.com/id/Cheap-DDS-Signal-Generator-Using-ADS9851-and-Ardui/

Video on youtube : https://youtu.be/jTeOIWke-dM
