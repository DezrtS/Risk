// -------------------------------------------------------------------- //
// Title: C++ Programming Assignment 1                                  //
// Due Date: 03/02/2024                                                 //
// Authors:    Denzil Walrond,     Christian Zambri,    Michael Atteh   //
// Student#:   100868217,          100787919,           100831528       //
// -------------------------------------------------------------------- //

// Headers and Namespaces
#include <iostream>
#include <string>
#include <thread>

#include "GraphicsInterface.h"
#include "Useful.h"
#include "RiskGame.h"

#undef main

class RiskGame;

// --------------------------------------------------------------------------------------------------------- //
// Main Function                                                                                             //
// --------------------------------------------------------------------------------------------------------- //

/**
 * @brief The starting function of the project.
 */
int main(int argc, char* argv[])
{
    for (int i = 0; i < 10; i++) {
        Useful::RollDice(2);
    }
    
    // Creates the graphics thread.
    std::thread graphicsThread(GraphicsInterface::GraphicsHandler);

    // Prompts the player for the amount of players playing.
    int playerCount = 0;
    while (playerCount <= 0) {
        std::cout << "\nHow many players are playing? (2-6)\n";
        try {
            std::string input;
            std::cin >> input;
            playerCount = std::stoi(input);

            // Check to see if the player count is outside of the allowed range.
            if (playerCount < 2 || playerCount > 6) {
                std::cout << "\nThe amount of players is outside of the allowed range\n";
                playerCount = 0;
            }
        }
        catch (...) {
            // Ask the player again if the player count they inputted is invalid.
            std::cout << "\nInvalid Input\n";
            playerCount = 0;
        }
    }

    // Creates the Risk Game, passes in the player count, and starts the game.
    RiskGame riskGame = RiskGame(playerCount);
    riskGame.PlayGame();

    // Terminates the graphics thread.
    GraphicsInterface::terminateGraphicsThread = true;
    graphicsThread.join();

    return 0;
}