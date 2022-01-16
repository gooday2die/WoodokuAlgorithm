//
// By Gooday2die @ https://github.com/gooday2die
//

#include "Algorithm.h"



bfResult BruteForceStupid::findBestFuture(Field curField, Shape* shapeList){
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
                                        //newField2.printField();
                                        SMALLTYPE score = newField2.peekScore();
                                        //printf("SCORE : %d", score);
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

void BruteForceStupid::run(Field curField){
    printf("NOW RUNNING GAME\n");
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 43);

    Shape curShapes[3];
    bfResult bestFuture;
    unsigned long score = 0;

    int i = 0;

    while(1){
        for (SMALLTYPE  i = 0 ; i < 3 ; i++){
            curShapes[i] = allShapes[dis(gen)];
            curShapes[i].printShape();
        }

        clock_t start = clock();

        bestFuture = findBestFuture(curField, curShapes);
        printf("Future cnt : %ld\n", bestFuture.futureCnt);
        if (bestFuture.futureCnt == 0){
            printf("\n\nGAMEOVER...\nSCORE : %ld", score);
            break;
        }

        clock_t end = clock();

        curField = bestFuture.BestFuture;
        bestFuture.Step1.printField();
        bestFuture.Step2.printField();
        curField.printField();
        SMALLTYPE newScore = 0;
        newScore = curField.calculateScore();
        newScore += curShapes[0].shapeSize();
        newScore += curShapes[1].shapeSize();
        newScore += curShapes[2].shapeSize();
        score += newScore;

        printf("Time Elapsed : %lf sec\n", (double)(end - start) / CLOCKS_PER_SEC);
        printf("BEST SCORE : %d\n", bestFuture.score);
        printf("MOVING : %d %d %d %d %d %d\n", bestFuture.coords_i, bestFuture.coords_j, bestFuture.coords_k, bestFuture.coords_l, bestFuture.coords_m, bestFuture.coords_n);
        printf("\nScore %ld (+%d)\n", score, newScore);
        //scanf("%d", &i);

        unsigned int microsecond = 100000;
        //usleep(3 * microsecond);//sleeps for 3 second
    }
    return;
}
