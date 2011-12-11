#ifndef HID_HPP
#define HID_HPP

#include "types.hpp"
#include "hidapi.h"

#include <cwchar>
#include <string>

class HIDMgr;
class HIDDevice {
public:
    HIDDevice(hid_device* device, HIDMgr* mgr);
    ~HIDDevice();

    void close();

    bool parseInfo(hid_device* dev = 0);

    void send(unsigned char report, unsigned char* data);
    void send(unsigned char* data);
private:
    hid_device_info* getDevice(wchar_t* serial);

    u16 mVendorID;
    u16 mProductID;
    hid_device* mDevice;
    std::wstring mSerial;
    std::wstring mProduct;
    std::wstring mManufacturer;
    HIDMgr* mMgr;
    char* mPath;
};

class HIDMgr
{
public:
    HIDMgr();
    ~HIDMgr();
    HIDDevice* open(u16 vendor, u16 product);
    void updateDevicesList();
    hid_device_info* getDevices() { return mDevices; }

private:
    hid_device_info* mDevices;
};

#endif /* HID_HPP */
