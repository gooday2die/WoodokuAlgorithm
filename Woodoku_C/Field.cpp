//
// By Gooday2die @ https://github.com/gooday2die
//

#include "Field.h"



SMALLTYPE Field::calculateScore(void){
    /**
     * A member function that calculates the current score on the field and then clears out the pixels.
     * This member function uses bit operations instead of typical ways to figure out the scores and clear them out.
     * This member function will be called 'A LOT' of times while our DFS algorithm. So I designed this function to
     * take as small amount of time as possible. That being said, I used as little if, conditions as possible and used
     * Bitwise operations instead.
     */
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
            //printf("ROW %d %d : %u \n", i , j, ((0xFFFFFFFF + (curRow == 0x1FF)) & curRow) << (9 * 2 - i));
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

        //blk_array[i] = blk_array[i] & (pixels[i] ^ 0x70381C0); // XOR FUCKS it up.
        //blk_array[i] = blk_array[i] & (pixels[i] ^ 0xe07038);
        //blk_array[i] = blk_array[i] & (pixels[i] ^ 0x1c0e07);
        if ((pixels[i] & 0x70381C0) == 0x70381C0) blk_array[i] &= (pixels[i] ^ 0x70381C0);
        if ((pixels[i] & 0xe07038) == 0xe07038) blk_array[i] &= (pixels[i] ^ 0xe07038);
        if ((pixels[i] & 0x1c0e07) == 0x1c0e07) blk_array[i] &= (pixels[i] ^ 0x1c0e07);

        //blk_array[i] = blk_array[i] & (~(pixels[i] & 0x70381C0));  // NOT WORKING . FUCKED UP HERE.
        //blk_array[i] = blk_array[i] & (~(pixels[i] & 0xe07038));
        //blk_array[i] = blk_array[i] & (~(pixels[i] & 0x1c0e07));

    }
    for(SMALLTYPE i = 0 ; i < 3 ; i++)

    // Do AND operations with all arrays will create a cleared field. Then save it to pixels.
    for(SMALLTYPE i = 0 ; i < 3 ; i++){
        //printf("RA : %u / CA : %u / BLK %u\n", row_array[i], col_array[i], blk_array[i]);
        pixels[i] = ((row_array[i] & col_array[i]) & blk_array[i]);
    }
    //printf("Streak : %d\n", streaks);
    score = streaks * 18 + 10 * (streaks > 1); // calculate score
    return (score & -(0 < score)); // if negative, return 0.
}

SMALLTYPE Field::peekScore(void){
    /**
 * A member function that calculates the current score on the field. This just peeks the score.
 * This member function uses bit operations instead of typical ways to figure out the scores and clear them out.
 * This member function will be called 'A LOT' of times while our DFS algorithm. So I designed this function to
 * take as small amount of time as possible. That being said, I used as little if, conditions as possible and used
 * Bitwise operations instead.
 */
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

    //printf("RA : %u \n", row_array[0]);
    // printf("RA : %u \n", row_array[1]);
    //printf("RA : %u \n", row_array[2]);


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

SMALLTYPE Field::getPixelValue(SMALLTYPE x, SMALLTYPE y){
    switch (y) {
        case 0: case 1: case 2: // if 1st sector
            return (pixels[0] >> (9 * (2 - y) + (8 - x)) & 1);

        case 3: case 4: case 5:
            return (pixels[1] >> (9 * (5 - y) + (8 - x)) & 1);

        case 6: case 7: case 8:
            return (pixels[2] >> (9 * (8 - y) + (8 - x)) & 1);
        default:
            return 0;
    }
}

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

void Field::printField(void){
    printf("\n===== FIELD =====\n");
    //printf("%u\n", pixels[0]);
    //printf("%u\n", pixels[1]);
    //printf("%u\n", pixels[2]);

    for (SMALLTYPE i = 0 ; i < 9 ; i++){
        for (SMALLTYPE j = 0 ; j < 9 ; j++){
            printf("\x1B[%dm# " , 32 - getPixelValue(j, i));
            //printf("%d ", getPixelValue(j, i));
        }
        printf("\n");
    }
    printf("\x1B[37m");
}