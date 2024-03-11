#include "Continent.h"

#include <iostream>

Continent::~Continent() {
    delete[] countries;
}

void Continent::SetupContinent(std::string name, int count, int armyBonus) {
    if (countryCount > 0) {
        std::cout << "\nAlready Setup Continent\n";
        return;
    }
    continentName = name;
    countryCount = count;
    countries = new Country * [countryCount];
    controlArmyBonus = armyBonus;
}

bool Continent::CheckIfContinentOwned() {
    if (countryCount == 0) {
        return false;
    }

    int ownedBy = countries[0]->GetOwner();

    for (int i = 1; i < countryCount; i++) {
        if (ownedBy != countries[i]->GetOwner()) {
            return false;
        }
    }

    return true;
}

float Continent::AllocateArmies(int playerId) {
    float armies = 0;

    for (int i = 0; i < countryCount; i++) {
        if (countries[i]->GetOwner() == playerId) {
            armies++;
        }
    }
    
    armies /= 3;

    if (controlledBy == playerId) {
        armies += controlArmyBonus;
    }

    return armies;
}

void Continent::PrintOwnedCountries(int playerId) {
    std::cout << "\nWithin the continent " << continentName << ",";
    bool ownsSomething = false;
    for (int i = 0; i < countryCount; i++) {
        if (countries[i]->GetOwner() == playerId) {
            ownsSomething = true;
            countries[i]->PrintCountry(false);
        }
    }

    if (!ownsSomething) {
        std::cout << "\n\t[Nothing]";
    }
}

int Continent::GetOwnedCountry(int playerId, int* num) {
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

    std::cout << "\n\n" << continentName << " (Army Bonus: " << controlArmyBonus << ")";
    std::cout << "\nListed Countries,";
    for (int i = 0; i < countryCount; i++) {
        countries[i]->PrintCountry(true);
    }
}