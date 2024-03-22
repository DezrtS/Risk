#include "Useful.h"

#include <string>
#include <random>

std::vector<std::string> Useful::SplitString(const std::string& str, const std::string& delimiter) {
    // Splits a string up into a vector of strings.
    std::vector<std::string> result;

    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    result.push_back(str.substr(start));

    return result;
}

void Useful::PrintVector(std::vector<std::string>& vec) {
    // Prints all elements in a string vector.
    for (int i = 0; i < vec.size(); i++) {
        std::cout << "\n" << vec[i];
    }
}

bool Useful::YesOrNo(std::string question) {
    std::string answer;
    // While the answer is invalid, keep asking the player to answer yes or no to the question.
    while (true) {
        std::cout << "\n" << question << " (Y/N)\n";
        std::cin >> answer;
        if (tolower(answer[0]) == 'y') {
            return true;
        }
        else if (tolower(answer[0]) == 'n') {
            return false;
        }
        std::cout << "\nInvalid answer, try again";
    }
}

int Useful::GetNumber(std::string question) {
    std::string answer;
    // While the answer is invalid, keep asking the player to enter a number for the question.
    while (true) {
        std::cout << "\n" << question << "\n";
        std::cin >> answer;
        try {
            int num = std::stoi(answer);
            return num;
        }
        catch (...) {
            std::cout << "\nInvalid number, try again";
        }
    }
}

int Useful::RollDice(int times) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 6);
    int diceTotal = 0;
    // Rolls a die a number of times and returns the sum of all of the rolls.
    for (int i = 0; i < times; i++) {
        int die = dist6(rng);
        std::cout << "\nOn roll #" << i + 1 << ", you rolled a " << die;
        diceTotal += die;
    }
    if (times > 1) {
        std::cout << "\nThe total combined rolls resulted in a value of " << diceTotal;
    }
    return diceTotal;
}

void Useful::ContinueWithEnter() {
    // Clears the console after the user enters something.
    std::cout << "\n\nEnter any number to continue: ";
    std::string answer;
    std::cin >> answer;
    system("CLS");
}

void Useful::ContinueWithoutEnter() {
    // Clears the console.
    system("CLS");
}