#include "gui/ft_input.h"
#include "gui/ft_window.h"
#include "gui/ft_button.h"
#include "hw/hw_comm.h"
#include "hw/hw_audio.h"
#include "hw/hw_led.h"

#include "ft_keyboard.h"
#include "ft_textpad.h"
#include "ft_matrix.h"
#include "ft_adc.h"
#include "ft_led.h"
#include "ft_lcdcolor.h"
#include "ft_lcdcontrast.h"

#include <stdio.h>
#include <string.h>

#define TEXT_LEN_MAX    (32)

FTColor ft_color_r = {0xff, 0, 0, 0};
FTColor ft_color_g = {0, 0xff, 0, 0};
FTColor ft_color_b = {0, 0, 0xff, 0};
FTColor ft_color_w = {0xff, 0xff, 0xff, 0};

static void on_version_handler(FTButton *button, void *data)
{
    const char *ver = hw_get_version();

    FTWindow *window = ft_textpad_new(ver, 1);
    ft_window_show(window);
}

static void on_keyboard_handler(FTButton *button, void *data)
{
    FTWindow *window = ft_keyboard_new();
    ft_window_show(window);
}

static void on_matrix_handler(FTButton *button, void *data)
{
    FTWindow *window = ft_matrix_new();
    ft_window_show(window);
}

static void on_led_handler(FTButton *button, void *data)
{
    FTWindow *window = ft_led_new();
    ft_window_show(window);
}

static void on_lcdcolor_handler(FTButton *button, void *data)
{
    FTWindow *window = ft_lcdcolor_new();
    ft_window_show(window);
}

static void on_lcdcontrast_handler(FTButton *button, void *data)
{
    FTWindow *window = ft_lcdcontrast_new();
    ft_window_show(window);
}

static void on_adc_handler(FTButton *button, void *data)
{
    FTWindow *window = ft_adc_new();
    ft_window_show(window);
}

static void on_echoloop_handler(FTButton *button, void *data)
{
    if (strcmp(button->text, "Echoloop") == 0)
    {
        ft_button_set_text(button, "Echoloop (ON)");
        ft_button_set_color(button, &ft_color_g);

        hw_audio_echoloop_set(HA_DEVICE_RECEIVER, 1);
    }
    else
    {
        ft_button_set_text(button, "Echoloop");
        ft_button_set_color(button, &ft_color_w);

        hw_audio_echoloop_set(HA_DEVICE_RECEIVER, 0);
    }
}

static void on_vibrator_handler(FTButton *button, void *data)
{
    if (strcmp(button->text, "Vibrator") == 0)
    {
        ft_button_set_text(button, "Vibrator (ON)");
        ft_button_set_color(button, &ft_color_g);

        hw_vibrator_set(-1);
    }
    else
    {
        ft_button_set_text(button, "Vibrator");
        ft_button_set_color(button, &ft_color_w);

        hw_vibrator_set(0);
    }
}

static void on_loudspk_handler(FTButton *button, void *data)
{
    if (strcmp(button->text, "Loud SPK") == 0)
    {
        ft_button_set_text(button, "Loud SPK (ON)");
        ft_button_set_color(button, &ft_color_g);

        hw_audio_play(HA_DEVICE_SPEAKER, "/system/usr/share/factorytest/sound.wav");
    }
    else
    {
        ft_button_set_text(button, "Loud SPK");
        ft_button_set_color(button, &ft_color_w);

        hw_audio_stop(HA_DEVICE_SPEAKER);
    }
}

static void on_ring_handler(FTButton *button, void *data)
{
    if (strcmp(button->text, "Ring") == 0)
    {
        ft_button_set_text(button, "Ring (ON)");
        ft_button_set_color(button, &ft_color_g);

        hw_audio_play(HA_DEVICE_SPEAKER, "/system/usr/share/factorytest/ring.wav");
    }
    else
    {
        ft_button_set_text(button, "Ring");
        ft_button_set_color(button, &ft_color_w);

        hw_audio_stop(HA_DEVICE_SPEAKER);
    }
}

static void on_receiver_handler(FTButton *button, void *data)
{
    if (strcmp(button->text, "Receiver") == 0)
    {
        ft_button_set_text(button, "Receiver (ON)");
        ft_button_set_color(button, &ft_color_g);

        hw_audio_play(HA_DEVICE_RECEIVER, "/system/data/factorytest/sound.wav");
    }
    else
    {
        ft_button_set_text(button, "Receiver");
        ft_button_set_color(button, &ft_color_w);

        hw_audio_stop(HA_DEVICE_RECEIVER);
    }
}

static void on_handset_handler(FTButton *button, void *data)
{
    if (strcmp(button->text, "Handset") == 0)
    {
        ft_button_set_text(button, "Handset (ON)");
        ft_button_set_color(button, &ft_color_g);

        hw_audio_echoloop_set(HA_DEVICE_HANDSET, 1);
    }
    else
    {
        ft_button_set_text(button, "Handset");
        ft_button_set_color(button, &ft_color_w);

        hw_audio_echoloop_set(HA_DEVICE_HANDSET, 0);
    }
}

static void on_camera_handler(FTButton *button, void *data)
{
    FTWindow *window;

    int status = hw_detect_camera();
    
    window = ft_textpad_new(status ? "Camera hardware has been found." : 
                                     "Camera hardware not found!", 1);

    ft_textpad_set_color(window, status ? &ft_color_g : &ft_color_r);
    ft_window_show(window);
}

static void on_bluetooth_handler(FTButton *button, void *data)
{
    FTWindow *window;

    int status = hw_detect_bluetooth();
    
    window = ft_textpad_new(status ? "Bluetooth hardware has been found." : 
                                     "Bluetooth hardware not found!", 1);

    ft_textpad_set_color(window, status ? &ft_color_g : &ft_color_r);
    ft_window_show(window);
}

static void on_wifi_handler(FTButton *button, void *data)
{
    FTWindow *window;

    int status = hw_detect_wifi();
    
    window = ft_textpad_new(status ? "WIFI hardware has been found." : 
                                     "WIFI hardware not found!", 1);
    
    ft_textpad_set_color(window, status ? &ft_color_g : &ft_color_r);
    ft_window_show(window);
}

static void on_fm_handler(FTButton *button, void *data)
{
    FTWindow *window;

    int status = hw_detect_fm();
    
    window = ft_textpad_new(status ? "FM hardware has been found." : 
                                     "FM hardware not found!", 1);

    ft_textpad_set_color(window, status ? &ft_color_g : &ft_color_r);
    ft_window_show(window);
}

static void on_gps_handler(FTButton *button, void *data)
{
    FTWindow *window;

    int status = hw_detect_gps();
    
    window = ft_textpad_new(status ? "GPS hardware has been found." : 
                                     "GPS hardware not found!", 1);

    ft_textpad_set_color(window, status ? &ft_color_g : &ft_color_r);
    ft_window_show(window);
}

int main(int argc, char *argv[])
{
    FTWindow *window;
    FTButton *button;

    window = ft_window_new();

    button = ft_button_new("Version");
    ft_button_set_handler(button, on_version_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);
    ft_widget_set_focus((FTWidget *)button);

    button = ft_button_new("Echoloop");
    ft_button_set_handler(button, on_echoloop_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Key");
    ft_button_set_handler(button, on_keyboard_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Vibrator");
    ft_button_set_handler(button, on_vibrator_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Loud SPK");
    ft_button_set_handler(button, on_loudspk_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);
/*
    button = ft_button_new("Ring");
    ft_button_set_handler(button, on_ring_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);
*/
    button = ft_button_new("LED");
    ft_button_set_handler(button, on_led_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("LCD");
    ft_button_set_handler(button, on_lcdcolor_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);
/*
    button = ft_button_new("Contrast");
    ft_button_set_handler(button, on_lcdcontrast_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);
    
    button = ft_button_new("Receiver");
    ft_button_set_handler(button, on_receiver_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);
*/
    button = ft_button_new("ADC");
    ft_button_set_handler(button, on_adc_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Handset");
    ft_button_set_handler(button, on_handset_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("M * M");
    ft_button_set_handler(button, on_matrix_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Camera");
    ft_button_set_handler(button, on_camera_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);
/*
    button = ft_button_new("Bluetooth");
    ft_button_set_handler(button, on_bluetooth_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);
*/
    button = ft_button_new("Wifi");
    ft_button_set_handler(button, on_wifi_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("FM");
    ft_button_set_handler(button, on_fm_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("GPS");
    ft_button_set_handler(button, on_gps_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    ft_window_show(window);

    wait_for_events();

    ft_frame_buffer_close();

    return FT_SUCCESS;
}

