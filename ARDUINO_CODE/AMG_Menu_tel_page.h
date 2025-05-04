#include "defines.h"
#ifdef AMG_MENU_D

#include "ic_display.h"
#include "Engine.h"


#define MAX_AMG_PAGES 2

class AMG_MENU {
    public:
        AMG_MENU(IC_DISPLAY *d, ENGINE_DATA *eng);
        void updateAMG();
        void nextAMGPage();
        void prevAMGPage();
    private:
        void updatePage1(int frequency);
        void updateGear(int frequency);
        void updatePage2(int frequency);
        bool isInPage = false;
        IC_DISPLAY *display;
        ENGINE_DATA *engine;
        unsigned long lastUpdateBody = 0L;
        unsigned long lastUpdateHeader = 0L;
        uint8_t displayPage = 1;
};
#endif