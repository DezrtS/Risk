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
    // Self explanitory private Player variables
    std::string playerName;
    int playerId;
    int armies = 0;
    int ownedCountries = 0;
    int cards = 0;
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

    void AddArmies(int armiesToAdd);

    int GetArmyCount();

    int GetOwnedCountries();

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
     * Runs the code and 3 stage methods associated with a player turn
     */
    void PlayTurn(RiskGame* game);

    /**
     * @brief Handles the reinforcement stage of a player turn
     */
    void DeployReinforcements(RiskGame* game);

    /**
     * @brief Handles the attack stage of a player turn
     */
    void PerformAttacks(RiskGame* game);

    /**
     * @brief Handles the fortification stage of a player turn
     */
    void SetupFortifications(RiskGame* game);

    /**
     * @brief Prints the details of the player.
     *
     * This method outputs the player's name and player number.
     */
    void PrintPlayer();
};