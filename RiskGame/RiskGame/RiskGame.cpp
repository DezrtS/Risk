#include "Player.h"
#include "Map.h"
#include "Useful.h"
#include "RiskGame.h"

class Useful;

RiskGame::RiskGame(int playerCount) : map(new Map), playerCount(playerCount), players(new Player* [playerCount]) {
    // Setups up and prints out the players.
    SetupPlayers();
    PrintPlayers();
    Useful::ContinueWithEnter();
    // Reads the map data from a file.
    map->ReadMapFile("Data Files\\RiskMapInfo.txt");
    // Allocates country ownership to the players.
    map->AllocateCountryOwnership(players, playerCount);
    Useful::ContinueWithEnter();
    // Prints out the map.
    std::cout << "Here is the map,";
    map->PrintMap();
    Useful::ContinueWithEnter();
}

RiskGame::~RiskGame() {
    delete[] players;
    delete map;
}

void RiskGame::PlayGame() {
    // Plays the game.
    while (true) {
        eliminatedPlayers = 0;
        for (int i = 0; i < playerCount; i++) {
            // Cycles through all the players and plays their turn.
            if (!players[i]->HasLost()) {
                players[i]->PlayTurn(this);
            }
            else {
                eliminatedPlayers++;
            }
        }
        // Checks to see if there is only one player left.
        if (eliminatedPlayers == playerCount - 1) {
            Useful::ContinueWithoutEnter();
            // Prints out that the game has ended.
            std::cout << "\nTHE GAME HAS ENDED";
            std::cout << "\nA Player has won.";
            return;
        }
    }
}

void RiskGame::SetupPlayers() {
    // Sets up player names and IDs.
    for (int i = 0; i < playerCount; i++) {
        std::cout << "\nEnter the name of player #" << i + 1 << "\n";
        std::string playerName;
        std::cin >> playerName;
        players[i] = new Player(playerName, i);
    }
}

void RiskGame::AllocateArmies(int playerId) {
    players[playerId]->AddArmies(map->AllocateArmies(playerId));
}

void RiskGame::PrintOwnedCountries(int playerId) {
    map->PrintOwnedCountries(playerId);
}

bool RiskGame::AttackCountry(Country* attacker, Country* defender) {
    Useful::ContinueWithoutEnter();
    // Gets the attacking player.
    Player* attackingPlayer = players[attacker->GetOwner()];
    Player* defendingPlayer;

    // Checks to see if the defending country is undefended.
    if (defender->GetArmyCount() <= 0) {
        // If the country is owned, print out who it was taken by.
        if (defender->GetOwner() >= 0) {
            defendingPlayer = players[defender->GetOwner()];
            std::cout << "\nThe country was owned by " << defendingPlayer->GetName() << ", but they left it undefended";
            defendingPlayer->AdjustOwnedCountries(-1);
        }
        // Assigned the defending country to the attacker.
        std::cout << "\nYou occupied the country";
        defender->SetOwner(attacker->GetOwner());
        attackingPlayer->AdjustOwnedCountries(1);
        defender->GetParentContinent()->CheckIfContinentOwned(true);
        Useful::ContinueWithEnter();
        return true;
    }

    // Get the defending player.
    defendingPlayer = players[defender->GetOwner()];

    // Prints out who is attacking who.
    std::cout << "\nCountry " << attacker->GetCountryName() << " owned by " << attackingPlayer->GetName() << " is attacking " << defender->GetCountryName() << " owned by " << defendingPlayer->GetName();
    Useful::ContinueWithEnter();
    std::cout << "\nStarting out with " << attackingPlayer->GetName() << ", you roll 1 die";
    int attackerRollCount = 1;
    // While neither the defender nor the attacker has exhausted either of their armies.
    while (true) {
        // Roll the first die.
        int attackerDie = Useful::RollDice(1);
        int tempRollCount = 0;
        // While the attacker can roll more die and chooses to roll more die, keep asking them if they would like to roll a die.
        while (Useful::YesOrNo("Would you like to roll another die for a chance at a better number?")) {
            // Take the larger value from the new roll vs the old roll.
            attackerDie = std::max(attackerDie, Useful::RollDice(1));
            // Increase the amount of die the attacker has rolled until they cannot roll anymore die.
            tempRollCount++;
            if (tempRollCount >= 2) {
                std::cout << "\nYou cannot roll anymore die";
                break;
            }
        }
        attackerRollCount += tempRollCount;
        Useful::ContinueWithEnter();
        // Initiate the defenders turn with their first roll.
        std::cout << "\nNow it's " << defendingPlayer->GetName() << " turn, you roll 1 die";
        int rollCount = 1;
        int defenderDie = Useful::RollDice(1);
        // While the defender can roll more die and chooses to roll more die, keep asking them if they would like to roll a die.
        while (Useful::YesOrNo("Would you like to roll another die for a chance at a better number?") && rollCount < attacker->GetArmyCount()) {
            // Take the larger value from the new roll vs the old roll.
            defenderDie = std::max(defenderDie, Useful::RollDice(1));
            // Limit the amount of dice the defender can roll.
            rollCount++;
            if (rollCount >= 2) {
                std::cout << "\nYou cannot roll anymore die";
                break;
            }
        }
        Useful::ContinueWithEnter();
        // Print out who got a higher roll value.
        std::cout << "\nThe highest roll for the attacker is " << attackerDie << " and the highest roll for the defender is " << defenderDie;
        // If the defender won, the attacker loses an army.
        if (defenderDie >= attackerDie) {
            std::cout << "\nThe attacker loses an army";
            attacker->AdjustArmyCount(-1);
            // If the attacker has exhausted all of their armies, the attack ends.
            if (attacker->GetArmyCount() <= 0) {
                std::cout << "\nThe attacker has exhausted all of their armies";
                Useful::ContinueWithEnter();
                return false;
            }
        }
        // If the attacker won, the defender loses an army.
        else {
            // Take away an army from the defending country.
            std::cout << "\nThe defender loses an army";
            defender->AdjustArmyCount(-1);
            // If the defender has exhausted all of their armies, they lose the country and the attack ends.
            if (defender->GetArmyCount() <= 0) {
                std::cout << "\nThe defender has exhausted all of their armies";
                Useful::ContinueWithEnter();
                // Changes ownership of the country.
                defender->SetOwner(attacker->GetOwner());
                attackingPlayer->AdjustOwnedCountries(1);
                defendingPlayer->AdjustOwnedCountries(-1);
                // Checks to see if the attacker has enough armies to deploy to the captured country.
                std::cout << "\nSince you won, and used " << attackerRollCount << " additional die to win against the defender, you have to deploy " << attackerRollCount << " armies to " << defender->GetCountryName();
                if (attackingPlayer->GetArmyCount() >= attackerRollCount) {
                    std::cout << "\nYour armies in reserve were deployed to defend " << defender->GetCountryName();
                    attackingPlayer->AddArmies(-attackerRollCount);
                    defender->AdjustArmyCount(attackerRollCount);
                }
                else if (attackingPlayer->GetArmyCount() + attacker->GetArmyCount() >= attackerRollCount) {
                    std::cout << "\nSome of your armies from " << attacker->GetCountryName() << " moved to aid your armies in reserve in defending " << defender->GetCountryName();
                    int playerArmyCount = attackingPlayer->GetArmyCount();
                    int difference = attackerRollCount - playerArmyCount;
                    attackingPlayer->AddArmies(-playerArmyCount);
                    attacker->AdjustArmyCount(-difference);
                    defender->AdjustArmyCount(attackerRollCount);
                }
                else {
                    std::cout << "\nYou do not have enough armies to spare, so, " << defender->GetCountryName() << " is left undefended";
                }
                // Check if the attacker now owns the continent they captured the defending country from.
                defender->GetParentContinent()->CheckIfContinentOwned(true);
                Useful::ContinueWithEnter();
                return true;
            }
        }
        std::cout << "\n\nThe attacker has " << attacker->GetArmyCount() << " armies remaining";
        std::cout << "\nThe defender has " << defender->GetArmyCount() << " armies remaining";
        Useful::ContinueWithEnter();
    }
}

Country* RiskGame::GetOwnedCountry(int playerId, int num) {
    if (num < 0) {
        return nullptr;
    }

    return map->GetOwnedCountry(playerId, num);
}

void RiskGame::PrintPlayers() {
    for (int i = 0; i < playerCount; i++) {
        players[i]->PrintPlayer();
    }
}