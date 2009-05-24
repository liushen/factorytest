#include "ft_type.h"
#include "gui/ft_input.h"
#include "gui/ft_draw.h"
#include "gui/ft_window.h"
#include "gui/ft_button.h"
#include "gui/ft_framebuffer.h"

int main(int argc, char *argv[])
{
    FBSurface *s = ft_frame_buffer_get_default();
    FTRect rect = {10, 10, 60, 60};
    FTRect rect1 = {0};
    FTDrawGC gc = {{255, 255, 255, 0}, {0, 0, 0, 0}, 0};
    int i;

    FTWindow *win = ft_window_new();

    FTButton *btn = ft_button_new("Version");
    ft_window_add_child(win, (FTWidget *)btn);
    ft_widget_set_focus(btn);

    btn = ft_button_new("Echoloop");
    ft_window_add_child(win, (FTWidget *)btn);

    btn = ft_button_new("Vibrator");
    ft_window_add_child(win, (FTWidget *)btn);

    btn = ft_button_new("Loud SPK");
    ft_window_add_child(win, (FTWidget *)btn);

    btn = ft_button_new("Ring");
    ft_window_add_child(win, (FTWidget *)btn);

    btn = ft_button_new("LED");
    ft_window_add_child(win, (FTWidget *)btn);

    btn = ft_button_new("LCD");
    ft_window_add_child(win, (FTWidget *)btn);

    btn = ft_button_new("Contrast");
    ft_window_add_child(win, (FTWidget *)btn);

    btn = ft_button_new("Receiver");
    ft_window_add_child(win, (FTWidget *)btn);

    btn = ft_button_new("ADC");
    ft_window_add_child(win, (FTWidget *)btn);

    btn = ft_button_new("Charger");
    ft_window_add_child(win, (FTWidget *)btn);

    btn = ft_button_new("Handset");
    ft_window_add_child(win, (FTWidget *)btn);

    btn = ft_button_new("RTC");
    ft_window_add_child(win, (FTWidget *)btn);

    btn = ft_button_new("Parell");
    ft_window_add_child(win, (FTWidget *)btn);

    btn = ft_button_new("M * M");
    ft_window_add_child(win, (FTWidget *)btn);

    ft_window_draw(win);
    ft_widget_set_visible(btn, 0);
/*
    FTPoint p = {0, 0};
    ft_draw_text(s, &p, "Hello world! 0123456789", &gc);

    for (i = 0; i < 80; i++)
    {
        ft_draw_box(s, &rect, &gc, 1);
        rect.x += 2;
        rect.y += 2;
        usleep(500);
    }
*/
    wait_for_events();

    ft_frame_buffer_close();

    return FT_SUCCESS;
}

