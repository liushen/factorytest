#include "ft_event.h"

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <linux/input.h>

#define FT_INPUT_DEVICE     "/dev/input/event0"
#define BUFFER_SIZE         64
#define EVENT_SIZE          sizeof(struct input_event)

void wait_for_events()
{
    static struct input_event events[BUFFER_SIZE];
    struct input_event *e;

    FTMouseEvent me;
    FTKeyEvent ke;
    FTEvent *event;

    int fd, byte, i;
    int x = 0, y = 0;

    fd = open(FT_INPUT_DEVICE, O_RDONLY);

    if (!fd)
    {
        perror("open "FT_INPUT_DEVICE);
        return;
    }

    while (1)
    {
        byte = read(fd, events, EVENT_SIZE * BUFFER_SIZE);

        if (byte < EVENT_SIZE)
            continue;

        for (i = 0; i < byte / EVENT_SIZE; i++)
        {
            e = &events[i];

            switch (e->type)
            {
                case EV_ABS:
                    if (e->code == 0)
                        x = e->value;
                    else
                        y = e->value;

                    break;

                case EV_KEY:
                    if (e->code == FT_KEY_MOUSE)
                    {
                        //printf("type=%d, code=%d, value=%d, x=%d, y=%d\n", 
                        //        e->type, e->code, e->value, x, y);

                        event = (FTEvent *)&me;
                        event->type = e->value ? FE_MOUSE_PRESS : FE_MOUSE_RELEASE;

                        me.x = x;
                        me.y = y;

                        ft_event_put(event);
                    }
                    else
                    {
                        event = (FTEvent *)&ke;
                        event->type = e->value ? FE_KEY_PRESS : FE_KEY_RELEASE;

                        ke.key = e->code;

                        ft_event_put(event);
                    }

                    break;
            }
        }
    }

#if 0
    fd_set rfds;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    FTEvent *event = (FTEvent *)&key;

    while (select(fd + 1, &rfds, NULL, NULL, NULL) >= 0)
    {
        if (read(fd, &e, sizeof(e)))
        {
            printf("type=%d, code=%d, value=%d\n", 
                    e.type, e.code, e.value);

            memset(&key, 0, sizeof(key));

            if (e.type != EV_KEY)
                continue;

            switch (e.value)
            {
                case 0:
                    event->type = FE_KEY_RELEASE;
                    key.key = e.code;

                    ft_event_put(event);
                    break;

                case 1:
                    event->type = FE_KEY_PRESS;
                    key.key = e.code;

                    ft_event_put(event);
                    break;

                default: break;
            }
        }
    }
#endif
}

