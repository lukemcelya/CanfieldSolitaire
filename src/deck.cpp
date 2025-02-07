#include "deck.h"

//Constructors
deck::deck()
{
	//Create a deck of all 52 cards
	deckList.reserve(52);
	for (int i = 1; i <= card::KING; ++i)
	{
		for (int j = 0; j <= card::SPADE; ++j)
		{
			card* newCard = new card(static_cast<card::ranks>(i), static_cast<card::suits>(j));
			addCard(newCard);
		}
	}
	deckSize = deckList.size();
	shuffle();
}

deck::deck(int size)
{
	deckSize = size;
}

deck::~deck()
{
	for (card* c : deckList)
	{
		delete c;
	}
	deckList.clear();
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
	this->deckSize++;
}

void deck::removeTopCard()
{
	deckList.pop_back();
	this->deckSize--;
}

std::vector<card*> deck::getDeckList() const
{
	return this->deckList;
}

card* deck::getTopCard() const
{
	if (deckList.empty())
	{
		return nullptr;
	}
	return this->deckList.back();
}

int deck::getDeckSize() const
{
	return this->deckSize;
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


