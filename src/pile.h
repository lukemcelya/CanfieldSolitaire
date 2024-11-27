#ifndef PILE_H
#define PILE_H
#include "card.h"
#include "deck.h"
#include <vector>

class pile
{
public:
	pile();
	~pile();
	void addCard(card* cardToAdd);
	void removeCard(card* cardToRemove);
	card* getTopCard();
	int getCardCount() const;
	bool validateMove();
	card::ranks getCardRank();
	card::suits getCardSuit();
	std::vector<card*> getPileList() const;
	std::string toString() const;
protected:
	std::vector<card*> pileList;
	int cardCount;

};

class foundation : public pile
{
public:
	foundation(deck& deckList);
	foundation(pile& pileList);
	~foundation();
	bool validateMove();
	bool checkClear();

private:
	bool pileFinish;
	static card::ranks startingRank;

};

class tableau : public pile
{
public:
	tableau(deck& deckList);
	~tableau();
	bool validateMove();
	card* getCard(int count);
	std::string printCards(int count);

private:
	bool pileFinish;
};

class reserve : public pile
{
public:
	reserve(deck& deckList);
	~reserve();
	std::string printTopCard();
	void shuffle();
	void flipDeck();
};

#endif
