//
// By Gooday2die @ https://github.com/gooday2die
//
#pragma once

#ifndef WOODOKUAI_FIELD_H
#define WOODOKUAI_FIELD_H

#endif //WOODOKUAI_FIELD_H

#include <stack>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define SMALLTYPE uint8_t

/**
 * A class for representing field.
 * Using 3 uint32_t variables for representing 9*9 field.
 */
class Field{
private:
    uint32_t pixels[3]{};
public:
    Field(){
        for(SMALLTYPE i = 0 ; i < 3 ; i++) pixels[i] = 0;
    }

    SMALLTYPE getPixelValue(SMALLTYPE, SMALLTYPE);
    void setPixelValue(SMALLTYPE, SMALLTYPE, SMALLTYPE);
    SMALLTYPE calculateScore(void);
    SMALLTYPE peekScore(void);
    SMALLTYPE countEmptySpaces(void);
    SMALLTYPE getEmptySpaceGroups(void);
    SMALLTYPE getFilledSpaceGroups(void);

    void printField(void);
};