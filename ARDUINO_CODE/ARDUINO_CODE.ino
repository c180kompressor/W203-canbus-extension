#pragma GCC optimize("-O3")
#pragma GCC optimize("-j4")

//#include <LowPower.h>  //Commented out, gave an error for some reason
#include "can_comm.h"
#include "defines.h"
#include "ic_display.h"
#include "Bluetooth.h"
#include "Audio_Display.h"
#include "Lights.h"
#include "wheel_controls.h"
//#include "Telephone_Display.h"
#include "Engine.h"
#include "Music.h"
#include "defines.h"
#include "DiagMode.h"
#include "AMG_Menu_tel_page.h"

BLUETOOTH *bt;
IC_DISPLAY *ic;
CANBUS_COMMUNICATOR *canB;
CANBUS_COMMUNICATOR *canC;
AUDIO_DISPLAY *audio;
//TELEPHONE_DISPLAY *tel;
LIGHT_CONTROLS *lights;
WHEEL_CONTROLS *wheel_controls;
ENGINE_DATA * eng;
DIAG_MODE* diag;
MUSIC* musicdata;
AMG_MENU* amg;

const char NEXT_TRACK_CMD[1] = {0x00};
const char PREV_TRACK_CMD[1] = {0x01};


// --  DEBUG DATA FOR FREE MEMEORY -- //
#ifdef DEBUG
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

unsigned long lastMemTime = millis();

const char * const PROGMEM MEMORY_STR_1 = "FREE SRAM: ";
const char * const PROGMEM MEMORY_STR_2 = " Bytes";
#endif
// --  DEBUG DATA FOR FREE MEMEORY -- //

bool showDiagMode = false;


void doLightShow() {
    for (int i = 0; i < 5; i++) {
        lights->flash_indicator(LIGHT_CONTROLS::LEFT, 250);
        delay(250);
        lights->flash_indicator(LIGHT_CONTROLS::RIGHT, 250);
        delay(250);
    }
    for (int i = 0; i < 4; i++) {
        lights->flash_hazard(500);
        delay(500);
        lights->flash_lights(true, false, 500);
        delay(500);
        lights->flash_lights(false, true, 500);
        delay(500);
    }
    lights->flash_lights(true, true, 2000);
    lights->flash_hazard(2000);
}

void setup() {
    pinMode(8, OUTPUT);
    pinMode(10, OUTPUT);
    Serial.begin(115200);
    SPI.begin();
    canC = new CANBUS_COMMUNICATOR(10, CAN_500KBPS, CAN_C_DEF);
    delay(100);
    canB = new CANBUS_COMMUNICATOR(8, CAN_83K3BPS, CAN_B_DEF);
    //Serial.println("done!");
    #ifdef ARDUINO_MEGA
    //bt = new BLUETOOTH();
    #else
    //bt = new BLUETOOTH(6, 7);
    #endif
    ic = new IC_DISPLAY(canB);
    audio = new AUDIO_DISPLAY(ic);
    musicdata = new MUSIC(audio);
    //tel = new TELEPHONE_DISPLAY(ic/*, bt*/);
    wheel_controls = new WHEEL_CONTROLS();
    eng = new ENGINE_DATA();
    amg = new AMG_MENU(ic,eng);

    #ifdef ARDUINO_MEGA
    eng = new ENGINE_DATA();
    diag = new DIAG_MODE(audio, eng);
    #endif

    Serial.println("Ready!");
    delay(900);
}

void HandleBluetoothRequest() {
    char* ptr = bt->read_message();
    uint8_t len = strlen(ptr);
    if (len > 0) {
        if (ptr[0] == 0x00) { // Music data message
            musicdata->setTrackName(ptr+1);
        } else if (ptr[0] == 0x01) {
            if (ptr[1] == 0x00) {
                musicdata->pause();
            } else if (ptr[1] == 0x01) {
               musicdata->play();
            }
        }
    }
}

void handleFrameRead() {
    can_frame *readB = canB->read_frame();
    if (readB->can_dlc != 0) {
        ic->processIcResponse(readB);
        handleKeyInputs(readB);
        if ((readB->can_id == 0x000C || readB->can_id == 0x0016) /*&& showDiagMode*/) {
            eng->readFrame(readB);
        } else if (readB->can_id == 0x0000) {
            if ((readB->data[0] & 0b000000001) > 0) {
                CAR_SLEEP = false;
            } else {
                CAR_SLEEP = true;
            }
        }

    }
    can_frame *read = canC->read_frame();
    if (read->can_dlc != 0 /*&& showDiagMode*/) {
        eng->readFrame(read);
    }
}

void handleKeyInputs(can_frame *f) {
    // User is in audio page
    if (ic->current_page == 0x03) {
        switch(wheel_controls->getPressed(f)) {
            case BUTTON_ARROW_UP:
                Serial.println("up key pressed");
                break;
            case BUTTON_ARROW_DOWN:
                Serial.println("down pressed");
                break;
            case BUTTON_TEL_DEC:
                Serial.println("tel dec key pressed amg menu prev page");
                amg->prevAMGPage();
                break;
            case BUTTON_TEL_ANS:
                Serial.println("tel key pressed amg menu next page");
                amg->nextAMGPage();
                break;
            default:
                break;
        } 
    } 
    // Telephone screen
    else if (ic->current_page == 0x05) {
      //Serial.println("we are in tel page");
        switch(wheel_controls->getPressed(f)) {
            case BUTTON_TEL_ANS:
                Serial.println("tel page tel button");
                amg->nextAMGPage();
                break;
            case BUTTON_TEL_DEC:
                Serial.println("tel page tel dec button");
                amg->prevAMGPage();
                break;
            default:
                break;
        }
    }
    // Other screen - Here the Arrows / Page buttons are used so we can only use the telephone buttons 
    else {
        switch(wheel_controls->getPressed(f)) {
            case BUTTON_TEL_ANS:
                Serial.println("other page tel button");
                //bt->write_message(NEXT_TRACK_CMD, 1); // Use telephone Answer button to seek track
                break;
            case BUTTON_TEL_DEC:
                Serial.println("other page tel dec button");
                //bt->write_message(PREV_TRACK_CMD, 1); // Use telephone decline button to repeat track
                break;
            default:
                break;
        }
    }
}

void loop() {
    // Dont need to do any of this if we are asleep
    if (!CAR_SLEEP) {
        //HandleBluetoothRequest();
        //musicdata->updateUI();
        //audio->update();
        //tel->update();
        amg->updateAMG();
    }
    handleFrameRead();
    if (CAR_SLEEP) {
        delay(1000);
        Serial.println("car sleep");
    }
    #ifdef DEBUG
    if (millis() - lastMemTime > 2000) {
        lastMemTime = millis();
        DPRINTLN(MEMORY_STR_1+String(freeRam())+MEMORY_STR_2);
    }
    #endif


}