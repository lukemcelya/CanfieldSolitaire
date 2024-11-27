#ifndef DECK_H
#define DECK_H

#include <vector>
#include <algorithm>
#include <random>
#include "card.h"

class deck
{
public:
	deck();
	~deck();
	void shuffle();
	void addCard(card* cardToAdd);
	void removeCard(card* cardToRemove);
	std::vector<card*> getDeckList() const;
	card* getTopCard() const;
	std::string toString() const;
private:
	std::vector <card*> deckList;
	int deckSize;
};

#endif