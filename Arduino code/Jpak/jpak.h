#ifndef JPAK_H
#define JPAK_H

#define MASK2   0x03
#define MASK3   0x07
#define MASK4   0x0F
#define MASK5   0x1F
#define MASK6   0x3F
#define MASK7   0x7F
#define MASK8   0xFF

#define RLE         1
#define UNPACKED    0

class Jpak {
public:
    Jpak(const unsigned char *);
    uint16_t getWidth();
    uint16_t getHeight();
    uint16_t getSize();
    uint16_t getFrameSize();
    uint16_t getAllow();
    void rewindStream();
    uint8_t readNextPixel();
    uint8_t getCurrentFrame();

private:
    const unsigned char * _stream;
    uint16_t _width, _height, _framesize, _allow, _currentframe;
    uint16_t _position, _bytecounter, _runcounter, _runlength;
    int8_t _bitcounter, _mode, _color;

    uint16_t readNextFrame();
};

#endif /* JPAK_H */


