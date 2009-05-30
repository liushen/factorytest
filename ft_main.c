#include "gui/ft_input.h"
#include "gui/ft_window.h"
#include "gui/ft_button.h"
#include "hw/hw_comm.h"
#include "hw/hw_audio.h"
#include "hw/hw_led.h"

#include "ft_keyboard.h"
#include "ft_textpad.h"
#include "ft_matrix.h"
#include "ft_led.h"
#include "ft_lcdcolor.h"

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

static void on_adc_handler(FTButton *button, void *data)
{
    const char *adc = hw_get_ADC();

    FTWindow *window = ft_textpad_new(adc, 1);
    ft_window_show(window);
}

static void on_camera_handler(FTButton *button, void *data)
{
    char text[TEXT_LEN_MAX];
    int camera_nr = hw_get_camera_nr();

    snprintf(text, TEXT_LEN_MAX, "Found %d camera(s).", camera_nr);

    FTWindow *window = ft_textpad_new(text, 1);
    ft_textpad_set_color(window, &ft_color_r);
    ft_window_show(window);
}

static void on_vibrator_handler(FTButton *button, void *data)
{
    if (strcmp(button->text, "Vibrator") == 0)
    {
        ft_button_set_text(button, "Vibrator (ON)");
        ft_button_set_color(button, &ft_color_g);

        hw_vibrator_set(1);
    }
    else
    {
        ft_button_set_text(button, "Vibrator");
        ft_button_set_color(button, &ft_color_w);

        hw_vibrator_set(0);
    }
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
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Key");
    ft_button_set_handler(button, on_keyboard_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Vibrator");
    ft_button_set_handler(button, on_vibrator_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Loud SPK");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Ring");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("LED");
    ft_button_set_handler(button, on_led_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("LCD");
    ft_button_set_handler(button, on_lcdcolor_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Contrast");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Receiver");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("ADC");
    ft_button_set_handler(button, on_adc_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Handset");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("M * M");
    ft_button_set_handler(button, on_matrix_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Camera");
    ft_button_set_handler(button, on_camera_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Bluetooth");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Wifi");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("FM");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("GPS");
    ft_window_add_child(window, (FTWidget *)button);

    ft_window_show(window);

    wait_for_events();

    ft_frame_buffer_close();

    return FT_SUCCESS;
}

