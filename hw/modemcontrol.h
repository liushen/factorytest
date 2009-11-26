/*
 * =====================================================================================
 *   Modemcontrol Driver for Broncho
 *   
 *   Copyright (c) 2006-2008 XiangYu
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2 as
 *   published by the Free Software Foundation.
 *
 *
 * =====================================================================================
 */
#ifndef _MODEMCONTROL_H_
#define _MODEMCONTROL_H_


#ifdef __KERNEL__
#include <linux/ioctl.h>
#include <linux/miscdevice.h>
#else
#include <sys/ioctl.h>
#endif

#define MODEMCONTROL_IOC_MAGIC  'h'
#define MODEM_PWR_ON				_IOW(MODEMCONTROL_IOC_MAGIC, 10, int)
#define MODEM_PWR_OFF				_IOW(MODEMCONTROL_IOC_MAGIC, 11, int)

#define MODEM_POWER_ON     0
#define MODEM_POWER_OFF    1

#endif /*_MODEMCONTROL_H_*/
