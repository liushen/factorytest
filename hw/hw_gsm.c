#undef LOG_TAG
#define LOG_TAG "Factory"

#include "hw_gsm.h"
#include "hw_audio.h"
#include "modemcontrol.h"

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <cutils/log.h>

#define AT_BUFFER_MAX   1023

int hw_gsm_open()
{
    int fd = open(HW_MODEM_DEV, O_RDWR);

    if (fd > 0)
    {
        ioctl(fd, MODEM_PWR_ON);
        close(fd);

        hw_gsm_send_at("AT+ESLP=0");

        return 1;
    }

    return 0;
}

int hw_gsm_close()
{
    int fd = open(HW_MODEM_DEV, O_RDWR);

    if (fd > 0)
    {
        ioctl(fd, MODEM_PWR_OFF);
        close(fd);

        return 1;
    }

    return 0;
}

#if 0
char *hw_gsm_send_at(const char *at)
{
    char cmd[AT_BUFFER_MAX+1];
    char resp[AT_BUFFER_MAX+1];
    FILE *fp;
    int len, i = 0;

    snprintf(cmd, AT_BUFFER_MAX, 
            "%s %s '%s'", AT_CMD_EXEC, AT_CMD_DEV, at);

    fp = popen(cmd, "r");
    len = fread(resp, 1, AT_BUFFER_MAX, fp);
    resp[len] = '\0';

    for (; i < len; i++)
    {
        if (resp[i] == '\r' || resp[i] == '\n')
            resp[i] = ' '; 
    }

    if (len > 0)
        return strdup(resp);

    return NULL;
}
#else
int create_cmd_fd(const char *dev)
{
    char resp[AT_BUFFER_MAX+1] = {0};

    /* BEGIN serial port set, this section is copy from linux-sio.c */
    struct termios oldtio, newtio;
    struct termios* newtio_p = &newtio;
    unsigned int tio_baudrate = HW_MODEM_SPEED;

	int fd = open(dev, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0)
    {   
        perror(dev);
        return 0;
    }

    memset(newtio_p, 0, sizeof(newtio));

    /* save current port settings */
    tcgetattr(fd, &oldtio);

	// fcntl(fd, F_SETFL, 0);
	newtio_p->c_cflag = CLOCAL | CREAD | CS8 | HUPCL | tio_baudrate | CRTSCTS;
	newtio_p->c_cflag &= ~tio_baudrate;
	newtio_p->c_cflag |= tio_baudrate;

	newtio_p->c_iflag = IGNBRK;	
    newtio_p->c_iflag &= ~(INLCR | ICRNL | IGNCR);;
    /* set input mode */
    newtio_p->c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	/* set raw output */
	newtio_p->c_oflag &= ~OPOST;
	newtio_p->c_oflag &= ~OLCUC;
	newtio_p->c_oflag &= ~ONLRET;
	newtio_p->c_oflag &= ~ONOCR;
	newtio_p->c_oflag &= ~OCRNL;

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, newtio_p);

    return fd;
}

char *hw_gsm_send_at(const char *at)
{
    static int fd = 0;
    struct timeval tv;
    char resp[AT_BUFFER_MAX+1] = {0};
    char *out = NULL;
    int retval, len = 0, i = 0;
    fd_set rfds;

    if (fd < 1)
        fd = create_cmd_fd(AT_CMD_DEV);

    if (fd < 1)
    {
        LOGE("create_cmd_fd(\"%s\") fail!", AT_CMD_DEV);
        return NULL;
    }

    LOGD("SEND: %s", at);
    write(fd, at, strlen(at));
    write(fd, "\n", 1);

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    while((retval = select(fd + 1, &rfds, NULL, NULL, &tv)) != -1)
    {
        if (retval < 1)
            break;

        if (len >= AT_BUFFER_MAX)
            break;

        len += read(fd, resp+len, AT_BUFFER_MAX-len);
        resp[len] = '\0';

        if (strstr(resp, "OK") || strstr(resp, "ERROR"))
            break;

        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
    }

    if (len < 1)
        return NULL;

    out = strrchr(resp, '+');
    out = out ? out : resp;

    for (i = 0; i < len; i++)
    {
        if (out[i] == '\r')
        {
            if (out[i-1] == '\n' && i > 0)
                out[i-1] = ' ';

            out[i] = ' ';
        }
    }

    LOGD("RECV: %s", out);
    return strdup(out);
}
#endif

char *hw_gsm_call(const char *no)
{
    char at[AT_BUFFER_MAX+1];

    snprintf(at, AT_BUFFER_MAX, "%s%s;", "ATD", no);

    return hw_gsm_send_at(at);
}

char *hw_gsm_list()
{
    return hw_gsm_send_at("AT+CLCC");
}

char *hw_gsm_end_call()
{
    return hw_gsm_send_at("ATH");
}

#ifdef HW_GSM_TEST
int exec_cmd(const char *dev, const char *cmd)
{
    fd_set rfds;
    struct timeval tv;
    int fd, retval, len;
    char resp[AT_BUFFER_MAX+1] = {0};

    /* BEGIN serial port set, this section is copy from linux-sio.c */
    struct termios oldtio, newtio;
    struct termios* newtio_p = &newtio;
    unsigned int tio_baudrate = HW_MODEM_SPEED;

	fd = open (dev, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd <0)
    {   
        perror(dev);
        return 0;
    }

    memset(newtio_p, 0, sizeof(newtio));

    /* save current port settings */
    tcgetattr(fd, &oldtio);

	//fcntl(fd, F_SETFL, 0);
	newtio_p->c_cflag = CLOCAL | CREAD | CS8 | HUPCL | tio_baudrate | CRTSCTS;
	newtio_p->c_cflag &= ~tio_baudrate;
	newtio_p->c_cflag |= tio_baudrate;

	newtio_p->c_iflag = IGNBRK;	
    newtio_p->c_iflag &= ~(INLCR | ICRNL | IGNCR);;
    /* set input mode */
    newtio_p->c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	/* set raw output */
	newtio_p->c_oflag &= ~OPOST;
	newtio_p->c_oflag &= ~OLCUC;
	newtio_p->c_oflag &= ~ONLRET;
	newtio_p->c_oflag &= ~ONOCR;
	newtio_p->c_oflag &= ~OCRNL;

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, newtio_p);
    /* END serial port set */

    write(fd, cmd, strlen(cmd));
    write(fd, "\n", 1);

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    while((retval = select(fd + 1, &rfds, NULL, NULL, &tv)) != -1)
    {
        len = read(fd, resp, AT_BUFFER_MAX);
        resp[len] = '\0';

        if (retval == 0)
            break;

        char *out = strrchr(resp, '+');

        printf("%s\n", out ? out : resp);

        if (strstr(resp, "OK"))
            break;

        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
    }

    /* restore the port settings */
    tcsetattr(fd, TCSANOW, &oldtio);
    close(fd);

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 2) /* power on modem */
    {
        if (argv[1][0] == '1')
        {
            printf("Power on modem...\n");
            return hw_gsm_open();
        }
        else
        {
            printf("Power off modem...\n");
            return hw_gsm_close();
        }
    }
    else if (argc == 3)
    {
        return exec_cmd(argv[1], argv[2]);
    }
    else
    {
        printf("Usage: %s <device> <AT command>\n", argv[0]);
        printf("       %s <0/1> power off/on modem.\n", argv[0]);
        return -1;
    }
}
#endif

