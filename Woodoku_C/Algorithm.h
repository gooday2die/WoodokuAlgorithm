//
// By Gooday2die @ https://github.com/gooday2die
//
#pragma once

#ifndef WOODOKUAI_ALGORITHM_H
#define WOODOKUAI_ALGORITHM_H

#endif //WOODOKUAI_ALGORITHM_H

#include "Shape.h"
#include <random>
#include<unistd.h>
#include <time.h>

/**
 * A struct for saving BestFuture results.
 */
typedef struct _bfResult{
    SMALLTYPE score;
    Field BestFuture;
    Field Step1;
    Field Step2;
    SMALLTYPE coords_i;
    SMALLTYPE coords_j;
    SMALLTYPE coords_k;
    SMALLTYPE coords_l;
    SMALLTYPE coords_m;
    SMALLTYPE coords_n;
    unsigned long futureCnt;
}bfResult;

/**
 * A class for brute forcing stupid.
 */
class BruteForceStupid{
private:
    Shape* allShapes;
public:
    /**
     * A constructor member function for class BruteForceStupid
     * @param shapeArray the shape array.
     */
    BruteForceStupid(Shape* shapeArray){
        allShapes = shapeArray;
    }
    void run(Field);
    bfResult findBestFuture(Field, Shape*);
};


