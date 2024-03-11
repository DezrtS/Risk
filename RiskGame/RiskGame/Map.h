#pragma once

#include "Continent.h"
#include "Player.h"
#include "Useful.h"

class Continent;
class Player;

/**
 * @brief Represents the map of the game.
 *
 * The Map class encapsulates information about a map, including continents and their
 * countries, as well as methods for setting up the map, reading map data from a file,
 * and printing information about the map.
 */
class Map {
private:
    // The number of continents in the map.
    int continentCount = 0;
    // Array of pointers to the continents within the map.
    Continent** continents;
public:
    /**
     * @brief Destructor for the Map class.
     *
     * Releases memory allocated for the continents array.
     */
    ~Map();

    /**
     * @brief Sets up the map with the given number of continents.
     *
     * @param count The number of continents to set up.
     */
    void SetupMap(int count);

    /**
     * @brief Gets a continent within this map.
     *
     * @param index The index of the desired continent.
     * 
     * @return Pointer to the desired continent.
     */
    Continent* GetContinent(int index);

    /**
     * @brief Allocates an equal set of countries to each player.
     *
     * @param players A pointer array to all of the active players.
     * @param playerCount The number of players in the game.
     */
    void AllocateCountryOwnership(Player** players, int playerCount);

    /**
     * @brief Allocates armies to a player based on the countries they own.
     *
     * @param playerId The Id of the player to allocate armies to.
     * 
     * @return The number of armies to allocate.
     */
    int AllocateArmies(int playerId);

    /**
     * @brief Prints the owned countries of a player.
     *
     * @param playerId The Id of the player whose owned countries will be printed.
     */
    void PrintOwnedCountries(int playerId);

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
     * @brief Reads map data from a specified file and sets up the map accordingly.
     *
     * The file follows a specific format:
     * - First line: Total number of continents in the map.
     * - Subsequent lines for each continent:
     *   - Continent name and control army bonus seperated by a comma: "continentName,controlArmyBonus"
     *   - Number of countries in the continent.
     *   - Comma-separated list of country names.
     *   - For each country:
     *     - Line specifying connections in the format: "CountryIndex:ConnectedCountryIndexes".
     * - Remaining lines specifying off-continent connections in the format: "ContinentIndex.CountryIndex:ConnectedCountryIndexes".
     *
     * @param path The path to the map data file.
     */
    void ReadMapFile(std::string path);

    /**
     * @brief Prints information about the map, including details of each continent.
     */
    void PrintMap();
};