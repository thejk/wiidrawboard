#ifndef WIIMOTE_H
#define WIIMOTE_H

#include "hid.hpp"

class WiiMote {
public:
    WiiMote(HIDDevice* device);
    void init();
    void setRegister(u32 reg, u8* data, u16 size);
    void setRegister(u32 reg, u8 data, u16 size);
    void cameraOn();
    void setLEDs(bool first, bool second, bool third, bool fourth);
    void parseCamera(u8* data, u32 len);
    void printButtons(HIDDevice* device);
private:
    HIDDevice* mDevice;
};

#endif // WIIMOTE_H
