#ifndef DECK_H
#define DECK_H

#include <vector>
#include <algorithm>
#include <random>
#include <sstream>
#include "card.h"

class deck
{
public:
	deck();
	deck(int);
	~deck();

	// Class functions
	void shuffle();
	void addCard(card *cardToAdd);
	void removeTopCard();
	std::string printPile();

	// Getters
	std::vector<card *> getDeckList() const;
	card *getTopCard() const;
	size_t getDeckSize() const;

private:
	std::vector<card *> deckList;
	size_t deckSize;
};

#endif