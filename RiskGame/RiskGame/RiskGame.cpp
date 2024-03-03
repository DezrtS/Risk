// -------------------------------------------------------------------- //
// Title: C++ Programming Assignment 1                                  //
// Due Date: 03/02/2024                                                 //
// Authors:    Denzil Walrond,     Christian Zambri,    Michael Atteh   //
// Student#:   100868217,          100787919,           100831528       //
// -------------------------------------------------------------------- //

// Headers and Namespaces
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#undef main

using namespace std;

// --------------------------------------------------------------------------------------------------------- //
// Extra Methods                                                                                             //
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
// Structs                                                                                                   //
// --------------------------------------------------------------------------------------------------------- //

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
} App;

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
    int armies = 0;
    int ownedCountries = 0;
    bool hasLost = false;
public:
    /**
     * @brief Constructs a Player object with the given name and player number.
     *
     * @param name The name of the player.
     * @param num The player number assigned to the player.
     */
    Player(string name, int num) : playerName(name), playerNum(num) {}

    /**
     * @brief Adjusts the number of countries a player owns.
     *
     * @param additionalOwnedCountries The number of additional countries the player now owns.
     */
    void AdjustOwnedCountries(int additionalOwnedCountries) {
        ownedCountries += additionalOwnedCountries;
        if (ownedCountries <= 0) {
            hasLost = true;
        }
    }

    /**
     * @brief Determines if a player has lost the game.
     * 
     * @return A boolean for if the player has lost.
     */
    bool HasLost() const {
        return hasLost;
    }

    /**
     * @brief Handles a player turn.
     *
     * Runs the code and 3 stage methods associated with a player turn
     */
    void PlayTurn() {
        cout << "\n" << playerName << " is playing their turn";
        DeployReinforcements();
        PerformAttacks();
        SetupFortifications();
        cin.get();
    }

    /**
     * @brief Handles the reinforcement stage of a player turn
     */
    void DeployReinforcements() {
        // Code goes here
    }

    /**
     * @brief Handles the attack stage of a player turn
     */
    void PerformAttacks() {
        // Code goes here
    }

    /**
     * @brief Handles the fortification stage of a player turn
     */
    void SetupFortifications() {
        // Code goes here
    }

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

    int GetCountryCount() const {
        return countryCount;
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
     * @brief Allocates an equal set of countries to each player.
     *
     * @param playerCount.
     */
    void AlocateCountryOwnership(int playerCount) {
        int totalCountryCount = 0;
        for (int i = 0; i < continentCount; i++) {
            if (continents[i]->IsEnabled()) {
                totalCountryCount += continents[i]->GetCountryCount();
            }
        }

        if (totalCountryCount <= 0) {
            // No countries to allocate
        }

        int countriesEach = totalCountryCount / playerCount;
        int countriesToSkip = totalCountryCount % playerCount;

        cout << "\n\nThere are " << totalCountryCount << " countries to hand out";
        cout << "\nEach player gets " << countriesEach << " countries each";
        cout << "\nIn order to keep the game balanced, we have to skip over " << countriesToSkip << " countries";

        // Code to assign countries to each player goes here
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
        map.AlocateCountryOwnership(playerCount);
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
     * @brief Main method to play the game.
     *
     * Currently only loops through each player and runs their PlayTurn() method.
     */
    void PlayGame() {
        for (int i = 0; i < playerCount; i++) {
            players[i]->PlayTurn();
        }
        PlayGame();
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
// SDL Specific Functions                                                                                    //
// --------------------------------------------------------------------------------------------------------- //

// A struct that holds the main graphical data for the game window.
App app;

/**
 * @brief Initializes the SDL library and sets the main game window data.
 */
void InitSDL(void) {
    // Window size
    int windowWidth = 960;
    int windowHeight = 540;

    int rendererFlags = SDL_RENDERER_ACCELERATED;

    app.window = SDL_CreateWindow("Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        0
    );

    // Error check
    if (!app.window) {
        printf("Failed to open %d x %d window: %s\n", windowWidth, windowHeight, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    // Error check
    if (!app.renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
}

/**
 * @brief Handles input for the main game window.
 * 
 * Currently only handles when the user presses the exit "X" on the game window
 */
void DoInput(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;

        default:
            break;
        }
    }
}

/**
 * @brief Clears the renderer and displays the main background colour.
 */
void PrepareScene(void) {
    SDL_SetRenderDrawColor(app.renderer, 51, 187, 255, 255);
    SDL_RenderClear(app.renderer);
}

/**
 * @brief Presents the scene.
 */
void PresentScene(void) {
    SDL_RenderPresent(app.renderer);
}

/**
 * @brief Loads an image from the project files.
 *
 * @param filename The path of the image to load.
 */
SDL_Surface* LoadImage(const char* filename) {
    SDL_Surface* image;
    image = IMG_Load(filename);
    if (image == NULL) {
        exit(1);
        return 0;
    }
    return image;
}

/**
 * @brief Creates a texture.
 *
 * @param filename The path of the image to load.
 */
SDL_Texture* CreateTexture(const char* filename) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, LoadImage(filename));
    return texture;
}

/**
 * @brief Creates a text texture.
 *
 * @param font The font to display the text in.
 * @param text The text to display.
 */
SDL_Texture* CreateText(TTF_Font* font, const char* text) {
    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    return texture;
}

/**
 * @brief Passes a texture to the renderer to be displayed.
 *
 * @param texture The texture to display on screen.
 * @param x The X coordinate of the texture on screen.
 * @param y The Y coordinate of the texture on screen.
 */
void Blit(SDL_Texture* texture, int x, int y)
{
    SDL_Rect dest{};

    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

// --------------------------------------------------------------------------------------------------------- //
// Main Graphics Function                                                                                    //
// --------------------------------------------------------------------------------------------------------- //

// A boolean to determine if the graphics thread should terminate.
bool terminateGraphicsThread = false;

/**
 * @brief Handles the main game window graphics.
 *
 * Creates the game textures, initializes library components, 
 * and runs all of the SDL specific methods when needed.
 */
void GraphicsHandler() {
    memset(&app, 0, sizeof(App));

    // Initializing library components
    InitSDL();
    TTF_Init();

    // Font creation
    TTF_Font* font = TTF_OpenFont("EazyChat.ttf", 28);

    // Creating and loading game textures
    SDL_Texture* mapTexture = CreateTexture("map2.png");

    SDL_Texture* northAmerica = CreateText(font, "North America");
    SDL_Texture* southAmerica = CreateText(font, "South America");
    SDL_Texture* europe = CreateText(font, "Europe");
    SDL_Texture* africa = CreateText(font, "Africa");
    SDL_Texture* aisa = CreateText(font, "Aisa");
    SDL_Texture* australia = CreateText(font, "Australia");
    //SDL_Texture* num0 = CreateText(font, "0");

    // Main graphics loop
    while (!terminateGraphicsThread) {
        PrepareScene();

        DoInput();

        // Displays textures on screen
        Blit(mapTexture, 0, 0);
        Blit(northAmerica, 100, 10);
        Blit(southAmerica, 10, 400);
        Blit(europe, 280, 185);
        Blit(africa, 380, 400);
        Blit(aisa, 850, 235);
        Blit(australia, 660, 480);

        PresentScene();

        SDL_Delay(16);
    }

    // Frees up rendering components
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
}

// --------------------------------------------------------------------------------------------------------- //
// Main Function                                                                                             //
// --------------------------------------------------------------------------------------------------------- //

/**
 * @brief The starting function of the project.
 */
int main(int argc, char* argv[])
{
    // Creates the graphics thread.
    thread graphicsThread(GraphicsHandler);

    // Prompts the player for the amount of players playing.
    int playerCount = 0;
    while (playerCount <= 0) {
        cout << "How many players are playing? (2-6)\n";
        try {
            string input;
            cin >> input;
            playerCount = stoi(input);

            // Check to see if the player count is outside of the allowed range.
            if (playerCount < 2 || playerCount > 6) {
                cout << "\nThe amount of players is outside of the allowed range\n";
                playerCount = 0;
            }
        }
        catch (...) {
            // Ask the player again if the player count they inputted is invalid.
            cout << "\nInvalid Input\n";
            playerCount = 0;
        }
    }

    // Creates the Risk Game, passes in the player count, and starts the game.
    RiskGame riskGame = RiskGame(playerCount);
    riskGame.PlayGame();

    // Terminates the graphics thread.
    terminateGraphicsThread = true;
    graphicsThread.join();

    return 0;
}