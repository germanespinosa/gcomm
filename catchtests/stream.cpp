#include<catch.h>
#include<gcomm.h>
#include<iostream>

using namespace gcomm;
using namespace std;

struct Simple_Stream : GStream {
    uint8_t buffer [1024];
    int next = 0;
    int size = 0;
    int offset = 0;

    Simple_Stream (){}

    Simple_Stream (string s) {
        set_byte(s.length());
        set_byte(0);
        uint8_t cs = 0;
        for (int i = 0; i < s.length(); i++) {
            uint8_t c = s.c_str()[i];
            set_byte(c);
            cs = cs ^ c;
        }
        set_byte(cs);
    }

    uint8_t get_byte() override{
        while(next==offset && active) {}
        return buffer[next++];
    }

    void set_byte (uint8_t b){
        buffer[offset++] = b;
    }
};

TEST_CASE("Stream") {
    string message("good morning!");
    Simple_Stream s(message);
    GStreamConnector c(s);
    char rx_buffer[1024];
    int p = 0;
    while (p == 0) p = c.read_buffer(rx_buffer, 1024);
    CHECK(p == message.length());
    rx_buffer[message.length()] = '\0';
    string c_message(rx_buffer);
    CHECK(c_message == message);
    c.write_buffer("hello\0", 6);
    p = 0;
    while (p == 0) p = c.read_buffer(rx_buffer, 1024);
    CHECK(p == 6);
    string c2_message(rx_buffer);
    CHECK(c2_message == "hello");
}