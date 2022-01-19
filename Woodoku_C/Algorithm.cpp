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


/**
 * A member function for ThreadHeuristicMethod. This would be used as a single Thread
 * @param curField the current field to search
 * @param shapeList the shape list
 * @param x the first shape to put
 * @param y the second shape to put
 * @param z the last shape to put
 * @param pResult the pointer to get results. Just like old C.
 */
void ThreadHeuristicMethod::algorithmThread(Field curField, Shape* shapeList, SMALLTYPE x, SMALLTYPE y, SMALLTYPE z, SMALLTYPE threadNo, bfResult* pResult){
    pResult->score = 0;
    pResult->futureCnt = 0;
    long sameCount = 0;
    SMALLTYPE curMAX = 0;
    /**for(SMALLTYPE i = 0 ; i < 9 ; i++){
        for (SMALLTYPE j = 0 ; j < 9 ; j++){
            for (SMALLTYPE k = 0 ; k < 9 ; k++){
                for (SMALLTYPE l = 0 ; l < 9 ; l++){
                    for(SMALLTYPE m = 0 ; m < 9 ; m++){
                        for(SMALLTYPE n = 0 ; n < 9 ; n++)
                            if (shapeList[x].canPutIn(i, j, curField)) {
                                Field newField0 = shapeList[x].putIn(i, j, curField);
                                if(shapeList[y].canPutIn(k, l, newField0)) {
                                    Field newField1 = shapeList[y].putIn(k, l, newField0);
                                    if (shapeList[z].canPutIn(m, n, newField1)) {
                                        Field newField2 = shapeList[z].putIn(m, n, newField1);

                                        SMALLTYPE score = newField2.peekScore();
                                        SMALLTYPE emptyGroups = newField2.getEmptySpaceGroups(); // number of empty groups.
                                        SMALLTYPE heuristicScore = score + ((81 - emptyGroups) / 10);

                                        pResult->futureCnt++;
                                        if (curMAX <= heuristicScore) {
                                            sameCount += curMAX == heuristicScore;
                                            curMAX = heuristicScore;
                                            pResult->BestFuture = newField2;
                                            pResult->Step1 = newField0;
                                            pResult->Step2 = newField1;
                                            pResult->score = score;
                                            pResult->coords_i = i;
                                            pResult->coords_j = j;
                                            pResult->coords_k = k;
                                            pResult->coords_l = l;
                                            pResult->coords_m = m;
                                            pResult->coords_n = n;
                                        }
                                    }
                                }
                            }
                    }
                }
            }
        }
    }
     **/
    for(SMALLTYPE i = 0 ; i < 9 ; i++){
        for (SMALLTYPE j = 0 ; j < 9 ; j++){
            for (SMALLTYPE k = 0 ; k < 9 ; k++){
                for (SMALLTYPE l = 0 ; l < 9 ; l++){
                    for(SMALLTYPE m = 0 ; m < 9 ; m++){
                        for(SMALLTYPE n = 0 ; n < 9 ; n++) {
                            SMALLTYPE score = 0;
                            if (shapeList[x].canPutIn(i, j, curField)) {
                                Field newField0 = shapeList[x].putIn(i, j, curField);
                                score += newField0.calculateScore();
                                if (shapeList[y].canPutIn(k, l, newField0)) {
                                    Field newField1 = shapeList[y].putIn(k, l, newField0);
                                    score += newField1.calculateScore();
                                    if (shapeList[z].canPutIn(m, n, newField1)) {
                                        Field newField2 = shapeList[z].putIn(m, n, newField1);
                                        score += newField2.peekScore();
                                        SMALLTYPE emptyGroups = newField2.getEmptySpaceGroups(); // number of empty groups.
                                        SMALLTYPE filledGroups = newField2.getFilledSpaceGroups();
                                        SMALLTYPE heuristicScore = score + ((81 - emptyGroups) / 10) + ((81 - filledGroups) / 10);

                                        pResult->futureCnt++;
                                        if (curMAX <= heuristicScore) {
                                            sameCount += curMAX == heuristicScore;
                                            curMAX = heuristicScore;
                                            pResult->BestFuture = newField2;
                                            pResult->Step1 = newField0;
                                            pResult->Step2 = newField1;
                                            pResult->score = score;
                                            pResult->coords_i = i;
                                            pResult->coords_j = j;
                                            pResult->coords_k = k;
                                            pResult->coords_l = l;
                                            pResult->coords_m = m;
                                            pResult->coords_n = n;
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
/**
 * A method for finding the best future with threads. The old HeuristicsMethod had just one option to find the best
 * future. The order was fixed to 1->2->3. However, with test results there were some cases that might have survived
 * and scored more if the order was a bit different. So this new member function searches ways all 6 cases (3!). Since
 * it will take up to 6 more calculations and times, this will use Threading.
 * @param curField
 * @param shapeList
 * @return
 */
bfResult resultList[6]; // generate non local variable. by CSNE
bfResult ThreadHeuristicMethod::findBestFuture(Field curField, Shape* shapeList){
    SMALLTYPE heuristicsList[6];
    SMALLTYPE bestIndex = 0;
    SMALLTYPE bestHeuristic = 0;
    long bestFutureCnt = 0;
    ThreadHeuristicMethod ta;
    // Generate threads.
    std::thread t1(&ThreadHeuristicMethod::algorithmThread, ta, curField, shapeList, 0, 1, 2, 1, &(resultList[0]));
    std::thread t2(&ThreadHeuristicMethod::algorithmThread, ta, curField, shapeList, 0, 2, 1, 2, &(resultList[1]));
    std::thread t3(&ThreadHeuristicMethod::algorithmThread, ta, curField, shapeList, 1, 0, 2, 3, &(resultList[2]));
    std::thread t4(&ThreadHeuristicMethod::algorithmThread, ta, curField, shapeList, 1, 2, 0, 4, &(resultList[3]));
    std::thread t5(&ThreadHeuristicMethod::algorithmThread, ta, curField, shapeList, 2, 0, 1, 5, &(resultList[4]));
    std::thread t6(&ThreadHeuristicMethod::algorithmThread, ta, curField, shapeList, 2, 1, 0, 6, &(resultList[5]));

    t1.join(); // After they are done, join them
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    for(SMALLTYPE i = 0 ; i < 6 ; i++) {
        heuristicsList[i] = resultList[i].score + ((81 - (resultList[i].BestFuture.getEmptySpaceGroups())) / 10) +
                ((81 - (resultList[i].BestFuture.getFilledSpaceGroups())) / 10);
        printf("Thread %d : Total Future %ld / MAX Score %d \n", i, resultList[i].futureCnt, heuristicsList[i]);

        if (bestHeuristic <= heuristicsList[i]) {
            if(bestFutureCnt <= resultList[i].futureCnt){
                bestHeuristic = heuristicsList[i];
                bestFutureCnt = resultList[i].futureCnt;
                bestIndex = i;
            }
        }
    }

    printf("Best index : %d\n", bestIndex);
    printf("MAX Score : %d / Empty Group Score : %d / Filled Group Score : %d\n", resultList[bestIndex].score,
           ((81 - resultList[bestIndex].BestFuture.getEmptySpaceGroups())) / 10,
           ((81 - resultList[bestIndex].BestFuture.getFilledSpaceGroups())) / 10);
    return resultList[bestIndex];
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
 * A class that runs algorithm
 * and the game. Using polymorphism, just throw algorithm class in it and it will
 * run the game and all those systems.
 * @param a the algorithm object
 * @param curField : starting field . Shall be empty and clean
 * @param allShapes : array of all shapes
 */
void runAlgorithm(Algorithm* a, Field curField, Shape* allShapes){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 48);
    printf("Started Game\n");
    Shape curShapes[3];
    bfResult bestFuture;
    unsigned long score = 0;

    while(1){
        SMALLTYPE shapeIndex[3];
        for (SMALLTYPE  i = 0 ; i < 3 ; i++){
            shapeIndex[i] = dis(gen);
            curShapes[i] = allShapes[shapeIndex[i]];
            curShapes[i].printShape();
        }
        curField.printField();

        clock_t start = clock();

        bestFuture = a->findBestFuture(curField, curShapes);
        printf("\x1B[%dmFuture cnt : %ld\n", (32 - (bestFuture.futureCnt < 500)), bestFuture.futureCnt);
        printf("\x1B[37m");
        // Print red if futures are less than 500
        if (bestFuture.futureCnt == 0){
            printf("\n\nGAMEOVER...\n");
            FILE *fp;
            fp = fopen("whokilled.txt", "a+");

            fprintf(fp, "%d %d %d : %ld\n", shapeIndex[0], shapeIndex[1], shapeIndex[2], score);
            fclose(fp);
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

        unsigned int microsecond = 100000;
        usleep(3 * microsecond);//sleeps for 3 second
    }
    printf("Score : %ld\n" , score);
    return;
}