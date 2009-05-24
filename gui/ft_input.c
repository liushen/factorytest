#include "ft_event.h"

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <linux/input.h>

#define KEY_INPUT_DEVICE    "/dev/input/event0"

void wait_for_events()
{
    struct input_event e;
    FTKeyEvent key;
    fd_set rfds;
    int fd;

    fd = open(KEY_INPUT_DEVICE, O_RDONLY);

    if (!fd)
    {
        perror("open()");
        return;
    }

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
}

