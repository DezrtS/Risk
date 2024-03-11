#include "Player.h"
#include "Map.h"
#include "Useful.h"
#include "RiskGame.h"

class Useful;

RiskGame::RiskGame(int playerCount) : map(new Map), playerCount(playerCount), players(new Player* [playerCount]) {
    SetupPlayers();
    PrintPlayers();
    Useful::ContinueWithEnter();
    map->ReadMapFile("Data Files\\RiskMapInfo.txt");
    map->AllocateCountryOwnership(players, playerCount);
    Useful::ContinueWithEnter();
    std::cout << "\nHere is the map,";
    map->PrintMap();
    Useful::ContinueWithEnter();
}

RiskGame::~RiskGame() {
    delete[] players;
    delete map;
}

void RiskGame::PlayGame() {
    for (int i = 0; i < playerCount; i++) {
        players[i]->PlayTurn(this);
    }
    PlayGame();
}

void RiskGame::SetupPlayers() {
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
    if (defender->GetArmyCount() <= 0) {
        std::cout << "\nYou occupied the country";
        defender->SetOwner(attacker->GetOwner());
        Useful::ContinueWithEnter();
        return;
    }

    Player* attackingPlayer = players[attacker->GetOwner()];
    Player* defendingPlayer = players[defender->GetOwner()];
    std::cout << "\nCountry " << attacker->GetCountryName() << " owned by " << attackingPlayer->GetName() << " is attacking " << defender->GetCountryName() << " owned by " << defendingPlayer->GetName();
    Useful::ContinueWithEnter();
    std::cout << "\nStarting out with " << attackingPlayer->GetName() << ", you roll 1 die";
    int attackerRollCount = 1;
    while (true) {
        int attackerDie = Useful::RollDice(1);
        while (Useful::YesOrNo("Would you like to roll another die for a chance at a better number?")) {
            attackerDie = std::max(attackerDie, Useful::RollDice(1));
            attackerRollCount++;
            if (attackerRollCount >= 3) {
                std::cout << "\nYou cannot roll anymore die";
            }
        }
        Useful::ContinueWithEnter();
        std::cout << "\nNow it's " << attackingPlayer->GetName() << " turn, you roll 1 die";
        int rollCount = 1;
        int defenderDie = Useful::RollDice(1);
        while (Useful::YesOrNo("Would you like to roll another die for a chance at a better number?")) {
            defenderDie = std::max(defenderDie, Useful::RollDice(1));
            rollCount++;
            if (rollCount >= 2) {
                std::cout << "\nYou cannot roll anymore die";
            }
        }
        Useful::ContinueWithEnter();
        std::cout << "\nThe highest roll for the attacker is " << attackerDie << " and the highest roll for the defender is " << defenderDie;
        if (defenderDie >= attackerDie) {
            std::cout << "\nThe attacker loses an army";
            attacker->AdjustArmyCount(-1);
            if (attacker->GetArmyCount() <= 0) {
                std::cout << "\nThe attacker has exhausted all of their armies";
                Useful::ContinueWithEnter();
                return false;
            }
        }
        else {
            std::cout << "\nThe defender loses an army";
            defender->AdjustArmyCount(-1);
            if (defender->GetArmyCount() <= 0) {
                std::cout << "\nThe defender has exhausted all of their armies";
                Useful::ContinueWithEnter();
                defender->SetOwner(attacker->GetOwner());
                // Add functionality to take into account attacking country armies
                std::cout << "\nSince you won, and used " << attackerRollCount << " die to win against the defender, you have to deploy " << attackerRollCount << " armies to " << defender->GetCountryName();
                if (attackingPlayer->GetArmyCount() >= attackerRollCount) {
                    attackingPlayer->AddArmies(-attackerRollCount);
                    defender->AdjustArmyCount(attackerRollCount);
                }
                else {
                    std::cout << "\nYou do not have enough armies to spare, so, " << defender->GetCountryName() << " is left undefended";
                }
                return true;
            }
        }
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