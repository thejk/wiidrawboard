#include "hidapi.h"
#include <iostream>
#include <wchar.h>
#include <string>
#include <sstream>
using namespace std;

typedef unsigned short u16;

class HIDDevice {
public:
	HIDDevice(hid_device* device) {
		mDevice = device;
	}
	bool parseInfo(hid_device* dev = NULL) {
		if(dev == NULL)
			dev = mDevice;
		wchar_t* buff = new wchar_t[128];
		memset(buff, '\0', 128);
		int r = hid_get_manufacturer_string(dev, buff, 128);
		if(!r)
			return false;
		mManufacturer = buff;

		r = hid_get_product_string(dev, buff, 128);
		if(!r)
			return false;
		mProduct = parseWString(buff);
		
		r = hid_get_serial_number_string(dev, buff, 128);
		if(!r)
			return false;
		mSerial = buff;

		return true;
	}
private:
	u16 parseWString(wstring str) {
		wstringstream ss;
		ss <<str;
		u16 retr = -1;
		ss >>retr;
		return retr;
	}
	hid_device* mDevice;
	wstring mSerial;
	u16 mProduct;
	wstring mManufacturer;
};

class HIDMgr
{
public:
	HIDMgr() {
		hid_init();
	}
	~HIDMgr() {
		hid_exit();
	}
	HIDDevice* open(u16 vendor, u16 product) {
		hid_device* raw = hid_open(vendor, product, NULL);
		if(raw == NULL)
			return NULL;
		else
			return new HIDDevice(raw);
	}
private:
	hid_device* mDevice;
};

int main(int argc, char** argv) {
	HIDMgr mgr;
	u16 vendor = 0x057E;
	u16 product = 0x0306;
	const wchar_t* tmp_serial = L"E0-E7-51-97-74-09";
	wchar_t* serial = new wchar_t[wcslen(tmp_serial)+1];
	memcpy(serial, tmp_serial, (wcslen(tmp_serial)+1)*sizeof(wchar_t));
	wcout <<"Serial: " <<wcslen(serial) <<" " <<serial <<endl;
	
	char pin[6];
	pin[5] = 0x0;
	pin[4] = 0x1;
	pin[3] = 0xb;
	pin[2] = 0xd0;
	pin[1] = 0xea;
	pin[0] = 0xe0;
	
	HIDDevice* device = mgr.open(vendor, product);
	cout <<(device!=NULL?"success":"failure") <<endl;
	
	/*
	hid_device_info* devs = hid_enumerate(0, 0);
	int count = 0;
	while(devs != 0) {
		wcout <<"manufacturer: " <<devs->manufacturer_string <<endl;
		cout <<"vendor: " <<hex <<devs->vendor_id <<endl;
		cout <<"product: " <<hex <<devs->product_id <<endl;
		devs = devs->next;
		count++;
	}
	cout <<count <<" devices" <<endl;
	hid_free_enumeration(devs)*/
}
