#ifndef HID_HPP
#define HID_HPP

#include "types.hpp"
#include "hidapi.h"

#include <cwchar>
#include <string>

class HIDMgr;
class HIDDevice {
public:
	HIDDevice(hid_device* device, HIDMgr* mgr)
	: mDevice(device), mMgr(mgr) { }

	~HIDDevice() {
		if(mDevice != 0)
			hid_close(mDevice);
	}

	bool parseInfo(hid_device* dev = 0);

private:
	hid_device_info* getDevice(wchar_t* serial);
	uint16_t parseWString(std::wstring str);

	hid_device* mDevice;
    std::wstring mSerial;
	uint16_t mProduct;
    std::wstring mManufacturer;
	HIDMgr* mMgr;
};

class HIDMgr
{
public:
	HIDMgr();
	~HIDMgr();
	HIDDevice* open(uint16_t vendor, uint16_t product);
	void updateDevicesList();
	hid_device_info* getDevices() { return mDevices; }

private:
	hid_device_info* mDevices;
};

#endif /* HID_HPP */
