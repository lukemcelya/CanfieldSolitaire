#ifndef CARD_H
#define CARD_H
#include <map>
#include <string>
#include <iostream>

class card
{
public:
	enum ranks
	{
		ACE = 1,
		TWO,
		THREE,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NINE,
		TEN,
		JACK,
		QUEEN,
		KING
	};

	enum suits
	{
		HEART,
		DIAMOND,
		CLUB,
		SPADE
	};

	enum colors
	{
		RED,
		BLACK
	};

	card(ranks r, suits s);
	~card();

	//Getters
	ranks getRank() const;
	suits getSuit() const;
	int getValue() const;
	colors getColor() const;

	//Setters
	void setRank(ranks r);
	void setSuit(suits s);
	void setValue(ranks r);
	void setColor(suits s);

	//Print
	std::string printStr();

	//Overload Operators
	bool operator==(const card& otherCard) const;
	friend std::ostream& operator<<(std::ostream& out, card& cardToPrint);

	//Maps
	static std::map<ranks, std::string> ranksToStr;
	static std::map<suits, std::string> suitsToStr;


private:
	ranks rank;
	suits suit;
	int value;
	colors color;
};

#endif
