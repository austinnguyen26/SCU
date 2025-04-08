/*
 * Name: Austin Nguyen Caroline Tapia
 * Email: anguyen10@scu.edu ctapia@scu.edu
 * Shuffle shuffles the deck randomly
 * Deal deals cards 
 */
#include <iostream>
#include "card.h"
#include "deck.h"
#include <cstdlib>
#include <random>
#include <iomanip>
#include <iostream>
#include <time.h>
using namespace std;

namespace lab2_anguyen10 {
	// implement Fisher-Yates here
	void Deck::shuffle(void) {
		std::random_device rd;			//Fisher Yates
		std::mt19937 gen(rd());			//Fisher Yates

		//starts at last card in deck and iterates backwards
		for (int i = CARDS_PER_DECK -1; i > 0; --i){
			std::uniform_int_distribution<int> distribution (0, i);	//generates random index from 0 to i
			int j = distribution(gen);			//assigns j to random index

			//swaps i and j
			Card temp = cards[i];
			cards[i] = cards[j];
			cards[j] = temp;
		}
		next = 0;		//resets the index
	}
	//bool function to check if deck is empty
	bool Deck::checkEmpty() const{
		return next >= Deck::CARDS_PER_DECK;
	}

	// deal out one card
	const Card &Deck::deal() {
		// shuffle();
		// if (checkEmpty()){
		// 	throw std::out_of_range("Deck is Empty.");
		// }

		//recalls shuffle function if not close to end of the deck
		if(CARDS_PER_DECK - next <= guard){
			shuffle();
		}

		const Card& dealtcard = cards[next++];
		return dealtcard;;	// replace this line with your implementation of the function.
	}
}

