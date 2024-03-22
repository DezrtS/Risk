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
    // The amount of eliminated players
    int eliminatedPlayers = 0;
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

    /**
     * @brief Allocates armies for a player based on the amount of countries and continents they own.
     *
     * @param playerId The Id of the player to allocate armies to.
     */
    void AllocateArmies(int playerId);

    /**
     * @brief Prints the countries owned by a player.
     *
     * @param playerId The Id of the player whose owned countries will be printed.
     */
    void PrintOwnedCountries(int playerId);

    /**
     * @brief Handles functionality for attacking another country.
     *
     * Determines the attacker and defender variables, and then allows them to compete
     * for control over the country. Both players try to roll the highest die they can
     * to determine who loses an army. When either the attacker's or defender's armies
     * are exhausted, the winning player keeps the country. If the attacker wins, they
     * have to deploy a number of armies greater than or equal to the number of die
     * used in the entire attack.
     *
     * @param attacker A pointer to the attacking country.
     * @param defender A pointer to the defending country.
     * 
     * @return A bool for whether the attacker captured the country or not.
     */
    bool AttackCountry(Country* attacker, Country* defender);

    /**
     * @brief Gets a country owned by a player.
     *
     * @param playerId The Id of the player whose country will be returned.
     * @param num The number of countries to skip over until the desired country is reached.
     *
     * @return Pointer to the desired country.
     */
    Country* GetOwnedCountry(int playerId, int num);

    /**
     * @brief Prints information about all players in the game.
     */
    void PrintPlayers();
};