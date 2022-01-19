//
// By Gooday2die @ https://github.com/gooday2die
//

#include "Algorithm.h"


/**
 * A member function for class SurvivalMethod for literally brute forcing every possible combinations and looking
 * for the best future with the most empty spaces. Calculates in 6 for loops starting from (0,0,0,0,0,0) to (9,9,9,9,9,9).
 * Worst case scenario, it would be 9^6.
 * @param curField The current field to brute force
 * @param shapeList The shapes to brute force with
 * @return returns Best Future result in bfResult struct type.
 */
bfResult SurvivalMethod::findBestFuture(Field curField, Shape* shapeList){
    bfResult result;
    result.score = 0;
    result.futureCnt = 0;
    SMALLTYPE curMAX = 0;
    for(SMALLTYPE i = 0 ; i < 9 ; i++){
        for (SMALLTYPE j = 0 ; j < 9 ; j++){
            for (SMALLTYPE k = 0 ; k < 9 ; k++){
                for (SMALLTYPE l = 0 ; l < 9 ; l++){
                    for(SMALLTYPE m = 0 ; m < 9 ; m++){
                        for(SMALLTYPE n = 0 ; n < 9 ; n++)
                            if (shapeList[0].canPutIn(i, j, curField)) {
                                Field newField0 = shapeList[0].putIn(i, j, curField);
                                if(shapeList[1].canPutIn(k, l, newField0)) {
                                    Field newField1 = shapeList[1].putIn(k, l, newField0);
                                    if (shapeList[2].canPutIn(m, n, newField1)) {
                                        Field newField2 = shapeList[2].putIn(m, n, newField1);
                                        SMALLTYPE score = newField2.countEmptySpaces();
                                        result.futureCnt++;
                                        if (curMAX <= score) {
                                            curMAX = score;
                                            result.BestFuture = newField2;
                                            result.Step1 = newField0;
                                            result.Step2 = newField1;
                                            result.score = score;
                                            result.coords_i = i;
                                            result.coords_j = j;
                                            result.coords_k = k;
                                            result.coords_l = l;
                                            result.coords_m = m;
                                            result.coords_n = n;
                                        }
                                    }
                                }
                            }
                    }
                }
            }
        }
    }
    printf("MAX Empty Space : %d", curMAX);
    return result;
}

bfResult OneByOneMethod::findBestFuture(Field curField, Shape* shapeList){
    bfResult result;
    result.score = 0;
    result.futureCnt = 0;
    bfResult results[3]; // block 0, 1
    results[0].futureCnt = 0;
    results[0].score = 0;
    results[1].futureCnt = 0;
    results[1].score = 0;
    results[2].futureCnt = 0;
    results[2].score = 0;
    SMALLTYPE curBest0 = 0 ;
    for (SMALLTYPE i = 0 ; i < 9 ; i++){
        for (SMALLTYPE j = 0 ; j < 9 ; j++){
            if(shapeList[0].canPutIn(j, i, curField)){ // try 0, 1, 2 and 0, 2, 1
                Field newField0 = shapeList[0].putIn(j, i, curField);
                SMALLTYPE score0 = newField0.peekScore();
                if(curBest0 <= score0){ // try 1 -> 2 order.
                    curBest0 = score0;
                    SMALLTYPE curBest1 = 0;
                    for(SMALLTYPE k = 0 ; k < 9 ; k++) {
                        for (SMALLTYPE l = 0; l < 9; l++) {
                            if (shapeList[1].canPutIn(l, k, newField0)) { // try 2.
                                Field newField1 = shapeList[1].putIn(l, k, newField0);
                                SMALLTYPE score1 = newField1.peekScore();
                                if (curBest1 <= score1) { // if we can put second piece, try last one.
                                    curBest1 = score1;
                                    SMALLTYPE curBest2 = 0;
                                    for (SMALLTYPE m = 0; m < 9; m++) {
                                        for (SMALLTYPE n = 0; n < 9; n++) {
                                            if (shapeList[2].canPutIn(n, m, newField1)) {
                                                Field newField2 = shapeList[2].putIn(l, k, newField1);
                                                //printf("Found at least one possible case : 0 -> 1 -> 2\n");
                                                SMALLTYPE score2 = newField2.peekScore();
                                                newField2.calculateScore();
                                                if (curBest2 <= score2) {
                                                    curBest2 = score2;
                                                    results[0].futureCnt++; // we at least have one survival case.
                                                    results[0].score = score0 + score1 + score2;
                                                    results[0].Step1 = newField0;
                                                    results[0].Step2 = newField1;
                                                    results[0].BestFuture = newField2;
                                                    results[0].coords_i = i;
                                                    results[0].coords_j = j;
                                                    results[0].coords_k = k;
                                                    results[0].coords_l = l;
                                                    results[0].coords_m = m;
                                                    results[0].coords_n = n;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    for(SMALLTYPE k = 0 ; k < 9 ; k++){
                        for(SMALLTYPE l = 0 ; l < 9 ; l++){
                            if (shapeList[2].canPutIn(l, k, newField0)){ // t.
                                Field newField1 = shapeList[2].putIn(l, k, newField0);
                                SMALLTYPE score1 = newField1.peekScore();
                                if(curBest1 <= score1){ // if we can put second piece, try last one.
                                    curBest1 = score1;
                                    for (SMALLTYPE m = 0 ; m < 9 ; m++){
                                        for (SMALLTYPE n = 0 ; n < 9 ; n++){
                                            if(shapeList[1].canPutIn(n, m , newField1)){
                                                Field newField2 = shapeList[1].putIn(l, k, newField1);
                                                //printf("Found at least one possible case : 0 -> 2 -> 1\n");
                                                SMALLTYPE score2 = newField2.peekScore();
                                                newField2.calculateScore();
                                                if ((score0 + score1 + score2) >= results[0].score){
                                                    results[0].futureCnt++;
                                                    results[0].score = score0 + score1 + score2; // update one more
                                                    results[0].Step1 = newField0;
                                                    results[0].Step2 = newField1;
                                                    results[0].BestFuture = newField2;
                                                    results[0].coords_i = i;
                                                    results[0].coords_j = j;
                                                    results[0].coords_k = k;
                                                    results[0].coords_l = l;
                                                    results[0].coords_m = m;
                                                    results[0].coords_n = n;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(shapeList[1].canPutIn(j, i, curField)){ // try 1, 0, 2 and 1, 2, 0
                Field newField0 = shapeList[0].putIn(j, i, curField);
                SMALLTYPE score0 = newField0.peekScore();
                if(curBest0 <= score0){ //
                    curBest0 = score0;
                    SMALLTYPE curBest1 = 0;
                    for(SMALLTYPE k = 0 ; k < 9 ; k++) {
                        for (SMALLTYPE l = 0; l < 9; l++) {
                            if (shapeList[0].canPutIn(l, k, newField0)) { // try 2.
                                Field newField1 = shapeList[0].putIn(l, k, newField0);
                                SMALLTYPE score1 = newField1.peekScore();
                                if (curBest1 <= score1) { // if we can put second piece, try last one.
                                    curBest1 = score1;
                                    SMALLTYPE curBest2 = 0;
                                    for (SMALLTYPE m = 0; m < 9; m++) {
                                        for (SMALLTYPE n = 0; n < 9; n++) {
                                            if (shapeList[2].canPutIn(n, m, newField1)) {
                                                Field newField2 = shapeList[2].putIn(l, k, newField1);
                                                //printf("Found at least one possible case : 1 -> 0 -> 2\n");
                                                SMALLTYPE score2 = newField2.peekScore();
                                                newField2.calculateScore();
                                                if (curBest2 <= score2) {
                                                    curBest2 = score2;
                                                    results[1].futureCnt++; // we at least have one survival case.
                                                    results[1].score = score0 + score1 + score2;
                                                    results[1].Step1 = newField0;
                                                    results[1].Step2 = newField1;
                                                    results[1].BestFuture = newField2;
                                                    results[1].coords_i = i;
                                                    results[1].coords_j = j;
                                                    results[1].coords_k = k;
                                                    results[1].coords_l = l;
                                                    results[1].coords_m = m;
                                                    results[1].coords_n = n;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    for(SMALLTYPE k = 0 ; k < 9 ; k++){
                        for(SMALLTYPE l = 0 ; l < 9 ; l++){
                            if (shapeList[2].canPutIn(l, k, newField0)){ // try 2.
                                Field newField1 = shapeList[2].putIn(l, k, newField0);
                                SMALLTYPE score1 = newField1.peekScore();
                                if(curBest1 <= score1){ // if we can put second piece, try last one.
                                    curBest1 = score1;
                                    for (SMALLTYPE m = 0 ; m < 9 ; m++){
                                        for (SMALLTYPE n = 0 ; n < 9 ; n++){
                                            if(shapeList[0].canPutIn(n, m , newField1)){
                                                Field newField2 = shapeList[0].putIn(l, k, newField1);
                                                //printf("Found at least one possible case : 1 -> 2 -> 0\n");
                                                SMALLTYPE score2 = newField2.peekScore();
                                                newField2.calculateScore();
                                                if ((score0 + score1 + score2) >= results[0].score){
                                                    results[1].futureCnt++;
                                                    results[1].score = score0 + score1 + score2; // update one more
                                                    results[1].Step1 = newField0;
                                                    results[1].Step2 = newField1;
                                                    results[1].BestFuture = newField2;
                                                    results[1].coords_i = i;
                                                    results[1].coords_j = j;
                                                    results[1].coords_k = k;
                                                    results[1].coords_l = l;
                                                    results[1].coords_m = m;
                                                    results[1].coords_n = n;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(shapeList[2].canPutIn(j, i, curField)){ // try 2, 0, 1 and 2, 1, 0
                Field newField0 = shapeList[2].putIn(j, i, curField);
                SMALLTYPE score0 = newField0.peekScore();
                if(curBest0 <= score0){ // try 1 -> 2 order.
                    curBest0 = score0;
                    SMALLTYPE curBest1 = 0;
                    for(SMALLTYPE k = 0 ; k < 9 ; k++) {
                        for (SMALLTYPE l = 0; l < 9; l++) {
                            if (shapeList[0].canPutIn(l, k, newField0)) { // try 2.
                                Field newField1 = shapeList[0].putIn(l, k, newField0);
                                SMALLTYPE score1 = newField1.peekScore();
                                if (curBest1 <= score1) { // if we can put second piece, try last one.
                                    curBest1 = score1;
                                    SMALLTYPE curBest2 = 0;
                                    for (SMALLTYPE m = 0; m < 9; m++) {
                                        for (SMALLTYPE n = 0; n < 9; n++) {
                                            if (shapeList[1].canPutIn(n, m, newField1)) {
                                                Field newField2 = shapeList[1].putIn(l, k, newField1);
                                                //printf("Found at least one possible case : 2 -> 0 -> 1\n");
                                                SMALLTYPE score2 = newField2.peekScore();
                                                newField2.calculateScore();
                                                if (curBest2 <= score2) {
                                                    curBest2 = score2;
                                                    results[2].futureCnt++; // we at least have one survival case.
                                                    results[2].score = score0 + score1 + score2;
                                                    results[2].Step1 = newField0;
                                                    results[2].Step2 = newField1;
                                                    results[2].BestFuture = newField2;
                                                    results[2].coords_i = i;
                                                    results[2].coords_j = j;
                                                    results[2].coords_k = k;
                                                    results[2].coords_l = l;
                                                    results[2].coords_m = m;
                                                    results[2].coords_n = n;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    for(SMALLTYPE k = 0 ; k < 9 ; k++){
                        for(SMALLTYPE l = 0 ; l < 9 ; l++){
                            if (shapeList[1].canPutIn(l, k, newField0)){ // try 2.
                                Field newField1 = shapeList[1].putIn(l, k, newField0);
                                SMALLTYPE score1 = newField1.peekScore();
                                if(curBest1 <= score1){ // if we can put second piece, try last one.
                                    curBest1 = score1;
                                    for (SMALLTYPE m = 0 ; m < 9 ; m++){
                                        for (SMALLTYPE n = 0 ; n < 9 ; n++){
                                            if(shapeList[0].canPutIn(n, m , newField1)){
                                                Field newField2 = shapeList[0].putIn(l, k, newField1);
                                                //printf("Found at least one possible case : 2 -> 1 -> 0\n");
                                                SMALLTYPE score2 = newField2.peekScore();
                                                newField2.calculateScore();
                                                if ((score0 + score1 + score2) >= results[0].score){
                                                    results[2].futureCnt++;
                                                    results[2].score = score0 + score1 + score2; // update one more
                                                    results[2].Step1 = newField0;
                                                    results[2].Step2 = newField1;
                                                    results[2].BestFuture = newField2;
                                                    results[2].coords_i = i;
                                                    results[2].coords_j = j;
                                                    results[2].coords_k = k;
                                                    results[2].coords_l = l;
                                                    results[2].coords_m = m;
                                                    results[2].coords_n = n;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    printf("FC 1 : %ld , FC 2 : %ld , FC 3 : %ld\n", results[0].futureCnt, results[1].futureCnt, results[2].futureCnt);
    if((results[0].futureCnt + results[1].futureCnt + results[2].futureCnt) != 0){
        SMALLTYPE bestScore = 0;
        SMALLTYPE index = 0;
        for(SMALLTYPE i = 0 ; i < 3 ; i++){
            if(bestScore <= results[i].score){
                bestScore = results[i].score;
                index = i;
            }
        }
        printf("BEST INDEX %d : \n", index);
        result.BestFuture = results[index].BestFuture;
        result = results[index];
    }
    else {
        printf("CANNOT FIND ANY... Its over...\n");
        result.futureCnt = 0;
    }
    printf("MAX Score : %d / Total Future Count : %ld\n", result.score, results[0].futureCnt + results[1].futureCnt + results[2].futureCnt);

    return result;
}

/**
 * Finding best future using simple heuristics. Heuristic is :
 * score + (81 - empty groups) / 10
 * @param curField The current field to brute force
 * @param shapeList The shapes to brute force with
 * @return returns Best Future result in bfResult struct type.
 */
bfResult HeuristicsMethod::findBestFuture(Field curField, Shape* shapeList){
    bfResult result;
    result.score = 0;
    result.futureCnt = 0;
    long sameCount = 0;
    SMALLTYPE curMAX = 0;
    for(SMALLTYPE i = 0 ; i < 9 ; i++){
        for (SMALLTYPE j = 0 ; j < 9 ; j++){
            for (SMALLTYPE k = 0 ; k < 9 ; k++){
                for (SMALLTYPE l = 0 ; l < 9 ; l++){
                    for(SMALLTYPE m = 0 ; m < 9 ; m++){
                        for(SMALLTYPE n = 0 ; n < 9 ; n++)
                            if (shapeList[0].canPutIn(i, j, curField)) {
                                Field newField0 = shapeList[0].putIn(i, j, curField);
                                if(shapeList[1].canPutIn(k, l, newField0)) {
                                    Field newField1 = shapeList[1].putIn(k, l, newField0);
                                    if (shapeList[2].canPutIn(m, n, newField1)) {
                                        Field newField2 = shapeList[2].putIn(m, n, newField1);

                                        SMALLTYPE score = newField2.peekScore();
                                        SMALLTYPE emptyGroups = newField2.getEmptySpaceGroups(); // number of empty groups.
                                        SMALLTYPE heuristicScore = score + ((81 - emptyGroups) / 10);
                                        result.futureCnt++;
                                        if (curMAX <= heuristicScore) {
                                            sameCount += curMAX == heuristicScore;
                                            curMAX = heuristicScore;
                                            result.BestFuture = newField2;
                                            result.Step1 = newField0;
                                            result.Step2 = newField1;
                                            result.score = score;
                                            result.coords_i = i;
                                            result.coords_j = j;
                                            result.coords_k = k;
                                            result.coords_l = l;
                                            result.coords_m = m;
                                            result.coords_n = n;
                                        }
                                    }
                                }
                            }
                    }
                }
            }
        }
    }

    if (result.futureCnt == 0){ // when we cannot find any possible future with 3 pieces all together
        printf("\x1B[31mNo possible future with all 3 blocks. Now using 2 blocks\n");
        SMALLTYPE blockScore[3];

        // Try with two blocks combination.
        bfResult results[3]; // block 0, 1
        results[0].futureCnt = 0;
        results[0].score = 0;
        results[1].futureCnt = 0;
        results[1].score = 0;
        results[2].futureCnt = 0;
        results[2].score = 0;

        for(SMALLTYPE i = 0 ; i < 9 ; i++) {
            for (SMALLTYPE j = 0; j < 9; j++) {
                for (SMALLTYPE k = 0; k < 9; k++) {
                    for (SMALLTYPE l = 0; l < 9; l++) {
                        if (shapeList[0].canPutIn(i, j, curField)) {
                            Field newField0 = shapeList[0].putIn(i, j, curField);
                            if (shapeList[1].canPutIn(k, l, newField0)) {
                                Field newField1 = shapeList[1].putIn(k, l, newField0);
                                SMALLTYPE score = newField1.peekScore();
                                SMALLTYPE emptyGroups = newField1.getEmptySpaceGroups(); // number of empty groups.
                                SMALLTYPE heuristicScore = score + ((81 - emptyGroups) / 10);
                                blockScore[0]++;
                                if (curMAX <= heuristicScore) {
                                    sameCount += curMAX == heuristicScore;
                                    curMAX = heuristicScore;
                                    results[0].BestFuture = newField1;
                                    results[0].Step1 = newField0;
                                    results[0].Step2 = newField1;
                                    results[0].score = score;
                                    results[0].coords_i = i;
                                    results[0].coords_j = j;
                                    results[0].coords_k = k;
                                    results[0].coords_l = l;
                                    results[0].coords_m = 0;
                                    results[0].coords_n = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
        for(SMALLTYPE i = 0 ; i < 9 ; i++) { // block 0, 2
            for (SMALLTYPE j = 0; j < 9; j++) {
                for (SMALLTYPE k = 0; k < 9; k++) {
                    for (SMALLTYPE l = 0; l < 9; l++) {
                        if (shapeList[0].canPutIn(i, j, curField)) {
                            Field newField0 = shapeList[0].putIn(i, j, curField);
                            if (shapeList[2].canPutIn(k, l, newField0)) {
                                Field newField1 = shapeList[2].putIn(k, l, newField0);
                                SMALLTYPE score = newField1.peekScore();
                                SMALLTYPE emptyGroups = newField1.getEmptySpaceGroups(); // number of empty groups.
                                SMALLTYPE heuristicScore = score + ((81 - emptyGroups) / 10);
                                blockScore[1]++;
                                if (curMAX <= heuristicScore) {
                                    sameCount += curMAX == heuristicScore;
                                    curMAX = heuristicScore;
                                    results[1].BestFuture = newField1;
                                    results[1].Step1 = newField0;
                                    results[1].Step2 = newField1;
                                    results[1].score = score;
                                    results[1].coords_i = i;
                                    results[1].coords_j = j;
                                    results[1].coords_k = k;
                                    results[1].coords_l = l;
                                    results[1].coords_m = 0;
                                    results[1].coords_n = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
        for(SMALLTYPE i = 0 ; i < 9 ; i++) { // block 1, 2
            for (SMALLTYPE j = 0; j < 9; j++) {
                for (SMALLTYPE k = 0; k < 9; k++) {
                    for (SMALLTYPE l = 0; l < 9; l++) {
                        if (shapeList[1].canPutIn(i, j, curField)) {
                            Field newField0 = shapeList[1].putIn(i, j, curField);
                            if (shapeList[2].canPutIn(k, l, newField0)) {
                                Field newField1 = shapeList[2].putIn(k, l, newField0);
                                SMALLTYPE score = newField1.peekScore();
                                SMALLTYPE emptyGroups = newField1.getEmptySpaceGroups(); // number of empty groups.
                                SMALLTYPE heuristicScore = score + ((81 - emptyGroups) / 10);
                                blockScore[2]++;
                                if (curMAX <= heuristicScore) {
                                    sameCount += curMAX == heuristicScore;
                                    curMAX = heuristicScore;
                                    results[2].BestFuture = newField1;
                                    results[2].Step1 = newField0;
                                    results[2].Step2 = newField1;
                                    results[2].score = score;
                                    results[2].coords_i = i;
                                    results[2].coords_j = j;
                                    results[2].coords_k = k;
                                    results[2].coords_l = l;
                                    results[2].coords_m = 0;
                                    results[2].coords_n = 0;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (result.futureCnt != 0){
            SMALLTYPE futureScore[3];
            SMALLTYPE maxIndex = 0;
            Field possibleField;

            for(SMALLTYPE i = 0 ; i < 3 ; i++){
                futureScore[i] = results[i].score + blockScore[i];
                maxIndex = i;
                possibleField = results[i].BestFuture;
            }
            possibleField.calculateScore(); // clear out possible field for 3rd piece input.

            if(maxIndex == 0){
                SMALLTYPE possibleMAX = 0;
                for(SMALLTYPE i = 0 ; i < 9 ; i++){
                    for(SMALLTYPE j = 0 ; j < 9 ; j++){
                        if(shapeList[2].canPutIn(j, i, possibleField)){
                            result.futureCnt++;
                            Field newField1 = shapeList[2].putIn(j, i, possibleField);
                            SMALLTYPE score = newField1.peekScore();
                            SMALLTYPE emptyGroups = newField1.getEmptySpaceGroups(); // number of empty groups.
                            SMALLTYPE heuristicScore = score + ((81 - emptyGroups) / 10);
                            if (possibleMAX <= heuristicScore) {
                                sameCount += possibleMAX == heuristicScore;
                                possibleMAX = heuristicScore;
                                results[2].BestFuture = newField1;
                                results[2].score = score;
                                results[2].coords_m = i;
                                results[2].coords_n = j;
                            }
                        }
                    }
                }
            }

            if(maxIndex == 1){
                SMALLTYPE possibleMAX = 0;
                for(SMALLTYPE i = 0 ; i < 9 ; i++){
                    for(SMALLTYPE j = 0 ; j < 9 ; j++){
                        if(shapeList[1].canPutIn(j, i, possibleField)){
                            result.futureCnt++;
                            Field newField1 = shapeList[1].putIn(j, i, possibleField);
                            SMALLTYPE score = newField1.peekScore();
                            SMALLTYPE emptyGroups = newField1.getEmptySpaceGroups(); // number of empty groups.
                            SMALLTYPE heuristicScore = score + ((81 - emptyGroups) / 10);
                            if (possibleMAX <= heuristicScore) {
                                sameCount += possibleMAX == heuristicScore;
                                possibleMAX = heuristicScore;
                                results[2].BestFuture = newField1;
                                results[2].score = score;
                                results[2].coords_m = i;
                                results[2].coords_n = j;
                            }
                        }
                    }
                }
            }

            if(maxIndex == 2){
                SMALLTYPE possibleMAX = 0;
                for(SMALLTYPE i = 0 ; i < 9 ; i++){
                    for(SMALLTYPE j = 0 ; j < 9 ; j++){
                        if(shapeList[0].canPutIn(j, i, possibleField)){
                            result.futureCnt++;
                            Field newField1 = shapeList[0].putIn(j, i, possibleField);
                            SMALLTYPE score = newField1.peekScore();
                            SMALLTYPE emptyGroups = newField1.getEmptySpaceGroups(); // number of empty groups.
                            SMALLTYPE heuristicScore = score + ((81 - emptyGroups) / 10);
                            if (possibleMAX <= heuristicScore) {
                                sameCount += possibleMAX == heuristicScore;
                                possibleMAX = heuristicScore;
                                results[2].BestFuture = newField1;
                                results[2].score = score;
                                results[2].coords_m = i;
                                results[2].coords_n = j;
                            }
                        }
                    }
                }
            }
            if (result.futureCnt == 1)
                printf("\x1B[32mThat was close. Phew...\n");

        }else {
            // Try with two blocks combination.
            printf("\x1B[31mNo possible future with all 2 blocks. Now using 1 block\n");
            SMALLTYPE curBest0 = 0;

           for (SMALLTYPE i = 0 ; i < 9 ; i++){
                for (SMALLTYPE j = 0 ; j < 9 ; j++){
                    if(shapeList[0].canPutIn(j, i, curField)){ // try 0, 1, 2 and 0, 2, 1
                        Field newField0 = shapeList[0].putIn(j, i, curField);
                        SMALLTYPE score0 = newField0.peekScore();
                        if(curBest0 <= score0){ // try 1 -> 2 order.
                            curBest0 = score0;
                            SMALLTYPE curBest1 = 0;
                            for(SMALLTYPE k = 0 ; k < 9 ; k++) {
                                for (SMALLTYPE l = 0; l < 9; l++) {
                                    if (shapeList[1].canPutIn(l, k, newField0)) { // try 2.
                                        Field newField1 = shapeList[1].putIn(l, k, newField0);
                                        SMALLTYPE score1 = newField1.peekScore();
                                        if (curBest1 <= score1) { // if we can put second piece, try last one.
                                            curBest1 = score1;
                                            SMALLTYPE curBest2 = 0;
                                            for (SMALLTYPE m = 0; m < 9; m++) {
                                                for (SMALLTYPE n = 0; n < 9; n++) {
                                                    if (shapeList[2].canPutIn(n, m, newField1)) {
                                                        Field newField2 = shapeList[2].putIn(l, k, newField1);
                                                        printf("Found at least one possible case : 0 -> 1 -> 2\n");
                                                        SMALLTYPE score2 = newField2.peekScore();
                                                        newField2.calculateScore();
                                                        if (curBest2 <= score2) {
                                                            curBest2 = score2;
                                                            results[0].futureCnt++; // we at least have one survival case.
                                                            results[0].score = score0 + score1 + score2;
                                                            results[0].Step1 = newField0;
                                                            results[0].Step2 = newField1;
                                                            results[0].BestFuture = newField2;
                                                            results[0].coords_i = i;
                                                            results[0].coords_j = j;
                                                            results[0].coords_k = k;
                                                            results[0].coords_l = l;
                                                            results[0].coords_m = m;
                                                            results[0].coords_n = n;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            for(SMALLTYPE k = 0 ; k < 9 ; k++){
                                for(SMALLTYPE l = 0 ; l < 9 ; l++){
                                    if (shapeList[2].canPutIn(l, k, newField0)){ // t.
                                        Field newField1 = shapeList[2].putIn(l, k, newField0);
                                        SMALLTYPE score1 = newField1.peekScore();
                                        if(curBest1 <= score1){ // if we can put second piece, try last one.
                                            curBest1 = score1;
                                            for (SMALLTYPE m = 0 ; m < 9 ; m++){
                                                for (SMALLTYPE n = 0 ; n < 9 ; n++){
                                                    if(shapeList[1].canPutIn(n, m , newField1)){
                                                        Field newField2 = shapeList[1].putIn(l, k, newField1);
                                                        printf("Found at least one possible case : 0 -> 2 -> 1\n");
                                                        SMALLTYPE score2 = newField2.peekScore();
                                                        newField2.calculateScore();
                                                        if ((score0 + score1 + score2) >= results[0].score){
                                                            results[0].futureCnt++;
                                                            results[0].score = score0 + score1 + score2; // update one more
                                                            results[0].Step1 = newField0;
                                                            results[0].Step2 = newField1;
                                                            results[0].BestFuture = newField2;
                                                            results[0].coords_i = i;
                                                            results[0].coords_j = j;
                                                            results[0].coords_k = k;
                                                            results[0].coords_l = l;
                                                            results[0].coords_m = m;
                                                            results[0].coords_n = n;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(shapeList[1].canPutIn(j, i, curField)){ // try 1, 0, 2 and 1, 2, 0
                        Field newField0 = shapeList[0].putIn(j, i, curField);
                        SMALLTYPE score0 = newField0.peekScore();
                        if(curBest0 <= score0){ //
                            curBest0 = score0;
                            SMALLTYPE curBest1 = 0;
                            for(SMALLTYPE k = 0 ; k < 9 ; k++) {
                                for (SMALLTYPE l = 0; l < 9; l++) {
                                    if (shapeList[0].canPutIn(l, k, newField0)) { // try 2.
                                        Field newField1 = shapeList[0].putIn(l, k, newField0);
                                        SMALLTYPE score1 = newField1.peekScore();
                                        if (curBest1 <= score1) { // if we can put second piece, try last one.
                                            curBest1 = score1;
                                            SMALLTYPE curBest2 = 0;
                                            for (SMALLTYPE m = 0; m < 9; m++) {
                                                for (SMALLTYPE n = 0; n < 9; n++) {
                                                    if (shapeList[2].canPutIn(n, m, newField1)) {
                                                        Field newField2 = shapeList[2].putIn(l, k, newField1);
                                                        printf("Found at least one possible case : 1 -> 0 -> 2\n");
                                                        SMALLTYPE score2 = newField2.peekScore();
                                                        newField2.calculateScore();
                                                        if (curBest2 <= score2) {
                                                            curBest2 = score2;
                                                            results[1].futureCnt++; // we at least have one survival case.
                                                            results[1].score = score0 + score1 + score2;
                                                            results[1].Step1 = newField0;
                                                            results[1].Step2 = newField1;
                                                            results[1].BestFuture = newField2;
                                                            results[1].coords_i = i;
                                                            results[1].coords_j = j;
                                                            results[1].coords_k = k;
                                                            results[1].coords_l = l;
                                                            results[1].coords_m = m;
                                                            results[1].coords_n = n;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            for(SMALLTYPE k = 0 ; k < 9 ; k++){
                                for(SMALLTYPE l = 0 ; l < 9 ; l++){
                                    if (shapeList[2].canPutIn(l, k, newField0)){ // try 2.
                                        Field newField1 = shapeList[2].putIn(l, k, newField0);
                                        SMALLTYPE score1 = newField1.peekScore();
                                        if(curBest1 <= score1){ // if we can put second piece, try last one.
                                            curBest1 = score1;
                                            for (SMALLTYPE m = 0 ; m < 9 ; m++){
                                                for (SMALLTYPE n = 0 ; n < 9 ; n++){
                                                    if(shapeList[0].canPutIn(n, m , newField1)){
                                                        Field newField2 = shapeList[0].putIn(l, k, newField1);
                                                        printf("Found at least one possible case : 1 -> 2 -> 0\n");
                                                        SMALLTYPE score2 = newField2.peekScore();
                                                        newField2.calculateScore();
                                                        if ((score0 + score1 + score2) >= results[0].score){
                                                            results[1].futureCnt++;
                                                            results[1].score = score0 + score1 + score2; // update one more
                                                            results[1].Step1 = newField0;
                                                            results[1].Step2 = newField1;
                                                            results[1].BestFuture = newField2;
                                                            results[1].coords_i = i;
                                                            results[1].coords_j = j;
                                                            results[1].coords_k = k;
                                                            results[1].coords_l = l;
                                                            results[1].coords_m = m;
                                                            results[1].coords_n = n;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(shapeList[2].canPutIn(j, i, curField)){ // try 2, 0, 1 and 2, 1, 0
                        Field newField0 = shapeList[2].putIn(j, i, curField);
                        SMALLTYPE score0 = newField0.peekScore();
                        if(curBest0 <= score0){ // try 1 -> 2 order.
                            curBest0 = score0;
                            SMALLTYPE curBest1 = 0;
                            for(SMALLTYPE k = 0 ; k < 9 ; k++) {
                                for (SMALLTYPE l = 0; l < 9; l++) {
                                    if (shapeList[0].canPutIn(l, k, newField0)) { // try 2.
                                        Field newField1 = shapeList[0].putIn(l, k, newField0);
                                        SMALLTYPE score1 = newField1.peekScore();
                                        if (curBest1 <= score1) { // if we can put second piece, try last one.
                                            curBest1 = score1;
                                            SMALLTYPE curBest2 = 0;
                                            for (SMALLTYPE m = 0; m < 9; m++) {
                                                for (SMALLTYPE n = 0; n < 9; n++) {
                                                    if (shapeList[1].canPutIn(n, m, newField1)) {
                                                        Field newField2 = shapeList[1].putIn(l, k, newField1);
                                                        printf("Found at least one possible case : 2 -> 0 -> 1\n");
                                                        SMALLTYPE score2 = newField2.peekScore();
                                                        newField2.calculateScore();
                                                        if (curBest2 <= score2) {
                                                            curBest2 = score2;
                                                            results[2].futureCnt++; // we at least have one survival case.
                                                            results[2].score = score0 + score1 + score2;
                                                            results[2].Step1 = newField0;
                                                            results[2].Step2 = newField1;
                                                            results[2].BestFuture = newField2;
                                                            results[2].coords_i = i;
                                                            results[2].coords_j = j;
                                                            results[2].coords_k = k;
                                                            results[2].coords_l = l;
                                                            results[2].coords_m = m;
                                                            results[2].coords_n = n;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            for(SMALLTYPE k = 0 ; k < 9 ; k++){
                                for(SMALLTYPE l = 0 ; l < 9 ; l++){
                                    if (shapeList[1].canPutIn(l, k, newField0)){ // try 2.
                                        Field newField1 = shapeList[1].putIn(l, k, newField0);
                                        SMALLTYPE score1 = newField1.peekScore();
                                        if(curBest1 <= score1){ // if we can put second piece, try last one.
                                            curBest1 = score1;
                                            for (SMALLTYPE m = 0 ; m < 9 ; m++){
                                                for (SMALLTYPE n = 0 ; n < 9 ; n++){
                                                    if(shapeList[0].canPutIn(n, m , newField1)){
                                                        Field newField2 = shapeList[0].putIn(l, k, newField1);
                                                        printf("Found at least one possible case : 2 -> 1 -> 0\n");
                                                        SMALLTYPE score2 = newField2.peekScore();
                                                        newField2.calculateScore();
                                                        if ((score0 + score1 + score2) >= results[0].score){
                                                            results[2].futureCnt++;
                                                            results[2].score = score0 + score1 + score2; // update one more
                                                            results[2].Step1 = newField0;
                                                            results[2].Step2 = newField1;
                                                            results[2].BestFuture = newField2;
                                                            results[2].coords_i = i;
                                                            results[2].coords_j = j;
                                                            results[2].coords_k = k;
                                                            results[2].coords_l = l;
                                                            results[2].coords_m = m;
                                                            results[2].coords_n = n;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            printf("FC 1 : %ld , FC 2 : %ld , FC 3 : %ld\n", results[0].futureCnt, results[1].futureCnt, results[2].futureCnt);
            if((results[0].futureCnt + results[1].futureCnt + results[2].futureCnt) != 0){
                SMALLTYPE bestScore = 0;
                SMALLTYPE index = 0;
                for(SMALLTYPE i = 0 ; i < 3 ; i++){
                    if(bestScore <= results[i].score){
                        bestScore = results[i].score;
                        index = i;
                    }
                }
                printf("BEST INDEX %d : \n", index);
                result.BestFuture = results[index].BestFuture;
            }
            else {
                printf("CANNOT FIND ANY... Its over...\n");
                result.futureCnt = 0;
            }
        }
    }

    printf("MAX Score : %d / Group Score : %d\n", result.score, (81 - result.BestFuture.getEmptySpaceGroups()) / 10);
    printf("Total Same counts : %ld\n", sameCount);
    return result;
}

/**
 * A member function for class BruteForceStupid for literally brute forcing every possible combinations and looking
 * for the best future with the best score. Calculates in 6 for loops starting from (0,0,0,0,0,0) to (9,9,9,9,9,9).
 * Worst case scenario, it would be 9^6.
 * @param curField The current field to brute force
 * @param shapeList The shapes to brute force with
 * @return returns Best Future result in bfResult struct type.
 */
bfResult ScoringMethod::findBestFuture(Field curField, Shape* shapeList) {
    bfResult result;
    result.score = 0;
    result.futureCnt = 0;
    SMALLTYPE curMAX = 0;
    for(SMALLTYPE i = 0 ; i < 9 ; i++){
        for (SMALLTYPE j = 0 ; j < 9 ; j++){
            for (SMALLTYPE k = 0 ; k < 9 ; k++){
                for (SMALLTYPE l = 0 ; l < 9 ; l++){
                    for(SMALLTYPE m = 0 ; m < 9 ; m++){
                        for(SMALLTYPE n = 0 ; n < 9 ; n++)
                            if (shapeList[0].canPutIn(i, j, curField)) {
                                Field newField0 = shapeList[0].putIn(i, j, curField);
                                if(shapeList[1].canPutIn(k, l, newField0)) {
                                    Field newField1 = shapeList[1].putIn(k, l, newField0);
                                    if (shapeList[2].canPutIn(m, n, newField1)) {
                                        Field newField2 = shapeList[2].putIn(m, n, newField1);
                                        SMALLTYPE score = newField2.peekScore();
                                        result.futureCnt++;
                                        if (curMAX <= score) {
                                            curMAX = score;
                                            result.BestFuture = newField2;
                                            result.Step1 = newField0;
                                            result.Step2 = newField1;
                                            result.score = score;
                                            result.coords_i = i;
                                            result.coords_j = j;
                                            result.coords_k = k;
                                            result.coords_l = l;
                                            result.coords_m = m;
                                            result.coords_n = n;
                                        }
                                    }
                                }
                            }
                    }
                }
            }
        }
    }
    printf("MAX Score : %d", curMAX);
    return result;
}

/**
 * A class that runs algorithm and the game. Using polymorphism, just throw algorithm class in it and it will
 * run the game and all those systems.
 * @param a the algorithm object
 * @param curField : starting field . Shall be empty and clean
 * @param allShapes : array of all shapes
 */
void runAlgorithm(Algorithm* a, Field curField, Shape* allShapes){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 48);

    Shape curShapes[3];
    bfResult bestFuture;
    unsigned long score = 0;

    while(1){
        for (SMALLTYPE  i = 0 ; i < 3 ; i++){
            curShapes[i] = allShapes[dis(gen)];
            curShapes[i].printShape();
        }
        curField.printField();

        clock_t start = clock();

        bestFuture = a->findBestFuture(curField, curShapes);
        printf("\x1B[%dmFuture cnt : %ld\n", (32 - (bestFuture.futureCnt < 500)), bestFuture.futureCnt);
        printf("\x1B[37m");
        // Print red if futures are less than 500
        if (bestFuture.futureCnt == 0){
            printf("\n\nGAMEOVER...\nSCORE : %ld\n", score);
            break;
        }

        clock_t end = clock();

        curField = bestFuture.BestFuture;
        bestFuture.Step1.printField();
        bestFuture.Step2.printField();
        SMALLTYPE newScore = 0;
        newScore = curField.calculateScore();
        newScore += curShapes[0].shapeSize();
        newScore += curShapes[1].shapeSize();
        newScore += curShapes[2].shapeSize();
        score += newScore;

        curField.printField();

        printf("Time Elapsed : %lf sec\n", (double)(end - start) / CLOCKS_PER_SEC);
        printf("Max Score : %d\n", bestFuture.score);
        printf("MOVING : %d %d %d %d %d %d\n", bestFuture.coords_i, bestFuture.coords_j, bestFuture.coords_k, bestFuture.coords_l, bestFuture.coords_m, bestFuture.coords_n);
        printf("\nScore %ld (+%d)\n", score, newScore);
        //scanf("%d", &i);

        unsigned int microsecond = 100000;
        usleep(3 * microsecond);//sleeps for 3 second
    }
    return;
}