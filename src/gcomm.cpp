#include <gcomm.h>
namespace  gcomm{
    template<typename TIN, typename TOUT>
    GComm<TIN,TOUT>::GComm(GConnector &conn) : _conn(conn) {}

    template<typename TIN, typename TOUT>
    bool GComm<TIN,TOUT>::read_object(TIN &i) {
        return _conn.read_buffer(&i, sizeof(TIN)) == sizeof(TIN);
    }

    template<typename TIN, typename TOUT>
    void GComm<TIN,TOUT>::write_object(TOUT o) {
        _conn.write_buffer(&o, sizeof(TOUT));
    }

}
