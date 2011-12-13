#include "wiimote.hpp"

#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;

void printblock(u8* data, u32 len) {
    for(int i = 0; i < len; ++i) {
        cout <<(u16)data[i] <<" ";
    }
    cout <<endl;
}

WiiMote::WiiMote(HIDDevice *device)
    : mDevice(device) { }

void WiiMote::init() {
    //set report mode to 36 = core buttons, 10 IR and 9 ext
    u8 reportMode[3] = { 0x12, 00, 0x36 };
    mDevice->send(reportMode, 3);
}

void WiiMote::setRegister(u32 reg, u8 *data, u16 size) {
    u8 arr[22] = { 0x16, reg & 0xff0000, reg & 0xff00, reg & 0xff, size };
    memcpy(arr+6, data, size);
    mDevice->send(arr, 22);
    usleep(10000);
}

void WiiMote::setRegister(u32 reg, u8 data, u16 size) {
    u8 arr[22] = { 0x16, reg & 0xff0000, reg & 0xff00, reg & 0xff, size };
    arr[6] = data;
    mDevice->send(arr, 22);
    usleep(10000);
}

void WiiMote::setLEDs(bool first, bool second, bool third, bool fourth) {
    u16 val = 0;
    if(first)
        val |= 0x10;
    if(second)
        val |= 0x20;
    if(third)
        val |= 0x40;
    if(fourth)
        val |= 0x80;
    u8 arr[2] = { 0x11, val };
    mDevice->send(arr, 2);
}

void WiiMote::cameraOn() {
    //send 0x4 to report 0x13 and 0x1a
    u8 r13[2] = { 0x13, 0x4 };
    mDevice->send(r13, 2);
    usleep(10000);
    u8 r1a[2] = { 0x1a, 0x4 };
    mDevice->send(r1a, 2);
    usleep(10000);

    setRegister(0xb00030, 0x08, 1);

    u8 sensblock1[9] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x90, 0x0, 0x41 };
    setRegister(0xb00000, sensblock1, 9);

    u8 sensblock2[2] = { 0x40, 0x0 };
    setRegister(0xb0001a, sensblock2, 2);

    setRegister(0xb00033, 1, 1);

    setRegister(0xb00030, 0x08, 1);
}

void WiiMote::printButtons(HIDDevice* device) {
    u8 buff[2] = { 0x15, 0x00 };
    int retr = device->send(buff, 2);
    cout <<dec <<"sent " <<retr <<" bytes." <<endl;
    if(retr == -1) {
        device->printLastError();
    }

    while(true) {
        u32 size = 0;
        u8* data = device->recv(size);
        if(size > 0) {
            cout <<"\e[A";
            cout.flush();
            printblock(data, size);
            device->send(buff, 2);
        }
    }
}

void WiiMote::parseCamera(u8* data, u32 len) {
    u8 ir[10];
    memcpy(ir, data+5, 10);

    u16 y1mask = 1<<7 | 1<<6;
    u16 x1mask = 1<<5 | 1<<4;
    u16 y2mask = 1<<3 | 1<<2;
    u16 x2mask = 1<<1 | 1;

    u16 x1 = ir[0];
    x1 |= (ir[2] & x1mask) <<4;
    u16 y1 = ir[1];
    y1 |= (ir[2] & y1mask) <<2;
    u16 x2 = ir[3];
    x2 |= (ir[2] & x2mask) <<8;
    u16 y2 = ir[4];
    y2 |= (ir[2] & y2mask) <<6;

    printf("(%4u, %4u), (%4u, %4u)\n", x1, y2, x2, y2);
}
