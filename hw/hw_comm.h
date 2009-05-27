#include "hw_audio.h"

#ifndef _HW_COMM_H_
#define _HW_COMM_H_

const char *hw_get_version();

const char *hw_get_ADC();

int  hw_get_camera_nr();

void hw_vibrator_set(int status);

#endif/*_HW_COMM_H_*/
