#include <linux/input.h>

#ifndef _FT_INPUT_H_
#define _FT_INPUT_H_

#if 0
#define FT_KEY_HOME     (102)
#define FT_KEY_BACK     (158)
#define FT_KEY_SEND     (231)
#define FT_KEY_END      (107)
#define FT_KEY_OK       (232)
#define FT_KEY_MENU     (229)

#define FT_KEY_VOL_ADD  (114)
#define FT_KEY_VOL_SUB  (115)
#define FT_KEY_POWER    (116)
#define FT_KEY_PHONE    (169)

#define FT_KEY_MOUSE    (330)
#else
#define FT_KEY_HOME     KEY_HOME
#define FT_KEY_BACK     KEY_BACK
#define FT_KEY_SEND     KEY_SEND
#define FT_KEY_END      KEY_END
#define FT_KEY_OK       KEY_ENTER   //KEY_CENTER
#define FT_KEY_MENU     KEY_MENU    //KEY_SOFT1

#define FT_KEY_VOL_ADD  KEY_VOLUMEUP
#define FT_KEY_VOL_SUB  KEY_VOLUMEDOWN
#define FT_KEY_POWER    KEY_POWER
#define FT_KEY_PHONE    KEY_PHONE

#define FT_KEY_MOUSE    BTN_TOUCH
#endif

void wait_for_events();

#endif/*_FT_INPUT_H_*/
