//
// Created by cs211 on 11/24/19.
//

#include <cstdlib>
#include "gbuffer.h"
#include "malloc.h"
namespace gcomm{
    GBuffer::GBuffer(uint32_t size) {
        _size = size;
        _offset = 0;
        _next = 0;
        _buffer = (uint8_t*) malloc(size);
        if (_buffer == NULL) exit(EXIT_FAILURE);
    }

    GBuffer::~GBuffer() {
        free(_buffer);
    }

    uint32_t GBuffer::pending() {
        if (_offset>=_next) return _offset - _next;
        return _size - _next + _offset;
    }
    void GBuffer::write_byte(uint8_t byte) {
        _buffer[_offset] = byte;
        _offset++;
        if (_offset >= _size) _offset = 0;
    }
    bool GBuffer::read_byte(uint8_t &byte) {
        if (pending()){
            byte = _buffer[_next];
            _next++;
            if (_next>=_size) _next = 0;
        }else return false;
    }
}