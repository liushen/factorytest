#include "hw_audio.h"
#include <unistd.h>

#ifndef _HW_COMM_H_
#define _HW_COMM_H_

#define HW_DEV_VIBRATOR     "/sys/class/timed_output/vibrator/enable"
#define HW_DEV_CAMERA       "/sys/class/camera/"    //ov2640
#define HW_DEV_GPS          "/sys/class/gps/"       //blox5
#define HW_DEV_FM           "/sys/class/fm/"        //fm_rda5800

#define HW_DEV_BATTERY_VOL  "/sys/class/power_supply/battery/voltage_now"
#define HW_DEV_BATTERY_CUR  "/sys/class/power_supply/battery/current_avg"

#define HW_WIFI_MODULE      "sd8xxx"

const char *hw_get_version();

const char *hw_get_ADC();

void hw_vibrator_set(int status);

int hw_detect_bluetooth();

int hw_detect_camera();

int hw_detect_wifi();

int hw_detect_fm();

int hw_detect_gps();

int hw_open_modem();

int hw_file_write(const char *file, const char *content);

char *hw_file_read(const char *file, size_t len);

#endif/*_HW_COMM_H_*/
