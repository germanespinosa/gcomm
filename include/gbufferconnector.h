#ifndef GCOMM_GBUFFERCONNECTOR_H
#define GCOMM_GBUFFERCONNECTOR_H

#include "gcomm.h"
namespace gcomm {
    struct GBufferConnector : GConnector {
    public:
        GBufferConnector(GBuffer &, GBuffer &);

        virtual uint16_t read_buffer(void *, uint16_t);

        virtual void write_buffer(const void *, uint16_t);

    private:
        GBuffer &_rx_buffer;
        GBuffer &_tx_buffer;
    };
}
#endif