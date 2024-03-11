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
    if (connectionCount > 0) {
        std::cout << "\nAlready Setup Connections\n";
        return;
    }
    connectionCount = count;
    connectedCountries = new Country * [connectionCount];
}

void Country::SetupOffContinentConnections(int count) {
    if (offConnectionCount > 0) {
        std::cout << "\nAlready Setup OffConnections\n";
        return;
    }
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

    return nullptr;
}

void Country::PrintCountry(bool goIntoDetail) {
    std::cout << "\n\t" << countryName << " : [" << armyCount << " armies]";
    if (!goIntoDetail) {
        return;
    }
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

void Country::PrintCountryConnections(bool skipOwnedVsUnowned) {
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