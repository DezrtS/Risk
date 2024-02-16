// -------------------------------------------------------------------- //
// Authors:    Denzil Walrond,     Christian Zambri,    Michael Atteh   //
// Student#:   100868217,          100787919,           100831528       //
// Date: 02/16/2024                                                     //
// -------------------------------------------------------------------- //

// Header and Namespace Imports
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// --------------------------------------------------------------------------------------------------------- //
// Extra Methodsd                                                                                            //
// --------------------------------------------------------------------------------------------------------- //

/**
 * @brief Splits a given string into substrings based on a specified delimiter.
 *
 * This static method takes a source string and a delimiter, and breaks the source
 * string into substrings wherever the delimiter occurs. The resulting substrings
 * are stored in a vector, which is then returned.
 *
 * @param str The source string to be split.
 * @param delimiter The string that acts as a delimiter for splitting the source string.
 *
 * @return A vector of strings containing the substrings obtained by splitting the source string.
 */
static vector<string> SplitString(const string& str, const string& delimiter) {
    vector<string> result;

    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    result.push_back(str.substr(start));

    return result;
}

/**
 * @brief Prints out the elements of a vector of strings to the console.
 *
 * This static method takes a vector of strings and loops through each element 
 * in the vector, printing out a new line followed by the element at the loop index
 *
 * @param vec the vector to be printed out.
 */
static void PrintVector(vector<string>& vec) {
    for (int i = 0; i < vec.size(); i++) {
        cout << "\n" << vec[i];
    }
}

// --------------------------------------------------------------------------------------------------------- //
// Classes                                                                                                   //
// --------------------------------------------------------------------------------------------------------- //

/**
 * @brief Represents a player in the game.
 *
 * The Player class encapsulates information about a player, including their
 * name and player number. It provides a method to print the player's details.
 */
class Player {
private:
    // Private Player variables
    string playerName;
    int playerNum;
public:
    /**
     * @brief Constructs a Player object with the given name and player number.
     *
     * @param name The name of the player.
     * @param num The player number assigned to the player.
     */
    Player(string name, int num) : playerName(name), playerNum(num) {}

    /**
     * @brief Prints the details of the player.
     *
     * This method outputs the player's name and player number.
     */
    void PrintPlayer() const {
        cout << "\n" << playerName << " is Player #" << playerNum;
    }
};


class Country {
private:
    // Private Country variables
    int connectionCount = 0;
    int offConnectionCount = 0;
    string countryName;
    Country** connectedCountries;
    Country** offContinentConnections;
    Continent* parentContinent;

    int armyCount = 0;
    int controlledBy = 0;
public:
    Country(Continent* continent) : parentContinent(continent) {}

    ~Country() {
        delete[] connectedCountries;
        delete[] offContinentConnections;
    }

    void SetupCountry(string& name) {
        countryName = name;
    }

    void SetupConnections(int count) {
        if (connectionCount > 0) {
            cout << "\nAlready Setup Connections\n";
            return;
        }
        connectionCount = count;
        connectedCountries = new Country * [connectionCount];
    }

    void SetupOffContinentConnections(int count) {
        if (offConnectionCount > 0) {
            cout << "\nAlready Setup OffConnections\n";
            return;
        }
        offConnectionCount = count;
        offContinentConnections = new Country * [offConnectionCount];
    }

    void AddConnection(int index, Country* country) {
        connectedCountries[index] = country;
    }

    void AddOffContinentConnection(int index, Country* country) {
        offContinentConnections[index] = country;
    }

    void PrintCountry() const {
        cout << "\n     " << countryName;
        cout << "\n         Connected Countries,";
        for (int i = 0; i < connectionCount; i++) {
            cout << "\n                 " << connectedCountries[i]->countryName;
        }
        if (offConnectionCount > 0) {
            cout << "\n         Off Continent Connected Countries,";
            for (int i = 0; i < offConnectionCount; i++) {
                cout << "\n                 " << offContinentConnections[i]->countryName;
            }
        }
    }
};

class Continent {
private:
    bool enabled = true;
    int countryCount = 0;
    string continentName;
    Country** countries;

    int controlArmyBonus = 0;
    int controlledBy = 0;
public:
    ~Continent() {
        delete[] countries;
    }

    void SetupContinent(string name, int count) {
        if (countryCount > 0) {
            cout << "\nAlready Setup Continent\n";
            return;
        }
        countryCount = count;
        continentName += name;
        countries = new Country*[countryCount];
    }
    
    Country* GetCountry(int index) const {
        return countries[index];
    }

    void SetCountry(int index, Country* country) {
        countries[index] = country;
    }

    void SetIsEnabled(bool enabled) {
        this->enabled = enabled;
    }

    bool IsEnabled() const {
        return enabled;
    }

    void PrintContinent() const {
        if (!enabled) {
            return;
        }

        cout << "\n\n" << continentName;
        cout << "\nListed Countries,";
        for (int i = 0; i < countryCount; i++) {
            countries[i]->PrintCountry();
        }
    }
};

class Map {
private:
    int continentCount = 0;
    Continent** continents;

public:
    ~Map() {
        delete[] continents;
    }

    void SetupMap(int count) {
        if (continentCount > 0) {
            cout << "\nAlready Setup Map\n";
            return;
        }
        continentCount = count;
        continents = new Continent*[continentCount];
    }

    void ReadMapFile(string path) {
        string line;
        ifstream dataFile(path);
        if (dataFile.is_open()) {
            try {
                getline(dataFile, line);
                int continentCount = stoi(line);
                SetupMap(continentCount);
                
                for (int i_a = 0; i_a < continentCount; i_a++) {
                    getline(dataFile, line);
                    string continentName = line;
                    Continent* continent = new Continent();
                    continents[i_a] = continent;
                    getline(dataFile, line);
                    int countryCount = stoi(line);
                    continent->SetupContinent(continentName, countryCount);
                    
                    getline(dataFile, line);
                    vector<string> countries = SplitString(line, ",");
                    
                    for (int i_b = 0; i_b < countryCount; i_b++) {
                        Country* country = new Country(continent);
                        country->SetupCountry(countries[i_b]);
                        continent->SetCountry(i_b, country);
                    }

                    for (int i_b = 0; i_b < countryCount; i_b++) {
                        getline(dataFile, line);
                        vector<string> connections = SplitString(line, ":");
                        vector<string> indexes = SplitString(connections[1], ",");
                        int countryIndex = stoi(connections[0]);
                        continents[i_a]->GetCountry(countryIndex)->SetupConnections(indexes.size());
                        
                        for (int i_c = 0; i_c < indexes.size(); i_c++) {
                            continents[i_a]->GetCountry(countryIndex)->AddConnection(i_c, continents[i_a]->GetCountry(stoi(indexes[i_c])));
                        }
                    }
                }

                while (getline(dataFile, line)) {
                    vector<string> connections = SplitString(line, ":");
                    vector<string> indexes = SplitString(connections[0], ".");
                    int continentIndex = stoi(indexes[0]);
                    int countryIndex = stoi(indexes[1]);
                    vector<string> connectionCountryIndexes = SplitString(connections[1], ",");
                    continents[continentIndex]->GetCountry(countryIndex)->SetupOffContinentConnections(connectionCountryIndexes.size());
                    for (int i = 0; i < connectionCountryIndexes.size(); i++) {
                        vector<string> offConnectionIndex = SplitString(connectionCountryIndexes[i], ".");
                        continents[continentIndex]->GetCountry(countryIndex)->AddOffContinentConnection(i, continents[stoi(offConnectionIndex[0])]->GetCountry(stoi(offConnectionIndex[1])));
                    }
                }
            }
            catch (...) {
                cout << "\nError Reading File\n";
                exit(1);
            }
        }
    }

    void PrintMap() const {
        for (int i = 0; i < continentCount; i++) {
            continents[i]->PrintContinent();
        }
    }
};

class RiskGame {
private:
    int playerCount;
    Player** players;
    Map map;

public:
    RiskGame(int playerCount) : playerCount(playerCount), players(new Player*[playerCount]) {
        SetupPlayers();
        PrintPlayers();
        map.ReadMapFile("Data Files\\RiskMapInfo.txt");
        map.PrintMap();
    }

    ~RiskGame() {
        delete[] players;
    }

    void SetupPlayers() {
        for (int i = 0; i < playerCount; i++) {
            cout << "\nEnter the name of player #" << i + 1 << "\n";
            string playerName;
            cin >> playerName;
            players[i] = new Player(playerName, i + 1);
        }
    }

    void PrintPlayers() {
        for (int i = 0; i < playerCount; i++) {
            players[i]->PrintPlayer();
        }
    }
};

// --------------------------------------------------------------------------------------------------------- //
// Main Function                                                                                             //
// --------------------------------------------------------------------------------------------------------- //

int main()
{
    int playerCount = 0;
    while (playerCount <= 0) {
        cout << "How many players are playing? (2-6)\n";
        try {
            string input;
            cin >> input;
            playerCount = stoi(input);

            if (playerCount < 2 || playerCount > 6) {
                cout << "\nThe amount of players is outside of the allowed range\n";
                playerCount = 0;
            }
        }
        catch (...) {
            cout << "\nInvalid Input\n";
            playerCount = 0;
        }
    }

    RiskGame riskGame = RiskGame(playerCount);

    return 0;
}