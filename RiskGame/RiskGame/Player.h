#pragma once

#include "RiskGame.h"

#include <iostream>

class RiskGame;

/**
 * @brief Represents a player in the game.
 *
 * The Player class encapsulates information about a player, including their
 * name and player number. It provides a method to print the player's details.
 */
class Player {
private:
    // The player's name
    std::string playerName;
    // The Id of the player
    int playerId;
    // The amount of armies the player has
    int armies = 0;
    // The amount of countries the player owns
    int ownedCountries = 0;
    // The number of cards the player has
    int cards = 0;
    // Whether or not the player has lost
    bool hasLost = false;
public:
    /**
     * @brief Constructs a Player object with the given name and player number.
     *
     * @param name The name of the player.
     * @param num The player number assigned to the player.
     */
    Player(std::string name, int id);

    /**
     * @brief Adjusts the number of countries a player owns.
     *
     * @param additionalOwnedCountries The number of additional countries the player now owns.
     */
    void AdjustOwnedCountries(int additionalOwnedCountries);

    /**
     * @brief Adds to the amount of armies a player has.
     *
     * @param armiesToAdd The number of armies to add.
     */
    void AddArmies(int armiesToAdd);

    /**
     * @brief Gets the amount of armies a player has.
     *
     * @return The amount of armies a player currently has.
     */
    int GetArmyCount();

    /**
     * @brief Gets the amount of countries a player owns.
     *
     * @return The amount of armies a player currently has.
     */
    int GetOwnedCountries();

    /**
     * @brief Gets the name of a player.
     *
     * @return The name of the player.
     */
    std::string GetName();

    /**
     * @brief Determines if a player has lost the game.
     *
     * @return A boolean for if the player has lost.
     */
    bool HasLost();

    /**
     * @brief Handles a player turn.
     *
     * Runs the code and 3 stage methods associated with a player turn.
     * 
     * @param game The game itself.
     */
    void PlayTurn(RiskGame* game);

    /**
     * @brief Handles the reinforcement stage of a player turn.
     *
     * @param game The game itself.
     */
    void DeployReinforcements(RiskGame* game);

    /**
     * @brief Handles the attack stage of a player turn.
     * 
     * @param game The game itself.
     */
    void PerformAttacks(RiskGame* game);

    /**
     * @brief Handles the fortification stage of a player turn.
     * 
     * @param game The game itself.
     */
    void SetupFortifications(RiskGame* game);

    /**
     * @brief Prints the details of the player.
     *
     * This method outputs the player's name and player number.
     */
    void PrintPlayer();
};