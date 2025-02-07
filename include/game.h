#ifndef GAME_H
#define GAME_H

#include "deck.h"
#include "card.h"
#include "pile.h"
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <limits>
#include <vector>
#include <map>

//Windows console handling for ASCII
#ifdef _WIN32
#include <Windows.h>
#undef max
#endif


//Console & Display
void setupConsole();
void printGameRules();
void printBoard(deck&, reserve&, foundation* [], tableau* []);

//Game Management
void startGame();
void resetStream();
bool checkWin(foundation* []);

//Card Movement
void moveCardFromDeck(card* cardToMove, deck&, pile&);
void moveCardFromPile(card* cardToMove, pile&, pile&);
void addToStock(deck&, deck&);
int foundationCount(foundation* []);
void handlePlayerMove(deck&, deck&, reserve&, foundation* [], tableau* []);
void displayTableauOptions(int, tableau* []);
void validateTableauMove(int, int, int, tableau* [], pile&);
void validateReserveMove(reserve&, pile&);
void validateStockMove(deck&, pile&);
void fillEmptyTableau(reserve&, tableau* []);

#endif