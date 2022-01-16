//
// By Gooday2die @ https://github.com/gooday2die
//
#pragma once

#ifndef WOODOKUAI_SHAPE_H
#define WOODOKUAI_SHAPE_H

#endif //WOODOKUAI_SHAPE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "Field.h"

#define SMALLTYPE uint8_t

class Shape{
    /**
     * A class for shapes
     */
private:
    unsigned int pixels = 0;  // pixel size = 4byte = 32bits. shape = total 25 bits. So Can represent.
public:
    void setPixels(bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool, bool,
                   bool, bool, bool, bool, bool, bool, bool, bool, bool);
    bool canPutIn(SMALLTYPE, SMALLTYPE, Field);
    Field putIn(SMALLTYPE, SMALLTYPE, Field);
    unsigned int getPixels();
    bool getPixel(SMALLTYPE, SMALLTYPE);
    void printShape();
    SMALLTYPE shapeSize();
};

