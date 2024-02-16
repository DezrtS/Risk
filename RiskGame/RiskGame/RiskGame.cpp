// -------------------------------------------------------------------- //
// Title: C++ Programming Assignment 1                                  //
// Date: 02/16/2024                                                     //
// Authors:    Denzil Walrond,     Christian Zambri,    Michael Atteh   //
// Student#:   100868217,          100787919,           100831528       //
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
 * @param vec The vector to be printed out.
 */
static void PrintVector(vector<string>& vec) {
    for (int i = 0; i < vec.size(); i++) {
        cout << "\n" << vec[i];
    }
}

// --------------------------------------------------------------------------------------------------------- //
// Classes                                                                                                   //
// --------------------------------------------------------------------------------------------------------- //

class Continent;

/**
 * @brief Represents a player in the game.
 *
 * The Player class encapsulates information about a player, including their
 * name and player number. It provides a method to print the player's details.
 */
class Player {
private:
    // Self explanitory private Player variables
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

/**
 * @brief Represents a country in the game.
 *
 * The Country class encapsulates information about a country, including its name,
 * connections to other countries within and outside its continent, and details
 * related to its army and control status. It provides methods for setting up
 * connections, adding connections, and printing information about the country.
 */
class Country {
private:
    // The number of connections to countries within the same continent.
    int connectionCount = 0;
    // The number of connections to countries outside of this country's continent.
    int offConnectionCount = 0;
    string countryName;
    // Array of pointers to the connected countries within the same continent.
    Country** connectedCountries;
    // Array of pointers to the connected countries outside of this country's continent.
    Country** offContinentConnections;
    // The continent this country belongs to.
    Continent* parentContinent;

    // The number of armies in this country.
    int armyCount = 0;
    // The player ID this country is controlled by.
    int controlledBy = 0;
public:
    /**
     * @brief Constructs a Country object within the specified continent.
     *
     * @param continent Pointer to the continent to which the country belongs.
     */
    Country(Continent* continent) : parentContinent(continent) {}

    /**
     * @brief Destructor for the Country class.
     *
     * Releases memory allocated for connectedCountries and offContinentConnections arrays.
     */
    ~Country() {
        delete[] connectedCountries;
        delete[] offContinentConnections;
    }

    /**
     * @brief Sets up the country with the given name.
     *
     * @param name The name of the country.
     */
    void SetupCountry(string& name) {
        countryName = name;
    }

    /**
     * @brief Sets up connections to other countries within the same continent.
     *
     * @param count The number of connections to set up.
     */
    void SetupConnections(int count) {
        if (connectionCount > 0) {
            cout << "\nAlready Setup Connections\n";
            return;
        }
        connectionCount = count;
        connectedCountries = new Country * [connectionCount];
    }

    /**
     * @brief Sets up connections to countries outside the continent.
     *
     * @param count The number of off-continent connections to set up.
     */
    void SetupOffContinentConnections(int count) {
        if (offConnectionCount > 0) {
            cout << "\nAlready Setup OffConnections\n";
            return;
        }
        offConnectionCount = count;
        offContinentConnections = new Country * [offConnectionCount];
    }

    /**
     * @brief Adds a connection to another country within the same continent.
     *
     * @param index The index of the connection.
     * @param country Pointer to the country to be connected.
     */
    void AddConnection(int index, Country* country) {
        connectedCountries[index] = country;
    }

    /**
     * @brief Adds a connection to a country outside the continent.
     *
     * @param index The index of the off-continent connection.
     * @param country Pointer to the off-continent country to be connected.
     */
    void AddOffContinentConnection(int index, Country* country) {
        offContinentConnections[index] = country;
    }

    /**
     * @brief Prints information about the country, including its connections.
     */
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

/**
 * @brief Represents a continent in the game.
 *
 * The Continent class encapsulates information about a continent, including its name,
 * a list of associated countries, and details related to control status and army bonuses.
 * It provides methods for setting up the continent, accessing its countries, and printing
 * information about the continent.
 */
class Continent {
private:
    // Boolean indicating if the continent is active in the game.
    bool enabled = true;
    // The number of countries in this continent.
    int countryCount = 0;
    string continentName;
    // Array of pointers to the countries within this continent.
    Country** countries;

    // The amount of bonus armies granted for controlling the entire continent.
    int controlArmyBonus = 0;
    // The player ID this continent is controlled by.
    int controlledBy = 0;
public:
    /**
     * @brief Destructor for the Continent class.
     *
     * Releases memory allocated for the countries array.
     */
    ~Continent() {
        delete[] countries;
    }

    /**
     * @brief Sets up the continent with the given name and country count.
     *
     * @param name The name of the continent.
     * @param count The number of countries in the continent.
     */
    void SetupContinent(string name, int count, int armyBonus) {
        if (countryCount > 0) {
            cout << "\nAlready Setup Continent\n";
            return;
        }
        continentName = name;
        countryCount = count;
        countries = new Country*[countryCount];
        controlArmyBonus = armyBonus;
    }
    
    /**
     * @brief Gets a pointer to the country at the specified index within the continent.
     *
     * @param index The index of the country within the continent.
     * @return Pointer to the Country object.
     */
    Country* GetCountry(int index) const {
        return countries[index];
    }

    /**
     * @brief Sets the country at the specified index within the continent.
     *
     * @param index The index of the country within the continent.
     * @param country Pointer to the Country object.
     */
    void SetCountry(int index, Country* country) {
        countries[index] = country;
    }

    /**
     * @brief Sets the enabled/disabled status of the continent.
     *
     * @param enabled Boolean indicating whether the continent is enabled (true) or disabled (false).
     */
    void SetIsEnabled(bool enabled) {
        this->enabled = enabled;
    }

    /**
     * @brief Checks if the continent is currently enabled.
     *
     * @return True if the continent is enabled, false otherwise.
     */
    bool IsEnabled() const {
        return enabled;
    }

    /**
     * @brief Prints information about the continent, including its listed countries.
     *
     * If the continent is disabled, nothing will be printed.
     */
    void PrintContinent() const {
        if (!enabled) {
            return;
        }

        cout << "\n\n" << continentName << " (Army Bonus: " << controlArmyBonus << ")";
        cout << "\nListed Countries,";
        for (int i = 0; i < countryCount; i++) {
            countries[i]->PrintCountry();
        }
    }
};

/**
 * @brief Represents the map of the game.
 *
 * The Map class encapsulates information about a map, including continents and their
 * countries, as well as methods for setting up the map, reading map data from a file,
 * and printing information about the map.
 */
class Map {
private:
    // The number of continents in the map.
    int continentCount = 0;
    // Array of pointers to the continents within the map.
    Continent** continents;
public:
    /**
     * @brief Destructor for the Map class.
     *
     * Releases memory allocated for the continents array.
     */
    ~Map() {
        delete[] continents;
    }

    /**
     * @brief Sets up the map with the given number of continents.
     *
     * @param count The number of continents to set up.
     */
    void SetupMap(int count) {
        if (continentCount > 0) {
            cout << "\nAlready Setup Map\n";
            return;
        }
        continentCount = count;
        continents = new Continent*[continentCount];
    }

    /**
     * @brief Reads map data from a specified file and sets up the map accordingly.
     *
     * The file follows a specific format:
     * - First line: Total number of continents in the map.
     * - Subsequent lines for each continent:
     *   - Continent name and control army bonus seperated by a comma: "continentName,controlArmyBonus"
     *   - Number of countries in the continent.
     *   - Comma-separated list of country names.
     *   - For each country:
     *     - Line specifying connections in the format: "CountryIndex:ConnectedCountryIndexes".
     * - Remaining lines specifying off-continent connections in the format: "ContinentIndex.CountryIndex:ConnectedCountryIndexes".
     *
     * @param path The path to the map data file.
     */
    void ReadMapFile(string path) {
        string line;
        // Create a ifstream object and check to see if the stream is open.
        ifstream dataFile(path);
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
                    vector<string> continentDetails = SplitString(line, ",");
                    string continentName = continentDetails[0];
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
                    vector<string> countries = SplitString(line, ",");
                    
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
                        vector<string> connectionDetails = SplitString(line, ":");
                        vector<string> connectionIndexes = SplitString(connectionDetails[1], ",");
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
                    vector<string> connectionDetails = SplitString(line, ":");
                    vector<string> indexes = SplitString(connectionDetails[0], ".");
                    int continentIndex = stoi(indexes[0]);
                    int countryIndex = stoi(indexes[1]);
                    vector<string> connectionIndexes = SplitString(connectionDetails[1], ",");
                    // Setup the off continent connections by passing in the amount of off continent connections to make.
                    continents[continentIndex]->GetCountry(countryIndex)->SetupOffContinentConnections(connectionIndexes.size());
                    
                    // Loop through each connection index and add the country it points to, to the other country
                    for (int i = 0; i < connectionIndexes.size(); i++) {
                        vector<string> offContinentConnectionIndex = SplitString(connectionIndexes[i], ".");
                        continents[continentIndex]->GetCountry(countryIndex)->AddOffContinentConnection(i, continents[stoi(offContinentConnectionIndex[0])]->GetCountry(stoi(offContinentConnectionIndex[1])));
                    }
                }
            }
            catch (...) {
                // Print out that there was an error and exit the program
                cout << "\nError Reading File\n";
                exit(1);
            }
        }
    }

    /**
     * @brief Prints information about the map, including details of each continent.
     */
    void PrintMap() const {
        for (int i = 0; i < continentCount; i++) {
            continents[i]->PrintContinent();
        }
    }
};

/**
 * @brief Represents a game of Risk with players and a map.
 *
 * The RiskGame class encapsulates information about a Risk game, including the number
 * of players, an array of player objects, and a map object. It provides methods for
 * setting up players, printing player information, and initializing the game map.
 */
class RiskGame {
private:
    // The number of players in the game.
    int playerCount;
    // Array of pointers to the players playing the game.
    Player** players;
    // The game map.
    Map map;
public:
    /**
     * @brief Constructor for the RiskGame class.
     *
     * Initializes the game with the specified number of players, sets up players and
     * reads map data from a file.
     *
     * @param playerCount The number of players in the game.
     */
    RiskGame(int playerCount) : playerCount(playerCount), players(new Player*[playerCount]) {
        SetupPlayers();
        PrintPlayers();
        map.ReadMapFile("Data Files\\RiskMapInfo.txt");
        map.PrintMap();
    }

    /**
     * @brief Destructor for the RiskGame class.
     *
     * Releases memory allocated for the players array.
     */
    ~RiskGame() {
        delete[] players;
    }

    /**
     * @brief Sets up player objects by prompting for player names.
     */
    void SetupPlayers() {
        for (int i = 0; i < playerCount; i++) {
            cout << "\nEnter the name of player #" << i + 1 << "\n";
            string playerName;
            cin >> playerName;
            players[i] = new Player(playerName, i + 1);
        }
    }

    /**
     * @brief Prints information about all players in the game.
     */
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
    // Prompts the player for the amount of players playing
    int playerCount = 0;
    while (playerCount <= 0) {
        cout << "How many players are playing? (2-6)\n";
        try {
            string input;
            cin >> input;
            playerCount = stoi(input);

            // Check to see if the player count is outside of the allowed range
            if (playerCount < 2 || playerCount > 6) {
                cout << "\nThe amount of players is outside of the allowed range\n";
                playerCount = 0;
            }
        }
        catch (...) {
            // Ask the player again if the player count they inputted is invalid
            cout << "\nInvalid Input\n";
            playerCount = 0;
        }
    }

    // Creates the Risk Game and passes in the player count
    RiskGame riskGame = RiskGame(playerCount);

    return 0;
}