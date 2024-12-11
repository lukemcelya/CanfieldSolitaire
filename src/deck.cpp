#include "deck.h"

deck::deck()
{
	//Create a deck of all 52 cards
	deckSize = 52;
	deckList.reserve(deckSize);
	for (int i = 1; i <= card::KING; ++i)
	{
		for (int j = 0; j <= card::SPADE; ++j)
		{
			card* newCard = new card(static_cast<card::ranks>(i), static_cast<card::suits>(j));
			addCard(newCard);
		}
	}

	shuffle();
}

deck::deck(int size)
{
	deckSize = size;
}

void deck::shuffle()
{
	//Shuffle deck
	std::random_device rd;
	unsigned seed = rd();
	std::default_random_engine rng;
	rng.seed(seed);
	std::shuffle(std::begin(deckList), std::end(deckList), rng);
}

void deck::addCard(card* cardToAdd)
{
	deckList.push_back(cardToAdd);
}

void deck::removeCard(card* cardToRemove)
{
	for (card* c : deckList)
	{
		if (*c == *cardToRemove)
		{
			deckList.erase(std::find(deckList.begin(), deckList.end() - 1, c));
			deckSize--;
		}
	}
}

void deck::removeTopCard()
{
	deckList.pop_back();
	deckSize--;
}

std::vector<card*> deck::getDeckList() const
{
	return deckList;
}

card* deck::getTopCard() const
{
	if (deckList.empty())
	{
		return nullptr;
	}
	return deckList.back();
}

int deck::getDeckSize() const
{
	return deckSize;
}

std::string deck::toString() const
{
	std::string output;
	for (card* c : deckList)
	{
		output += card::ranksToStr[c->getRank()] + card::suitsToStr[c->getSuit()] + "\n";
	}
	return output;
}

std::string deck::printPile()
{
	std::ostringstream output;
	output << "Stock:   " << std::endl
		<< "┌───────┐" << std::endl
		<< "│" << this->getTopCard()->printStr() << "░░░░░│" << std::endl
		<< "│░░░░░░░│" << std::endl
		<< "│░░░░░░░│" << std::endl
		<< "│░░░░░░░│" << std::endl
		<< "│░░░░░░░│" << std::endl
		<< "└───────┘" << std::endl;
	return output.str();
}

deck::~deck()
{
	for (card* c : deckList)
	{
		delete c;
	}
	deckList.clear();
}

