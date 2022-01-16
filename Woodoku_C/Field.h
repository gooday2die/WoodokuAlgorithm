//
// By Gooday2die @ https://github.com/gooday2die
//
#pragma once

#ifndef WOODOKUAI_FIELD_H
#define WOODOKUAI_FIELD_H

#endif //WOODOKUAI_FIELD_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>


#define SMALLTYPE uint8_t


class Field{
    /**
     * A class for field. Total pixels are 9 * 9
     * So can be represented as 27 * 3
     * 27bit < 32 bit
     * So we are going to divide 3 parts from top to bottom.
     * 1st pixels 8 byte will cover from 0 ~ 2 row
     * 2nd pixels 8 byte will cover from 3 ~ 5 row
     * 3rd pixels 8 byte will cover from 6 ~ 8 row
     */
private:
    uint32_t pixels[3]{};
public:
    Field(){
        for(SMALLTYPE i = 0 ; i < 3 ; i++) pixels[i] = 0;
    }

    SMALLTYPE getPixelValue(SMALLTYPE, SMALLTYPE);
    void setPixelValue(SMALLTYPE, SMALLTYPE, SMALLTYPE);
    SMALLTYPE calculateScore(void);
    uint8_t getSector(SMALLTYPE);
    void printField(void);
};