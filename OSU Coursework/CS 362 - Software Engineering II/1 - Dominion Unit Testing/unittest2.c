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
    
    unsigned int min_players = 2;
    unsigned int max_players = MAX_PLAYERS;
    unsigned int max_hand = MAX_HAND;
    unsigned int arbitrary_bonus_max = 1000;
    unsigned int copper_worth = 1;
    unsigned int silver_worth = 2;
    unsigned int gold_worth = 3;
    
    
    for(int i = min_players ; i <= max_players ; i++){
        initializeGame(i, k, 65432, &G);
        
        for(int j = 0 ; j < i ; j++){
            G.handCount[j] = max_hand;
            
            for(int k = 0 ; k < max_hand ; k++){
                G.hand[j][k] = copper;
            }
            
            updateCoins(j, &G, 0);
            testing_assert(G.coins == (max_hand*copper_worth));
        
            updateCoins(j, &G, arbitrary_bonus_max);
            testing_assert(G.coins == ((max_hand*copper_worth) + arbitrary_bonus_max));
                    
            
            for(int k = 0 ; k < max_hand ; k++){
                G.hand[j][k] = silver;
            }
            
            updateCoins(j, &G, 0);
            testing_assert(G.coins == (max_hand*silver_worth));
        
            updateCoins(j, &G, arbitrary_bonus_max);
            testing_assert(G.coins == ((max_hand*silver_worth) + arbitrary_bonus_max));
            
            for(int k = 0 ; k < max_hand ; k++){
                G.hand[j][k] = gold;
            }
            
            updateCoins(j, &G, 0);
            testing_assert(G.coins == (max_hand*gold_worth));
        
            updateCoins(j, &G, arbitrary_bonus_max);
            testing_assert(G.coins == ((max_hand*gold_worth) + arbitrary_bonus_max));
            
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
