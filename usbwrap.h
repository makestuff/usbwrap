/* 
 * Copyright (C) 2009-2010 Chris McClelland
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBUSB_H
#define LIBUSB_H

#include "types.h"

#ifdef WIN32
	#define _CRT_SECURE_NO_WARNINGS
	#include <string.h>
	#define strcasecmp(x,y) _stricmp(x,y)
#else
	#include <strings.h>
	#include <sys/types.h>
	#include <linux/limits.h>
#endif
#include <usb.h>
#include <stdio.h>

typedef enum {
	USB_SUCCESS = 0,
	USB_NO_BUSES,
	USB_DEVICE_NOT_FOUND,
	USB_CANNOT_OPEN_DEVICE,
	USB_CANNOT_SET_CONFIGURATION,
	USB_CANNOT_CLAIM_INTERFACE,
	USB_CANNOT_SET_ALTINT
} USBStatus;

#define usbClose(x) usb_close(x)
typedef struct usb_dev_handle UsbDeviceHandle;

void usbInitialise(void);
USBStatus usbOpenDevice(uint16 vid, uint16 pid, int configuration, int iface, int alternateInterface, UsbDeviceHandle **devHandlePtr);
const char *usbStrError(void);

#endif
