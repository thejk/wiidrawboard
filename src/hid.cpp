#include "hid.hpp"

#include <sstream>

using namespace std;

bool HIDDevice::parseInfo(hid_device* dev) {
    if(dev == 0)
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

hid_device_info* HIDDevice::getDevice(wchar_t* serial) {
    hid_device_info* dev = mMgr->getDevices();
    while(dev != 0) {
        if(wcscmp(dev->serial_number, serial) == 0)
            return dev;
    }
    return 0;
}

uint16_t HIDDevice::parseWString(wstring str) {
    wstringstream ss;
    ss <<str;
    uint16_t retr = -1;
    ss >>retr;
    return retr;
}

HIDMgr::HIDMgr() {
    mDevices = 0;
    hid_init();
}

HIDMgr::~HIDMgr() {
    hid_exit();
}

HIDDevice* HIDMgr::open(uint16_t vendor, uint16_t product) {
    updateDevicesList();
    hid_device* raw = hid_open(vendor, product, 0);
    if(raw == 0)
        return 0;
    else
        return new HIDDevice(raw, this);
}

void HIDMgr::updateDevicesList() {
    mDevices = hid_enumerate(0, 0);
}
