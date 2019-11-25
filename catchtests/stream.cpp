#include<catch.h>
#include<gcomm.h>
#include<iostream>

using namespace gcomm;
using namespace std;

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
