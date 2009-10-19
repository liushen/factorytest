#include "ft_event.h"

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <linux/input.h>

#define FT_INPUT_DEVICE     "/dev/input/event"
#define FT_INPUT_MAX        8
#define BUFFER_SIZE         64
#define EVENT_SIZE          sizeof(struct input_event)

static int *open_all_inputs(int *maxfd)
{
    static int fds[FT_INPUT_MAX+1] = {0};

    char input[BUFFER_SIZE] = {0};
    int fd, max = 0, i = 0;

    for (i = 0; i < FT_INPUT_MAX; i++)
    {
        snprintf(input, BUFFER_SIZE, "%s%d", FT_INPUT_DEVICE, i);

        fd = open(input, O_RDONLY);

        if (fd < 0)
            break;

        fds[i] = fd;

        if (fd > max)
            max = fd;
    }

    *maxfd = max;

    return fds;
}

static void init_fds(int *fds, fd_set *set)
{
    int i = 0;

    FD_ZERO(set);

    for (i = 0; fds[i]; i++)
    {
        FD_SET(fds[i], set);
    }
}

static void dispatch_event(struct input_event *e)
{
    static int x = 0, y = 0, key, code;

    FTMouseEvent me;
    FTKeyEvent ke;
    FTEvent *event;

    // printf("EVENT: type=%d, code=%d, value=%d, x=%d, y=%d\n", 
    //         e->type, e->code, e->value, x, y);

    switch (e->type)
    {
        case EV_ABS:
            if (e->code == REL_X)
            {
                x = e->value;
            }
            else if (e->code == REL_Y)
            {
                y = e->value;
            }

            break;

        case EV_SYN:
            if (code == FT_KEY_MOUSE)
            {
                event = (FTEvent *)&me;
                event->type = key ? FE_MOUSE_PRESS : FE_MOUSE_RELEASE;

                me.x = x;
                me.y = y;

                ft_event_put(event);
            }

            break;

        case EV_REL:
            event = (FTEvent *)&ke;

            if (e->code == REL_X)
            {
                if (e->value < 0)
                    ke.key = FT_KEY_LEFT;
                else
                    ke.key = FT_KEY_RIGHT;
            }
            else /* REL_Y */
            {
                if (e->value < 0)
                    ke.key = FT_KEY_UP;
                else
                    ke.key = FT_KEY_DOWN;
            }

            event->type = FE_KEY_PRESS;
            ft_event_put(event);

            event->type = FE_KEY_RELEASE;
            ft_event_put(event);

        case EV_KEY:
            if (e->code == FT_KEY_MOUSE)
            {
                key = e->value;
            }
            else
            {
                event = (FTEvent *)&ke;
                event->type = e->value ? FE_KEY_PRESS : FE_KEY_RELEASE;

                ke.key = e->code;

                ft_event_put(event);
            }

            code = e->code;

            break;
    }
}

void wait_for_events()
{
    struct input_event events[BUFFER_SIZE];
    struct input_event *e;

    fd_set rfds;
    int maxfd, retval, i, byte = 0;
    int *fds;

    fds = open_all_inputs(&maxfd);

    init_fds(fds, &rfds);

    while ((retval = select(maxfd + 1, &rfds, NULL, NULL, NULL)))
    {
        if (retval < 0)
            continue;

        for (i = 0; fds[i]; i++)
        {
            if (FD_ISSET(fds[i], &rfds))
            {
                byte = read(fds[i], events, EVENT_SIZE * BUFFER_SIZE);
                break;
            }
        }

        init_fds(fds, &rfds);

        if (byte < EVENT_SIZE)
            continue;

        for (i = 0; i < byte / EVENT_SIZE; i++)
        {
            e = &events[i];

            dispatch_event(e);
        }
    }
}

