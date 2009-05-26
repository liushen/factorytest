#include "ft_type.h"
#include "ft_keyboard.h"
#include "ft_matrix.h"
#include "ft_lcdcolor.h"
#include "gui/ft_input.h"
#include "gui/ft_window.h"
#include "gui/ft_button.h"

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

static void on_lcdcolor_handler(FTButton *button, void *data)
{
    FTWindow *window = ft_lcdcolor_new();
    ft_window_show(window);
}

int main(int argc, char *argv[])
{
    FTWindow *window;
    FTButton *button;

    window = ft_window_new();

    button = ft_button_new("Version");
    ft_window_add_child(window, (FTWidget *)button);
    ft_widget_set_focus((FTWidget *)button);

    button = ft_button_new("Echoloop");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Key");
    ft_button_set_handler(button, on_keyboard_handler, button);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Vibrator");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Loud SPK");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Ring");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("LED");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("LCD");
    ft_button_set_handler(button, on_lcdcolor_handler, button);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Contrast");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Receiver");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("ADC");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Charger");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Handset");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("RTC");
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("M * M");
    ft_button_set_handler(button, on_matrix_handler, button);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Camera");
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

