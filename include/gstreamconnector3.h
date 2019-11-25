# if !defined(__GSTREAMCONNECTOR3_H__)
# define __GSTREAMCONNECTOR3_H__
#include <stdint.h>
#include <thread>
#include <mutex>
#include "gcomm.h"
#include "gbuffer.h"

namespace gcomm {
    struct GBufferConnector : GConnector {
    public:
        GBufferConnector(GBuffer &, GBuffer &);

        virtual uint16_t read_buffer(void *, uint16_t);

        virtual void write_buffer(const void *, uint16_t);

    private:
        GBuffer &_rx_buffer;
        GBuffer &_tx_buffer;
        std::mutex _rx_buffer_access;
    };
}
#endif