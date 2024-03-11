#include "Player.h"
#include "Useful.h"
#include "Country.h"

#include <iostream>
#include <string>

Player::Player(std::string name, int id) : playerName(name), playerId(id) {}

void Player::AdjustOwnedCountries(int additionalOwnedCountries) {
    ownedCountries += additionalOwnedCountries;
    if (ownedCountries <= 0) {
        hasLost = true;
    }
}

void Player::AddArmies(int armiesToAdd) {
    armies += armiesToAdd;
}

int Player::GetArmyCount() {
    return armies;
}

int Player::GetOwnedCountries() {
    return ownedCountries;
}

std::string Player::GetName() {
    return playerName;
}

bool Player::HasLost() {
    return hasLost;
}

void Player::PlayTurn(RiskGame* game) {
    std::cout << "\n\n" << playerName << " is playing their turn";
    DeployReinforcements(game);
    PerformAttacks(game);
    SetupFortifications(game);
    std::cout << "\n\n" << playerName << " has finished their turn";
    Useful::ContinueWithEnter();
}

void Player::DeployReinforcements(RiskGame* game) {
    std::cout << "\n\n" << playerName << " is beginging the reinforcement stage";
    std::cout << "\nBefore allocation, you had " << armies << " armies";
    game->AllocateArmies(playerId);
    std::cout << "\nAfter allocation, you have " << armies << " armies";
    int exchangeMultiplier = 1;
    while (cards >= 3) {
        std::cout << "\nYou have " << cards << " cards";
        if (Useful::YesOrNo("Would you like to trade in 3 cards to recieve " + std::to_string(5 * exchangeMultiplier) + " armies?")) {
            cards -= 3;
            armies += 5 * exchangeMultiplier;
            exchangeMultiplier++;
            std::cout << "\nYou exchanged 3 cards and raised your total armies up to " << armies << " armies";
        }
        else {
            if (armies < 3) {
                std::cout << "\nA minimum of 3 armies is required, so an exchange has been automatically made.";
                cards -= 3;
                armies += 5 * exchangeMultiplier;
            }
            break;
        }
    }

    Useful::ContinueWithEnter();

    std::cout << "\nMoving onto the placing armies part of the reinforcement stage, here are the countries you currently own,";
    int answer = 0;
    while (armies > 0) {
        game->PrintOwnedCountries(playerId);
        answer = Useful::GetNumber("Enter the index of the country you would like to inspect (-1 to finish)");
        if (answer == -1) {
            Useful::ContinueWithoutEnter();
            break;
        }
        Country* chosenCountry = game->GetOwnedCountry(playerId, answer);
        
        if (chosenCountry == nullptr) {
            Useful::ContinueWithoutEnter();
            std::cout << "\nInvalid country index, try again";
            continue;
        }

        Useful::ContinueWithoutEnter();
        chosenCountry->PrintCountry(true);
        std::cout << "\nYou have " << armies << " armies";
        std::cout << "\nAnd the number of armies on the chosen country are " << chosenCountry->GetArmyCount() << " armies,";
        while (Useful::YesOrNo("Would you like to deploy an army at this country?")) {
            chosenCountry->AdjustArmyCount(1);
            armies--;
            if (armies == 0) {
                break;
            }
            std::cout << "\nYou now have " << armies << " armies,";
            std::cout << "\nAnd the number of armies on the chosen country have raised to " << chosenCountry->GetArmyCount() << " armies,";
        }
        Useful::ContinueWithoutEnter();
    }

    if (armies == 0) {
        std::cout << "\nYou end the reinforcement stage with no more armies to deploy";
    }
    else {
        std::cout << "\nYou end the reinforcement stage with " << armies << " armies left";
    }

    Useful::ContinueWithEnter();
}

void Player::PerformAttacks(RiskGame* game) {
    bool conqueredCountry = false;
    std::cout << "\n\n" << playerName << " is beginging the attack stage";

    int answer = 0;
    while (true) {
        game->PrintOwnedCountries(playerId);
        answer = Useful::GetNumber("Enter the index of the country you would like to inspect with at least 1 army (-1 to finish)");
        if (answer == -1) {
            Useful::ContinueWithoutEnter();
            break;
        }
        Country* chosenCountry = game->GetOwnedCountry(playerId, answer);

        if (chosenCountry == nullptr) {
            Useful::ContinueWithoutEnter();
            std::cout << "\nInvalid country index, try again";
            continue;
        }
        else if (chosenCountry->GetArmyCount() < 1) {
            Useful::ContinueWithoutEnter();
            std::cout << "\nThe chosen country does not have more than 1 army, choose another one";
            continue;
        }

        Useful::ContinueWithoutEnter();
        chosenCountry->PrintCountryConnections(true);

        while (Useful::YesOrNo("Would you attack any of the neighboring countries?")) {
            answer = Useful::GetNumber("Enter the index of the country you would like to attack (-1 to go back)");
            if (answer == -1) {
                break;
            }
            Country* attackedCountry = chosenCountry->GetConnectedCountry(answer, true);
            if (attackedCountry == nullptr) {
                Useful::ContinueWithoutEnter();
                std::cout << "\nInvalid country index, try again";
                chosenCountry->PrintCountryConnections(true);
                continue;
            }
            if (game->AttackCountry(chosenCountry, attackedCountry)) {
                conqueredCountry = true;
            }
            else {
                break;
            }
            chosenCountry->PrintCountryConnections(true);
        }
        Useful::ContinueWithoutEnter();
    }

    if (conqueredCountry) {
        cards++;
    }
}

void Player::SetupFortifications(RiskGame* game) {
    std::cout << "\n\n" << playerName << " is beginging the fortification stage";
    int answer = 0;
    while (true) {
        game->PrintOwnedCountries(playerId);
        answer = Useful::GetNumber("Enter the index of the country you would like to inspect (-1 to finish)");
        if (answer == -1) {
            Useful::ContinueWithoutEnter();
            break;
        }
        Country* chosenCountry = game->GetOwnedCountry(playerId, answer);

        if (chosenCountry == nullptr) {
            Useful::ContinueWithoutEnter();
            std::cout << "\nInvalid country index, try again";
            continue;
        }

        Useful::ContinueWithoutEnter();
        chosenCountry->PrintCountryConnections(false);

        while (Useful::YesOrNo("Would you move some armies to a neighboring country?")) {
            answer = Useful::GetNumber("Enter the index of the country you would like to move armies to (-1 to go back)");
            if (answer == -1) {
                break;
            }
            Country* movingTo = chosenCountry->GetConnectedCountry(answer, false);
            if (movingTo == nullptr) {
                Useful::ContinueWithoutEnter();
                std::cout << "\nInvalid country index, try again";
                chosenCountry->PrintCountryConnections(false);
                continue;
            }

            while (true) {
                answer = Useful::GetNumber("How many armies would you like to move? (-1 to cancel)");

                if (answer == -1) {
                    break;
                }
                if (answer <= 0 || chosenCountry->GetArmyCount() < answer) {
                    Useful::ContinueWithoutEnter();
                    std::cout << "\nInvalid amount, try again";
                    chosenCountry->PrintCountryConnections(false);
                    continue;
                }
                Useful::ContinueWithoutEnter();
                std::cout << "\nMoving " << answer << " armies from " << chosenCountry->GetCountryName() << " to " << movingTo->GetCountryName();
                chosenCountry->AdjustArmyCount(-answer);
                movingTo->AdjustArmyCount(answer);
                Useful::ContinueWithEnter();
                return;
            }
        }
        Useful::ContinueWithoutEnter();
    }
}

void Player::PrintPlayer() {
    std::cout << "\n" << playerName << " is Player #" << playerId << " and controls " << armies << " armies";
}