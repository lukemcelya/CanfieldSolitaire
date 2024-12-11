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
	this->pileList.push_back(cardToAdd);
	this->cardCount++;
}

void pile::removeCard(card* cardToRemove)
{
	for (card* c : pileList)
	{
		if (*c == *cardToRemove)
		{
			pileList.erase(std::find(pileList.begin(), pileList.end() - 1, c));
			this->cardCount--;
		}
	}
}

void pile::removeTopCard()
{
	pileList.pop_back();
	this->cardCount--;
}

card* pile::getTopCard()
{
	if (cardCount > 0)
	{
		return pileList.back();
	}
	return nullptr;
}

int pile::getCardCount() const
{
	return cardCount;
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

int foundation::foundationPileCount = 0;
card::ranks foundation::startingRank = card::ACE;

foundation::foundation(deck& deckList) : pile()
{
	if (foundationPileCount == 0)
	{
		this->startingRank = deckList.getTopCard()->getRank();
	}
	pileFinish = false;
	this->addCard(deckList.getTopCard());
	deckList.removeTopCard();
	this->pileSuit = deckList.getTopCard()->getSuit();
	++foundationPileCount;
}

foundation::foundation() : pile()
{
	pileFinish = false;
	++foundationPileCount;
}

foundation::~foundation()
{
	
}

std::string foundation::printPiles(foundation* foundationPile[])
{
	std::ostringstream output;

	//The output for these piles depends on how many piles there are, since console output is printed line by line,
	//so this is a switch statement that prints the first row of cards depending on how many foundation piles are currently active
	//denoted by the static int placed in the class
	std::string topCardStr[4];
	for (int i = 0; i < 4; ++i)
	{
		if (foundationPile[i]->cardCount > 0)
		{
			topCardStr[i] = "│" + foundationPile[i]->getTopCard()->printStr() + "░░░░░│  ";
		}
		else
		{
			topCardStr[i] = "│░░░░░░░│  ";
		}
	}

	output << "             " << "┌───────┐" << "  ┌───────┐" << "  ┌───────┐" << "  ┌───────┐" << std::endl;
	output << "             " << topCardStr[0] << topCardStr[1] << topCardStr[2] << topCardStr[3]  << std::endl;
	output << "             " << "│░░░░░░░│" << "  │░░░░░░░│" << "  │░░░░░░░│" << "  │░░░░░░░│" << std::endl;
	output << "             " << "│░░░░░░░│" << "  │░░░░░░░│" << "  │░░░░░░░│" << "  │░░░░░░░│" << std::endl;
	output << "             " << "│░░░░░░░│" << "  │░░░░░░░│" << "  │░░░░░░░│" << "  │░░░░░░░│" << std::endl;
	output << "             " << "│░░░░░░░│" << "  │░░░░░░░│" << "  │░░░░░░░│" << "  │░░░░░░░│" << std::endl;
	output << "             " << "└───────┘" << "  └───────┘" << "  └───────┘" << "  └───────┘" << std::endl;
	output << "                 1          2          3          4     " << std::endl; 

	return output.str();
}

int foundation::getPileCount()
{
	return foundationPileCount;
}

bool foundation::validateMove(card* cardToCheck)
{
	//If there are no cards in this foundation pile and the card being checked
	//has an equal rank to the starting rank of all foundation piles
	//then return true
	if (this->cardCount == 0)
	{
		if (cardToCheck->getRank() == this->startingRank)
		{
			this->pileSuit = cardToCheck->getSuit();
			return true;
		}
		else
		{
			return false;
		}
	}

	//If this pile has already been started, check that the suit of the card being checked
	//matches this pile, and if it does check that it's rank is one greater than the current top
	//card on the pile, otherwise return false
	else if (this->cardCount > 0)
	{
		if (cardToCheck->getSuit() == this->pileSuit)
		{
			card::ranks rankPlusOne = static_cast<card::ranks>(static_cast<int>(this->getTopCard()->getRank()) + 1);
			if (cardToCheck->getRank() == rankPlusOne)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool foundation::checkClear()
{
	if (this->cardCount == 13)
	{
		this->pileFinish = true;
		return true;
	}
	else
	{
		return false;
	}
}

tableau::tableau(deck& deckList)
{
	pileFinish = false;
	cardCount = 0;
	this->addCard(deckList.getTopCard());
	deckList.removeTopCard();
}

tableau::~tableau()
{

}

bool tableau::validateMove(card* cardToCheck)
{
	//Set what the current color and rank are
	card::colors currentColor = this->getTopCard()->getColor();
	card::ranks currentRank = this->getTopCard()->getRank();

	//If cards are the same color return false
	if (cardToCheck->getColor() == currentColor)
	{
		return false;
	}
	else
	{
		//Otherwise, set what one rank lower would be and compare cards
		card::ranks rankMinusOne;
		if (currentRank == card::ACE)
		{
			rankMinusOne = card::KING;
		}
		else
		{
			rankMinusOne = static_cast<card::ranks>(static_cast<int>(currentRank) - 1);
		}

		//If this card is equal to what the next lowest rank is return true
		if (cardToCheck->getRank() == rankMinusOne)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

card* tableau::getCard(int count)
{
	return pileList[count];
}

int tableau::getCardIndex(card* cardToFind)
{
	for (int i = 0; i < this->getCardCount(); ++i)
	{
		if (*pileList[i] == *cardToFind)
		{
			return i;
		}
	}
	return -1;
}

std::string tableau::printCards(int count)
{
	std::string output;
	output += card::ranksToStr[pileList[count]->getRank()] + card::suitsToStr[pileList[count]->getSuit()];
	return output;
}

std::string tableau::printPiles(tableau* tableauPile[])
{
	//Clear all of the strings
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 13; ++j)
		{
			tableauPile[i]->pileStrings[j].str("");
		}
	}

	//Get size of largest vector
	size_t maxSize = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (tableauPile[i]->pileList.size() > maxSize)
		{
			maxSize = tableauPile[i]->pileList.size();
		}
	}
	//Seven lines for the bottom card, two for all of the rest (piles with less cards need to have the same number
	//of lines, just blank)
	int maxLineCount = (7 + ((maxSize - 1) * 2));
	
	//Loop to set up strings for each tableau pile
	for (int i = 0; i < 4; ++i)
	{
		size_t cardCount = 0;
		size_t pileSize = tableauPile[i]->pileList.size();
		for (int j = 0; j < maxLineCount; ++j)
		{
			//If it's the first card of the pile, or the start of a new card, add the top portion to the string
			if (j == 0 || (j % 2 == 0 && cardCount < pileSize))
			{
				tableauPile[i]->pileStrings[j] << "┌───────┐";
			}
			//If it's the second line of the card being drawn add the card rank and suit
			else if ((j % 2 != 0) && (cardCount < pileSize))
			{
				tableauPile[i]->pileStrings[j] << "│" << tableauPile[i]->pileList[cardCount]->printStr() << "░░░░░│";
				cardCount++;
			}
			//Add the bottom portion of the last card
			else if (cardCount++ == pileSize)
			{
				tableauPile[i]->pileStrings[j++] << "│░░░░░░░│";
				tableauPile[i]->pileStrings[j++] << "│░░░░░░░│";
				tableauPile[i]->pileStrings[j++] << "│░░░░░░░│";
				tableauPile[i]->pileStrings[j++] << "│░░░░░░░│";
				tableauPile[i]->pileStrings[j] << "└───────┘";
			}
			//Add blank space if other piles are longer
			else if (cardCount > pileSize)
			{
				tableauPile[i]->pileStrings[j] << "         ";
			}
		}
	}

	//Return string output
	std::ostringstream output;
	for (int i = 0; i < maxLineCount; ++i)
	{
		if (i == 0)
		{
			output << "Tableaus:    ";
		}
		else
		{
			output << "             ";
		}
		output << tableauPile[0]->pileStrings[i].str() << "  " << tableauPile[1]->pileStrings[i].str() << "  " << tableauPile[2]->pileStrings[i].str() << "  " << tableauPile[3]->pileStrings[i].str() << std::endl;
	}

	return output.str();
}

reserve::reserve(deck& deckList)
{
	for (int i = 0; i < 13; ++i)
	{
		this->addCard(deckList.getTopCard());
		deckList.removeTopCard();
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

std::string reserve::printPile()
{
	std::ostringstream output;
	output << "Reserve: " << std::endl
		<< "┌───────┐" << std::endl
		<< "│" << this->getTopCard()->printStr() << "░░░░░│" << std::endl
		<< "│░░░░░░░│" << std::endl
		<< "│░░░░░░░│" << std::endl
		<< "│░░░░░░░│" << std::endl
		<< "│░░░░░░░│" << std::endl
		<< "└───────┘" << std::endl;
	return output.str();
}  

bool reserve::validateMove(card*)
{
	std::cerr << "Cannot move to reserve pile\n";
	return false;
}