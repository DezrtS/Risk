#pragma once

#include "Continent.h"

#include <iostream>

class Continent;

/**
 * @brief Represents a country in the game.
 *
 * The Country class encapsulates information about a country, including its name,
 * connections to other countries within and outside its continent, and details
 * related to its army and control status. It provides methods for setting up
 * connections, adding connections, and printing information about the country.
 */
class Country {
private:
    // The number of connections to countries within the same continent.
    int connectionCount = 0;
    // The number of connections to countries outside of this country's continent.
    int offConnectionCount = 0;
    std::string countryName;
    // Array of pointers to the connected countries within the same continent.
    Country** connectedCountries;
    // Array of pointers to the connected countries outside of this country's continent.
    Country** offContinentConnections;
    // The continent this country belongs to.
    Continent *parentContinent;

    // The number of armies in this country.
    int armyCount = 0;
    // The player ID this country is controlled by.
    int controlledByPlayer = -1;
public:
    /**
     * @brief Constructs a Country object within the specified continent.
     *
     * @param continent Pointer to the continent to which the country belongs.
     */
    Country(Continent* continent);

    /**
     * @brief Destructor for the Country class.
     *
     * Releases memory allocated for connectedCountries and offContinentConnections arrays.
     */
    ~Country();

    /**
     * @brief Sets up the country with the given name.
     *
     * @param name The name of the country.
     */
    void SetupCountry(std::string& name);

    /**
     * @brief Sets up connections to other countries within the same continent.
     *
     * @param count The number of connections to set up.
     */
    void SetupConnections(int count);
    /**
     * @brief Sets up connections to countries outside the continent.
     *
     * @param count The number of off-continent connections to set up.
     */
    void SetupOffContinentConnections(int count);

    /**
     * @brief Adds a connection to another country within the same continent.
     *
     * @param index The index of the connection.
     * @param country Pointer to the country to be connected.
     */
    void AddConnection(int index, Country* country);

    /**
     * @brief Adds a connection to a country outside the continent.
     *
     * @param index The index of the off-continent connection.
     * @param country Pointer to the off-continent country to be connected.
     */
    void AddOffContinentConnection(int index, Country* country);

    /**
     * @brief Adjusts the army count of a country.
     *
     * @param armiesToAdd The amount of armies to add to the army count.
     */
    void AdjustArmyCount(int armiesToAdd);

    /**
     * @brief Sets the owner of a country.
     *
     * @param playerId The Id of the player that owns this country.
     */
    void SetOwner(int playerId);

    /**
     * @brief Gets the owner of a country.
     *
     * @return The Id of the owner of this country.
     */
    int GetOwner();

    /**
     * @brief Gets the army count of a country.
     *
     * @return The number of armies on this country.
     */
    int GetArmyCount();

    /**
     * @brief Gets the name of a country.
     *
     * @return The name of this country.
     */
    std::string GetCountryName();

    /**
     * @brief Gets a connected country to this country.
     *
     * @param num The number of connected countries to skip over until the desired country is reached.
     * @param skipOwnedVsUnowned A bool determining if owned or unowned countries should be completely skipped over.
     * 
     * @return Pointer to the desired country.
     */
    Country* GetConnectedCountry(int num, bool skipOwnedVsUnowned);

    /**
     * @brief Gets the continent a country is located in.
     *
     * @return The continent this country is in.
     */
    Continent* GetParentContinent();

    /**
     * @brief Prints information about the country, including its connections.
     * 
     * @param goIntoDetail A bool determining if a country should go into detail about its properties.
     */
    void PrintCountry(bool goIntoDetail);

    /**
     * @brief Prints information about the country, including its connections with the ability to skip owned or unowned countries.
     *
     * @param skipOwnedVsUnowned A bool determining if owned or unowned countries should be completely skipped over.
     */
    void PrintCountryWithSkip(bool skipOwnedVsUnowned);
};