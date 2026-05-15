/*
 * Name: Austin Nguyen Caroline Tapia
 * Email: anguyen10@scu.edu ctapia@scu.edu
 * Description: Poker Hands is a function that prints out 1 of each kind of hand in poker,
 * (Pair to Royal Flush), I added a print stateemnt to tell me the runtime of the function
 * because we were running into issues with how long our function was running for
 * 
 * Poker Stats is a function that checks for stability within the trials through our 
 * threshold variable and prints out the statistics of the probability of each hand in poker
*/

#include <ctime>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <cmath>
#include "card.h"
#include "deck.h"
#include "poker.h"

using namespace std;
using namespace lab2_anguyen10;

// Generate one sample hand for each rank
void pokerHands(Poker &poker) {
    
    time_t runtime = clock();
    double elapsedTime = static_cast<double>(runtime) / CLOCKS_PER_SEC;
    bool foundRanks[Poker::POKER_ARRAY_SIZE] = {false}; // Track found ranks
    int rankCount = 0; // Count of unique ranks found

    while (rankCount < Poker::POKER_ARRAY_SIZE) {
        poker.dealHand(); // Deal a new hand
        Poker::Rank rank = poker.rankHand(); // Sends hand to function to get ranked

        if (!foundRanks[rank]) { 
            foundRanks[rank] = true; // If rank has not been found yet mark as seen
            rankCount++; 
            cout << poker << " " << Poker::RankNames[rank] << endl; 
        }
    }
    runtime = clock() - runtime; // Stop the clock
    cout << elapsedTime << endl;

}



void pokerStats(Poker &poker){

    constexpr double threshold(0.000005);
    constexpr int handCount(5000);
    unsigned long rankCount[Poker::POKER_ARRAY_SIZE]{0};
    double rankPercent[2][Poker::POKER_ARRAY_SIZE]{0};
    int index(0);
    bool stability(false);
    int cycleCount = 0;

    time_t runtime = clock();
    unsigned long totalHands(0UL);
    int cycle(handCount);
    while(!stability){
        int cycle(handCount);
        while(cycle-->0){
            poker.dealHand();
            Poker::Rank r(poker.rankHand());
            rankCount[r]++;
        }
        totalHands+=handCount;

        for(auto j = 0; j < 9; j++)
            rankPercent[index][j] = (double) rankCount[j] / (double) totalHands; 
        stability = true;
        for (auto j = 0; j< 9 && stability; j++)
            stability = fabs(rankPercent[index][j] - rankPercent[1-index][j]) <= threshold;
        index = 1-index;
        cycleCount++;
    
    runtime = clock() - runtime; // Stop the clock

    
        }

    double time = static_cast<double>(runtime) / CLOCKS_PER_SEC;
    cout << fixed << setprecision(2);  // Print percentages with 2 decimal places
    cout << "Dealt " << totalHands << " hands. Elapsed Time: " << time << " seconds." << endl;
    cout << "Average " << time / cycleCount << " seconds per " << handCount << " hands" << endl;

    for (auto j = 0; j < 9; ++j) {
        double percentage = rankPercent[index][j] * 100;  // Convert to percentage
        cout << Poker::RankNames[j] << ": " << rankCount[j] << " (" << percentage << "%)" << endl; 
    }
}


int main(void) {
    Poker poker;
    cout << "Sample hand for each Rank:" << endl;
    pokerHands(poker);
    cout << endl << "Statistics:" << endl;
    pokerStats(poker);
    return EXIT_SUCCESS;
}