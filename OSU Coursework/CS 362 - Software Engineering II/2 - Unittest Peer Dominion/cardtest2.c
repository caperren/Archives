/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   unittest1.c
 * Author: corwinperren
 *
 * Created on February 1, 2017, 9:02 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

int testing_assert(int expression, int should_print);

long sucesses = 0;
long failures = 0;

int main(int argc, char** argv) {
    struct gameState G, G_copy;
    int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
        sea_hag, tribute, smithy};

    
    unsigned int arbitrary_hand_count_max = MAX_HAND - 3;
    
    for(int l = 0 ; l < 4 ; l++){
        for(int i = 1 ; i < arbitrary_hand_count_max ; i++){
            initializeGame(4, k, 65432, &G);
            G.handCount[0] = arbitrary_hand_count_max;
            G.whoseTurn = l;

            G_copy = G;

            drawCard(0, &G_copy);
            drawCard(0, &G_copy);
            drawCard(0, &G_copy);
            discardCard(i, l, &G_copy, 0);

            playSmithy(&G, i, G.whoseTurn);

            testing_assert(G.handCount[l] == G_copy.handCount[l], 0);

            for(int j = 0 ; j < arbitrary_hand_count_max ; j++){
                testing_assert((G.hand[l][j] >= curse) && (G.hand[l][j] <= treasure_map), 0);
                testing_assert((G_copy.hand[l][j] >= curse) && (G_copy.hand[l][j] <= treasure_map), 0);
            }

        }
    }
    
    printf("Run complete!\n");
    printf("SUCCESSES: %ld\n", sucesses);
    
    if(failures > 0){
        printf("Some tests failed!!!\n");
        printf("FAILURES: %ld\n", failures);
    }
    
    return (EXIT_SUCCESS);
}

int testing_assert(int expression, int should_print) {
    if (expression) {
        if(should_print){
            printf("TEST SUCCEEDED!\n");
        }
        sucesses++;
        return 1;
    } else {
        if(should_print){
            printf("TEST FAILED!\n");
        }
        failures++;
        return 0;
    }
}


