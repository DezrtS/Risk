#include "Continent.h"

#include <iostream>

Continent::~Continent() {
    delete[] countries;
}

void Continent::SetupContinent(std::string name, int count, int armyBonus) {
    // If there are already countries in this contintent, it has already been setup.
    if (countryCount > 0) {
        std::cout << "\nAlready Setup Continent\n";
        return;
    }
    // Setup continent name, country count, and control army bonus.
    continentName = name;
    countryCount = count;
    countries = new Country * [countryCount];
    controlArmyBonus = armyBonus;
}

bool Continent::CheckIfContinentOwned(bool printResult) {
    // If there are not countries in this continent, return that the continent cannot be owned.
    if (countryCount == 0) {
        return false;
    }

    // Get the owner of the first country.
    int ownedBy = countries[0]->GetOwner();

    // Cycle throught all of the countries, and if the owner of the first country is different from any of the other country owners, return that the continent cannot be owned.
    for (int i = 1; i < countryCount; i++) {
        if (ownedBy != countries[i]->GetOwner()) {
            controlledBy = -1;
            return false;
        }
    }

    if (printResult) {
        std::cout << "\n" << continentName << " has been fully taken over";
    }
    // Change the continent owner.
    controlledBy = ownedBy;
    return true;
}

float Continent::AllocateArmies(int playerId) {
    float armies = 0;

    // Cycle through the countries, and if the given player owns a country, increase the amount of armies they get.
    for (int i = 0; i < countryCount; i++) {
        if (countries[i]->GetOwner() == playerId) {
            armies++;
        }
    }
    
    // Divide the amount of armies by 3 to account for the 1/3 an army for each country owned.
    armies /= 3;

    // If the player owns this continent, give them the control army bonus.
    if (controlledBy == playerId) {
        armies += controlArmyBonus;
        std::cout << "\nAdding owned continent army bonus from" << continentName;
    }

    return armies;
}

void Continent::PrintOwnedCountries(int playerId) {
    // Prints only the player's owned countries.
    std::cout << "\nWithin the continent " << continentName << ",";
    bool ownsSomething = false;
    for (int i = 0; i < countryCount; i++) {
        if (countries[i]->GetOwner() == playerId) {
            ownsSomething = true;
            countries[i]->PrintCountry(false);
        }
    }

    // If they do not own anything, print that they own nothing.
    if (!ownsSomething) {
        std::cout << "\n\t[Nothing]";
    }
}

int Continent::GetOwnedCountry(int playerId, int* num) {
    // Cycle through the owned country of a player until the country they desire is reached.
    for (int i = 0; i < countryCount; i++) {
        if (countries[i]->GetOwner() == playerId) {
            if (*num == 0) {
                return i;
            }
            else {
                *num = *num - 1;
            }
        }
    }
    return -1;
}

Country* Continent::GetCountry(int index) {
    return countries[index];
}

void Continent::SetCountry(int index, Country* country) {
    countries[index] = country;
}

int Continent::GetCountryCount() {
    return countryCount;
}

void Continent::SetIsEnabled(bool enabled) {
    this->enabled = enabled;
}

bool Continent::IsEnabled() {
    return enabled;
}

void Continent::PrintContinent() {
    if (!enabled) {
        return;
    }
    // Prints out the continent name, army bonus, and listed countries under it.
    std::cout << "\n\n" << continentName << " (Army Bonus: " << controlArmyBonus << ")";
    std::cout << "\nListed Countries,";
    for (int i = 0; i < countryCount; i++) {
        countries[i]->PrintCountry(true);
    }
}