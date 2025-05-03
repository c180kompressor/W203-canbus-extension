#include "AMG_MENU_tel_page.h"


AMG_MENU::AMG_MENU(IC_DISPLAY *d, ENGINE_DATA *eng) {
    this->display = d;
    this->engine = eng;
}


void AMG_MENU::update() {
    if (IC_DISPLAY::current_page == IC_PAGE_TELEPHONE) {
        if (!isInPage) { // Not in page so init it now (first)
            isInPage = true;
            /*if (strlen(this->carrier) == 0) {
                //DPRINTLN("REQUEST CARRIER");
                //const char req[1] = { 0x03 };
                //bluetooth->write_message(req, 1);
            } else {*/
                //this->createHeader(carrier);
                //char *text[2] = {"ENGT 100C OILT 100C","MAF 50C OIL 5.2L"};
                //char *text[2] = {"ENGT " + engine->getCoolantTemp() + "OILT "+engine->getOilTemp(), "MAF " + engine->getIntakeTemp() + "OIL "+engine->getOilLevel()};
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
                //snprintf(line0, sizeof(line0), "ENGT %s OILT %s", engine->getCoolantTemp(), engine->getOilTemp());
                sprintf(line5, "%s %s", line1, line2);
                sprintf(line6, "%s %s",line3,  line4);
                //char *text[2] = { strcat(line1,line2), strcat(line3,line4) };   // lifetime is guaranteed
                char *text[2] = { line5,line6 };
                display->setBodyTel(2, text);

                // char head[16]={"GEAR: " + engine->getGearingManual()};
                char head[16];
                snprintf(head, sizeof(head), "GEAR: %s", engine->getGearingManual());
                display->setHeader(IC_PAGE_TELEPHONE,head,IC_TEXT_FMT_LEFT_JUSTIFICATION | IC_TEXT_FMT_HIGHLIGHTED);
            //}
            lastUpdateBody = millis();
        } else {
            if (millis() - lastUpdateBody >= 1000) {
                lastUpdateBody = millis();
                /* if (strlen(this->carrier) == 0) {
                    //DPRINTLN("REQUEST CARRIER");
                    //const char req[1] = { 0x03 };
                    //bluetooth->write_message(req, 1);
                } else {*/
                    //this->createHeader(carrier);
                    //char *text[2] = {"ENGT 100C OILT 100C","MAF 50C OIL 5.2L"};
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
                //snprintf(line0, sizeof(line0), "ENGT %s OILT %s", engine->getCoolantTemp(), engine->getOilTemp());
                sprintf(line5, "%s %s", line1, line2);
                sprintf(line6, "%s %s",line3,  line4);
                //char *text[2] = { strcat(line1,line2), strcat(line3,line4) };   // lifetime is guaranteed
                char *text[2] = { line5,line6 };
                display->setBodyTel(2, text);
                //}
            }
            if (millis() - lastUpdateHeader >= 250){
                lastUpdateHeader=millis();
                // char head[16]={"GEAR: " + engine->getGearingManual()};
                char head[16];
                snprintf(head, sizeof(head), "GEAR: %s", engine->getGearingManual());
                display->setHeader(IC_PAGE_TELEPHONE,head,IC_TEXT_FMT_LEFT_JUSTIFICATION | IC_TEXT_FMT_HIGHLIGHTED);
            }
        }
    }   else {
        isInPage = false;
    }
}

// void TELEPHONE_DISPLAY::setCarrier(char* c) {
//     memset(carrier, 0x00, sizeof(carrier));
//     strcpy(carrier, c);
//     this->createHeader(carrier);
// }

// void TELEPHONE_DISPLAY::createHeader(const char * src) {
//     if (strlen(src) > 0) {
//         display->setHeader(IC_PAGE_TELEPHONE, src, IC_TEXT_FMT_LEFT_JUSTIFICATION);
//     }
// }
