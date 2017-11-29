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
    
    initializeGame(4, k, 65432, &G);
            
    unsigned int num_card_types = 27;
    
    for(int i = 0 ; i < MAX_PLAYERS ; i++){
        G.whoseTurn = i;
        G.handCount[i] = MAX_HAND;
        
        for(int j = 0 ; j < MAX_HAND ; j++){
            for(int k = 0 ; k < num_card_types ; k++){
                G.hand[i][j] = k;
                testing_assert(handCard(j, &G) == k);
            }
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

