#include "AMG_MENU_tel_page.h"


AMG_MENU::AMG_MENU(IC_DISPLAY *d, ENGINE_DATA *eng) {
    this->display = d;
    this->engine = eng;
}


void AMG_MENU::updateAMG() {
    if (IC_DISPLAY::current_page == IC_PAGE_TELEPHONE) {
        if (!isInPage) { // Not in page so init it now (first)
            this->isInPage = true;
            //display->initPage(IC_PAGE_TELEPHONE, "GEAR", IC_TEXT_FMT_LEFT_JUSTIFICATION, IC_SYMB_NONE, IC_SYMB_NONE, 4);
            this->updatePage1(1000);
            this->updateGear(250);
        } else {
            this->updateGear(250);
            switch(this->displayPage)
            {
                case 1:
                    this->updatePage1(1000);
                    break;
                case 2:
                    this->updatePage2(1000);
                    break;
                default:
                    break;
            }
        }
    }   else {
        isInPage = false;
    }
}

void AMG_MENU::updatePage1(int frequency) {
    if (millis() - this->lastUpdateBody >= frequency) {
        this->lastUpdateBody = millis();
        char line1[10];
        char line2[10];
        char line3[10];
        char line4[10];
        char line5[32];
        char line6[32];
        sprintf(line1,"ENGT %s",engine->getCoolantTemp());
        sprintf(line2,"OILT %s",engine->getOilTemp());
        sprintf(line3,"MAFT %s",engine->getIntakeTemp());
        sprintf(line4,"OIL %s",engine->getOilLevel());
        sprintf(line5, "%s %s", line1, line2);
        sprintf(line6, "%s %s",line3,  line4);
        char *text[2] = { line5,line6 };
        display->setBodyTel(2, text);
    }
}

void AMG_MENU::updatePage2(int frequency) {
    if (millis() - this->lastUpdateBody >= frequency) {
        this->lastUpdateBody = millis();
        char line1[10];
        char line2[20];
        sprintf(line1,"BAT %sV",engine->getVBatt());
        sprintf(line2,"INST.CONS %s",engine->getMPG());
        char *text[2] = { line1,line2 };
        display->setBodyTel(2, text);
    }
}

void AMG_MENU::updateGear(int frequency) {
    if (millis() - lastUpdateHeader >= frequency){
        lastUpdateHeader=millis();
        char head[11];
        sprintf(head, "GEAR: %s", engine->getGearingManual());
        display->setHeader(IC_PAGE_TELEPHONE, head, IC_TEXT_FMT_LEFT_JUSTIFICATION | IC_TEXT_FMT_HIGHLIGHTED);
    }
}

void AMG_MENU::nextAMGPage() {
    this->displayPage++;
    if (this->displayPage > MAX_AMG_PAGES) {
        this->displayPage = 1;
    }
}

void AMG_MENU::prevAMGPage() {
    this->displayPage--;
    if (this->displayPage == 0) {
        this->displayPage = MAX_AMG_PAGES;
    }
}