#pragma once

#include "Country.h"

#include <iostream>

class Country;

/**
 * @brief Represents a continent in the game.
 *
 * The Continent class encapsulates information about a continent, including its name,
 * a list of associated countries, and details related to control status and army bonuses.
 * It provides methods for setting up the continent, accessing its countries, and printing
 * information about the continent.
 */
class Continent {
private:
    // Boolean indicating if the continent is active in the game.
    bool enabled = true;
    // The number of countries in this continent.
    int countryCount = 0;
    std::string continentName;
    // Array of pointers to the countries within this continent.
    Country** countries;

    // The amount of bonus armies granted for controlling the entire continent.
    int controlArmyBonus = 0;
    // The player ID this continent is controlled by.
    int controlledBy = -1;
public:
    /**
     * @brief Destructor for the Continent class.
     *
     * Releases memory allocated for the countries array.
     */
    ~Continent();

    /**
     * @brief Sets up the continent with the given name and country count.
     *
     * @param name The name of the continent.
     * @param count The number of countries in the continent.
     * @param armyBonus The number armies a player gets as a bonus for controlling the entire continent.
     */
    void SetupContinent(std::string name, int count, int armyBonus);

    /**
     * @brief Checks if the continent is fully owned by 1 player.
     *
     * @return True if the continent is owned by 1 player, false otherwise.
     */
    bool CheckIfContinentOwned();

    /**
     * @brief Counts up all armies to allocate to a player.
     *
     * @param playerId The id of the player whose armies are being allocated.
     * 
     * @return The number of armies to allocate to a player, accounting for continent bonuses and how each country equates to 1/3 of an army.
     */
    float AllocateArmies(int playerId);

    /**
     * @brief Prints out all countries that are owned by a player.
     *
     * @param playerId The id of the player.
     */
    void PrintOwnedCountries(int playerId);

    /**
     * @brief Gets the index of a country owned by a player.
     *
     * @param playerId The id of the player.
     * @param num The number of countries to skip over until the desired country is reached.
     * 
     * @return The index of the desired country.
     */
    int GetOwnedCountry(int playerId, int* num);

    /**
     * @brief Gets a pointer to the country at the specified index within the continent.
     *
     * @param index The index of the country within the continent.
     * 
     * @return Pointer to the Country object.
     */
    Country* GetCountry(int index);

    /**
     * @brief Sets the country at the specified index within the continent.
     *
     * @param index The index of the country within the continent.
     * @param country Pointer to the Country object.
     */
    void SetCountry(int index, Country* country);

    /**
     * @brief Getter for the countryCount variable.
     *
     * @return the amount of countries this continent has.
     */
    int GetCountryCount();

    /**
     * @brief Sets the enabled/disabled status of the continent.
     *
     * @param enabled Boolean indicating whether the continent is enabled (true) or disabled (false).
     */
    void SetIsEnabled(bool enabled);

    /**
     * @brief Checks if the continent is currently enabled.
     *
     * @return True if the continent is enabled, false otherwise.
     */
    bool IsEnabled();

    /**
     * @brief Prints information about the continent, including its listed countries.
     *
     * If the continent is disabled, nothing will be printed.
     */
    void PrintContinent();
};