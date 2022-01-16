//
// By Gooday2die @ https://github.com/gooday2die
//

#include "Shape.h"



void Shape::setPixels(bool p1,bool p2,bool p3,bool p4,bool p5,bool p6,bool p7,bool p8,bool p9,bool p10,bool p11,
                      bool p12,bool p13,bool p14,bool p15,bool p16,bool p17,bool p18,bool p19,bool p20,bool p21,
                      bool p22,bool p23,bool p24,bool p25) {
    /**
     * A function that sets pixels to a shape.
     */
    bool boolArray[25] = {p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                          p13, p14, p15, p16, p17, p18, p19, p20, p21, p22,
                          p23, p24, p25};

    for (SMALLTYPE i = 0; i < 25; i++) {
        pixels = pixels << 1;
        pixels += boolArray[i];
    }
}

bool Shape::getPixel(SMALLTYPE x, SMALLTYPE y){
    return ((pixels >> ((4 - y) * 5 + (4 - x))) & 1); // shift and get LSB
}

bool Shape::canPutIn(SMALLTYPE x, SMALLTYPE y, Field field) {
    if ((x >= 0 && y >= 0) && (x < 9 && y < 9)) {
        for (SMALLTYPE i = 0; i < 5; i++) {
            for (SMALLTYPE j = 0; j < 5; j++)
                if (field.getPixelValue(j + x, i + y) + (getPixel(j, i)) > 1) return false;
        }
        return true;
    } else{
        return false;
    }
}

Field Shape::putIn(SMALLTYPE x, SMALLTYPE y, Field field){
    for (SMALLTYPE i = 0; i < 5; i++) {
        for (SMALLTYPE j = 0; j < 5; j++) {
            field.setPixelValue(j + x, i + y, (field.getPixelValue(j + x, i + y) + getPixel(j, i)));
        }
    }
    return field;
}

unsigned int Shape::getPixels(void){
    return pixels;
}

void Shape::printShape(void){
    for (SMALLTYPE i = 0 ; i < 5 ; i++) {
        for (SMALLTYPE j = 0; j < 5; j++) {
            printf("%d ", getPixel(j, i));
        }
        printf("\n");
    }
    printf("\n");
}

SMALLTYPE Shape::shapeSize(){
    SMALLTYPE returnVal = 0;
    for(SMALLTYPE i = 0 ; i < 25 ; i++) returnVal += ((pixels >> i) & 1);
    return returnVal;
}