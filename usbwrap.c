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
#include "usbwrap.h"

static char errMsg[256];

// Get the last error message, or junk if no error occurred.
//
const char *usbStrError(void) {
	return errMsg;
}

// Initialise USB
//
void usbInitialise(void) {
	usb_init();
	usb_find_busses();
	usb_find_devices();
}

// Find the descriptor of the first occurance of the specified device
//
USBStatus usbOpenDevice(unsigned short vid, unsigned short pid, int configuration, int interface, int alternateInterface, UsbDeviceHandle **devHandlePtr) {
	struct usb_bus *bus;
	struct usb_device *thisDevice;
	UsbDeviceHandle *deviceHandle;
	int returnCode;
	bus = usb_get_busses();
	if ( bus ) {
		// This system has one or more USB buses...let's step through them looking for the specified VID/PID
		//
		do {
			thisDevice = bus->devices;
			while ( thisDevice && (thisDevice->descriptor.idVendor != vid || thisDevice->descriptor.idProduct != pid) ) {
				thisDevice = thisDevice->next;
			}
			bus = bus->next;
		} while ( bus && !thisDevice );  // will break out if I run out of buses, or if device found
		if ( !thisDevice ) {
			// The VID/PID was not found after scanning all buses
			//
			*devHandlePtr = NULL;
			sprintf(errMsg, "Device not found");
			return USB_DEVICE_NOT_FOUND;
		} else {
			// The VID/PID was found; let's see if we can open the device
			//
			deviceHandle = usb_open(thisDevice);
			if ( deviceHandle == NULL ) {
				*devHandlePtr = NULL;
				sprintf(errMsg, "usb_open(): %s", usb_strerror());
				return USB_CANNOT_OPEN_DEVICE;
			}
			*devHandlePtr = deviceHandle;  // Return the valid device handle anyway, even if subsequent operations fail
			returnCode = usb_set_configuration(deviceHandle, configuration);
			if ( returnCode < 0 ) {
				sprintf(errMsg, "usb_set_configuration(): %s", usb_strerror());
				return USB_CANNOT_SET_CONFIGURATION;
			}
			returnCode = usb_claim_interface(deviceHandle, interface);
			if ( returnCode < 0 ) {
				sprintf(errMsg, "usb_claim_interface(): %s", usb_strerror());
				return USB_CANNOT_CLAIM_INTERFACE;
			}
			returnCode = usb_set_altinterface(deviceHandle, alternateInterface);
			if ( returnCode < 0 ) {
				sprintf(errMsg, "usb_set_altinterface(): %s", usb_strerror());
				return USB_CANNOT_SET_ALTINT;
			}
			return USB_SUCCESS;
		}
	} else {
		// No USB buses on this system!?!?
		//
		*devHandlePtr = NULL;
		sprintf(errMsg, "No USB buses found");
		return USB_NO_BUSES;
	}
}
