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

    
    
    unsigned int arbitrary_hand_count_max = MAX_HAND - 1;
    
    for(int i = 0 ; i < 4 ; i++){
        initializeGame(4, k, 65432, &G);
        G.deckCount[i] = MAX_DECK;
        G.whoseTurn = i;
        
        for(int j = 0 ; j < arbitrary_hand_count_max ; j++){
            int hand_count = G.handCount[i];
            int num_actions = G.numActions;
            
            playGreat_Hall(&G, j);
            testing_assert(G.handCount[i] == hand_count);
            testing_assert(G.numActions == num_actions + 1);
            
            for(int j = 0 ; j < G.handCount[i] ; j++){
                testing_assert((G.hand[i][j] >= curse) && (G.hand[i][j] <= treasure_map));
            }
        }
        
    }
    
//    printf("Hand: %d\n", G.handCount[0]);
//    drawCard(0, &G);
//    printf("Hand: %d\n", G.handCount[0]);
//    G.numActions++;
//    discardCard(0, 0, &G, 0);
//    printf("Hand: %d\n\n", G.handCount[0]);
//    
//    printf("Hand: %d\n", G.handCount[0]);
//    playGreat_Hall(&G, 0);
//    printf("Hand: %d\n\n", G.handCount[0]);
    
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

