#include "Engine.h"



ENGINE_DATA::ENGINE_DATA() {
}

void ENGINE_DATA::readFrame(can_frame *f) {
    this->engineOn = true;
    if (f->can_id == 0x608) {
        this->coolant_temp = uint8_t(f->data[0]);
        this->intake_temp = uint8_t(f->data[1]);
        this->consumption = (int) (f->data[5] << 8) | (f->data[6]);
        if (this->consumption < 0) {
            this->consumption = 0;
        }
    }
    #ifndef MANUAL_GEARBOX
        else if (f->can_id == 0x0418) {
            this->transmission_temp = uint8_t(f->data[2]) - 40;
            this->actualGear = (f->data[4]) & 0b00001111;
            this->targetGear = ((f->data[4]) & 0b11110000) >> 4;
        }
    #else
        else if (f->can_id == 0x0240) {
            ReverseEngaged = uint8_t(((f->data[1]) & 0b00001000) >> 3);
            ParkingBrakeEngaged = uint8_t(((f->data[4]) & 0b00010000) >> 4);
        }
    #endif
    else if (f->can_id == 0x0308) {
        this->rpm=(int) ((f->data[1] << 8) | (f->data[2]));
        this->oil_temp = uint8_t(f->data[5]);
        this->oil_level = uint8_t(f->data[6]);
    }
    else if (f->can_id == 0x000C) {
        this->speed_km = f->data[1];
        this->rpm_sampled=this->rpm;
    }
    else if(f->can_id == 0x0016){
        v_batt = uint8_t(f->data[0]);
    }
    
}

#ifndef MANUAL_GEARBOX
const char* ENGINE_DATA::getTransmissionTemp() {
    if (this->engineOn == false) {
        return ENGINE_OFF;
    } else if (this->transmission_temp == 0xFF) {
        return UNKNOWN_VAL;
    } else {
        memset(buffer, 0x00, sizeof(buffer));
        sprintf(buffer, "%d C", transmission_temp);
        return buffer;
    }
}

const char* ENGINE_DATA::getGearingAuto() {
    if (this->engineOn == false) {
        return ENGINE_OFF;
    }
    if (this->actualGear == 0xFF || this->targetGear == 0xFF) {
        return UNKNOWN_VAL;
    } else if (this->actualGear == 0x00 || this->targetGear == 0x00) {
        return GEAR_NEUTRAL;
    } else if (this->targetGear == 11 || this->actualGear == 11) {
        return GEAR_REVERSE;
    } else if (this->targetGear == 12 || this->actualGear == 12) {
        return GEAR_REVERSE_2;
    } else if (this->targetGear == 13 || this->actualGear == 13) {
        return GEAR_PARK;
    } else {
        memset(buffer, 0x00, sizeof(buffer));
        if (targetGear == actualGear) {
            sprintf(buffer, "%d", actualGear);
        } else if (targetGear > actualGear){
            sprintf(buffer, "%d -> %d", actualGear, targetGear);
        } else {
            sprintf(buffer, "%d <- %d", targetGear, actualGear);
        }
        return buffer;
    }
}

#else

const char* ENGINE_DATA::getGearingManual() {
    if (this->engineOn == false) {
        return ENGINE_OFF;
    }
    if(this->ParkingBrakeEngaged == 0x01){
        return GEAR_PARK;
    }
    if(this->ReverseEngaged == 0x01){
        return GEAR_REVERSE;
    }
    else{
      memset(buffer, 0x00, sizeof(buffer));
      // Check each gear to see if the measured RPM matches the calculated RPM
      int detectedGear = -1;
      for (int gear = 1; gear <= NUM_GEARS_MANUAL; gear++) {
          // Check if measured RPM is within the margin
          if (fabs(this->rpm_sampled - (this->speed_km * 1000.0 / 60.0 / wheelDiameterMeters / 3.14159 * finalDriveRatio*GEAR_RATIOS_MANUAL[gear-1])) < RPM_MARGIN) {
              detectedGear = gear;
              break;
          }

      }
      if(detectedGear>0){
        sprintf(buffer, "%d", detectedGear);
        return buffer;
      }
      else{
      return GEAR_NEUTRAL;
      }
      
    }
    
}
#endif

const char* ENGINE_DATA::getIntakeTemp() {
    if (this->engineOn == false) {
        return ENGINE_OFF;
    }
    if (this->intake_temp == 0xFF) {
        return UNKNOWN_VAL;
    } else {
        memset(buffer, 0x00, sizeof(buffer));
        sprintf(buffer, "%dC", this->intake_temp - 40);
        return buffer; 
    }
}

const char* ENGINE_DATA::getCoolantTemp() {
    if (this->engineOn == false) {
        return ENGINE_OFF;
    }
    if (this->coolant_temp == 0xFF) {
        return UNKNOWN_VAL;
    } else {
        memset(buffer, 0x00, sizeof(buffer));
        sprintf(buffer, "%dC", this->coolant_temp - 40);
        return buffer;
    }
}

const char* ENGINE_DATA::getConsumption() {
    if (!this->engineOn) {
        return ENGINE_OFF;
    }
    if (millis() - lastMpgTime >= 1000) {
        float d = millis() - lastMpgTime;
        lastMpgTime = millis();
        sprintf(buffer, "%d ul/s", this->consumption);
    }
    return buffer;
}


const char* ENGINE_DATA::getMPG() {
    if (!this->engineOn) {
        return ENGINE_OFF;
    }
    if (millis() - lastMpgTime >= 1000) {
        lastMpgTime = millis();
        //if(this->)
        if (this->speed_km == 0) {
            #ifdef REGION_UK
                sprintf(buffer, "0.0 MPG"); 
            #elif defined(REGION_US)
                sprintf(buffer, "0.0 MPG");
            #else
                sprintf(buffer, "Inf l/100"); 
            #endif
        } else if (this->consumption == 0) {
            #ifdef REGION_UK
                sprintf(buffer, "Inf MPG"); 
            #elif defined(REGION_US)
                sprintf(buffer, "Inf MPG");
            #else
                sprintf(buffer, "0.0l/100");
            #endif
        } else {
            float l_per_hour = 3600.0 * (this->consumption / 1000000.0);
            float km_l = this->speed_km / l_per_hour;
            #ifdef REGION_UK
                float mpg = km_l * 2.82481; // Miles per gallon (UK)
            #elif defined(REGION_US)
                float mpg = km_l * 2.35215; // Miles per gallon (US)
            #else
                float mpg = 100.0/km_l; // LITERS PER 100KM like the rest of the fking world
                if(mpg > 99) mpg=99.9;
            #endif
            char str[7];
            dtostrf(mpg, 4, 1, str);
            sprintf(buffer, "%sl/100", str);
        }
    }
    return buffer;
}

const char* ENGINE_DATA::getOilTemp() {
    if (!this->engineOn) {
        return ENGINE_OFF;
    }
    if (this->engineOn == false) {
        return ENGINE_OFF;
    }
    if (this->coolant_temp == 0xFF) {
        return UNKNOWN_VAL;
    } else {
        memset(buffer, 0x00, sizeof(buffer));
        sprintf(buffer, "%dC", this->oil_temp - 40);
        return buffer;
    }
}

const char* ENGINE_DATA::getOilLevel() {
    if (!this->engineOn) {
        return ENGINE_OFF;
    }
    if (this->engineOn == false) {
        return ENGINE_OFF;
    }
    if (this->coolant_temp == 0xFF) {
        return UNKNOWN_VAL;
    } else {
        memset(buffer, 0x00, sizeof(buffer));
        char str[7];
        dtostrf((float(this->oil_level)*0.02+1.3), 4, 1, str);
        sprintf(buffer, "%sL", str);
        return buffer;
    }
}

const char* ENGINE_DATA::getVBatt() {
    char str[7];
    dtostrf((float(this->v_batt) / 10), 4, 1, str);
    memset(buffer, 0x00, sizeof(buffer));
    sprintf(buffer, "%s", str);
    return buffer;
}


