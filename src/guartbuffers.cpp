#include "gcomm.h"
using namespace std;
namespace gcomm {

    GUartBuffers::GUartBuffers(GUart::Baudrate baudrate) : GUartBuffers("/dev/serial0", baudrate, 10240, 10240) {}

    GUartBuffers::GUartBuffers( const char *devicepath, GUart::Baudrate baudrate, uint32_t rx_buffer_size, uint32_t tx_buffer_size)
        : rx_buffer(rx_buffer_size)
        , tx_buffer(tx_buffer_size)
        , _guart(devicepath, baudrate){
        _active = true;
        _rx_thread = std::thread(_rx_update_, std::ref(*this));
        _tx_thread = std::thread(_tx_update_, std::ref(*this));
    }

    void GUartBuffers::_rx_update_(GUartBuffers &guartbuffers) {
        while (guartbuffers._active){
            guartbuffers.rx_buffer.write_byte(guartbuffers._guart.read_byte());
        }
    }

    void GUartBuffers::_tx_update_(GUartBuffers &guartbuffers) {
        while (guartbuffers._active){
            uint8_t byte;
            if (guartbuffers.tx_buffer.read_byte(byte)) {
                guartbuffers._guart.write_byte(byte);
            }
        }
    }

}