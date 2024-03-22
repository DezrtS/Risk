#include "Map.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

Map::~Map() {
    delete[] continents;
}

void Map::SetupMap(int count) {
    // If there are already continents in this map, it has already been setup.
    if (continentCount > 0) {
        std::cout << "\nAlready Setup Map\n";
        return;
    }
    // Setup the continent count, and the continent array.
    continentCount = count;
    continents = new Continent * [continentCount];
}

Continent* Map::GetContinent(int index) {
    return continents[index];
}

void Map::AllocateCountryOwnership(Player** players, int playerCount) {
    int totalCountryCount = 0;
    // Adds up all of the possible countries.
    for (int i = 0; i < continentCount; i++) {
        if (continents[i]->IsEnabled()) {
            totalCountryCount += continents[i]->GetCountryCount();
        }
    }

    if (totalCountryCount <= 0) {
        // No countries to available to allocate, so game cannot run.
        exit(0);
    }

    // Decide how many countries each player gets based on the total amount of countries and amount of players.
    int countriesEach = totalCountryCount / playerCount;
    int countriesToSkip = totalCountryCount % playerCount;

    // Prints out the results of how many countries each player gets.
    std::cout << "There are " << totalCountryCount << " countries to hand out";
    std::cout << "\nEach player gets " << countriesEach << " countries each";
    std::cout << "\nIn order to keep the game balanced, we have to skip over " << countriesToSkip << " countries";

    srand(time(NULL));
    int disabledOffset = 0;
    int skippedCount = 0;
    // Randomly distributes the available countries.
    for (int i_a = 0; i_a < continentCount - disabledOffset; i_a++) {
        // If a continent is disabled, skip over it.
        if (!continents[i_a]->IsEnabled()) {
            disabledOffset++;
        }
        Continent* continent = continents[i_a + disabledOffset];
        int countryCount = continent->GetCountryCount();
        for (int i_b = 0; i_b < countryCount; i_b++) {
            int playerId = rand() % playerCount;
            int failSafe = 0;
            // Runs until it can find someone, or no one to assign the country to.
            while (players[playerId]->GetOwnedCountries() >= countriesEach) {
                playerId++;
                if (playerId >= playerCount) {
                    if (skippedCount < countriesToSkip) {
                        skippedCount++;
                        playerId = -1;
                        break;
                    }
                    playerId = 0;
                }
            }

            // Set the owner of the country, and adjust the owner's amount of owned countries.
            if (playerId >= 0) {
                continent->GetCountry(i_b)->SetOwner(playerId);
                players[playerId]->AdjustOwnedCountries(1);
            }
        }
        // Check to see if any of the players fully own a continent.
        continents[i_a + disabledOffset]->CheckIfContinentOwned(false);
    }
}

int Map::AllocateArmies(int playerId) {
    float armies = 0;
    // Allocates armies to a player.
    for (int i = 0; i < continentCount; i++) {
        armies += continents[i]->AllocateArmies(playerId);
    }
    return (int)armies;
}

void Map::PrintOwnedCountries(int playerId) {
    // Prints out the owned countries of a player.
    for (int i = 0; i < continentCount; i++) {
        continents[i]->PrintOwnedCountries(playerId);
    }
}

Country* Map::GetOwnedCountry(int playerId, int num) {
    for (int i = 0; i < continentCount; i++) {
        int index = continents[i]->GetOwnedCountry(playerId, &num);
        if (index >= 0) {
            return continents[i]->GetCountry(index);
        }
    }
    return nullptr;
}

void Map::ReadMapFile(std::string path) {
    std::string line;
    // Create a ifstream object and check to see if the stream is open.
    std::ifstream dataFile(path);
    if (dataFile.is_open()) {
        // Creates a try catch block to handle any errors while reading the map file.
        try {
            // Read the total amount of continents.
            getline(dataFile, line);
            int continentCount = stoi(line);
            SetupMap(continentCount);

            // Loop through 1 continent data block each time.
            for (int i_a = 0; i_a < continentCount; i_a++) {
                // Read the continent name and control army bonus.
                getline(dataFile, line);
                std::vector<std::string> continentDetails = Useful::SplitString(line, ",");
                std::string continentName = continentDetails[0];
                // Create a new Continent object
                Continent* continent = new Continent();
                continents[i_a] = continent;
                // Read the total amount of countries in the continent.
                getline(dataFile, line);
                int countryCount = stoi(line);
                // Setup the continent values
                continent->SetupContinent(continentName, countryCount, stoi(continentDetails[1]));

                // Read the list of countries and split them up into a vector of strings.
                getline(dataFile, line);
                std::vector<std::string> countries = Useful::SplitString(line, ",");

                // Loop through each country
                for (int i_b = 0; i_b < countryCount; i_b++) {
                    // Create a new country object and setup its variables.
                    Country* country = new Country(continent);
                    country->SetupCountry(countries[i_b]);
                    // Add the country to the current continent.
                    continent->SetCountry(i_b, country);
                }

                // Loop though all of the country connections within the current continent.
                for (int i_b = 0; i_b < countryCount; i_b++) {
                    // Read the connection details which includes the indexes of which countries connect to what.
                    getline(dataFile, line);
                    std::vector<std::string> connectionDetails = Useful::SplitString(line, ":");
                    std::vector<std::string> connectionIndexes = Useful::SplitString(connectionDetails[1], ",");
                    int countryIndex = stoi(connectionDetails[0]);
                    // Setup the connections by passing in the amount of connections to make.
                    continents[i_a]->GetCountry(countryIndex)->SetupConnections(connectionIndexes.size());

                    // Loop through each connection index and add the country it points to, to the other country.
                    for (int i_c = 0; i_c < connectionIndexes.size(); i_c++) {
                        continents[i_a]->GetCountry(countryIndex)->AddConnection(i_c, continents[i_a]->GetCountry(stoi(connectionIndexes[i_c])));
                    }
                }
            }

            // Loop through and read the remaining lines in the data file which include all of the off continent connections.
            while (getline(dataFile, line)) {
                // Split up the connection details so that the first element includes the country everything in the second index connects to.
                std::vector<std::string> connectionDetails = Useful::SplitString(line, ":");
                std::vector<std::string> indexes = Useful::SplitString(connectionDetails[0], ".");
                int continentIndex = stoi(indexes[0]);
                int countryIndex = stoi(indexes[1]);
                std::vector<std::string> connectionIndexes = Useful::SplitString(connectionDetails[1], ",");
                // Setup the off continent connections by passing in the amount of off continent connections to make.
                continents[continentIndex]->GetCountry(countryIndex)->SetupOffContinentConnections(connectionIndexes.size());

                // Loop through each connection index and add the country it points to, to the other country
                for (int i = 0; i < connectionIndexes.size(); i++) {
                    std::vector<std::string> offContinentConnectionIndex = Useful::SplitString(connectionIndexes[i], ".");
                    continents[continentIndex]->GetCountry(countryIndex)->AddOffContinentConnection(i, continents[stoi(offContinentConnectionIndex[0])]->GetCountry(stoi(offContinentConnectionIndex[1])));
                }
            }
        }
        catch (...) {
            // Print out that there was an error and exit the program
            std::cout << "\nError Reading File\n";
            exit(1);
        }
    }
}

void Map::PrintMap() {
    for (int i = 0; i < continentCount; i++) {
        continents[i]->PrintContinent();
    }
}