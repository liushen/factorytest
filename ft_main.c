#undef LOG_TAG
#define LOG_TAG "Factory"

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
#include "ft_testcase.h"
#include "ft_keyboard.h"
#include "ft_textpad.h"
#include "ft_notepad.h"
#include "ft_matrix.h"
#include "ft_adc.h"
#include "ft_led.h"
#include "ft_gsm.h"
#include "ft_lcdcolor.h"
#include "ft_lcdcontrast.h"
#include "ft_headset.h"
#include "ft_fm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/reboot.h>
#include <cutils/log.h>

#define BUF_LEN_MAX             128
#define BLUETOOTH_TEST_NR       1

#define TEXT_DEVICE_FOUND       "The device detected!"
#define TEXT_DEVICE_NOT_FOUND   "The device is not detected!"

typedef struct _FTestItem FTestItem;

struct _FTestItem
{
    int         id;
    const char *desc;
    FBHandler   handler;
    int       (*test)();
};

typedef struct _FTContext FTContext;

struct _FTContext
{
    int         autotest;
    int         start;
    int         result;
};

static FTContext ft_context;

FTColor ft_color_r = {0xff, 0, 0, 0};
FTColor ft_color_g = {0, 0xff, 0, 0};
FTColor ft_color_b = {0, 0, 0xff, 0};
FTColor ft_color_y = {0xff, 0xff, 0, 0};
FTColor ft_color_w = {0xff, 0xff, 0xff, 0};

static void on_auto_test_handler(FTButton *button, void *data);
static void on_tools_handler(FTButton *button, void *data);

static void on_clear_results_handler(FTButton *button, void *data)
{
    ft_config_reset();
    exit(0);
}

static void on_coupling_test_handler(FTButton *button, void *data)
{
    FTWindow *window;

    window = ft_textpad_new("Power off modem...", 1);
    ft_window_show(window);
    hw_gsm_close();
    sleep(3);

    ft_textpad_set_text(window, "Watting for modem ready...");
    hw_gsm_open();
    sleep(3);

    ft_textpad_set_text(window, "Modem OK.");
}

static void on_shutdown_handler(FTButton *button, void *data)
{
#ifdef HAVE_ANDROID_OS
    sync();
    reboot(RB_POWER_OFF);
#endif
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

    window = ft_textpad_new(NULL, 1);

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

#if 0 /* scan bt devices */
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

    window = ft_fm_new();
    ft_window_show(window);
}

static void on_gps_handler(FTButton *button, void *data)
{
    FTWindow *window;

    int status = hw_detect_gps();
    char key[BUF_LEN_MAX];

    sprintf(key, "%d", FT_ITEM_GPS);
    ft_config_set_int(key, (status > 0) ? FT_STATUS_OK : FT_STATUS_FAIL);
}

static void on_memory_card_handler(FTButton *button, void *data)
{
    FTWindow *window;

    int status = hw_detect_memory_card();
    char key[BUF_LEN_MAX];

    sprintf(key, "%d", FT_ITEM_SD);
    ft_config_set_int(key, (status > 0) ? FT_STATUS_OK : FT_STATUS_FAIL);
}

static void on_g_sensor_handler(FTButton *button, void *data)
{
    FTWindow *window;

    int status = hw_detect_g_sensor();
    char key[BUF_LEN_MAX];

    sprintf(key, "%d", FT_ITEM_G_SENS);
    ft_config_set_int(key, (status > 0) ? FT_STATUS_OK : FT_STATUS_FAIL);
}

static void on_infrared_sensor_handler(FTButton *button, void *data)
{
    FTWindow *window;

    int status = hw_detect_proximity();
    char key[BUF_LEN_MAX];

    sprintf(key, "%d", FT_ITEM_INFRA_SENS);
    ft_config_set_int(key, (status > 0) ? FT_STATUS_OK : FT_STATUS_FAIL);
}

static void on_optical_sensor_handler(FTButton *button, void *data)
{
    FTWindow *window;

    int status = hw_detect_ambient();
    char key[BUF_LEN_MAX];

    sprintf(key, "%d", FT_ITEM_OPTI_SENS);
    ft_config_set_int(key, (status > 0) ? FT_STATUS_OK : FT_STATUS_FAIL);
}

#define IMEI_INFO_FILE  "/parameters/modem/imei"
#define SN_INFO_FILE    "/parameters/modem/sn"
#define AT_BUFFER_MAX   1023

#define SEND_AT(cmd, resp) {\
    int retry = 0; \
    do { \
        if ((resp = hw_gsm_send_at(cmd))) break; \
        sleep(1); \
    } while (retry++ < 10); \
    }
    
static void on_imei_sn_handler(FTButton *button, void *data)
{
    char *imei_info = NULL;
    char *sn_info = NULL;
    char *imei = NULL;
    char *sn = NULL;
    char tmp[AT_BUFFER_MAX+1] = {0};

    FTWindow *window = ft_textpad_new("Wait modem on...", 1);

    ft_textpad_set_id(window, FT_ITEM_IMEI_SN);
    ft_window_show(window);
#if 0
    imei_info = hw_file_read(IMEI_INFO_FILE, BUF_LEN_MAX);
    sn_info = hw_file_read(SN_INFO_FILE, BUF_LEN_MAX);

    if (imei_info == NULL || sn_info == NULL)
    {
        ft_textpad_set_color(window, &ft_color_r);
        ft_textpad_set_text(window, "No IMEI/SN info!");
        return;
    }

    SEND_AT("AT+EGMR=0,7", imei);   /* read IMEI */
    SEND_AT("AT+EGMR=0,5", sn);     /* read SN */

    if (!strstr(imei, imei_info))   /* write IMEI (15 digits) */
    {
        snprintf(tmp, AT_BUFFER_MAX, "AT+EGMR=1,7,\"%s\"", imei_info);
        free(imei);
        SEND_AT(tmp, imei);
    }

    if (!strstr(sn, sn_info))       /* write SN (less than 30 chars) */
    {
        snprintf(tmp, AT_BUFFER_MAX, "AT+EGMR=1,5,\"%s\"", sn_info);
        free(sn);
        SEND_AT(tmp, sn);
    }

    if (strstr(imei, "ERROR") || strstr(sn, "ERROR"))
    {
        snprintf(tmp, AT_BUFFER_MAX, "IMEI:\n%s\nSN:\n%s", imei, sn);
        ft_textpad_set_color(window, &ft_color_r);
        ft_textpad_set_text(window, tmp);
    }
    else
    {
        snprintf(tmp, AT_BUFFER_MAX, "IMEI: %s\n%sSN: %s\n%s", imei_info, imei, sn_info, sn);
        ft_textpad_set_text(window, tmp);
    }
#else
    SEND_AT("AT+EGMR=0,7", imei);   /* read IMEI */
    SEND_AT("AT+EGMR=0,5", sn);     /* read SN */

    snprintf(tmp, AT_BUFFER_MAX, "IMEI:\n%s\nSN:\n%s", imei, sn);
    ft_textpad_set_text(window, tmp);
#endif
    free(imei_info);
    free(imei);
    free(sn_info);
    free(sn);
}

const FTestItem ft_test_items[] = 
{
    {-1, "Auto Test", on_auto_test_handler, NULL},
    {-1, "Tools", on_tools_handler, NULL},
    {FT_ITEM_MOTOR, "Vibrator", on_vibrator_handler, NULL},
    {FT_ITEM_LCD, "LCD", on_lcdcolor_handler, NULL},
    {FT_ITEM_LED, "LED", on_led_handler, NULL},
    {FT_ITEM_SPK, "Speaker", on_loudspk_handler, NULL},
    {FT_ITEM_KEY, "Key", on_keyboard_handler, NULL},
    {FT_ITEM_CAMERA, "Camera", on_camera_handler, NULL},
    {FT_ITEM_LOOP, "Echoloop", on_echoloop_handler, NULL},
    {FT_ITEM_HEADSET, "Headset", on_headset_handler, NULL},
    {FT_ITEM_FM, "FM", on_fm_handler, NULL},
    {FT_ITEM_GSM, "GSM", on_gsm_handler, NULL},
    {FT_ITEM_ADC, "ADC", on_adc_handler, NULL},
    {FT_ITEM_MATRIX, "M * M", on_matrix_handler, NULL},
    {FT_ITEM_G_SENS, "G-Sensor", on_g_sensor_handler, ft_testcase_gsensor},
    {FT_ITEM_GPS, "GPS", on_gps_handler, ft_testcase_gps},
    {FT_ITEM_SD, "T-Card", on_memory_card_handler, ft_testcase_sdcard},
    {FT_ITEM_WIFI, "Wifi", on_wifi_handler, ft_testcase_wifi},
    {FT_ITEM_BT, "Bluetooth", on_bluetooth_handler, ft_testcase_bluetooth},
    // {FT_ITEM_IMEI_SN, "IMEI/SN", on_imei_sn_handler},
    // {FT_ITEM_INFRA_SENS, "Infra-sens", on_infrared_sensor_handler},
    // {FT_ITEM_OPTI_SENS, "Opti-sens", on_optical_sensor_handler},
};

static void on_config_changed(const char *key, const char *value, void *data)
{
    FTButton **buttons = (FTButton **)data;
    int id = atoi(key);
    int i = 0;

    if (id < 0)
        return;

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

        case FT_ITEM_GSM:
            hw_gsm_end_call();
            break;

        case FT_ITEM_WIFI:
            hw_wifi_close();
            break;
    }

    for (; i < FT_N_ELEMENTS(ft_test_items); i++)
    {
        if (id == ft_test_items[i].id)
        {
            ft_status_button_set_status((FTStatusButton *)buttons[i], atoi(value));

            if (ft_context.autotest)
            {
                ft_context.start = 1;
                ft_context.result = atoi(value);
            }
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

static void *autotest_thread(void *arg)
{
    FTWindow *window;
    int index = 2;

    ft_context.autotest = 1;
    ft_context.start = 1;
    ft_context.result = -1;

    window = ft_notepad_new();
    ft_window_show(window);
    ft_notepad_append(window, "======== [AUTO TEST] ========", NULL); 

    while (1)
    {
        if (ft_context.start)
        {
            char result[32];

            /* draw result of last test */
            if (ft_context.result == FT_STATUS_OK)
            {
                strcpy(result, "[OK]");
                ft_notepad_append(window, result, &ft_color_g);
            }
            else if (ft_context.result == FT_STATUS_FAIL)
            {
                strcpy(result, "[FAIL]");
                ft_notepad_append(window, result, &ft_color_r);
                ft_notepad_append_line(window,
                        "======== [TEST FAIL] ========", &ft_color_r); 
                break;
            }
            else if (ft_context.result == FT_STATUS_NORMAL)
            {
                strcpy(result, "[SKIP]");
                ft_notepad_append(window, result, &ft_color_y);
            }

            if (!ft_context.autotest)
            {
                ft_notepad_append_line(window,
                        "======== [ALL DONE] ========", &ft_color_g); 
                break;
            }

            const FTestItem *item = &ft_test_items[index];

            ft_notepad_append_line(window, item->desc, NULL);
            ft_notepad_append(window, " ... ", NULL);

            if (item->test)
            {
                char key[BUF_LEN_MAX];

                sprintf(key, "%d", item->id);
                ft_context.result = item->test() ? FT_STATUS_OK : FT_STATUS_FAIL;
                ft_config_set_int(key, ft_context.result);
            }
            else
            {
                item->handler(NULL, NULL);
                ft_context.start = 0;
            }

            if (++index >= FT_N_ELEMENTS(ft_test_items))
            {
                ft_context.autotest = 0;
            }
        }

        usleep(100000);
    }

    return NULL;
}

static void on_auto_test_handler(FTButton *button, void *data)
{
    pthread_t tid;

    pthread_create(&tid, NULL, autotest_thread, NULL);
}

static void on_tools_handler(FTButton *button, void *data)
{
    const char *ver = hw_get_version();

    FTWindow *window = ft_textpad_new(ver, 1);

    button = ft_button_new("Clear Results");
    ft_button_set_handler(button, on_clear_results_handler, data);
    ft_window_add(window, (FTWidget *)button, 0);

    button = ft_button_new("IMEI/SN");
    ft_button_set_handler(button, on_imei_sn_handler, data);
    ft_window_add(window, (FTWidget *)button, 1);

    button = ft_button_new("Coupling Test");
    ft_button_set_handler(button, on_coupling_test_handler, data);
    ft_window_add(window, (FTWidget *)button, 2);

    button = ft_button_new("Shutdown");
    ft_button_set_handler(button, on_shutdown_handler, data);
    ft_window_add(window, (FTWidget *)button, 3);

    ft_textpad_set_id(window, FT_ITEM_VER);
    ft_window_show(window);
}

