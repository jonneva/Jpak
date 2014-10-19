#include <Arduino.h>
#include "jpak.h"

Jpak::Jpak(const unsigned char * bmp) {
    _width = pgm_read_byte((uint32_t)(bmp));
    _height = pgm_read_byte((uint32_t)(bmp+1));
    _framesize = pgm_read_byte((uint32_t)(bmp+2));
    _allow = pgm_read_byte((uint32_t)(bmp+3));
    _stream = bmp + 4;
    _position = 0;
    _bitcounter =0;
    _bytecounter = 0;
    _runcounter = 0;
    _runlength = 0;
    _mode = 0;
    _currentframe = readNextFrame();
}


uint16_t Jpak::getWidth() {
    return _width;
}

uint16_t Jpak::getHeight() {
    return _height;
}

uint16_t Jpak::getSize() {
    return _height*_width;
}

uint16_t Jpak::getFrameSize() {
    return _framesize;
}

uint16_t Jpak::getAllow() {
    return _allow;
}

void Jpak::rewindStream() {
    _position = 0;
    _bitcounter =0;
    _bytecounter = 0;
    _runcounter = 0;
    _runlength = 0;
    _mode = 0;
    _currentframe = readNextFrame();
}

uint8_t Jpak::readNextPixel() {
    uint8_t value;
    if (_mode == RLE) {
        // RLE
        if (_runcounter < _runlength) {
            _runcounter++;
            return _color;
        } else {
            _currentframe = readNextFrame();
            return readNextPixel();
        }
    } else {
        // UNPACKED
        if (_bitcounter == -1) {
            _currentframe = readNextFrame();
            return readNextPixel();
        } else {
            value = (_currentframe & (1<<_bitcounter)) ? 1:0 ;
            _bitcounter--;
            return value;
        }
    }
    return 0xFF;
}

uint16_t Jpak::readNextFrame() {
    uint16_t value;
    if (_framesize == 8 ) {
    // Special case, frame is 1 byte, no shifting needed
        value = pgm_read_byte((uint32_t)(_stream+_bytecounter));
        if (value & ( 1 << (_framesize-1))) {
            // RLE PACKED
            _mode = RLE;
            _color = value & (1 << (_framesize-2)) ? 1:0 ;
            _bitcounter = 0;
            _runcounter = 0;
            _runlength = (value & MASK6);
        } else {
            // UNPACKED
            _mode = UNPACKED;
            _bitcounter = _framesize-2;
            _runcounter = 0;
        }
        _bytecounter++;
        return value;
    } else {
    // Frame requires 2 bytes
    }
}

uint8_t Jpak::getCurrentFrame() {
    return (uint8_t) _currentframe;
}
