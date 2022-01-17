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
 * A class for brute forcing stupidly.
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

/**
 * A class for surviving method. Counts the best future with the most empty spaces.
 * Does not work that well.
 */
class SurvivalMethod{
private:
    Shape* allShapes;
public:
    SurvivalMethod(Shape* shapeArray){
        allShapes = shapeArray;
    }
    void run(Field);
    bfResult findBestFuture(Field, Shape*);
};

/**
 * A class for Heuristics method. figures out the best scoring and the least empty group numbers.
 * using simple heuristics :
 * Best scoring + (81 - number of empty groups) / 10
 */
class heuristicsMethod{
private:
    Shape* allShapes;
public:
    heuristicsMethod(Shape* shapeArray){
        allShapes = shapeArray;
    }
    void run(Field);
    bfResult findBestFuture(Field, Shape*);
};