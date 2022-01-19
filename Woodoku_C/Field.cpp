//
// By Gooday2die @ https://github.com/gooday2die
//

#include "Field.h"


/**
 * A member function that calculates the score by the current field and clears out the pixels.
 * If you are going to just "Check" score, please use SMALLTYPE Field::peekScore() instead.
 * This member function would be called just one time for clearing out the field and calculating the score.
 * @return returns SMALLTYPE value of score.
 */
SMALLTYPE Field::calculateScore(void){
    SMALLTYPE streaks = 0;
    char score = 0;
    // Those following arrays are generated since I am going to do a bit operation later with each cleared out fields.
    // Operating AND operation between all arrays will make the cleared out field.
    uint32_t row_array[3]; // for row
    uint32_t col_array[3]; // for col
    uint32_t blk_array[3]; // for block

    for (SMALLTYPE i = 0 ; i < 3 ; i++){
        // We are going to make copied fields and then do AND operation later.
        row_array[i] = 0;
        col_array[i] = 0xFFFFFFFF;
        blk_array[i] = 0xFFFFFFFF;
    }

    // Calculate rows
    // Using bit operations. Will be going ROW 0, 3, 6/  1, 4, 7/  2, 5 ,8 order.
    // We are going to turn any row that is 0x1FF to 0x0000. Else would be its original value.
    // Bit Operation would be (0xFFFFFFFF + (variable == 0x1FF)) & variable.
    // That will make 0x1FF to 0x000 and other things as it were before.
    // We need to push it back to right spot, so use OR operation and left shift operations.
    // printf("%u , %u , %u\n", pixels[0], pixels[1], pixels[2]);
    for (SMALLTYPE i = 0 ; i < 3 ; i++){
        for(SMALLTYPE j = 0 ; j < 3 ; j++) {
            uint16_t curRow = (pixels[j] >> (9 * (2 - i)) & 0x1FF);
            streaks += (curRow == 0x1FF);
            row_array[j] = row_array[j] | (((0xFFFFFFFF + (((pixels[j] >> (9 * (2 - i))) & 0x1FF) == 0x1FF)) &
                                           (((pixels[j] >> (9 * (2 - i))) & 0x1FF))) << (9 * (2 - i)));
        }
    }

    // Calculate rows
    // Using bit operations. Will be going ROW 0 ~ 8 order.
    // Generate a column based on the results
    // Then do AND operation with 0xFFFFFFFF. Then generate a column array
    for(SMALLTYPE i = 0 ; i < 9 ; i++){
        uint32_t curCol[3] = {0,0,0};
        SMALLTYPE curColSum = 0;
        for(SMALLTYPE j = 0 ; j < 3 ; j++) {
            curCol[j] = curCol[j] | (((pixels[j] >> (26 - i)) & 1) << (26 - i)); // fucking operator precedence. & , <<
            curCol[j] = curCol[j] | (((pixels[j] >> (17 - i)) & 1) << (17 - i));
            curCol[j] = curCol[j] | (((pixels[j] >> (8 - i)) & 1) << (8 - i));
            curColSum += ((pixels[j] >> (26 - i)) & 1) + ((pixels[j] >> (17 - i)) & 1) + ((pixels[j] >> (8 - i)) & 1);
        }
        streaks += curColSum == 9; // this counts 0s as well
        if ((curCol[0] == curCol[1]) && (curCol[1] == curCol[2])){
            for(SMALLTYPE j = 0 ; j < 3 ; j++)
                col_array[j] &= curCol[j] ^ pixels[j]; // clear blocks
        }
    }

    // Going to compare with those following values and then do a XOR operation for clearing field itself.
    // 1st sector : 0x70381C0
    // 2nd sector : 0xe07038
    // 3rd sector : 0x1c0e07
    for (SMALLTYPE i = 0 ; i < 3 ; i++){
        uint32_t tmpValue = 0;
        streaks += (pixels[i] & 0x70381C0) == 0x70381C0; // check streaks
        streaks += (pixels[i] & 0xe07038) == 0xe07038;
        streaks += (pixels[i] & 0x1c0e07) == 0x1c0e07;

        if ((pixels[i] & 0x70381C0) == 0x70381C0) blk_array[i] &= (pixels[i] ^ 0x70381C0); // clear it out.
        if ((pixels[i] & 0xe07038) == 0xe07038) blk_array[i] &= (pixels[i] ^ 0xe07038);
        if ((pixels[i] & 0x1c0e07) == 0x1c0e07) blk_array[i] &= (pixels[i] ^ 0x1c0e07);
    }

    // Do AND operations with all arrays will create a cleared field. Then save it to pixels.
    for(SMALLTYPE i = 0 ; i < 3 ; i++){
        pixels[i] = ((row_array[i] & col_array[i]) & blk_array[i]);
    }

    score = streaks * 18 + 10 * (streaks > 1); // calculate score
    return (score & -(0 < score)); // if negative, return 0.
}

/**
 * A member function that calculates the score by the current field.
 * If you are going to "Check" score and "Clear out" the field, please use SMALLTYPE Field::calculateScore() instead.
 * This member function is intended to not change the field and just to peek the score. This member function will be
 * called a lot of times, so I had to optimize this member function to use as little amount of time as possible using
 * bitwise operations. It would be difficult to understand but will make sense if you read the code.
 * @return SMALLTYPE value of score.
 */
SMALLTYPE Field::peekScore(void){
    SMALLTYPE streaks = 0;
    char score = 0;

    // Calculate rows
    // Using bit operations. Will be going ROW 0, 3, 6/  1, 4, 7/  2, 5 ,8 order.
    // We are going to turn any row that is 0x1FF to 0x0000. Else would be its original value.
    // Bit Operation would be (0xFFFFFFFF + (variable == 0x1FF)) & variable.
    // That will make 0x1FF to 0x000 and other things as it were before.
    // We need to push it back to right spot, so use OR operation and left shift operations.
    // printf("%u , %u , %u\n", pixels[0], pixels[1], pixels[2]);
    for (SMALLTYPE i = 0 ; i < 3 ; i++){
        for(SMALLTYPE j = 0 ; j < 3 ; j++) {
            uint16_t curRow = (pixels[j] >> (9 * (2 - i)) & 0x1FF);
            streaks += (curRow == 0x1FF);
        }
    }

    // Calculate rows
    // Using bit operations. Will be going ROW 0 ~ 8 order.
    // Generate a column based on the results
    // Then do AND operation with 0xFFFFFFFF. Then generate a column array
    for(SMALLTYPE i = 0 ; i < 9 ; i++){
        SMALLTYPE curColSum = 0;
        for(SMALLTYPE j = 0 ; j < 3 ; j++) {
            curColSum += ((pixels[j] >> (26 - i)) & 1) + ((pixels[j] >> (17 - i)) & 1) + ((pixels[j] >> (8 - i)) & 1);
        }
        streaks += curColSum == 9; // this counts 0s as well
    }

    // Going to compare with those following values and then do a XOR operation for clearing field itself.
    // 1st sector : 0x70381C0
    // 2nd sector : 0xe07038
    // 3rd sector : 0x1c0e07
    for (SMALLTYPE i = 0 ; i < 3 ; i++){
        streaks += (pixels[i] & 0x70381C0) == 0x70381C0; // check streaks
        streaks += (pixels[i] & 0xe07038) == 0xe07038;
        streaks += (pixels[i] & 0x1c0e07) == 0x1c0e07;
    }

    score = streaks * 18 + 10 * (streaks > 1); // calculate score
    return (score & -(0 < score)); // if negative, return 0.
}

/**
 * A member function that gets a single pixel value from x, y
 * @param x the x of field
 * @param y the y of field
 * @return returns SMALLTYPE value of pixel value.
 */
SMALLTYPE Field::getPixelValue(SMALLTYPE x, SMALLTYPE y){
    switch (y) {
        case 0: case 1: case 2: // if 1st sector
            return (pixels[0] >> (9 * (2 - y) + (8 - x)) & 1);

        case 3: case 4: case 5: // if 2nd sector
            return (pixels[1] >> (9 * (5 - y) + (8 - x)) & 1);

        case 6: case 7: case 8: // if 3rd sector
            return (pixels[2] >> (9 * (8 - y) + (8 - x)) & 1);
        default:
            return 0;
    }
}

/**
 * A member function that sets pixel values into specific coordinates
 * @param x the x value
 * @param y the y value
 * @param value the value to set
 */
void Field::setPixelValue(SMALLTYPE x, SMALLTYPE y, SMALLTYPE value){
    switch (y) {
        case 0: case 1: case 2: // if 1st sector.
            pixels[0] = pixels[0] | (value << (9 * (2 - y) + (8 - x))) ;
            break;
        case 3: case 4: case 5:
            pixels[1] = pixels[1] | (value << (9 * (5 - y) + (8 - x))) ;
            break;
        case 6: case 7: case 8:
            pixels[2] = pixels[2] | (value << (9 * (8 - y) + (8 - x))) ;
            break;
    }
}

/**
 * A member function that prints out the field using color.
 * Red would be 1 represented.
 * Green would be 0 represented.
 */
void Field::printField(void){
    printf("\n===== FIELD =====\n");

    for (SMALLTYPE i = 0 ; i < 9 ; i++){
        for (SMALLTYPE j = 0 ; j < 9 ; j++){
            printf("\x1B[%dm# " , 32 - getPixelValue(j, i));
            //printf("%d ", getPixelValue(j, i));
        }
        printf("\n");
    }
    printf("\x1B[37m");
}

/**
 * A member function that returns empty spaces from the field
 * @return number of empty spaces
 */
SMALLTYPE Field::countEmptySpaces(void){
    SMALLTYPE filledSpaces = 0;
    for(SMALLTYPE i = 0 ; i < 27 ; i++){
        filledSpaces += ((pixels[0] >> i) & 1);
        filledSpaces += ((pixels[1] >> i) & 1);
        filledSpaces += ((pixels[2] >> i) & 1);
    }

    return 81 - filledSpaces;
}

/**
 * A member function that returns group numbers of empty spaces from the field.
 * using uint8_t for result. this will represent like following.
 * [][][][] / [][][][]
 *    X           Y
 * For example, if the coordinate was (1,2), it would be represented like
 * 0001 0010 = 0x12.
 * The algorithm uses DFS for finding groups.
 * @return the number of groups of empty spaces
 */
SMALLTYPE Field::getEmptySpaceGroups(void){
    std::stack <SMALLTYPE> coordStack;
    Field visitedField = Field();
    SMALLTYPE groupCnt = 0;

    for(SMALLTYPE i = 0 ; i < 9 ; i++){
        for(SMALLTYPE j = 0 ; j < 9 ; j++){
            if (!visitedField.getPixelValue(j,i)){ // if cur pos not visited
                coordStack.push((((j & 0xF) << 4) | (i & 0xF))); // push that coord.
                while (!coordStack.empty()) {
                    SMALLTYPE curCoord = coordStack.top(); // pop stack.
                    coordStack.pop();

                    SMALLTYPE x = (curCoord >> 4) & 0xF;
                    SMALLTYPE y = curCoord & 0xF;
                    // if not visited AND is in range from 0 ~ 9 and is 0 then visit.
                    // do DFS here.
                    if (((!visitedField.getPixelValue(x, y)) && (((x < 9) && (y < 9)) && ((x >= 0) && (y >= 0)))) &&
                            (getPixelValue(x, y) == 0)){
                        visitedField.setPixelValue(x, y, 1); // set visited
                        // DFS rule : up, down, left, right
                        coordStack.push((((x & 0xF) << 4) | ((y - 1) & 0xF))); // push UP
                        coordStack.push((((x & 0xF) << 4) | ((y + 1) & 0xF))); // push DOWN
                        coordStack.push((((x - 1) & 0xF) << 4) | ((y & 0xF))); // push LEFT
                        coordStack.push((((x + 1) & 0xF) << 4) | ((y & 0xF))); // push LEFT
                    }
                }
                groupCnt += (getPixelValue(j, i) != 1); // count groups.
            }
        }
    }
    return groupCnt;
}

SMALLTYPE Field::getFilledSpaceGroups(void){
    std::stack <SMALLTYPE> coordStack;
    Field visitedField = Field();
    SMALLTYPE groupCnt = 0;

    for(SMALLTYPE i = 0 ; i < 9 ; i++){
        for(SMALLTYPE j = 0 ; j < 9 ; j++){
            if (!visitedField.getPixelValue(j,i)){ // if cur pos not visited
                coordStack.push((((j & 0xF) << 4) | (i & 0xF))); // push that coord.
                while (!coordStack.empty()) {
                    SMALLTYPE curCoord = coordStack.top(); // pop stack.
                    coordStack.pop();

                    SMALLTYPE x = (curCoord >> 4) & 0xF;
                    SMALLTYPE y = curCoord & 0xF;
                    // if not visited AND is in range from 0 ~ 9 and is 0 then visit.
                    // do DFS here.
                    if (((!visitedField.getPixelValue(x, y)) && (((x < 9) && (y < 9)) && ((x >= 0) && (y >= 0)))) &&
                        (getPixelValue(x, y) == 1)){
                        visitedField.setPixelValue(x, y, 1); // set visited
                        // DFS rule : up, down, left, right
                        coordStack.push((((x & 0xF) << 4) | ((y - 1) & 0xF))); // push UP
                        coordStack.push((((x & 0xF) << 4) | ((y + 1) & 0xF))); // push DOWN
                        coordStack.push((((x - 1) & 0xF) << 4) | ((y & 0xF))); // push LEFT
                        coordStack.push((((x + 1) & 0xF) << 4) | ((y & 0xF))); // push LEFT
                    }
                }
                groupCnt += (getPixelValue(j, i) != 0); // count groups.
            }
        }
    }
    return groupCnt;
}