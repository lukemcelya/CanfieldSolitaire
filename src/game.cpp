#include "game.h"


void printGameRules()
{
    std::cout << "Welcome to Canfield!\nThe Solitaire game you will never win! ;)\n\n" <<
        "Rules:\n" <<
        "You have a Deck of 52 cards.\n" <<
        "13 cards are moved from your Deck to your Reservation Pile.\n" <<
        "1 card is moved from your Deck to your first Foundation Pile (this determines the starting rank of all 4 Foundation Piles).\n" <<
        "1 card is moved from your Deck to each of your 4 Tableau Piles\n" <<
        "3 cards are moved at a time from your Deck to your Stock pile to be used whenever you need it.\n" <<
        "The objective of the game is to get all 52 cards into their appropriate Foundation Piles (each Foundation contains all ranks of one suit).\n" <<
        "You can move any cards from Tableaus, Stock, and Reservation Piles to an appropriate Foundation or Tableau Pile at any time.\n" <<
        "Foundation Piles are built up and are circular, once you reach King, move to Ace > Two > Three > etc.\n" <<
        "Tableau Piles are built down (Two > Ace > King > Queen > etc.) and alternating in color.\n" <<
        "Good Luck and Have Fun!\n\n";
}

void setupConsole()
{
//Set console to output ASCII
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
}

void startGame()
{
    printGameRules();

    std::cout << "Press 1 to start new game or 0 to quit.\n";
    int input;
    std::cin >> input;
    while (!std::cin || input < 0 || input > 1)
    {
        std::cout << "Invalid entry. Press 1 to start new game or 0 to quit.\n";
        if (!std::cin)
        {
            resetStream();
        }
        std::cin >> input;
    }

    if (input == 0)
    {
        return;
    }

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
    bool firstLoop = true;
    while (gameLoop)
    {
        system("cls");
        //Add to stock for the first iteration
        if (firstLoop)
        {
            addToStock(mainDeck, stock);
            firstLoop = false;
        }

        //Print the board in it's current state
        printBoard(stock, reservePile, foundationPile, tableauPile);

        //Print amount of cards in foundation piles
        std::cout << "Foundation Pile Count: " << foundationCount(foundationPile) << std::endl;

        //Make move selection
        handlePlayerMove(mainDeck, stock, reservePile, foundationPile, tableauPile);

        //Fill any empty tableaus from reserve pile
        fillEmptyTableau(reservePile, tableauPile);

        //Check win
        if (checkWin(foundationPile))
        {
            std::cout << "Congratulations! You won!\n\n";
            gameLoop = false;
        }

    }

    std::cout << "Thank you for playing!\n";

    //Free memory
    for (int i = 0; i < 4; ++i)
    {
        delete foundationPile[i];
        delete tableauPile[i];
    }
}

void resetStream()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

//Moves a card from deck to another pile
void moveCardFromDeck(card* cardToMove, deck& deck, pile& pile)
{
    pile.addCard(cardToMove);
    deck.removeTopCard();
}

//Moves a card from one pile to another
void moveCardFromPile(card* cardToMove, pile& removePile, pile& addPile)
{
    addPile.addCard(cardToMove);
    removePile.removeTopCard();
}

//Moves a card from a tableau to another pile
void moveCardFromTableau(card* cardToMove, tableau& removePile, pile& addPile)
{
    if (cardToMove != removePile.getTopCard())
    {
        //Initially I had this incrementing the index for each card removed, but when a card is deleted,
        //the index of the card below it is decremented, so it wasn't working as expected. It just
        //needs to remove the card at that index location of the vector until there are no cards left past
        //that index
        int cardIndex = removePile.getCardIndex(cardToMove);
        int cardCount = removePile.getCardCount();
        for (int i = cardIndex; i < cardCount; ++i)
        {
            addPile.addCard(removePile.getCard(cardIndex));
            removePile.removeCard(removePile.getCard(cardIndex));
        }
    }
    else
    {
        addPile.addCard(cardToMove);
        removePile.removeTopCard();
    }
}

//Prints the board's game state to console
void printBoard(deck& stock, reserve& reservePile, foundation* foundationPile[], tableau* tableauPile[])
{
    std::cout << "                       Canfield\n\n";
    std::cout << foundation::printPiles(foundationPile);
    std::cout << std::endl;
    std::cout << reservePile.printPile();
    std::cout << std::endl;
    std::cout << tableau::printPiles(tableauPile);
    std::cout << std::endl;
    std::cout << stock.printPile();

}

//Adds cards to the stock pile from the deck
void addToStock(deck& mainDeck, deck& stock)
{
    int mdSize = mainDeck.getDeckSize();
    //Move 3 cards from deck to stock (if deck is empty, move all of stock to deck and start over)
    if (mdSize == 0)
    {
        for (int i = 0; i < stock.getDeckSize(); ++i)
        {
            mainDeck.addCard(stock.getTopCard());
            stock.removeTopCard();
        }
        //Update mdSize to reflect new deck size
        mdSize = mainDeck.getDeckSize();
    }

    //Determine how many cards to move based on the size of the deck
    int stockMoveCount;
    if (mdSize >= 3)
    {
        stockMoveCount = 3;
    }
    else
    {
        stockMoveCount = mdSize;
    }

    //Move cards from deck to stock
    for (int i = 0; i < stockMoveCount; ++i)
    {
        stock.addCard(mainDeck.getTopCard());
        mainDeck.removeTopCard();
    }
}

//Gets a count of how many foundations have at least one card placed
int foundationCount(foundation* foundationPile[])
{
    int count = 0;
    for (int i = 0; i < 4; ++i)
    {
        count += foundationPile[i]->getCardCount();
    }
    return count;
}

//Set up an appropriate move function depending on selection
void handlePlayerMove(deck& mainDeck, deck& stock, reserve& reservePile, foundation* foundationPile[], tableau* tableauPile[])
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
        "7. Add To Stock\n" <<
        "8. Quit\n";
    int moveSelection;
    std::cin >> moveSelection;
    std::cout << std::endl;
    //Validate input
    while (!std::cin || moveSelection < 1 || moveSelection > 8)
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
    case 8:
        std::cout << "Thank you for playing!\n";
        exit(0);
        break;
    }
}

//Displays available movements when a tableau is selected
void displayTableauOptions(int moveSelection, tableau* tableauPile[])
{
    std::cout << "Which card would you like to move?\n";
    for (int i = 0; i < tableauPile[moveSelection - 1]->getCardCount(); ++i)
    {
        std::cout << (i + 1) << ". " << tableauPile[moveSelection - 1]->getCard(i)->printStr() << std::endl;
    }
}

//Validates a move from a tableau, especially if multiple cards are involved
void validateTableauMove(int moveSelection, int cardSelection, int pileSelection, tableau* tableauPile[], pile& pileToValidate)
{
    if (pileToValidate.validateMove(tableauPile[moveSelection - 1]->getCard(cardSelection - 1)))
    {
        moveCardFromTableau(tableauPile[moveSelection - 1]->getCard(cardSelection - 1), *tableauPile[moveSelection - 1], pileToValidate);
    }
    else
    {
        system("clear");
        std::cout << "Invalid move. Please try again.\n";
    }
}

//Validates a move from the reserve pile
void validateReserveMove(reserve& reservePile, pile& pileToValidate)
{
    if (pileToValidate.validateMove(reservePile.getTopCard()))
    {
        moveCardFromPile(reservePile.getTopCard(), reservePile, pileToValidate);
    }
    else
    {
        system("clear");
        std::cout << "Invalid move. Please try again.\n";
    }
}

//Validates a move from the stock pile
void validateStockMove(deck& stock, pile& pileToValidate)
{
    if (pileToValidate.validateMove(stock.getTopCard()))
    {
        moveCardFromDeck(stock.getTopCard(), stock, pileToValidate);
    }
    else
    {
        system("clear");
        std::cout << "Invalid move. Please try again.\n";
    }
}

//Fills an empty tableau directly from the reserve
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

//Checks if all foundation piles are full
bool checkWin(foundation* foundationPile[])
{
    //Count how many foundation piles contain all 13 cards
    int finishedFoundations = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (foundationPile[i]->getCardCount() == 13)
        {
            ++finishedFoundations;
        }
    }

    //Return as true if all 4 piles have 13 cards, else return false
    if (finishedFoundations == 4)
    {
        return true;
    }
    return false;
}