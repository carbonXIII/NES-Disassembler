#ifndef TOOLS_H_
#define TOOLS_H_

typedef unsigned char byte;
typedef unsigned short word;

template <int bitno, typename T = byte>
struct Bit{
    byte data;
    
    enum {mask = 1u << bitno};
    
    bool operator () () const {
        return !!(data & mask);
    }
};

union FlagByte{
    byte data;
    
    Bit<0> b0;
    Bit<1> b1;
    Bit<2> b2;
    Bit<3> b3;
    Bit<4> b4;
    Bit<5> b5;
    Bit<6> b6;
    Bit<7> b7;
};

#endif
