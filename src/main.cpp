/*
*/

#include "deck.h"
#include "card.h" 
#include "pile.h"
#include <iostream>
#include <limits>
#include <conio.h>
#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>
#ifdef _WIN32
#include <Windows.h>
#undef max
#endif

void resetStream();
void moveCardFromDeck(card* cardToMove, deck&, pile&);
void moveCardFromPile(card* cardToMove, pile&, pile&);
void printBoard(deck&, reserve&, foundation*[], tableau*[]);
void addToStock(deck&, deck&);
void moveIO(deck&, deck&, reserve&, foundation*[], tableau*[]);
void displayTableauOptions(int, tableau* []);
void validateTableauMove(int, int, int, tableau* [], pile&);
void validateReserveMove(reserve&, pile&);
void validateStockMove(deck&, pile&);
void fillEmptyTableau(reserve&, tableau*[]);

int main()
{
    //Set console to output ASCII
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif


    //Create deck, reserve, foundation and tableau piles
    deck mainDeck;
    deck stock(0);
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
        else
        {
			foundationPile[i] = new foundation();
        }
    }

    //Main game loop
    bool gameLoop = true;
    int loopCount = 0;
    while (gameLoop)
    {
        if (loopCount == 0)
        {
           addToStock(mainDeck, stock);
        }

        //Print the board in it's current state
        printBoard(stock, reservePile, foundationPile, tableauPile);

        //Make move selection
        moveIO(mainDeck, stock, reservePile, foundationPile, tableauPile);

        //Fill any empty tableaus from reserve pile
		fillEmptyTableau(reservePile, tableauPile);

        ++loopCount;
       //_getch();

        //system("cls");
    }

    //Free memory
    for (int i = 0; i < 4; ++i)
    {
        delete foundationPile[i];
        delete tableauPile[i];
    }

    return 0;
}

void resetStream()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void moveCardFromDeck(card* cardToMove, deck& deck, pile& pile)
{
    pile.addCard(cardToMove);
    deck.removeTopCard();
}

void moveCardFromPile(card* cardToMove, pile& removePile, pile& addPile)
{
    addPile.addCard(cardToMove);
    removePile.removeTopCard();
}

void moveCardFromTableau(card* cardToMove, tableau& removePile, pile& addPile)
{
    if (cardToMove != removePile.getTopCard())
    {
        for (int i = removePile.getCardIndex(cardToMove); i < removePile.getCardCount(); ++i)
        {
			addPile.addCard(removePile.getCard(i));
			removePile.removeCard(removePile.getCard(i));
        }
    }
	addPile.addCard(cardToMove);
	removePile.removeTopCard();
}

void printBoard(deck& stock, reserve& reservePile, foundation* foundationPile[], tableau* tableauPile[])
{
    std::cout << foundation::printPiles(foundationPile);
    std::cout << std::endl;
    std::cout << reservePile.printPile();
    std::cout << std::endl;
    std::cout << tableau::printPiles(tableauPile);
    std::cout << std::endl;
    std::cout << stock.printPile();

}

void addToStock(deck& mainDeck, deck& stock)
{
    int mdSize = mainDeck.getDeckSize();
    //Move 3 cards from deck to stock (if deck is empty, move all of stock to deck and start over)
    if (mdSize == 0)
    {
        for (int i = 0; i < stock.getDeckSize(); ++i)
        {
            mainDeck.addCard(stock.getTopCard());
            stock.removeCard(stock.getTopCard());
        }
    }
    int stockMoveCount;
    if (mdSize >= 3)
    {
        stockMoveCount = 3;
    }
    else
    {
        stockMoveCount = mdSize;
    }
    for (int i = 0; i < stockMoveCount; ++i)
    {
        stock.addCard(mainDeck.getTopCard());
        mainDeck.removeTopCard();
    }
}

void moveIO(deck& mainDeck, deck& stock, reserve& reservePile, foundation* foundationPile[], tableau* tableauPile[])
{
    //Select while pile to move from
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "Pick your move:\n" <<
        "1. Move From Tableau 1\n" <<
        "2. Move From Tableau 2\n" <<
        "3. Move From Tableau 3\n" <<
        "4. Move From Tableau 4\n" <<
        "5. Move From Reserve\n" <<
        "6. Move From Stock\n" << 
        "7. Add To Stock\n";
    int moveSelection;
    std::cin >> moveSelection;
    std::cout << std::endl;
    //Validate input
    while (!std::cin || moveSelection < 1 || moveSelection > 7)
    {
        std::cout << "Invalid selection. Please try again.\n";
        if (!std::cin)
        {
            resetStream();
        }
        std::cin >> moveSelection;
    }

    std::string moveSelectionStr[] = { "Tableau 1", "Tableau 2", "Tableau 3", "Tableau 4", "Foundation 1", "Foundation 2", "Foundation 3", "Foundation 4" };
    std::map<int, pile*> pileMap = { {1, tableauPile[0]}, {2, tableauPile[1]}, {3, tableauPile[2]}, {4, tableauPile[3]}, {5, foundationPile[0]}, {6, foundationPile[1]}, {7, foundationPile[2]}, {8, foundationPile[3]} };
    int pileSelection;
	//Select which card to move or which pile to move to
    switch (moveSelection)
    {
    case 1:
    case 2:
    case 3:
    case 4:
		displayTableauOptions(moveSelection, tableauPile);
        int cardSelection;
        std::cin >> cardSelection;
        //Validate input
		while (!std::cin || cardSelection < 1 || cardSelection > tableauPile[moveSelection - 1]->getCardCount()) //If card selection is > number of cards in selected tableau pile
		{
			std::cout << "Invalid selection. Please try again.\n";
			if (!std::cin)
			{
				resetStream();
			}
			std::cin >> cardSelection;
		}

        //Choose a pile to move to
		std::cout << "Which pile would you like to move to?\n";
		for (int i = 0; i < 8; ++i)
		{
			//Don't display the pile that the card is already in
            if (moveSelection != (i + 1))
            {
				std::cout << (i + 1) << ". " << moveSelectionStr[i] << std::endl;
            }
		}
        std::cin >> pileSelection;
		//Validate input, make sure the pile selection is not the same as the move selection
		while (!std::cin || pileSelection < 1 || pileSelection > 8 || pileSelection == moveSelection)
		{
			std::cout << "Invalid selection. Please try again.\n";
			if (!std::cin)
			{
				resetStream();
			}
			std::cin >> pileSelection;
		}
        //Call function based on move pile, card chosen, pile to move to
		validateTableauMove(moveSelection, cardSelection, pileSelection, tableauPile, *pileMap[pileSelection]);

		break;
	case 5:
	case 6:
        //Choose a pile to move to
		std::cout << "Which pile would you like to move to?\n";
        for (int i = 0; i < 8; ++i)
        {
			std::cout << (i + 1) << ". " << moveSelectionStr[i] << std::endl;
        }
		std::cin >> pileSelection;
        //Validate input
        while (!std::cin || pileSelection < 1 || pileSelection > 8)
        {
            std::cout << "Invalid selection. Please try again.\n";
            if (!std::cin)
            {
                resetStream();
            }
            std::cin >> pileSelection;
        }

		//Call function based on move pile, card chosen, pile to move to
        if (moveSelection == 5)
        {
			validateReserveMove(reservePile, *pileMap[pileSelection]);
		}
        else if (moveSelection == 6)
        {
            validateStockMove(stock, *pileMap[pileSelection]);
        }
        break;
    case 7:
        addToStock(mainDeck, stock);
		break;
	}
}

void displayTableauOptions(int moveSelection, tableau* tableauPile[])
{
    std::cout << "Which card would you like to move?\n";
    for (int i = 0; i < tableauPile[moveSelection - 1]->getCardCount(); ++i)
    {
        std::cout << (i + 1) << ". " << tableauPile[moveSelection - 1]->getCard(i)->printStr() << std::endl;
    }
}

void validateTableauMove(int moveSelection, int cardSelection, int pileSelection, tableau* tableauPile[], pile& pileToValidate)
{
    if (pileToValidate.validateMove(tableauPile[moveSelection - 1]->getCard(cardSelection - 1)))
    {
		moveCardFromTableau(tableauPile[moveSelection - 1]->getCard(cardSelection - 1), *tableauPile[moveSelection - 1], pileToValidate);
	}
    else
    {
        std::cout << "Invalid move. Please try again.\n";
    }
}

void validateReserveMove(reserve& reservePile, pile& pileToValidate)
{
	if (pileToValidate.validateMove(reservePile.getTopCard()))
	{
		moveCardFromPile(reservePile.getTopCard(), reservePile, pileToValidate);
	}
	else
	{
		std::cout << "Invalid move. Please try again.\n";
	}
}

void validateStockMove(deck& stock, pile& pileToValidate)
{
    if (pileToValidate.validateMove(stock.getTopCard()))
    {
        moveCardFromDeck(stock.getTopCard(), stock, pileToValidate);
    }
    else
    {
        std::cout << "Invalid move. Please try again.\n";
    }
}

void fillEmptyTableau(reserve& reservePile, tableau* tableauPile[])
{
    //Fill empty tableau piles from reserve pile if there are any reserve cards left
	for (int i = 0; i < 4; ++i)
	{
		if (tableauPile[i]->getCardCount() == 0)
		{
            if (reservePile.getCardCount() > 0)
            {
                tableauPile[i]->addCard(reservePile.getTopCard());
                reservePile.removeTopCard();
            }
		}
	}
}