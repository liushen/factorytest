#include "hw_audio.h"

#ifndef _HW_COMM_H_
#define _HW_COMM_H_

#define HW_DEV_VIBRATOR     "/sys/class/motor/broncho-motor/power"

const char *hw_get_version();

const char *hw_get_ADC();

void hw_vibrator_set(int status);

int hw_get_camera_nr();

int hw_detect_bluetooth();

int hw_detect_wifi();

int hw_detect_fm();

int hw_detect_gps();

#endif/*_HW_COMM_H_*/
