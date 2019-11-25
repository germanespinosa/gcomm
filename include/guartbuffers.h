#ifndef GCOMM_GUARTBUFFERS_H
#define GCOMM_GUARTBUFFERS_H

#include "gcomm.h"
#include <thread>
namespace gcomm{
    struct GUartBuffers {
        GUartBuffers(GUart::Baudrate);
        GUartBuffers(const char *, GUart::Baudrate, uint32_t, uint32_t);
        GBuffer rx_buffer;
        GBuffer tx_buffer;

    private:
        GUart _guart;
        static void _rx_update_(GUartBuffers &);
        static void _tx_update_(GUartBuffers &);
        std::thread _rx_thread;
        std::thread _tx_thread;
        bool _active;
    };

}

#endif //GCOMM_GUARTBUFFERS_H
