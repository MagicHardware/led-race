# led-race
Minimalist car race with a strip of LEDs.
Connect two switches between common GND and pins 6 and 7 and one led strip ws2812 or ws2813 to  GND, + 5V and A0 to a arduino Nano. After sometime of notuse it will dispaly light efect on track. Branch Arduino_IDE have basics .ino project it is EOL(end of life), it have some fixes in implmentation compared to the original.   
Link to git repository: https://gitlab.com/oman17/led-race.   
Build instruction: https://create.arduino.cc/projecthub/gbarbarov/open-led-race-a0331a  
Link to original repostory: https://github.com/gbarbarov/led-race



___
#### HOW TO RUN:

0. Install Eclipse
1. Install arduino plugin in eclipse, HELP-> Eclispe Marketplace.
2. Add nessesary lib, HELP->Arduino Download Menager
platforms : Arduino AVR Boards, 
libraries: Adafruit NeoPixel
3. Project-> Properties -> Libraries , check all lib and click applay
4. Connect PC toarduino via USB cable, run Telemetry-> TelemetryViewer_v0.6.jar
5. Open Layout Telemetry_default.txt , set 5700 boudrate  , and connect it will display basic information about race

___
#### HOW TO PLAY: 
-  When program start it turns White if you press player 1 and player 2 buttom and hold it until light turn off you will set the obstacles. Now you control setting obstacles by player one buttom. You set : beggining, centger and end of obstacles, height and type ( red- LOOP, green-  RAMP). At the end it will turn green light two times,that mean it accept the obstacles. If youy press player one buttom it will end the seting of obstacles proces.
- If you don't press any buttom the race will begin. 
- The fastest you press the fasstes you run. Whit is the obtacles 
- When the race end it will turn color of the car color
- You can change eclipsenumber of loops in _NUMBER_OF_LOOP define, "main.h"
- Als progress and best time will be display in PC aplication

___
#### DEFINE EXPLAIN:  
#define _DEBUG_MODE		- debug using serial, can caouse memory RAM problem  
#define _DEBUG_GRAV_MAP	- send grav map to pc, catch with RealTerm , .txt->.csv, plot it with matlab  
#define _TELEMTRY_VIEW 	- use this wth telemtry_viewer (http://www.farrellf.com/TelemetryViewer/) , and open Layout Telemetry_default.txt , 5700 boudrate  
#define _BLUETHOOT		-  future extension, telemtry via bluthoot  
#define _OLED			- not ready future, use SSD1306 0.96 monochrome OLED to dispaly progress  
___
#### HOW TO ADD PLAYER:  
&nbsp;&nbsp;&nbsp;	Edit:  
&nbsp;&nbsp;&nbsp;&nbsp;	_NUMBER_OF_PLAYER   
&nbsp;&nbsp;&nbsp;&nbsp;	Color_player[]  
&nbsp;&nbsp;&nbsp;&nbsp;	Pin_player[]  

___
#### Libraries:  
- led-race : https://github.com/gbarbarov/led-race
- MemoryFree:  https://github.com/maniacbug/MemoryFree
- Adafruit: NeoPixel, GFX, SSD1306 (GFX,SSD1306 until problem with to less RAM this lib not nessesary)
- Adruino : Wire, SPI , SoftwareSerial, HID, EEPROM

___
#### Issue:  
When reset eclipse the library don't include anymore ?

___
#### TODO:  
-deoxyegen documentation

___
#### Other folder:  
- Matlab : Process data with matalb aplication, eg. plot gravity map (track) https://www.mathworks.com/products/matlab.html
- Visual Code : this is project in   visual studio code , you can copy file to this folderand use this software if you want, this use Arduino. After install plugins  pres F1, type: "Arduino: upload".
- Telemetry: this folder contains visualisation of race. It connect via USB cable to Windows PC . http://www.farrellf.com/TelemetryViewer/

___
#### License:  
GPLv3 