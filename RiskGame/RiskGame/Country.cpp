#include "Country.h"

#include <iostream>

Country::Country(Continent* continent) : parentContinent(continent) {}

Country::~Country() {
    delete[] connectedCountries;
    delete[] offContinentConnections;
}

void Country::SetupCountry(std::string& name) {
    countryName = name;
}

void Country::SetupConnections(int count) {
    // If there are already connections from this country, it has already been setup.
    if (connectionCount > 0) {
        std::cout << "\nAlready Setup Connections\n";
        return;
    }
    // Setup the connection count and connected countries array.
    connectionCount = count;
    connectedCountries = new Country * [connectionCount];
}

void Country::SetupOffContinentConnections(int count) {
    // If there are already off continent connections from this country, it has already been setup.
    if (offConnectionCount > 0) {
        std::cout << "\nAlready Setup OffConnections\n";
        return;
    }
    // Setup the off continent connection count and off continent connected countries array.
    offConnectionCount = count;
    offContinentConnections = new Country * [offConnectionCount];
}

void Country::AddConnection(int index, Country* country) {
    connectedCountries[index] = country;
}

void Country::AddOffContinentConnection(int index, Country* country) {
    offContinentConnections[index] = country;
}

void Country::AdjustArmyCount(int armiesToAdd) {
    armyCount += armiesToAdd;
}

void Country::SetOwner(int playerId) {
    controlledByPlayer = playerId;
}

int Country::GetOwner() {
    return controlledByPlayer;
}

int Country::GetArmyCount() {
    return armyCount;
}

std::string Country::GetCountryName() {
    return countryName;
}

Country* Country::GetConnectedCountry(int num, bool skipOwnedVsUnowned) {
    // Cycles through the connections from this country and returns the desired country.
    // Skips over either owned or unowned countries.
    for (int i = 0; i < connectionCount; i++) {
        if (connectedCountries[i]->GetOwner() == controlledByPlayer) {
            if (skipOwnedVsUnowned) {
                continue;
            }
        }
        else {
            if (!skipOwnedVsUnowned) {
                continue;
            }
        }
        if (num == 0) {
            return connectedCountries[i];
        }
        num--;
    }

    // If no regular connected countries were returned, cycle through the off continent connection list and return the desired country.
    // Skips over either owned or unowned countries.
    for (int i = 0; i < offConnectionCount; i++) {
        if (offContinentConnections[i]->GetOwner() == controlledByPlayer) {
            if (skipOwnedVsUnowned) {
                continue;
            }
        }
        else {
            if (!skipOwnedVsUnowned) {
                continue;
            }
        }
        if (num == 0) {
            return offContinentConnections[i];
        }
        num--;
    }
    // If no countries were returned, the user inputted an invalid index.
    return nullptr;
}

Continent* Country::GetParentContinent() {
    return parentContinent;
}

void Country::PrintCountry(bool goIntoDetail) {
    // Prints the elements of a country.
    std::cout << "\n\t" << countryName << " : [" << armyCount << " armies]";
    if (!goIntoDetail) {
        return;
    }
    // Prints the more indepth details of a country.
    std::cout << "\n\t\t\tControlled by player [#" << GetOwner() << "]";
    std::cout << "\n\t\tConnected Countries,";
    for (int i = 0; i < connectionCount; i++) {
        std::cout << "\n\t\t\t" << connectedCountries[i]->countryName;
        std::cout << "\n\t\t\t\tControlled by player [#" << connectedCountries[i]->GetOwner() << "] : [" << connectedCountries[i]->armyCount << " armies]";
    }
    if (offConnectionCount > 0) {
        std::cout << "\n\t\tOff Continent Connected Countries,";
        for (int i = 0; i < offConnectionCount; i++) {
            std::cout << "\n\t\t\t" << offContinentConnections[i]->countryName;
            std::cout << "\n\t\t\t\tControlled by player [#" << offContinentConnections[i]->GetOwner() << "] : [" << offContinentConnections[i]->armyCount << " armies]";
        }
    }
}

void Country::PrintCountryWithSkip(bool skipOwnedVsUnowned) {
    // Prints the elements of a country.
    // Skips over either owned or unowned connected countries.
    std::cout << "\n\t" << countryName << " : [" << armyCount << " armies]";
    std::cout << "\n\t\tConnected Countries,";
    for (int i = 0; i < connectionCount; i++) {
        if (connectedCountries[i]->GetOwner() == controlledByPlayer) {
            if (skipOwnedVsUnowned) {
                continue;
            }
        }
        else {
            if (!skipOwnedVsUnowned) {
                continue;
            }
        }
        std::cout << "\n\t\t\t" << connectedCountries[i]->countryName << " : [" << connectedCountries[i]->armyCount << " armies]";
    }
    if (offConnectionCount > 0) {
        std::cout << "\n\t\tOff Continent Connected Countries,";
        for (int i = 0; i < offConnectionCount; i++) {
            if (offContinentConnections[i]->GetOwner() == controlledByPlayer) {
                if (skipOwnedVsUnowned) {
                    continue;
                }
            }
            else {
                if (!skipOwnedVsUnowned) {
                    continue;
                }
            }
        std::cout << "\n\t\t\t" << offContinentConnections[i]->countryName << ": [" << offContinentConnections[i]->armyCount << " armies] ";
        }
    }
}