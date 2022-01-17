//
// By Gooday2die @ https://github.com/gooday2die
//
#pragma once

#ifndef WOODOKUAI_ALGORITHM_H
#define WOODOKUAI_ALGORITHM_H

#endif //WOODOKUAI_ALGORITHM_H

#include "Shape.h"
#include <random>
#include <unistd.h>
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
 * A bass class for algorithms.
 * Member function findBestResult will be overridden by child classes and it would be called from runAlgorithm function.
 */
class Algorithm{
public:
    Algorithm(){
    }

    virtual bfResult findBestFuture(Field, Shape*){
        bfResult emptyResult;
        return emptyResult;
    };
};

/**
 * An algorithm class for scoring method.
 * Just measures scores and finds out the combinations.
 */

class ScoringMethod : public Algorithm{
public:
    ScoringMethod() : Algorithm(){
        printf("Algorithm Info : Scoring Method\n");
    }
    bfResult findBestFuture(Field, Shape*) override;
};

/**
 * An algorithm class for surviving method. Counts the best future with the most empty spaces.
 * Does not work that well.
 */
class SurvivalMethod : public Algorithm{
public:
    SurvivalMethod() : Algorithm(){
        printf("Algorithm Info : Survival Method\n");
    }
    bfResult findBestFuture(Field, Shape*);
};

/**
 * A class for Heuristics method. figures out the best scoring and the least empty group numbers.
 * using simple heuristics :
 * Best scoring + (81 - number of empty groups) / 10
 */
class HeuristicsMethod : public Algorithm{
public:
    HeuristicsMethod() : Algorithm(){
        printf("Algorithm Info : Heuristics Method\n");
    }
    bfResult findBestFuture(Field, Shape*) override;
};

/**
 * A function for polymorphism.
 */
void runAlgorithm(Algorithm*, Field, Shape*);