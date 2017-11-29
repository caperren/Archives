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

int testing_assert(int expression);

int main(int argc, char** argv) {
    struct gameState G;
    int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
        sea_hag, tribute, smithy};

    unsigned int arbitrary_hand_count_max = MAX_HAND - 2;
    
    for(int l = 0 ; l < 4 ; l++){
        for(int i = 1 ; i < arbitrary_hand_count_max ; i++){
            initializeGame(4, k, 65432, &G);
            G.handCount[l] = arbitrary_hand_count_max;
            G.whoseTurn = l;
            
            playAdventurer(&G);
            
            testing_assert(G.handCount[l] == arbitrary_hand_count_max + 2);
            testing_assert((G.hand[l][G.handCount[l] - 1] >= copper) && (G.hand[l][G.handCount[l] - 1] <= gold));
            testing_assert((G.hand[l][G.handCount[l] - 2] >= copper) && (G.hand[l][G.handCount[l] - 2] <= gold));
            
            initializeGame(4, k, 65432, &G);
            G.handCount[l] = arbitrary_hand_count_max;
            G.whoseTurn = l;
            G.deckCount[l] = 0;
            
            for(int m = 0 ; m < MAX_DECK ; m += 3){
                G.discard[l][m] = copper;
                G.discard[l][m + 1] = silver;
                G.discard[l][m + 2] = gold;
                G.discardCount[l] += 3;
            }
            
            
            playAdventurer(&G);

            testing_assert(G.handCount[l] == arbitrary_hand_count_max + 2);
            testing_assert((G.hand[l][G.handCount[l] - 1] >= copper) && (G.hand[l][G.handCount[l] - 1] <= gold));
            testing_assert((G.hand[l][G.handCount[l] - 2] >= copper) && (G.hand[l][G.handCount[l] - 2] <= gold));
            
        }
    }
    
    
    
    return (EXIT_SUCCESS);
}

int testing_assert(int expression) {
    if (expression) {
        printf("TEST SUCCEEDED!\n");
        return 1;
    } else {
        printf("TEST FAILED!\n");
        return 0;
    }
}

