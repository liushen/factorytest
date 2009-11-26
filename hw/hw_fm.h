#ifndef _HW_FM_H_
#define _HW_FM_H_

#define HW_FW_DEVICE    "/sys/class/fm/fm_rda5800/device"

int hw_detect_fm();

int hw_fm_start();

int hw_fm_stop();

int hw_fm_set_freq(int freq);

int hw_fm_get_freq();

#endif/*_HW_FM_H_*/
