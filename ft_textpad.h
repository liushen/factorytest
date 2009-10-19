#include "gui/ft_window.h"

#ifndef _FT_TEXTPAD_H_
#define _FT_TEXTPAD_H_

FTWindow *ft_textpad_new(const char *text, int center);

void ft_textpad_set_text(FTWindow *textpad, const char *text);

void ft_textpad_set_color(FTWindow *textpad, FTColor *color);

void ft_textpad_set_id(FTWindow *textpad, int test_id);

void ft_textpad_destroy(FTWidget *widget);

#endif/*_FT_TEXTPAD_H_*/
