#include "hw_comm.h"
#include "modemcontrol.h"
#include <cutils/properties.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>

#define VER_LEN_MAX 255
#define ADC_LEN_MAX 255
#define BUF_LEN_MAX 128

const char *hw_get_version()
{
    static char version[VER_LEN_MAX] = {0};
    char build[PROPERTY_VALUE_MAX] = {0};

    char *hw_ver = hw_file_read("/proc/hw_version", 16);
    char *p = NULL;

    property_get("ro.build.description", build, NULL);

    if ((p = strchr(build, ' ')))
        *p = '\0';

    snprintf(version, VER_LEN_MAX, 
            "Version No: %s\nSerial No: %s", hw_ver, build);

    free(hw_ver);

    return version;
}

const char *hw_get_adc()
{
    static char adc[ADC_LEN_MAX];
    char *vol = hw_file_read(HW_DEV_BATTERY_VOL, 16);
    char *cur = hw_file_read(HW_DEV_BATTERY_CUR, 16);

    snprintf(adc, ADC_LEN_MAX, "VBAT: %.3fV\nCurrent: %.3fA", 
             vol ? atoi(vol) * 0.001 : 0, 
             cur ? atoi(cur) * 0.001 : 0);

    free(vol);
    free(cur);

    return adc;
}

void hw_vibrator_set(int status)
{
    char buf[BUF_LEN_MAX];

    sprintf(buf, "%d", status);

    hw_file_write(HW_DEV_VIBRATOR, buf);
}

int hw_wireless_ready()
{
    char state[PROPERTY_VALUE_MAX] = {0};

    property_get("init.svc."HW_WIFI_SERVICE, state, NULL);

    return strcmp(state, "running") != 0;
}

int hw_detect_devices(const char *path)
{
    struct dirent* ent = NULL;
    DIR *dir = NULL;
    int count = 0;

    dir = opendir(path);

    if (dir == NULL)
        return 0;

    while ((ent = readdir(dir)))
    {
        if ((ent->d_type == DT_DIR) && (*ent->d_name != '.'))
        {
            count++;
        }
    }

    closedir(dir);

    return count;
}

int hw_detect_ambient()
{
    return access(HW_DEV_AMBIENT, F_OK) == 0;
}

int hw_detect_proximity()
{
    return access(HW_DEV_PROXIMITY, F_OK) == 0;
}

int hw_detect_g_sensor()
{
    return hw_detect_devices(HW_DEV_GSENSOR);
}

int hw_detect_camera()
{
    return hw_detect_devices(HW_DEV_CAMERA);
}

int hw_detect_gps()
{
    return hw_detect_devices(HW_DEV_GPS);
}

int hw_detect_wifi()
{
    char *mods, *p = NULL;

    mods = hw_file_read("/proc/modules", 0);

    if (mods)
    {
        p = strstr(mods, HW_WIFI_MODULE);
        free(mods);
    }

    return (p != NULL);
}

int hw_detect_memory_card()
{
    return access(HW_DEV_MMC, F_OK) == 0;
}

int hw_file_write(const char *file, const char *content)
{
    ssize_t size;

    int fd = open(file, O_WRONLY);

    if (fd < 0)
    {
        perror(file);
        return -1;
    }

    size = write(fd, content, strlen(content));
    close(fd);

    return size;
}

char *hw_file_read(const char *file, size_t len)
{
    char buf[BUF_LEN_MAX+1];
    ssize_t size;

    int fd = open(file, O_RDONLY);

    if (fd < 0)
    {
        perror(file);
        return NULL;
    }

    if (len == 0 || len > BUF_LEN_MAX)
    {
        len = BUF_LEN_MAX;
    }

    size = read(fd, buf, len);
    close(fd);

    if (size > 0)
    {
        if (buf[size] == '\n')
            buf[size] = '\0';
        else
            buf[size+1] = '\0';

        return strdup(buf);
    }

    return NULL;
}

