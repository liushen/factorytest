#include "hw_bluetooth.h"
#include <bluedroid/bluetooth.h>
#include <stdio.h>
#include <string.h>

int hw_bluetooth_enable()
{
    if (bt_is_enabled())
        return 1;

    return bt_enable() == 0;
}

int hw_bluetooth_disable()
{
    if (!bt_is_enabled())
        return 1;

    return bt_disable() == 0;
}

int hw_bluetooth_is_enabled()
{
    return bt_is_enabled();
}

char *hw_bluetooth_scan()
{
    char buf[1024] = {0};
    FILE *fp = NULL;
    char *p = NULL;
    int i = 0;

    fp = popen("hcitool scan", "r");
    fread(buf, 1, sizeof(buf) - 1, fp);

    if ((p = strstr(buf, "\n")))
        p++;

    if (!p)
        return strdup("Device or resource busy,\nplease try again later!");

    if (*p == '\0')
        return strdup("No device found!");

    for (; p[i]; i++)
    {
        if (p[i] == '\t')
            p[i] = ' ';
    }

    pclose(fp);

    return strdup(p);
}

