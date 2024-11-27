#include "card.h"

card::card(ranks r, suits s)
{
	setRank(r);
	setSuit(s);
	setValue(r);
	setColor(s);
}

card::~card()
{
}

//Getters
card::ranks card::getRank() const
{
	return rank;
}

card::suits card::getSuit() const
{
	return suit;
}

int card::getValue() const
{
	return value;
}

card::colors card::getColor() const
{
	return color;
}

//Setters
void card::setRank(ranks r)
{
	this->rank = r;
}

void card::setSuit(suits s)
{
	this->suit = s;
}

void card::setValue(ranks r)
{
	this->value = static_cast<int>(r);
}

void card::setColor(suits s)
{
	if (s == HEART || s == DIAMOND)
	{
		this->color = RED;
	}
	else
	{
		this->color = BLACK;
	}
}

//Print
std::string card::printStr()
{
	std::string print = ranksToStr[rank] + " " + suitsToStr[suit];
	return print;
}

//Overload Operators
bool card::operator==(const card& otherCard) const
{
	if (this->rank == otherCard.rank && this->suit == otherCard.suit)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::ostream& operator<<(std::ostream& out, card& cardToPrint)
{
	out << cardToPrint.printStr();
	return out;
}

//Maps
std::map<card::ranks, std::string> card::ranksToStr =
{
	{ACE, "A"},
	{TWO, "2"},
	{THREE, "3"},
	{FOUR, "4"},
	{FIVE, "5"},
	{SIX, "6"},
	{SEVEN, "7"},
	{EIGHT, "8"},
	{NINE, "9"},
	{TEN, "10"},
	{JACK, "J"},
	{QUEEN, "Q"},
	{KING, "K"}
};

std::map<card::suits, std::string> card::suitsToStr =
{
	{HEART, "H"},
	{DIAMOND, "D"},
	{CLUB, "C"},
	{SPADE, "S"}
};