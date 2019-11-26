#include <iostream>
#include <fstream>
#include "HashMap.hpp"

/**   The number of valid parameters        */
const int NUM_OF_PARM = 4;
const int EMPTY = 0;

/**   Error Messages        */
const std::string ERROR_NUM_OF_PARM =
        "Usage: SpamDetector <database path> <message path> <threshold>";
const std::string ERROR_INVALID = "Invalid input";
const std::string ERROR_ALLOC = "Memory allocation failed";

/**  Output messages         */
const std::string SPAM_MSG = "SPAM";
const std::string NOT_SPAM_MSG = "NOT_SPAM";


/**
 * @brief function that parses the corresponding sentence and number file and stores it in data map
 * @param filePath Path to the information file
 * @param dataBase database object to fill
 */
void getData(const char *filePath, HashMap<std::string, int> & dataBase)
{
    // Open the file
    std::ifstream dataBaseFile;
    dataBaseFile.open(filePath);
    if (!dataBaseFile.is_open())
    {
        throw std::ifstream::failure("Unable to open file");
    }

    std::string line, numS;
    std::string::size_type sz;

    // Read the file by lines
    while (getline(dataBaseFile, line, ','))
    {
        getline(dataBaseFile, numS, '\n');
        int num = std::stoi(numS, &sz);

        // Line integrity check
        if (line.size() == EMPTY || numS.size() == EMPTY || sz < numS.size() || num < EMPTY ||
            line.find('\n') != std::string::npos)
        {
            dataBaseFile.close();
            throw std::invalid_argument("Invalid file");
        }
        for (auto & c: line)
        { c = toupper(c); } // Convert to capital letters
        dataBase.insert(line, num);
    }
    dataBaseFile.close();
}

/**
 * @brief Search the suspicious phrases in the given text file
 * @param pathToFile Path to the text file
 * @param dataBase The database of suspected sentences
 * @return The number of bad points in the file
 */
int searchInFile(const char *pathToFile, const HashMap<std::string, int> & dataBase)
{
    // Open the file
    std::ifstream mailFile;
    mailFile.open(pathToFile);
    if (!mailFile.is_open())
    {
        throw std::ifstream::failure("Unable to open file");
    }

    int sum = EMPTY;
    std::string line;

    // Browse the entire file by rows
    while (!mailFile.eof())
    {
        getline(mailFile, line);
        for (auto & c: line)
        { c = toupper(c); } // Convert to capital letters
        // Search all sentences in the database in a row
        for (auto & pair : dataBase)
        {
            size_t pos = EMPTY;
            while ((pos = line.find(pair.first, pos)) != std::string::npos)
            {
                sum += pair.second;
                pos++;
            }
        }
    }
    mailFile.close();
    return sum;
}

/**
 * @brief Main function
 * @param argc Number of arguments
 * @param argv The argument vector
 * @return 0 If the software was successful, 1 otherwise
 */
int main2(int argc, const char *argv[])
{
    //Checking a number of valid arguments
    if (argc != NUM_OF_PARM)
    {
        std::cerr << ERROR_NUM_OF_PARM << std::endl;
        return EXIT_FAILURE;
    }

    std::string limitStr(argv[3]);
    HashMap<std::string, int> database;
    try
    {
        // Receiving information from the files
        getData(argv[1], database);
        int badPoints = searchInFile(argv[2], database);

        // Convert and check the border number
        std::string::size_type sz;
        int limitPoints = std::stoi(limitStr, &sz);
        if (sz < limitStr.size() || limitPoints <= 0)
        {
            throw std::invalid_argument("The resulting arguments are invalid");
        }

        // Print the output
        if (badPoints >= limitPoints)
        {
            std::cout << SPAM_MSG << std::endl;
        }
        else
        {
            std::cout << NOT_SPAM_MSG << std::endl;
        }
    }
    catch (std::bad_alloc & e)
    {
        std::cerr << ERROR_ALLOC << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception & e)
    {
        std::cerr << ERROR_INVALID << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


