#pragma once

class Player;
class Map;
class Country;

/**
 * @brief Represents a game of Risk with players and a map.
 *
 * The RiskGame class encapsulates information about a Risk game, including the number
 * of players, an array of player objects, and a map object. It provides methods for
 * setting up players, printing player information, and initializing the game map.
 */
class RiskGame {
private:
    // The number of players in the game.
    int playerCount;
    // Array of pointers to the players playing the game.
    Player** players;
    // The game map.
    Map* map;
public:
    /**
     * @brief Constructor for the RiskGame class.
     *
     * Initializes the game with the specified number of players, sets up players and
     * reads map data from a file.
     *
     * @param playerCount The number of players in the game.
     */
    RiskGame(int playerCount);

    /**
     * @brief Destructor for the RiskGame class.
     *
     * Releases memory allocated for the players array.
     */
    ~RiskGame();

    /**
     * @brief Main method to play the game.
     *
     * Currently only loops through each player and runs their PlayTurn() method.
     */
    void PlayGame();

    /**
     * @brief Sets up player objects by prompting for player names.
     */
    void SetupPlayers();

    void AllocateArmies(int playerId);

    void PrintOwnedCountries(int playerId);

    bool AttackCountry(Country* attacker, Country* defender);

    Country* GetOwnedCountry(int playerId, int num);

    /**
     * @brief Prints information about all players in the game.
     */
    void PrintPlayers();
};