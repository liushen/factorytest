#include "ft_gsm.h"
#include "ft_config.h"
#include "ft_textpad.h"
#include "gui/ft_button.h"
#include "hw/hw_gsm.h"
#include "hw/hw_audio.h"

#include <stdio.h>
#include <string.h>

typedef struct _FGContext FGContext;

struct _FGContext
{
    int result;
};

static FGContext fg_context;

static void on_ok_handler(FTButton *button, void *data)
{
    fg_context.result = FT_STATUS_OK;

    ft_window_close(data);
}

static void on_fail_handler(FTButton *button, void *data)
{
    fg_context.result = FT_STATUS_FAIL;

    ft_window_close(data);
}

static void on_gsm_call_handler(FTButton *button, void *data)
{
    FTWindow *window = (FTWindow *)data;

    char *resp = hw_gsm_call(button->text + strlen("Call "));

    if (resp)
    {
        ft_textpad_set_text(window, resp);
        free(resp);
    }
}

static void on_gsm_end_call_handler(FTButton *button, void *data)
{
    FTWindow *window = (FTWindow *)data;

    char *resp = hw_gsm_end_call();

    if (resp)
    {
        ft_textpad_set_text(window, resp);
        free(resp);
    }
}

static void on_gsm_op_handler(FTButton *button, void *data)
{
    FTWindow *window = (FTWindow *)data;

    char *resp = hw_gsm_send_at("AT+COPS?");

    if (resp)
    {
        ft_textpad_set_text(window, resp);
        free(resp);
    }
}

static void on_gsm_route_handler(FTButton *button, void *data)
{
    if (strcmp(button->text, "Speaker") == 0)
    {
        hw_audio_set_route(HA_ROUTE_CALL_SPK);
        ft_button_set_text(button, "Handset");
    }
    else
    {
        hw_audio_set_route(HA_ROUTE_CALL_HS);
        ft_button_set_text(button, "Speaker");
    }
}

void ft_gsm_destroy(FTWidget *widget)
{
    char key[32];

    sprintf(key, "%d", FT_ITEM_LED);

    ft_config_set_int(key, fg_context.result);
    ft_window_destroy(widget);
}

FTWindow *ft_gsm_new()
{
    FTWindow *window;
    FTButton *button;

    window = ft_textpad_new("", 1);

    button = ft_button_new("Call 112");
    ft_button_set_handler(button, on_gsm_call_handler, window);
    ft_window_add(window, (FTWidget *)button, 0);

    button = ft_button_new("Call 10086");
    ft_button_set_handler(button, on_gsm_call_handler, window);
    ft_window_add(window, (FTWidget *)button, 1);

    button = ft_button_new("Call 10010");
    ft_button_set_handler(button, on_gsm_call_handler, window);
    ft_window_add(window, (FTWidget *)button, 2);

    button = ft_button_new("Oprator");
    ft_button_set_handler(button, on_gsm_op_handler, window);
    ft_window_add(window, (FTWidget *)button, 3);

    button = ft_button_new("Speaker");
    ft_button_set_handler(button, on_gsm_route_handler, window);
    ft_window_add(window, (FTWidget *)button, 4);

    button = ft_button_new("End call");
    ft_button_set_handler(button, on_gsm_end_call_handler, window);
    ft_window_add(window, (FTWidget *)button, 5);

    fg_context.result = FT_STATUS_NORMAL;

    hw_audio_set_route(HA_ROUTE_CALL_HS);

    return window;
}

