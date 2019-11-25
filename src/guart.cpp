#include "gcomm.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <fstream>

using namespace std;
namespace gcomm {

    GUart::GUart(Baudrate baudrate) : GUart("/dev/serial0", baudrate) {}

    GUart::GUart(const char *devicepath ,Baudrate baudrate) {
        _file = -1;
        _file = open(devicepath, O_RDWR | O_NOCTTY | O_NDELAY);//Open in non blocking read/write mode
        if (_file < 0) exit(EXIT_FAILURE);
        struct termios options;
        tcgetattr(_file, &options);
        options.c_cflag = ((int) baudrate) | CS8 | CLOCAL | CREAD;//<Set baud rate
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        tcflush(_file, TCIFLUSH);
        tcsetattr(_file, TCSANOW, &options);
    }

    uint8_t GUart::read_byte() {
        uint8_t byte;
        while (read(_file, &byte, 1) != 1 && active);
        return byte;
    }

    void GUart::write_byte(uint8_t byte) {
        write(_file, &byte, 1);
    }
}