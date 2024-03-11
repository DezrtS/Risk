#pragma once

#include <iostream>
#include <vector>

static class Useful
{
public:
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
    static std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter);

    /**
     * @brief Prints out the elements of a vector of strings to the console.
     *
     * This static method takes a vector of strings and loops through each element
     * in the vector, printing out a new line followed by the element at the loop index
     *
     * @param vec The vector to be printed out.
     */
    static void PrintVector(std::vector<std::string>& vec);

    static bool YesOrNo(std::string question);

    static int GetNumber(std::string question);

    static int RollDice(int times);

    static void ContinueWithEnter();

    static void ContinueWithoutEnter();
};