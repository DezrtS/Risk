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
    // Starts a player's turn.
    std::cout << playerName << " is playing their turn";
    // Starts the reinforcements stage.
    DeployReinforcements(game);
    // Starts the attack stage.
    PerformAttacks(game);
    // Starts the fortification stage. 
    SetupFortifications(game);
    // Ends a player's turn.
    std::cout << playerName << " has finished their turn";
    Useful::ContinueWithEnter();
}

void Player::DeployReinforcements(RiskGame* game) {
    std::cout << "\n\n" << playerName << " is beginging the reinforcement stage";
    // Allocates and prints out the player's armies.
    std::cout << "\nBefore allocation, you had " << armies << " armies";
    game->AllocateArmies(playerId);
    std::cout << "\nAfter allocation, you have " << armies << " armies\n";

    // Prints and counts up the cards and determines if the player has 3 distinct cards, or a total of 5 cards required to make a trade.
    PrintCards();
    bool has3DifferentCards = true;
    int cardCount = 0;
    for (int i = 0; i < 3; i++) {
        cardCount += cards[i];
        if (cards[i] <= 0) {
            has3DifferentCards = false;
        }
    }

    // Counts up the cards and determines if the player has 3 of the same cards.
    bool has3SameCards = true;
    for (int i = 0; i < 3; i++) {
        if (cards[i] <= 3) {
            has3SameCards = true;
            break;
        }
    }

    // If a player holds 5 or more cards, automatically trade them in for armies.
    if (cardCount >= 5) {
        has3DifferentCards = false;
        for (int i = 0; i < 3; i++) {
            cards[i] = 0;
        }
        std::cout << "\nYou can only hold a maximum of 5 cards, so an exchange has been automatically made.";
        armies += 5 * exchangeMultiplier;
        exchangeMultiplier++;
        std::cout << "\n5 cards were exchanged to raise your total armies up to " << armies << " armies";
    }

    // If the player has 3 distinct cards or 3 of the same cards, give them the option to make a trade.
    if (has3DifferentCards || has3SameCards) {
        if (Useful::YesOrNo("Would you like to trade in 3 cards to recieve " + std::to_string(5 * exchangeMultiplier) + " armies?")) {
            if (has3DifferentCards) {
                for (int i = 0; i < 3; i++) {
                    cards[i]--;
                }
            }
            else if (has3SameCards) {
                for (int i = 0; i < 3; i++) {
                    if (cards[i] >= 3) {
                        cards[i] -= 3;
                    }
                }
            }

            armies += 5 * exchangeMultiplier;
            exchangeMultiplier++;
            std::cout << "\nYou exchanged 3 cards and raised your total armies up to " << armies << " armies";
        }
        else {
            // If a player passes on a trade but has less than 3 armies, an exchange is automatically made for them.
            if (armies < 3) {
                std::cout << "\nA minimum of 3 armies is required, so an exchange has been automatically made.";
                if (has3DifferentCards) {
                    for (int i = 0; i < 3; i++) {
                        cards[i]--;
                    }
                }
                else if (has3SameCards) {
                    for (int i = 0; i < 3; i++) {
                        if (cards[i] >= 3) {
                            cards[i] -= 3;
                        }
                    }
                }
                armies += 5 * exchangeMultiplier;
            }
        }
    }

    Useful::ContinueWithEnter();

    // Enters the placing armies stage of the reinforcement stage.
    std::cout << "Moving onto the placing armies part of the reinforcement stage, here are the countries you currently own,";
    int answer = 0;
    // While the player still has armies to place, keep asking them if they want to place armies.
    while (armies > 0) {
        // Print the player's owned countries and allow them to inspect one.
        game->PrintOwnedCountries(playerId);
        answer = Useful::GetNumber("Enter the index of the country you would like to inspect (-1 to finish)");
        if (answer == -1) {
            Useful::ContinueWithoutEnter();
            break;
        }
        Country* chosenCountry = game->GetOwnedCountry(playerId, answer);
        
        // Handles invalid inputs.
        if (chosenCountry == nullptr) {
            Useful::ContinueWithoutEnter();
            std::cout << "\nInvalid country index, try again";
            continue;
        }

        Useful::ContinueWithoutEnter();
        // Print the country details the the amount of armies the player has to place.
        chosenCountry->PrintCountry(true);
        std::cout << "\nYou have " << armies << " armies";
        std::cout << "\nAnd the number of armies on the chosen country are " << chosenCountry->GetArmyCount() << " armies,";
        // Ask the player if they would like to deploy any armies, and if so, remove 1 army from their reserve and add it to the country.
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

    // Prints out whether the player deployed any armies during the reinforcement stage of their turn.
    if (armies == 0) {
        std::cout << "You ended the reinforcement stage with no more armies to deploy";
    }
    else {
        std::cout << "You ended the reinforcement stage with " << armies << " armies left";
    }

    Useful::ContinueWithEnter();
}

void Player::PerformAttacks(RiskGame* game) {
    bool conqueredCountry = false;
    std::cout << playerName << " is beginging the attack stage";

    // While the player still wants to attack other countries, keep asking them which country they would like to attack.
    int answer = 0;
    while (true) {
        // Print the player's owned countries and allow them to inspect one with at least 1 army.
        game->PrintOwnedCountries(playerId);
        answer = Useful::GetNumber("Enter the index of the country you would like to inspect with at least 1 army (-1 to finish)");
        if (answer == -1) {
            Useful::ContinueWithoutEnter();
            break;
        }
        Country* chosenCountry = game->GetOwnedCountry(playerId, answer);

        // Handles invalid inputs.
        if (chosenCountry == nullptr) {
            Useful::ContinueWithoutEnter();
            std::cout << "Invalid country index, try again";
            continue;
        }
        else if (chosenCountry->GetArmyCount() < 1) {
            Useful::ContinueWithoutEnter();
            std::cout << "The chosen country does not have more than 1 army, choose another one";
            continue;
        }

        Useful::ContinueWithoutEnter();
        chosenCountry->PrintCountryWithSkip(true);

        // Ask the player if they would like to attack a neighbouring country
        while (Useful::YesOrNo("Would you attack any of the neighboring countries?")) {
            answer = Useful::GetNumber("Enter the index of the country you would like to attack (-1 to go back)");
            if (answer == -1) {
                break;
            }
            // Get the chosen country the player asked for.
            Country* attackedCountry = chosenCountry->GetConnectedCountry(answer, true);
            // Handles invalid input.
            if (attackedCountry == nullptr) {
                Useful::ContinueWithoutEnter();
                std::cout << "\nInvalid country index, try again";
                chosenCountry->PrintCountryWithSkip(true);
                continue;
            }

            // Initiate an attack between the country the player chose to inspect, and the neighbouring country they chose to attack.
            if (game->AttackCountry(chosenCountry, attackedCountry)) {
                conqueredCountry = true;
            }
            else {
                break;
            }
            chosenCountry->PrintCountryWithSkip(true);
        }
        Useful::ContinueWithoutEnter();
    }
    // If the player conquered a country during their turn, give them a random type of card.
    if (conqueredCountry) {
        cards[rand() % 3]++;
    }

    // Prints out whether the player conquered any countries during the attack stage of their turn.
    if (conqueredCountry) {
        std::cout << "You ended the attack stage with at least 1 country conquered";
    }
    else {
        std::cout << "You ended the attack stage without conquring a country";
    }
    Useful::ContinueWithEnter();
}

void Player::SetupFortifications(RiskGame* game) {
    std::cout << playerName << " is beginging the fortification stage";
    int answer = 0;
    // While the player is still able to move, or wants to mvoe their armies between countries, keep asking them which country they would like to move armies from.
    while (true) {
        // Print the player's owned countries and allow them to inspect one.
        game->PrintOwnedCountries(playerId);
        answer = Useful::GetNumber("Enter the index of the country you would like to inspect (-1 to finish)");
        if (answer == -1) {
            Useful::ContinueWithoutEnter();
            break;
        }
        Country* chosenCountry = game->GetOwnedCountry(playerId, answer);

        // Handles invalid inputs.
        if (chosenCountry == nullptr) {
            Useful::ContinueWithoutEnter();
            std::cout << "Invalid country index, try again";
            continue;
        }

        Useful::ContinueWithoutEnter();
        chosenCountry->PrintCountryWithSkip(false);

        // Ask the player if they would like to move armies to a neighbouring country.
        while (Useful::YesOrNo("Would you move some armies to a neighboring country?")) {
            answer = Useful::GetNumber("Enter the index of the country you would like to move armies to (-1 to go back)");
            if (answer == -1) {
                break;
            }
            // Get the chosen country the player is moving to.
            Country* movingTo = chosenCountry->GetConnectedCountry(answer, false);
            // Handles invalid input.
            if (movingTo == nullptr) {
                Useful::ContinueWithoutEnter();
                std::cout << "Invalid country index, try again";
                chosenCountry->PrintCountryWithSkip(false);
                continue;
            }

            // While the player has not moved or cancelled their movement, keep asking them how many armies they want to move.
            while (true) {
                answer = Useful::GetNumber("How many armies would you like to move? (-1 to cancel)");

                if (answer == -1) {
                    break;
                }
                // Handles invalid input.
                if (answer <= 0 || chosenCountry->GetArmyCount() < answer) {
                    Useful::ContinueWithoutEnter();
                    std::cout << "Invalid amount, try again";
                    chosenCountry->PrintCountryWithSkip(false);
                    continue;
                }
                Useful::ContinueWithoutEnter();
                // Moves armies from one country to another.
                std::cout << "Moving " << answer << " armies from " << chosenCountry->GetCountryName() << " to " << movingTo->GetCountryName();
                chosenCountry->AdjustArmyCount(-answer);
                movingTo->AdjustArmyCount(answer);
                Useful::ContinueWithEnter();
                return;
            }
        }
        Useful::ContinueWithoutEnter();
    }
    
    // Prints out whether the player moved any armies during the fortification stage of their turn.
    if (answer != -1) {
        std::cout << "You ended the fortification stage after moving at least 1 army";
    }
    else {
        std::cout << "You ended the fortification stage without moving any armies";
    }
    Useful::ContinueWithEnter();
}

void Player::PrintPlayer() {
    std::cout << "\n" << playerName << " is Player #" << playerId << " and controls " << armies << " armies";
}

void Player::PrintCards() {
    // Prints out each kind of card.
    std::cout << "\nYou have " << cards[0] << " Infantry card(s)";
    std::cout << "\nYou have " << cards[1] << " Cavalry card(s)";
    std::cout << "\nYou have " << cards[2] << " Artillery card(s)";
}