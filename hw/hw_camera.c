#include "hw_camera.h"
#include <stdlib.h>
#include <unistd.h>

#define HC_TEST_EXEC    "camera_test"

void hw_camera_open()
{
    if (fork() == 0)
    {
        execlp(HC_TEST_EXEC, HC_TEST_EXEC, NULL);
        exit(0);
    }
}

void hw_camera_close()
{
    system("killall -KILL "HC_TEST_EXEC);
}

