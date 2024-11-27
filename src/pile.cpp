#include "pile.h"

pile::pile()
{
	pileList.reserve(13);
	cardCount = 0;
}

pile::~pile()
{
	for (card* c : pileList)
	{
		delete c;
	}
	this->pileList.clear();
}

void pile::addCard(card* cardToAdd)
{
	this->pileList.emplace_back(cardToAdd);
	this->cardCount++;
}

void pile::removeCard(card* cardToRemove)
{
	for (card* c : pileList)
	{
		if (*c == *cardToRemove)
		{
			pileList.erase(std::remove(pileList.begin(), pileList.end() - 1, c));
			this->cardCount--;
		}
	}
}

card* pile::getTopCard()
{
	return pileList.back();
}

int pile::getCardCount() const
{
	return cardCount;
}

bool pile::validateMove()
{
	return false;
}

card::ranks pile::getCardRank()
{
	return this->getTopCard()->getRank();
}

card::suits pile::getCardSuit()
{
	return this->getTopCard()->getSuit();
}

std::vector<card*> pile::getPileList() const
{
	return pileList;
}

std::string pile::toString() const
{
	std::string output;
	for (card* c : this->pileList)
	{
		output += card::ranksToStr[c->getRank()] + card::suitsToStr[c->getSuit()] + "\n";
	}
	return output;
}

foundation::foundation(deck& deckList)
{
	pileFinish = false;
	this->addCard(deckList.getTopCard());
	deckList.removeCard(deckList.getTopCard());
}

foundation::foundation(pile& pileList)
{
	pileFinish = false;
	this->addCard(pileList.getTopCard());
	pileList.removeCard(pileList.getTopCard());
}

foundation::~foundation()
{
	
}

tableau::tableau(deck& deckList)
{
	pileFinish = false;
	cardCount = 0;
	this->addCard(deckList.getTopCard());
	deckList.removeCard(deckList.getTopCard());
}

tableau::~tableau()
{

}

card* tableau::getCard(int count)
{
	return pileList[count];
}

std::string tableau::printCards(int count)
{
	std::string output;
	output += card::ranksToStr[pileList[count]->getRank()] + card::suitsToStr[pileList[count]->getSuit()];
	return output;
}

reserve::reserve(deck& deckList)
{
	for (int i = 0; i < 13; ++i)
	{
		this->addCard(deckList.getTopCard());
		deckList.removeCard(deckList.getTopCard());
	}
}

reserve::~reserve()
{
	for (card* c : pileList)
	{
		delete c;
	}
	pileList.clear();
}

std::string reserve::printTopCard()
{
	std::string output;
	output += card::ranksToStr[pileList.back()->getRank()] + card::suitsToStr[pileList.back()->getSuit()];
	return output;
}