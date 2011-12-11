#include "hid.hpp"
#include "drawboard.hpp"

#include <iostream>
#include <QApplication>

using namespace std;

int test_gui(int argc, char** argv) {
    QApplication app(argc, argv);

    Drawboard* board = new Drawboard();
    board->show();
    return app.exec();
}

void test_bluetooth(int argc, char** argv) {
    HIDMgr mgr;
    mgr.updateDevicesList();
    mgr.printDevices();

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
}

int main(int argc, char** argv) {
    bool gui = false;
    bool bluetooth = true;
    if(gui) {
        return test_gui(argc, argv);
    }
    else if(bluetooth) {
        test_bluetooth(argc, argv);
        return 0;
    }
}
