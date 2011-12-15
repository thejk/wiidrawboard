#ifndef WIIMOTE_H
#define WIIMOTE_H

#include "hid.hpp"
#include <QObject>
#include <QPair>
#include <vector>

class WiiMote : QObject {
    Q_OBJECT

public:
    WiiMote(HIDDevice* device);
    void init();
    void setRegister(u32 reg, u8* data, u16 size);
    void setRegister(u32 reg, u8 data, u16 size);
    void cameraOn();
    void setLEDs(bool first, bool second, bool third, bool fourth);
    void parseCamera(u8* data, u32 len);
    void printButtons(HIDDevice* device);

signals:
    void objectMoved(u8 index, u16 x, u16 y);

private:
    std::vector<QPair<u16, u16> > mObjects;
    HIDDevice* mDevice;
};

#endif // WIIMOTE_H
