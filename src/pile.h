#ifndef PILE_H
#define PILE_H
#include "card.h"
#include "deck.h"
#include <vector>
#include <sstream>

class pile
{
public:
	pile();
	~pile();
	void addCard(card* cardToAdd);
	void removeCard(card* cardToRemove);
	void removeTopCard();
	virtual bool validateMove(card*) = 0;
	card* getTopCard();
	int getCardCount() const;
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
	foundation();
	~foundation();
	static std::string printPiles(foundation* foundationPile[]);
	static int getPileCount();
	bool validateMove(card*);
	bool checkClear();

private:
	bool pileFinish;
	card::suits pileSuit;
	static card::ranks startingRank;
	static int foundationPileCount;

};

class tableau : public pile
{
public:
	tableau(deck& deckList);
	~tableau();
	bool validateMove(card*);
	card* getCard(int count);
	int getCardIndex(card* cardToFind);
	std::string printCards(int count);
	static std::string printPiles(tableau* tableauPile[]);

private:
	bool pileFinish;
	std::ostringstream pileStrings[31];
};

class reserve : public pile
{
public:
	reserve(deck& deckList);
	~reserve();
	bool validateMove(card*);
	std::string printTopCard();
	std::string printPile();
};

#endif
