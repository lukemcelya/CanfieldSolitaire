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
	void shuffle();
	void addCard(card* cardToAdd);
	void removeCard(card* cardToRemove);
	void removeTopCard();
	std::vector<card*> getDeckList() const;
	card* getTopCard() const;
	std::string toString() const;
	int getDeckSize() const;
	std::string printPile();
private:
	std::vector <card*> deckList;
	int deckSize;
};

#endif