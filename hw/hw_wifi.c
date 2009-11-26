#include "hw_wifi.h"
#include <stdio.h>
#include <stdlib.h>

int hw_wifi_open()
{
    return system("iwpriv mlan0 deepsleep 0") == 0;
}

int hw_wifi_close()
{
    return system("iwpriv mlan0 deepsleep 1") == 0;
}

char *hw_wifi_scan()
{
    char buf[4096] = {0};
    char *s, *e, *p, *out;
    FILE *fp = NULL;
    int len = 0;

    fp = popen("iwlist mlan0 scanning", "r");
    len = fread(buf, 1, sizeof(buf) - 1, fp);

    s = buf;
    p = out = malloc(len + 1);

    while ((s = strstr(s, "ESSID:")))
    {
        s += strlen("ESSID:");
        e = strstr(s, "\n");

        if (e)
        {
            strncpy(p, s, e - s);
            p += e - s;
            *p++ = '\n';
        }
        else
        {
            strcpy(p, s);
            p += strlen(s);
        }
    }
    
    if (p == out)
        strcpy(p, "No wireless AP found!");
    else
        *p = '\0';

    pclose(fp);

    return out;
}

