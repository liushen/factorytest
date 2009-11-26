#ifndef _HW_GSM_H_
#define _HW_GSM_H_

#define HW_MODEM_SPEED  B115200
#define HW_MODEM_DEV    "/dev/modem"
#define AT_CMD_DEV      "/dev/ttyS0"
#define AT_CMD_EXEC     "at_test"

int hw_gsm_open();

int hw_gsm_close();

char *hw_gsm_call(const char *no);

char *hw_gsm_list();

char *hw_gsm_end_call();

char *hw_gsm_send_at(const char *at);

#endif/*_HW_GSM_H_*/
