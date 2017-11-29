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

    initializeGame(2, k, 65432, &G);

    //If province cards are empty or are least three supply piles are, game end
    //else game is not over. 1 is game end, 0 is game continues

    unsigned int victory_cards_max = 12;
    unsigned int num_supply_piles = treasure_map+1;
    unsigned int max_count_for_any_supply = 60;

    int i;
    for (i = 0; i < victory_cards_max; i++) {
        int num_zeroed = 0;

        for (int k = 0; k < num_supply_piles; k++) {
            G.supplyCount[k] = max_count_for_any_supply;
        }

        for (; num_zeroed < num_supply_piles; num_zeroed++) {
            for (int j = 0; j < num_zeroed; j++) {
                G.supplyCount[j] = 0;
            }
            
            G.supplyCount[province] = i;
            
            if ((G.supplyCount[province] == 0) || (num_zeroed >= 3)) {
                testing_assert(isGameOver(&G) == 1);
            } else {
                testing_assert(isGameOver(&G) == 0);
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
