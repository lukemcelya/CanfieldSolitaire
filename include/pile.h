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

	//Class functions
	void addCard(card* cardToAdd);
	void removeCard(card* cardToRemove);
	void removeTopCard();
	virtual bool validateMove(card*) = 0;

	//Getters
	card* getTopCard();
	int getCardCount() const;
	card::ranks getCardRank();
	card::suits getCardSuit();
	std::vector<card*> getPileList() const;
protected:
	std::vector<card*> pileList;
	int cardCount;

};

//Foundation class
class foundation : public pile
{
public:
	foundation(deck& deckList);
	foundation();
	~foundation();

	//Class functions
	static std::string printPiles(foundation* foundationPile[]);
	bool validateMove(card*);
	bool checkClear();

	//Getters
	static int getPileCount();

private:
	bool pileFinish;
	card::suits pileSuit;
	static card::ranks startingRank;
	static int foundationPileCount;

};

//Tableau class
class tableau : public pile
{
public:
	tableau(deck& deckList);
	~tableau();

	//Class functions
	bool validateMove(card*);
	std::string printCards(int count);
	static std::string printPiles(tableau* tableauPile[]);

	//Getters
	card* getCard(int count);
	int getCardIndex(card* cardToFind);

private:
	bool pileFinish;
	std::ostringstream pileStrings[31];
};

//Reserve class
class reserve : public pile
{
public:
	reserve(deck& deckList);
	~reserve();

	//Class functions
	bool validateMove(card*);
	std::string printTopCard();
	std::string printPile();
};

#endif
