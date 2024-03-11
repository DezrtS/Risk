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

    Continent* GetContinent(int index);

    /**
     * @brief Allocates an equal set of countries to each player.
     *
     * @param playerCount.
     */
    void AllocateCountryOwnership(Player** players, int playerCount);

    int AllocateArmies(int playerId);

    void PrintOwnedCountries(int playerId);

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