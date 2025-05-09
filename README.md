# READ FIRST
This is an extension of RAND_ASH 's original project [W203-Canbus](https://github.com/rnd-ash/W203-canbus). If you are not familiar with it go check it first to learn how it works and what functionality it has. I have left the original readme page from his repo at the bottom of this one.

# Features added/changed
* Reading if reverse gear is engaged from the corresponding can message
* Reading if parking brake is engaged from corresponding can message
* Writing a manual transmission gear estimation function using speed, rpm, gear ratios and park and reverse information
* Defining a manual gearbox option for compilation, you can compile varibales and function for both manual and automatic separately
* Making diag mode for manual gearbox besides the one for automatic
* Reading battery voltage from the corresponding can message and adding it to diag mode
* Reading oil level from the corresponding can message and figuring out the oil level conversion formula and adding it to diag mode
* Calculating consumption(MPG) in l/100km and adding it into diag mode and adding it as an option for compilation
* Renaming the DIAG MODE to AMG MENU [demo here](https://youtube.com/shorts/wPJ_9cQJGuE?feature=share), inspired of the AMG MENU's in newer Benzes like 211,204,212..
* Utilising the 4 lines of the Telephone page to create a 2-page AMG-menu there instead of using the Audio page where moving with the up and down arrows is needed as seen in the video in the previous bullet point [demo here](https://youtube.com/shorts/EDKpwcX9LJM?si=Tk5T5PGbTE1oiOo7):
![](DSC_3554.JPG)
![](DSC_3555.JPG)
* Created a pcb to plug in both can-shields, the arduino, the bluetooth module and a DC-DC converter to hopefuly make the setup more neat and tidy. Waiting for the print of the PCB to try it out.

# Fixes that needed to be done to make it work
I found a few things needed to be changed/adapted to make it work on my car:
* FIX FOR ARDUINO CRASHING ALL THE TIME
>comment out everything related to bluetooth if you dont have bluetooth
* THE FIX FOR THE ARDUINO CRASHING WHEN RECEIVING CAN C FRAMES:
>A condition needs to be added when calling
```cpp
        eng->readFrame(readC)
		eng->readFrame(readB)
```
>in function 
```cpp
		void handleFrameRead()
```
>the condition is that
```cpp
		eng->readFrame(readC)
		eng->readFrame(readB)
```
>is only called when 
```cpp
		showDiagMode
```
>is true, otherwise since we are at arduino UNO
```cpp
		eng
```
>is not initialized and we are calling a function from just an empty pointer.
* THE FIX FOR WHEN ACTUALLY WRITING TO CAN AND GETTING ACKNOWLEDGMENT BACK BUT NO DISPLAY
>This was happening when ntg2 was not connected, and arduino canb was plugged into ntg2 can wires. I was able to send packages 24 26 and 29 and I was getting acknowledgments back from the IC. However I was not sending and receiving packages 20 and 21 to make the initialization sequence and I did not find in the arduino code where packages 20 and 21 are sent and received so plugging the ntg2 back in the car made sense and it made the text on IC start displaying reliably.

* THE FIX FOR THE SPEED NOT WORKING
>speed_km variable was always giving 0 and we were never ending up in the condition that would change it. The fix is that in the main file ARDUINO_CODE.ino this:
```cpp
		readB->can_dlc == 0x000C
```
>needs to be changed to this(makes sense right)
```cpp
	    readB->can_id == 0x000C
```
# FIGURING OUT OIL LEVEL
>This is how I figured out kind of accurate oil level display on my car  
>oil level(l) - 8bit value read from can  

>5.0 - 188  
>4.4 - 155  
>4.1 - 148  
>3.7 - 129  
>3.6 - 120  
>3.3 - 98  
>3.2 - 94  

>the formula  
>>level=value*0.02+1.3  
>seems to work alright for my car m271 engine with 5.5 liters oil in total. For different car it wouldnt work because the 8bit value for example for 6.5 liters is already more than 255, tryout for your car to find it.

# LIST OF FUTURE PLANS/IMPLEMENTATIONS
* for gearbox display - have to figure out some way to make it work better during accelerations. Now it seems that either the rpm or the speed is coming in faster than the other and during harder acceleration the arduino thinks it is in neutral. At constant speed or deceleration or acceleration above 50-60km/h there is not problem. It seems to be that either the RPM or the speed is coming in with some dalay or maybe the calculation that I am doing is too slow. Current implementation demo [here](https://youtube.com/shorts/oqdt2RCeWmY?feature=share).

* Once the pcb arrives I will try the bluetooth module as well.
* Look for some way to obtain the boost pressure of the charge air, which would be a very cool addition to the DIAG MODE/AMG MENU. The boost pressure should be related to the MAP sensor reading and this is available on the diagnostic can that the ELM327 devices can read so it should be obtainable somehow
* Look for a way to obtain the oil pressure and include it in the IC display along with the other engine information.
* Create some kind of 3d printable box to put all of the hardware inside
* Connect the Arduino to the LPG indicator/switch and make some kind of display on the IC which is showing: the estimated LPG left in the tank, the estimated range with the LPG left, an indication if the car is driving on LPG or petrol, potentialy make some kind of correction for the higher consumption on LPG compared to gasoline
* If the previous point is working maaybe also try to turn the LPG ON or OFF from the IC instead of the button on the switch/indicator - inspired by the factroy CNG instalations on the W211 and W212 and their display implementations
* Connect external tyre pressure monitoring sensors and some wireless receiver to the arduino and create a screen which imitates the factory TPMS or potentially even mimic the factory TPMS signals and make them display on the factory screen page for TPMS, saving the hassle of finding the right OEM receiver and connecting it to the ESP modules and more potential problems.
* Include a screen in the diag menu which can show if the inverter of my car is turned on or not and potentialy include a way to turn it on or off.

# ORIGINAL README PAGE FROM [W203-CANBUS](https://github.com/rnd-ash/W203-canbus)

## Mercedes canbus project for older W203/W209/W211 cars
A proof of concept application using an Arduino and Android phone to add extra features to these older Mercedes cars by tapping into their systems via canbus.

## This project is now no longer my primary project for the car
If you want to fix things, feel free to submit pull requests to this repository

If you have an android head unit in your car, feel free to checkout a much more feature rich version of this that runs on the headunit [here](https://github.com/rnd-ash/MBUX-Port)

## DEMO VIDEOS
Can be accessed via [this link](https://www.youtube.com/playlist?list=PLxrw-4Vt7xtt3VkDx_Z35n4SdYG9C3N7i)

### Project status
So far, the project has grown well out of proportion and has ended up being a way to modernise the W203 slightly. The arduino now requires a HC-06 bluetooth module, which then connects to the android application running on an android device. This now serves to give 'hands free music control' to the car, the steering wheel button presses in the AUX Page are now relayed to the phone in order to change tracks. The phone also then tells the Arduino via bluetooth what track is playing. The arduino will then send can frames to the IC Cluster to display the current track playing.

### Controls enabled via the android application
* Turn on / off indicators and hazard lights
* Set custom interval for indicator clicks (faster / slower)
* Unlock doors
* Lock doors
* Toggle ESP
* Retract rear headrests
* Display custom text on the IC display
* Set scroll speed of text across the IC display (only if text > 8 characters)

### Repo contents:
* IC_CUSTOM_TEXT - Contains Arduino code for project
* androidApp - Contains Android app code for project
* can_read - Simple tool to read can frames from car
* Replayer - Replays data captured by can_read in realtime. (Used to analyze can frames and work out functions of frames)

### Reverse engineering documentation
See [here](https://docs.google.com/spreadsheets/d/1krPDmjjwmlta4jAVcDMoWbseAokUYnBAHn67pOo00C0/edit?usp=sharing)

### Hardware setup
![Arduino setup](setup.png?raw=true "Arduino hardware setup")

Currently, I have 2 MCP2515 modules. 1 with a 8Mhz Clock thats connected to CAN C, and 1 with a 16Mhz clock that is connected to Can B. (I found out that Can B is NOT compatible with the 8Mhz clock due to its odd bitrate of 83.3Kbps). If you end up using 2 modules with 16Mhz clocks, please change the following line in IC_CUSTOM_TEXT/canbuscomm.cpp:
```cpp
this->canC->setBitrate(CAN_500KBPS, MCP_8MHZ);
```
to
```cpp
this->canC->setBitrate(CAN_500KBPS);
```
Failing to do so may result in some absolutely wild behaviour with the rest of the car!

#### Connection to Can B
I am using the X30/7 connector to connect to Canbus B. This is a hub (with multiple wires coming out of it) located on the panel below the drivers steering wheel. you will notice it has a few free slots. This is where I have ran the cables to.

#### Connection to Can C
Canbus C is a little more difficult. I could not find a connector like for Bus B. Instead, I ran wires to the back of the Instrument cluster. Connected to the instrument are 2 green wires. The Solid green wire is CanH+, and the green wire with a white stripe is CanL-.

#### Project Limitations
Currently, I have only worked out how to display 1 line of maximum 8 characters on the IC Display. This means that if the track name is more than 8 characters, the Arduino will shift the string by 1 every 150MS, resulting in the text scrolling across the screen to display a longer message. However, the IC Displays response time is appaling, resulting in somtimes a slushy text animation. idealy, I would like to find out how to lower the size of the text on the display in order to display more than 8 characters at a time
