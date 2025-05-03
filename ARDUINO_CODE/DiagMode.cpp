#include "DiagMode.h"


DIAG_MODE::DIAG_MODE(AUDIO_DISPLAY *disp, ENGINE_DATA *eng) {
    this->display = disp;
    this->engine = eng;
}


// wrote two updateUI functions one for manual and one for automatic gearbox
// because manual does not have ATF Temp so one screen is blank

#ifdef MANUAL_GEARBOX
void DIAG_MODE::updateUI() {
    //display->setSymbols(IC_SYMB_UP_ARROW, IC_SYMB_PLAY);
    switch (this->displayPage)
    {
    case 1:
        display->setHeader("AMG MENU", IC_TEXT_FMT_CENTER_JUSTIFICATION | IC_TEXT_FMT_HIGHLIGHTED , 500);
        display->setBody("SCROLL", IC_TEXT_FMT_CENTER_JUSTIFICATION | IC_TEXT_FMT_HIGHLIGHTED , 500);
        break;
    case 2:
        display->setHeader("Est Gear", IC_TEXT_FMT_LEFT_JUSTIFICATION, 500);
        display->setBody(engine->getGearingManual(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 250);
        break;
    case 3:
        display->setHeader("BATT V", IC_TEXT_FMT_LEFT_JUSTIFICATION, 500);
        display->setBody(engine->getVBatt(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 250);
        break;
    case 4:
        display->setHeader("Eng Temp", IC_TEXT_FMT_LEFT_JUSTIFICATION, 500);
        display->setBody(engine->getCoolantTemp(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 500);
        break;
    case 5:
        display->setHeader("Oil Temp", IC_TEXT_FMT_LEFT_JUSTIFICATION, 500);
        display->setBody(engine->getOilTemp(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 500);
        break;
    case 6:
        display->setHeader("MAF Temp", IC_TEXT_FMT_LEFT_JUSTIFICATION, 500);
        display->setBody(engine->getIntakeTemp(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 500);
        break;
    case 7:
        display->setHeader("Fuel usage", IC_TEXT_FMT_LEFT_JUSTIFICATION, 500);
        display->setBody(engine->getConsumption(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 500);
        break;
    case 8:
        #ifdef REGION_UK
            display->setHeader("MPG Live (UK)", IC_TEXT_FMT_LEFT_JUSTIFICATION, 500);
        #elif defined(REGION_US)
            display->setHeader("MPG Live (US)", IC_TEXT_FMT_LEFT_JUSTIFICATION, 500);
        #else
            display->setHeader("INST CONS", IC_TEXT_FMT_LEFT_JUSTIFICATION, 500);
        #endif
        display->setBody(engine->getMPG(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 500);
        break;
    case 9:
        display->setHeader("Oil Level", IC_TEXT_FMT_LEFT_JUSTIFICATION, 500);
        display->setBody(engine->getOilLevel(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 500);
    default:
        break;
    }
}
#else
void DIAG_MODE::updateUI() {
    //display->setSymbols(IC_SYMB_UP_ARROW, IC_SYMB_PLAY);
    switch (this->displayPage)
    {
    case 1:
        display->setHeader("DIAG MODE", IC_TEXT_FMT_CENTER_JUSTIFICATION , 2000);
        display->setBody("SCROLL", IC_TEXT_FMT_CENTER_JUSTIFICATION | IC_TEXT_FMT_HIGHLIGHTED , 250);
        break;
    case 2:
        display->setHeader("Tar/Cur gear", IC_TEXT_FMT_LEFT_JUSTIFICATION, 2000);
        display->setBody(engine->getGearingAuto(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 250);
        break;
    case 3:
        display->setHeader("ATF Temp", IC_TEXT_FMT_LEFT_JUSTIFICATION, 2000);
        display->setBody(engine->getTransmissionTemp(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 250);
        break;
    case 4:
        display->setHeader("Oil Temp", IC_TEXT_FMT_LEFT_JUSTIFICATION, 2000);
        display->setBody(engine->getOilTemp(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 250);
        break;
    case 5:
        display->setHeader("BATT V", IC_TEXT_FMT_LEFT_JUSTIFICATION, 2000);
        display->setBody(engine->getVBatt(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 250);
        break; 
    case 6:
        display->setHeader("Eng Temp", IC_TEXT_FMT_LEFT_JUSTIFICATION, 2000);
        display->setBody(engine->getCoolantTemp(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 250);
        break;
    case 7:
        display->setHeader("MAF Temp", IC_TEXT_FMT_LEFT_JUSTIFICATION, 2000);
        display->setBody(engine->getIntakeTemp(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 250);
        break;
    case 8:
        display->setHeader("Fuel usage", IC_TEXT_FMT_LEFT_JUSTIFICATION, 2000);
        display->setBody(engine->getConsumption(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 250);
        break;
    case 9:
        #ifdef REGION_UK
            display->setHeader("MPG Live (UK)", IC_TEXT_FMT_LEFT_JUSTIFICATION, 2000);
        #elif defined(REGION_US)
            display->setHeader("MPG Live (US)", IC_TEXT_FMT_LEFT_JUSTIFICATION, 2000);
        #else
            display->setHeader("INST CONS", IC_TEXT_FMT_LEFT_JUSTIFICATION, 2000);
        #endif
        display->setBody(engine->getMPG(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 250);
        break;
    case 10:
        display->setHeader("Oil Level", IC_TEXT_FMT_LEFT_JUSTIFICATION, 2000);
        display->setBody(engine->getOilLevel(), IC_TEXT_FMT_CENTER_JUSTIFICATION, 250);
    default:
        break;
    }
}


#endif

void DIAG_MODE::nextDiagPage() {
    this->displayPage++;
    if (this->displayPage > MAX_DIAG_PAGES) {
        this->displayPage = 1;
    }
}

void DIAG_MODE::prevDiagPage() {
    this->displayPage--;
    if (this->displayPage == 0) {
        this->displayPage = MAX_DIAG_PAGES;
    }
}