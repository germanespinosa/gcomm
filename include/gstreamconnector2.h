# if !defined(__GSTREAMCONNECTOR2_H__)
# define __GSTREAMCONNECTOR2_H__
#include <stdint.h>
#include <thread>
#include <mutex>
#include "gcomm.h"
#define BUFFERLENGTH 1024

namespace gcomm {
    struct GStreamConnector2 : GConnector {
    public:
        GStreamConnector2(GStream &, uint64_t);

        GStreamConnector2(GStream &);

        virtual uint16_t read_buffer(void *, uint16_t);

        virtual void write_buffer(const void *, uint16_t);

        bool time_out();

        ~GStreamConnector2();

    private:
        uint8_t get_byte();
        bool set_byte(uint8_t);
        GStream &_stream;
        uint16_t _rx_length;
        uint16_t _pending();
        static void _rx_update_(GStreamConnector2 &);
        static void _tx_update_(GStreamConnector2 &);
        std::thread _rx_thread;
        std::thread _tx_thread;
        uint8_t _rx_buffer[BUFFERLENGTH];
        uint16_t _rx_offset;
        uint16_t _rx_next;
        uint8_t _tx_buffer[BUFFERLENGTH];
        uint16_t _tx_offset;
        uint16_t _tx_next;
        uint64_t _timeout;
        std::mutex _rx_buffer_access;
        bool _active;
    };
}
#endif