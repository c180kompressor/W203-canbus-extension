#ifndef ENGINE_H

#define ENGINE_H

#include "defines.h"
#include "can_comm.h"
#include <stdio.h>



const char * const PROGMEM ENGINE_OFF = "Engine off";
const char * const PROGMEM UNKNOWN_VAL = "Unknown";


const char * const PROGMEM GEAR_NEUTRAL = "Neutral";
const char * const PROGMEM GEAR_REVERSE = "Reverse";

#ifndef MANUAL_GEARBOX
const char * const PROGMEM GEAR_PARK = "Park";
const char * const PROGMEM GEAR_REVERSE_2 = "Reverse 2";

#else
  // Vehicle parameters
  const float PROGMEM finalDriveRatio = 3.27;         // Final drive ratio (replace with your own)(https://www.automobile-catalog.com/)
  const float PROGMEM wheelDiameterMeters = 0.635;    // Wheel diameter (calculate based on tyre)(https://tiresize.com/calculator/)
  // Gear ratios for forward gears (replace with your own ratios)(https://www.automobile-catalog.com/)
  const float PROGMEM gearRatios[] = {4.459, 2.614, 1.723, 1.245, 1.0, 0.838};
  const int PROGMEM numGears = 6; // for now set manualy
  // Tolerance for RPM matching (e.g., ±50 RPM)
  const float PROGMEM rpmMargin = 50.0;
#endif

const char * const PROGMEM FUEL_CONSUMP_INF = "Inf";
const char * const PROGMEM FUEL_CONSUMP_0 = "0.00";

class ENGINE_DATA{

    public:
        ENGINE_DATA();
        void readFrame(can_frame *f);
        const char* getTransmissionTemp();
        const char* getGearingAuto();
        #ifdef MANUAL_GEARBOX
        const char* getGearingManual();
        #endif
        const char* getIntakeTemp();
        const char* getCoolantTemp();
        const char* getConsumption();
        const char* getOilTemp();
        const char* getMPG();
        boolean engineOn = false;
    private:
        char buffer[20] = {0x00};
        uint8_t transmission_temp = 0xFF;
        int speed_km = 0;
        #ifdef MANUAL_GEARBOX
        int rpm = 0;
        uint8_t ReverseEngaged = 0xFF;
        #endif
        uint8_t targetGear = 0xFF;
        uint8_t actualGear = 0xFF;
        uint8_t coolant_temp = 0xFF;
        uint8_t intake_temp = 0xFF;
        uint8_t oil_temp = 0xFF;
        unsigned long lastMpgTime = 0;
        long consumption = 0xFFFF;
};

#endif

