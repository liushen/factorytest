#include "hw_audio.h"
#include "hw_fm.h"
#include <unistd.h>

#ifndef _HW_COMM_H_
#define _HW_COMM_H_

#define HW_DEV_VIBRATOR     "/sys/class/timed_output/vibrator/enable"
#define HW_DEV_AMBIENT      "/sys/class/hwmon/hwmon0/ambient"
#define HW_DEV_PROXIMITY    "/sys/class/hwmon/hwmon0/proximity"
#define HW_DEV_GSENSOR      "/sys/class/g_sensor/"
#define HW_DEV_CAMERA       "/sys/class/camera/"    //ov2640
#define HW_DEV_GPS          "/sys/class/gps/"       //blox5
#define HW_DEV_MMC          "/dev/block/mmcblk0"

#define HW_DEV_BATTERY_VOL  "/sys/class/power_supply/battery/voltage_now"
#define HW_DEV_BATTERY_CUR  "/sys/class/power_supply/battery/current_avg"

#define HW_WIFI_MODULE      "sd8xxx"
#define HW_WIFI_SERVICE     "gh3801_init"
#define HW_ADC_CHECK_EXEC   "record calibrate read"

const char *hw_get_version();

const char *hw_get_adc();

void hw_vibrator_set(int status);

int hw_wireless_ready();

int hw_detect_devices(const char *path);

int hw_detect_camera();

int hw_detect_wifi();

int hw_detect_gps();

int hw_detect_ambient();

int hw_detect_proximity();

int hw_detect_g_sensor();

int hw_detect_memory_card();

int hw_file_write(const char *file, const char *content);

char *hw_file_read(const char *file, size_t len);

#endif/*_HW_COMM_H_*/
