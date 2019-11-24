#include "gcomm.h"
#include"gstreamconnector2.h"
#include <sys/time.h>
#include <iostream>

using namespace std;

namespace gcomm {

    GStreamConnector2::~GStreamConnector2(){
        _active = false;
        _stream.stop();
        _tx_thread.join();
        _rx_thread.join();
    }

    GStreamConnector2::GStreamConnector2(GStream &stream) : GStreamConnector2(stream, 0) {
    }

    GStreamConnector2::GStreamConnector2(GStream &stream, uint64_t timeout) : _stream(stream) {
        _active = true;
        _rx_length = _rx_offset = _rx_next = _tx_offset = _tx_next = 0;
        _rx_thread = std::thread(_rx_update_, std::ref(*this));
        _tx_thread = std::thread(_tx_update_, std::ref(*this));
        _timeout = timeout;
    }

#define CHECK_BYTE byte = get_byte();if (byte==255){ _rx_length = 0; _rx_buffer_access.unlock(); return 0; }

    uint16_t GStreamConnector2::read_buffer(void *dest, uint16_t max_size) {
        uint8_t byte;
        _rx_buffer_access.lock();
        if (!_rx_length && _pending() >= 2) {
            CHECK_BYTE
            uint16_t l = byte;
            CHECK_BYTE
            uint16_t h = byte;
            _rx_length = (h << 8) + l;
        }
        if (_rx_length && _pending() > _rx_length) {
            uint8_t *dest_c = (uint8_t *) dest;
            uint8_t check_sum = 0;
            for (int index = 0; index < _rx_length; index++) {
                CHECK_BYTE
                if (byte==127) {
                    CHECK_BYTE
                    byte += 127;
                }
                if (index < max_size) dest_c[index] = byte;
                check_sum = check_sum ^ byte;
            }
            uint16_t length = _rx_length;
            _rx_length = 0;
            CHECK_BYTE
            _rx_buffer_access.unlock();
            return check_sum == byte ? length : 0;
        }
        _rx_buffer_access.unlock();
        return 0;
    }

    void GStreamConnector2::write_buffer(const void *data, uint16_t size) {
        uint8_t *buff = (uint8_t *) data;
        uint8_t check_sum = 0;
        for (uint8_t index = 0; index < size; index++) {
            if (buff[index]==255 || buff[index]==127) size ++;
        }
        set_byte(255);
        set_byte(*((uint8_t *) &size));
        set_byte(*(((uint8_t *) &size) + 1));
        for (uint8_t index = 0; index < size; index++) {
            switch(buff[index]) {
                case 255:
                    set_byte(127);
                    set_byte(128);
                    break;
                case 127:
                    set_byte(127);
                    set_byte(0);
                    break;
                default:
                    set_byte(buff[index]);
            }
            check_sum = check_sum ^ buff[index];
        }
        set_byte(check_sum);
    }

    void GStreamConnector2::_rx_update_(GStreamConnector2 &comm) {
        timeval current_time, last_byte;
        gettimeofday(&last_byte, NULL);
        while (comm._active) {
            uint8_t byte = comm._stream.get_byte();
            gettimeofday(&current_time, NULL);
            uint64_t elapsed =
                    (current_time.tv_usec - last_byte.tv_usec) + (current_time.tv_sec - last_byte.tv_sec) * 1000000;
            last_byte = current_time;
            if (comm._timeout && elapsed > comm._timeout) {
                comm._rx_buffer_access.lock();
                comm._rx_next = comm._rx_offset;
                comm._rx_length = 0;
                comm._rx_buffer_access.unlock();
            }
            comm._rx_buffer[comm._rx_offset] = byte;
            comm._rx_offset = ((1 + comm._rx_offset) % BUFFERLENGTH);
        }
    }

    void GStreamConnector2::_tx_update_(GStreamConnector2 &comm) {
        while (comm._active) {
            if (comm._tx_next != comm._tx_offset) {
                uint8_t byte = comm._tx_buffer[comm._tx_next];
                comm._stream.set_byte(byte);
                comm._tx_next = ((1 + comm._tx_next) % BUFFERLENGTH);
            }
        }
    }

    uint16_t GStreamConnector2::_pending() {
        if (_rx_offset >= _rx_next) return _rx_offset - _rx_next;
        return BUFFERLENGTH - _rx_next + _rx_offset;
    }

    uint8_t GStreamConnector2::get_byte() {
        uint8_t byte = _rx_buffer[_rx_next];
        _rx_next = ((1 + _rx_next) % BUFFERLENGTH);
        return byte;
    }

    bool GStreamConnector2::set_byte(uint8_t byte) {
        _tx_buffer[_tx_offset] = byte;
        _tx_offset = ((1 +_tx_offset) % BUFFERLENGTH);
        return true;
    }

}