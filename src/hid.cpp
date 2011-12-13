#include "hid.hpp"

#include <cstring>
#include <sstream>
#include <iostream>
using namespace std;

////
// HIDDevice

HIDDevice::HIDDevice(hid_device* device, HIDMgr* mgr)
: mDevice(device), mMgr(mgr) { }

HIDDevice::~HIDDevice() {
    close();
}

void HIDDevice::close() {
    if(mDevice != 0)
        hid_close(mDevice);
}

bool HIDDevice::parseInfo(hid_device* dev) {
    if(dev == 0)
        dev = mDevice;

    //get serial
    u16 len = 18; //e.g. "E0-E7-51-97-74-09" plus '\0'
    wchar_t* buff = new wchar_t[len];
    memset(buff, '\0', len);
    int r = hid_get_serial_number_string(dev, buff, len);
    if(!r)
        return false;
    mSerial = buff;

    //Parse the rest from the right info-instance instead
    //since the hid_device contains very limited information.
    hid_device_info* info = getDevice(buff);
    mManufacturer = info->manufacturer_string;
    mProduct = info->product_string;
    mVendorID = info->vendor_id;
    mProductID = info->product_id;
    mPath = info->path;
    return true;
}

hid_device_info* HIDDevice::getDevice(wchar_t* serial) {
    hid_device_info* dev = mMgr->getDevices();
    while(dev != 0) {
        if(wcscmp(dev->serial_number, serial) == 0)
            return dev;
        dev = dev->next;
    }
    return 0;
}

s32 HIDDevice::send(unsigned char *data, u16 size) {
    return hid_write(mDevice, data, size);
}

s32 HIDDevice::send(unsigned char report, unsigned char *data, u16 size) {
    u8* buff = new u8[size+1];
    buff[0] = report;
    memcpy(&buff[1], data, size);
    return send(buff, size+1);
}

u8* HIDDevice::recv(u32 &size) {
    u8* buff = new u8[128];
    size = hid_read(mDevice, buff, 128);
    return buff;
}

void HIDDevice::printLastError() {
    const wchar_t* error = hid_error(mDevice);
    wcout <<"Error " <<error <<"." <<endl;
}

////
// HIDMgr

HIDMgr::HIDMgr() {
    mDevices = 0;
    hid_init();
}

HIDMgr::~HIDMgr() {
    hid_exit();
}

HIDDevice* HIDMgr::open(u16 vendor, u16 product) {
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

void HIDMgr::printDevices() {
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
    hid_free_enumeration(devs);
}
