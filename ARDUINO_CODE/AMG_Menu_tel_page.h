#include "defines.h"
#ifdef AMG_MENU_D
//#define TELEPHONE_DISPLAY_H

#include "ic_display.h"
//#include "Bluetooth.h"
#include "Engine.h"

//const char * const PROGMEM CARRIER_UNKNOWN = "Unk. Carrier";

class AMG_MENU {
    public:
        AMG_MENU(IC_DISPLAY *d, ENGINE_DATA *eng);
        void update();
        //char carrier[20] = {"IVAN K"};
        //void setCarrier(char* c);
    private:
        bool isInPage = false;
        IC_DISPLAY *display;
        ENGINE_DATA *engine;
        unsigned long lastUpdateBody = 0L;
        unsigned long lastUpdateHeader = 0L;
        //void createHeader(const char * src);
        //BLUETOOTH *bluetooth;
};
#endif