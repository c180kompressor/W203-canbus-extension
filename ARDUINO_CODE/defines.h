//#define DEBUG
#ifdef DEBUG
  #define DPRINT(...) Serial.print(__VA_ARGS__)
  #define DPRINTLN(...) Serial.println(__VA_ARGS__)
#else
  #define DPRINT(...)
  #define DPRINTLN(...)
#endif

// comment out if you own a Arduino uno! or other smaller board
//#define ARDUINO_MEGA

// Uncomment out if you want MPG(either UK or US) instead of L/100KM
//#define REGION_UK
//#define REGION_US

#define SYSTEM_SRAM_SIZE 2048

// Audio page settings
#define UPDATE_FREQ_HEADER 1000
#define UPDATE_FREQ_SCROLL_BODY 200
#define DIAG_MODE_UPDATE_FREQ 250

// Steering wheel button press detections
#define WHEEL_BUTTON_LONG_PRESS_MS 1000 // If button is held down for this long then its registered as a long tap

// Manual Gearbox definition(comment out if automatic)
#define MANUAL_GEARBOX

// AMG MENU (Diag mode in Telephone page on 4 lines) (have not tried it yet)
#define AMG_MENU_D