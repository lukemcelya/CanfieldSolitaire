/*
*/

#include "deck.h"
#include "card.h" 
#include "pile.h"
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <iomanip>

void moveCardFromDeck(card* cardToMove, deck&, pile&);
void moveCardFromPile(card* cardToMove, pile&, pile&);
void setColor(card* c);

int main()
{
    //Create deck, reserve, foundation and tableau piles
    deck mainDeck;
    reserve reservePile(mainDeck);
    foundation* foundationPile[4];
    tableau* tableauPile[4];
    for (int i = 0; i < 4; ++i)
    {
        tableauPile[i] = new tableau(mainDeck);

        if (i == 0)
        {
            //Only create the first foundation pile
            foundationPile[i] = new foundation(mainDeck);
        }
    }

    bool gameLoop = true;
    
    /*
    std::cout << "Deck:\n" << mainDeck.toString() << std::endl;
    std::cout << "Reserve:\n" << reservePile.toString() << std::endl;
    std::cout << "Foundation 1:\n" << foundationPile[0]->toString() << std::endl;
    for (int i = 0; i < 4; ++i)
    {
        std::cout << "Tableau " << (i + 1) << ":\n" << tableauPile[i]->toString() << std::endl;
    }
    */
    

    while (gameLoop)
    {

        //Output foundation piles and reserve pile (setColor changes text to red if card is a heart or diamond)
        setColor(foundationPile[0]->getTopCard());
        std::cout << std::left << std::setw(15) << std::setfill(' ') << " " << std::setw(5) << std::setfill(' ') << foundationPile[0]->toString() << std::endl;
        setColor(reservePile.getTopCard());
        std::cout << std::setw(15) << std::setfill(' ') << reservePile.printTopCard();

        //Get sizes of each tableau, to know how many rows to make
        std::vector<int> tableauSizes;
        for (int i = 0; i < 4; ++i)
        {
            tableauSizes.push_back(tableauPile[i]->getCardCount());
        }
        int maxSize = *std::max_element(tableauSizes.begin(), tableauSizes.end());

        for (int i = 0; i < maxSize; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (tableauPile[j]->getCardCount() >= i)
                {
                    setColor(tableauPile[j]->getCard(i));
                    std::cout << std::setw(15) << std::setfill(' ') << tableauPile[j]->printCards(i);
                }
            }
            std::cout << std::endl;
        }

        tableauPile[0]->addCard(reservePile.getTopCard());
        reservePile.removeCard(reservePile.getTopCard());

       _getch();

        system("cls");
    }

    for (int i = 0; i < 4; ++i)
    {
        delete foundationPile[i];
        delete tableauPile[i];
    }

    return 0;
}

void moveCardFromDeck(card* cardToMove, deck& deck, pile& pile)
{
    deck.removeCard(cardToMove);
    pile.addCard(cardToMove);
}

void moveCardFromPile(card* cardToMove, pile& removePile, pile& addPile)
{
    addPile.addCard(cardToMove);
    removePile.removeCard(cardToMove);
}

void setColor(card* c)
{
    if (c->getColor() == card::RED)
    {
        std::cout << "\033[31m";
    }
    else
    {
        std::cout << "\033[37m";
    }
}
