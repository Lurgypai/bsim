#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

#include "termcolor.hpp"

#include "netdevice_manager.h"
#include "router.h"

void prepareConsole() {
    std::cout << "\033[2J\033[H" << std::flush;
    std::cout << "==================== TEST =====================\n";

    std::cout << std::setfill('0');
    for(int row = 0; row != 9; ++row) {
        for(int col = 0; col != 9; ++col) {
            std::cout << std::setw(2) << row * 10 + col << " ↔ ";
        }
        std::cout << std::setw(2) << row * 10 + 9 << '\n';
        for(int i = 0 ; i != 10; ++i) {
            std::cout << " ↕   ";
        }
        std::cout << '\n';
    }
    for(int col = 0; col != 9; ++col) {
        std::cout << std::setw(2) << 90 + col << " ↔ ";
    }
    std::cout << "99\n";
}

void moveCursor(int row, int col) {
    std::cout << "\033[" << row << ";" << col << "H" << std::flush;
}

void drawDevice(const NetDevice& device, int row, int col) {
    // translate to "screen space"
    moveCursor(row * 2 + 2, col * 5 + 1);
    if(!device.isSending() && !device.isReceiving()) std::cout << termcolor::grey;
    else if(device.isSending() && !device.isReceiving()) std::cout << termcolor::green;
    else if(!device.isSending() && device.isReceiving()) std::cout << termcolor::yellow;
    else if(device.isSending() && device.isReceiving()) std::cout << termcolor::red;

    std::cout << std::setw(2) << device.getId() << termcolor::reset;

    auto nextDevice = device.getNextDevice();
    if(!nextDevice) return;

    int nextX = nextDevice->getId() % 10;
    int nextY = nextDevice->getId() / 10;

    int dirX = nextX - col;
    int dirY = nextY - row;

    std::cout << termcolor::green;
    if(dirX == 1) {
        moveCursor(row * 2 + 2, col * 5 + 4);
        std::cout << "→";
    }
    else if (dirY == 1) {
        moveCursor(row * 2 + 3, col * 5 + 2);
        std::cout << "↑";
    }
    else if (dirY == -1) {
        moveCursor(row * 2 + 1, col * 5 + 2);
        std::cout << "↓";
    }

    if(!device.isReceiving()) {
        std::cout << termcolor::white;
        if (col != 0) {
            moveCursor(row * 2 + 2, col * 5 - 1);
            std::cout << "↔";
        }
        if(row != 0) {
            moveCursor(row * 2 + 1, col * 5 + 2);
            std::cout << "↕";
        }
        if(row != 9) {
            moveCursor(row * 2 + 3, col * 5 + 2);
            std::cout << "↕";
        }
    }

    std::cout << termcolor::reset;
}

class TestRouter : public RouterImpl {
public:
    TestRouter(NetDeviceManager& manager_) : manager{manager_} {};
    NetDeviceManager& manager;

private:
    NetDevice* findNext(NetDeviceId curId, NetDeviceId destId) override {
        // convert id into x y pos
        int curX = curId % 10;
        int curY = curId / 10;

        int destX = destId % 10;
        int destY = destId / 10;

        int nextX = curX;
        int nextY = curY;

        int diffX = destX - curX;
        int diffY = destY - curY;
        if(std::abs(diffX) > std::abs(diffY)) nextX += (diffX > 0 ? 1 : -1);
        else if (diffY != 0) nextY += (diffY > 0 ? 1 : -1);

        /*
        std::cout << "cur " << curX << ", " << curY
            << "  dest: " << destX << ", " << destY
            << "  next: " << nextX << ", " << nextY << '\n';
            */
        NetDeviceId nextDeviceId = nextX + nextY * 10;
        return &manager.getNetDevice(nextDeviceId);
    };
};

int main(int argc, char** argv) {

    NetDeviceManager manager;
    manager.genNetDevices(100);

    Router routerBase;
    routerBase.loadImpl<TestRouter>(manager);
    NetDevice::router = &routerBase;

    constexpr int PACKET_COUNT = 2000;
    for(int i = 0; i != PACKET_COUNT; ++i) {
        int scaledown = i / 20;
        NetDeviceId source = (scaledown / 10) * 10;
        NetDeviceId dest = 9 + (scaledown % 10) * 10;

        // std::cout << "source: " << source << ", dest: " << dest << '\n';
        manager.getNetDevice(source).queue(Packet{i, dest});
    }

    /*
    constexpr int PACKET_COUNT = 10;
    for(int i = 0; i != PACKET_COUNT; ++i) {
        NetDeviceId source = i * 10;
        NetDeviceId dest = (9 - i) * 10 + 9;

        std::cout << "source: " << source << ", dest: " << dest << '\n';
        manager.getNetDevice(source).queue(Packet{i, dest});
    }
    */

    /*
    constexpr int PACKET_COUNT = 2;
    manager.getNetDevice(0).queue(Packet{0, 99});
    manager.getNetDevice(90).queue(Packet{1, 9});
    */

    prepareConsole();

    int totalRecv = 0;

    while(totalRecv != PACKET_COUNT) {
        manager.updateDevices(1);

        totalRecv = 0;
        for(int i = 0; i != 10; ++i) {
            NetDeviceId destId = i * 10 + 9;
            totalRecv += manager.getNetDevice(destId).getReceivedCount();
        }

        
        for(const auto& device : manager) {
            int x = device.getId() % 10;
            int y = device.getId() / 10;
            drawDevice(device, y, x);
        }
        

        moveCursor(23, 1);
        std::cout << "Total Received: " << totalRecv << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds{1});
    }

    return 0;
}
