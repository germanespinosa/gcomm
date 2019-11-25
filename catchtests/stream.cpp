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
/*
TEST_CASE ("Base change") {
    for (int i=0 ; i<=65279; i++) { //65279
        if (b255(b254(i)) != i)
        CHECK(b255(b254(i,true),true) == i);
    }
}
*/
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

struct data{
    uint8_t a;
};

struct big_data{
    uint8_t a[5000];
};

TEST_CASE("Buffer small") {

    GBuffer bin(1024);
    GBufferConnector sc(bin,bin);
    data d{127};
    sc.write_buffer(&d, sizeof(data));

    CHECK(bin.pending() == 6);
    CHECK((int)bin._buffer[0] == 255);
    CHECK((int)bin._buffer[1] == 2);
    CHECK((int)bin._buffer[2] == 0);
    CHECK((int)bin._buffer[3] == 127);
    CHECK((int)bin._buffer[4] == 0);
    CHECK((int)bin._buffer[5] == 127);

    data d2;
    CHECK(sc.read_buffer(&d2, sizeof(data)) == sizeof(data));
    CHECK((int)d.a == (int)d2.a);
}

TEST_CASE("Buffer big") {

    GBuffer bin(8000);
    CHECK(bin.pending()==0);
    GBufferConnector sc(bin,bin);
    big_data d;
    for (int i=0;i<5000;i++) d.a[i] = i%256;
    sc.write_buffer(&d, sizeof(big_data));
    CHECK(bin.pending()==5043);
    big_data d2;
    CHECK(sc.read_buffer(&d2, sizeof(big_data)) == sizeof(big_data));
    for (int i=0;i<5000;i++)
    CHECK((int)d.a[i] == (int)d2.a[i]);

}
