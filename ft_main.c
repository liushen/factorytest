#include "gui/ft_input.h"
#include "gui/ft_window.h"
#include "gui/ft_button.h"
#include "gui/ft_status_button.h"
#include "hw/hw_comm.h"
#include "hw/hw_audio.h"
#include "hw/hw_led.h"
#include "hw/hw_camera.h"
#include "hw/hw_bluetooth.h"
#include "hw/hw_wifi.h"
#include "hw/hw_gsm.h"

#include "ft_config.h"
#include "ft_keyboard.h"
#include "ft_textpad.h"
#include "ft_matrix.h"
#include "ft_adc.h"
#include "ft_led.h"
#include "ft_gsm.h"
#include "ft_lcdcolor.h"
#include "ft_lcdcontrast.h"
#include "ft_headset.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/reboot.h>

#define BUF_LEN_MAX             32
#define BLUETOOTH_TEST_NR       1

#define TEXT_DEVICE_FOUND       "The device detected!"
#define TEXT_DEVICE_NOT_FOUND   "The device is not detected!"

FTColor ft_color_r = {0xff, 0, 0, 0};
FTColor ft_color_g = {0, 0xff, 0, 0};
FTColor ft_color_b = {0, 0, 0xff, 0};
FTColor ft_color_w = {0xff, 0xff, 0xff, 0};

typedef struct _FTestItem FTestItem;

struct _FTestItem
{
    int         id;
    const char *desc;
    FBHandler   handler;
};

static void on_clear_test_handler(FTButton *button, void *data)
{
    ft_config_reset();
    exit(0);
}

static void on_shutdown_handler(FTButton *button, void *data)
{
#ifdef HAVE_ANDROID_OS
    sync();
    reboot(RB_POWER_OFF);
#endif
}

static void on_version_handler(FTButton *button, void *data)
{
    const char *ver = hw_get_version();

    FTWindow *window = ft_textpad_new(ver, 1);

    button = ft_button_new("Clear Test");
    ft_button_set_handler(button, on_clear_test_handler, data);
    ft_window_add(window, (FTWidget *)button, 0);

    button = ft_button_new("Shutdown");
    ft_button_set_handler(button, on_shutdown_handler, data);
    ft_window_add(window, (FTWidget *)button, 1);

    ft_textpad_set_id(window, FT_ITEM_VER);
    ft_window_show(window);
}

static void on_keyboard_handler(FTButton *button, void *data)
{
    FTWindow *window = ft_keyboard_new();
    ft_window_show(window);
}

static void on_led_handler(FTButton *button, void *data)
{
    FTWindow *window = ft_led_new();
    ft_window_show(window);
}

static void on_vibrator_handler(FTButton *button, void *data)
{
    FTWindow *window;

    window = ft_textpad_new("Vibrator", 1);
    
    ft_textpad_set_id(window, FT_ITEM_MOTOR);
    ft_window_show(window);

    hw_vibrator_set(-1);
}

static void on_loudspk_handler(FTButton *button, void *data)
{
    FTWindow *window;

    window = ft_textpad_new("Loud SPK", 1);
    
    ft_textpad_set_id(window, FT_ITEM_SPK);
    ft_window_show(window);

    hw_audio_play(HA_DEVICE_SPEAKER, "/system/usr/share/factorytest/sound.wav");
}

static void on_echoloop_handler(FTButton *button, void *data)
{
    FTWindow *window;

    window = ft_textpad_new("Echoloop", 1);
    
    hw_audio_echoloop_set(HA_DEVICE_SPEAKER, 1);

    ft_textpad_set_id(window, FT_ITEM_LOOP);
    ft_window_show(window);
}

static void on_headset_handler(FTButton *button, void *data)
{
    FTWindow *window;

    window = ft_headset_new();
    
    ft_textpad_set_id(window, FT_ITEM_HEADSET);
    ft_window_show(window);

    hw_audio_echoloop_set(HA_DEVICE_HEADSET, 1);
}

static void on_lcdcolor_handler(FTButton *button, void *data)
{
    FTWindow *window = ft_lcdcolor_new();
    ft_window_show(window);
}

static void on_matrix_handler(FTButton *button, void *data)
{
    FTWindow *window = ft_matrix_new();
    ft_window_show(window);
}

static void on_adc_handler(FTButton *button, void *data)
{
    FTWindow *window = ft_adc_new();

    ft_textpad_set_id(window, FT_ITEM_ADC);
    ft_window_show(window);
}

static void ft_camera_handler(FTEvent *event, void *data)
{
    FTWindow *window = (FTWindow *)data;

    if (event->type == FE_KEY_RELEASE ||
        event->type == FE_MOUSE_RELEASE)
    {
        hw_camera_close();
        ft_window_close(window);

        window = ft_textpad_new("Camera", 1);
        ft_textpad_set_id(window, FT_ITEM_CAMERA);
        ft_window_show(window);
    }
}

static void on_camera_handler(FTButton *button, void *data)
{
    FTWindow *window;
    FTWidget *widget;

    window = ft_window_new();
    widget = (FTWidget *)window;

    widget->handler = ft_camera_handler;
    widget->data = window;

    ft_window_show(window);
    hw_camera_open();
}

static void on_gsm_handler(FTButton *button, void *data)
{
    FTWindow *window;

    window = ft_gsm_new();

    ft_textpad_set_id(window, FT_ITEM_GSM);
    ft_window_show(window);
}

static void on_bluetooth_handler(FTButton *button, void *data)
{
    FTWindow *window = NULL;
    char *devices = NULL;
    int i = 0;

    window = ft_textpad_new("", 1);

    ft_textpad_set_id(window, FT_ITEM_BT);
    ft_window_show(window);

    for (i = 0; i < BLUETOOTH_TEST_NR; i++) 
    {
        ft_textpad_set_text(window, "Loading ...");

        while (!hw_wireless_ready()) sleep(1);

        ft_textpad_set_text(window, "Activating Bluetooth...");

        if (!hw_bluetooth_enable())
        {
            ft_textpad_set_text(window, "Unenable Bluetooth!");
            ft_textpad_set_color(window, &ft_color_r);

            return;
        }

#if 1 /* scan bt devices */
        ft_textpad_set_text(window, "Scanning ...");

        devices = hw_bluetooth_scan();
        ft_textpad_set_text(window, devices);
        free(devices);

        return;
#endif
        ft_textpad_set_text(window, "Bluethooth acvivated.");

        if (!hw_bluetooth_disable())
        {
            ft_textpad_set_text(window, "Unenable Bluetooth!");
            ft_textpad_set_color(window, &ft_color_r);

            return;
        }

        ft_textpad_set_text(window, "Bluethooth deactivated.");
    }
}

static void on_wifi_handler(FTButton *button, void *data)
{
    FTWindow *window;

    window = ft_textpad_new("Loading ...", 1);

    ft_textpad_set_id(window, FT_ITEM_WIFI);
    ft_window_show(window);

    while (!hw_wireless_ready()) sleep(1);

    ft_textpad_set_text(window, "Scanning ...");

    if (hw_wifi_open())
    {
        char *devices = NULL;

        devices = hw_wifi_scan();
        ft_textpad_set_text(window, devices);
        free(devices);
    }
    else
    {
        ft_textpad_set_text(window, TEXT_DEVICE_NOT_FOUND);
        ft_textpad_set_color(window, &ft_color_r);
    }
}

static void on_fm_handler(FTButton *button, void *data)
{
    FTWindow *window;

    window = ft_textpad_new("87.5", 1);

    ft_textpad_set_id(window, FT_ITEM_FM);
    ft_window_show(window);

    hw_fm_start();
    hw_fm_set_freq(87500);
}

static void on_gps_handler(FTButton *button, void *data)
{
    FTWindow *window;

    int status = hw_detect_gps();
#if 0 
    window = ft_textpad_new(status ? TEXT_DEVICE_FOUND : TEXT_DEVICE_NOT_FOUND, 1);

    ft_textpad_set_color(window, status ? &ft_color_g : &ft_color_r);
    ft_textpad_set_id(window, FT_ITEM_GPS);

    ft_window_show(window);
#else
    char key[32];

    sprintf(key, "%d", FT_ITEM_GPS);
    ft_config_set_int(key, (status > 0) ? FT_STATUS_OK : FT_STATUS_FAIL);
#endif
}

static void on_memory_card_handler(FTButton *button, void *data)
{
    FTWindow *window;

    int status = hw_detect_memory_card();
#if 0 
    window = ft_textpad_new(status ? TEXT_DEVICE_FOUND : TEXT_DEVICE_NOT_FOUND, 1);

    ft_textpad_set_color(window, status ? &ft_color_g : &ft_color_r);
    ft_textpad_set_id(window, FT_ITEM_SD);

    ft_window_show(window);
#else
    char key[32];

    sprintf(key, "%d", FT_ITEM_SD);
    ft_config_set_int(key, (status > 0) ? FT_STATUS_OK : FT_STATUS_FAIL);
#endif
}

static void on_g_sensor_handler(FTButton *button, void *data)
{
    FTWindow *window;

    int status = hw_detect_g_sensor();
#if 0 
    window = ft_textpad_new(status ? TEXT_DEVICE_FOUND : TEXT_DEVICE_NOT_FOUND, 1);

    ft_textpad_set_color(window, status ? &ft_color_g : &ft_color_r);
    ft_textpad_set_id(window, FT_ITEM_G_SENS);

    ft_window_show(window);
#else
    char key[32];

    sprintf(key, "%d", FT_ITEM_G_SENS);
    ft_config_set_int(key, (status > 0) ? FT_STATUS_OK : FT_STATUS_FAIL);
#endif
}

static void on_infrared_sensor_handler(FTButton *button, void *data)
{
    FTWindow *window;

    int status = hw_detect_proximity();
#if 0 
    window = ft_textpad_new(status ? TEXT_DEVICE_FOUND : TEXT_DEVICE_NOT_FOUND, 1);

    ft_textpad_set_color(window, status ? &ft_color_g : &ft_color_r);
    ft_textpad_set_id(window, FT_ITEM_INFRA_SENS);

    ft_window_show(window);
#else
    char key[32];

    sprintf(key, "%d", FT_ITEM_INFRA_SENS);
    ft_config_set_int(key, (status > 0) ? FT_STATUS_OK : FT_STATUS_FAIL);
#endif
}

static void on_optical_sensor_handler(FTButton *button, void *data)
{
    FTWindow *window;

    int status = hw_detect_ambient();
#if 0    
    window = ft_textpad_new(status ? TEXT_DEVICE_FOUND : TEXT_DEVICE_NOT_FOUND, 1);

    ft_textpad_set_color(window, status ? &ft_color_g : &ft_color_r);
    ft_textpad_set_id(window, FT_ITEM_OPTI_SEND);

    ft_window_show(window);
#else
    char key[32];

    sprintf(key, "%d", FT_ITEM_OPTI_SENS);
    ft_config_set_int(key, (status > 0) ? FT_STATUS_OK : FT_STATUS_FAIL);
#endif
}

const FTestItem ft_test_items[] = 
{
    {FT_ITEM_VER, "Version", on_version_handler},
    {FT_ITEM_KEY, "Key", on_keyboard_handler},
    {FT_ITEM_LED, "LED", on_led_handler},
    {FT_ITEM_MOTOR, "Vibrator", on_vibrator_handler},
    {FT_ITEM_SPK, "Loud SPK", on_loudspk_handler},
    {FT_ITEM_LOOP, "Echoloop", on_echoloop_handler},
    {FT_ITEM_HEADSET, "Headset", on_headset_handler},
    {FT_ITEM_LCD, "LCD", on_lcdcolor_handler},
    {FT_ITEM_MATRIX, "M * M", on_matrix_handler},
    {FT_ITEM_ADC, "ADC", on_adc_handler},
    {FT_ITEM_CAMERA, "Camera", on_camera_handler},
    {FT_ITEM_GSM, "GSM", on_gsm_handler},
    {FT_ITEM_BT, "Bluetooth", on_bluetooth_handler},
    {FT_ITEM_WIFI, "Wifi", on_wifi_handler},
    {FT_ITEM_FM, "FM", on_fm_handler},
    {FT_ITEM_GPS, "GPS", on_gps_handler},
    {FT_ITEM_SD, "SD Card", on_memory_card_handler},
    {FT_ITEM_G_SENS, "G-sensor", on_g_sensor_handler},
    {FT_ITEM_INFRA_SENS, "Infra-sens", on_infrared_sensor_handler},
    {FT_ITEM_OPTI_SENS, "Opti-sens", on_optical_sensor_handler},
};

static void on_config_changed(const char *key, const char *value, void *data)
{
    FTButton **buttons = (FTButton **)data;
    int id = atoi(key);
    int i = 0;

    switch (id) /* close device */
    {
        case FT_ITEM_MOTOR:
            hw_vibrator_set(0);
            break;

        case FT_ITEM_SPK:
            hw_audio_stop(HA_DEVICE_SPEAKER);
            break;

        case FT_ITEM_LOOP:
            hw_audio_echoloop_set(HA_DEVICE_SPEAKER, 0);
            break;

        case FT_ITEM_HEADSET:
            hw_audio_echoloop_set(HA_DEVICE_HEADSET, 0);
            break;

        case FT_ITEM_WIFI:
            hw_wifi_close();
            break;

        case FT_ITEM_FM:
            hw_fm_stop();
            break;
    }

    for (; i < FT_N_ELEMENTS(ft_test_items); i++)
    {
        if (id == ft_test_items[i].id)
        {
            ft_status_button_set_status((FTStatusButton *)buttons[i], atoi(value));
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    static FTButton *buttons[32];
    char text[BUF_LEN_MAX];
    int i = 0;

    FTWindow *window;
    FTButton *button;

    window = ft_window_new();

    for (i = 0; i < FT_N_ELEMENTS(ft_test_items); i++)
    {
        char key[BUF_LEN_MAX];
        int value = 0;

        snprintf(text, BUF_LEN_MAX, 
                "%02d.%s", i+1, ft_test_items[i].desc);
        sprintf(key, "%d", ft_test_items[i].id);

        buttons[i] = (FTButton *)ft_status_button_new(text);
        ft_button_set_handler(buttons[i], ft_test_items[i].handler, window);
        ft_window_add_child(window, (FTWidget *)buttons[i]);

        if ((value = ft_config_get_int(key)) != -1)
        {
            ft_status_button_set_status((FTStatusButton *)buttons[i], value);
        }
    }

    ft_config_set_handler(on_config_changed, buttons);
    ft_widget_set_focus((FTWidget *)buttons[0]);
    ft_window_show(window);

    if (fork() == 0)
    {
        /* power on modem... */
        execlp(AT_CMD_EXEC, AT_CMD_EXEC, "1", NULL);
        exit(0);
    }

    /* event loop */
    wait_for_events();

    /* unmap, close fb device */
    ft_frame_buffer_close();

    return 0;
}

