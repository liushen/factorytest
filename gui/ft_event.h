#include <time.h>
#include "ft_thread.h"

#ifndef _FT_EVENTS_H_
#define _FT_EVENTS_H_

#define EVENTS_MAX      (16)

#define FT_KEY_HOME     (102)
#define FT_KEY_BACK     (158)
#define FT_KEY_SEND     (231)
#define FT_KEY_END      (107)
#define FT_KEY_OK       (232)
#define FT_KEY_MENU     (229)

#define FT_KEY_VOL_ADD  (114)
#define FT_KEY_VOL_SUB  (115)
#define FT_KEY_POWER    (116)

#define FT_KEY_MOUSE    (330)

typedef int    FTEventId; 
typedef int    FTEventKey; 
typedef time_t FTEventTime;

typedef struct _FTEvent         FTEvent;
typedef struct _FTKeyEvent      FTKeyEvent;
typedef struct _FTMouseEvent    FTMouseEvent;

typedef enum
{
    FE_KEY_EVENT = 0,
    FE_KEY_PRESS,
    FE_KEY_RELEASE,

    FE_MOUSE_EVENT,
    FE_MOUSE_PRESS,
    FE_MOUSE_RELEASE,

    FE_UI_EVENT,
    FE_FOCUS_CHANGED,
    FE_BUTTON_CLICKED,
    FE_BUTTON_FOCUSED,
} FTEventType;

struct _FTEvent
{
    FTEventType type;
    FTEventTime time;
    FTEventId   id;
};

struct _FTKeyEvent
{
    FTEvent     event;
    FTEventKey  key;
};

struct _FTMouseEvent
{
    FTEvent     event;
    FTEventKey  key;
    int         x, y;
};

typedef void (*FEHandler)(FTEvent *event, void *data);

FTEvent *ft_event_get();

FTEvent *ft_event_get_last();

void ft_event_put(FTEvent *event);

void ft_event_set_handler(FEHandler func, void *data);

void ft_event_clean();

#endif/*_FT_EVENTS_H_*/
