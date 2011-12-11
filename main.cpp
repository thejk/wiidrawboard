#include "hidapi.h"
#include <iostream>
#include <wchar.h>
using namespace std;

typedef unsigned short u16;

class HIDDevice {
public:
	HIDDevice(hid_device* device) {
		mDevice = device;
	}
private:
	hid_device* mDevice;
	u16 mVendor;
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
	bool open(u16 vendor, u16 product, hid_device** device) {
		hid_device* dev = hid_open(vendor, product, NULL));
		*device = dev;
		return dev != NULL;
	}
private:
	hid_device* mDevice;
};

int main(int argc, char** argv) {
	HIDMgr mgr;
	unsigned short vendor = 0x057E;
	unsigned short product = 0x0306;
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
	
	cout <<"foo" <<endl;
	bool done = mgr.open(vendor, product);
	hid_device* device = hid_open(vendor, product, NULL);
	cout <<(done?"success":"failure") <<endl;
	hid_device_info* dev = hid_enumerate(0, 0);
	
	/*int count = 0;
	while(dev != 0) {
		wcout <<"manufacturer: " <<dev->manufacturer_string <<endl;
		cout <<"vendor: " <<hex <<dev->vendor_id <<endl;
		cout <<"product: " <<hex <<dev->product_id <<endl;
		dev = dev->next;
		count++;
	}
	cout <<count <<" devices" <<endl;*/
}
