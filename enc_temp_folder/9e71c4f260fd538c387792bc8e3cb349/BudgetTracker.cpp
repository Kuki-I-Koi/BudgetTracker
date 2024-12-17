#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>          // I don't even know if I use all of these I was just experimenting and tinkering solutions
#include <ctime>              // without removing the old stuff, and now scared of removing something so it won't break LOL
#include <chrono> 
#include <iomanip>
#include <sstream>
#include <fstream>

struct entry {
    int cost;
    std::string category;
    std::string description;
    std::string creationDate;

    void displayInfo() const {
        std::cout << "\n--- Entry Info ---\n";
        std::cout << "Cost: " << cost << "\n";
        std::cout << "Category: " << category << "\n";
        std::cout << "Description: " << description << "\n";
        std::cout << "Created on: " << creationDate << "\n";
    }
};

// Function to get the current date and time as a string
std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();                        // <- Gets the current time 
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm localTime = *std::localtime(&now_time);             // <- Convert it to local time and format as a string

    std::ostringstream dateTimeStream;
    dateTimeStream << (localTime.tm_year + 1900) << "-"
        << (localTime.tm_mon + 1) << "-"
        << localTime.tm_mday << " "
        << localTime.tm_hour << ":"
        << localTime.tm_min << ":"
        << localTime.tm_sec;

    return dateTimeStream.str();
}


// Function to display the list of entries
void displayEntries (std::vector<entry>& Entries) {
    if (Entries.empty()) {
        std::cout << "\nThe list is empty. No entries available.\n";
    }
    else {
        std::sort(Entries.begin(), Entries.end(), [](const entry& a, const entry& b) {
            return a.creationDate < b.creationDate;                       //<-  Sort entries by creation date (oldest first)
            });

        std::cout << "\n--- List of All Entries (Sorted by Creation Date) ---\n";
        for (const auto& entry : Entries) {
            entry.displayInfo();
        }
    }
}

// Function to search for entries by category (case-insensitive)
void searchByCategory(const std::vector<entry>& Entries) {
    if (Entries.empty()) {
        std::cout << "\nThe list is empty. No entries available to search.\n";
        return;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // <- clears the buffer

    std::string searchCategory;
    std::cout << "\nEnter the name to search for: ";
    std::getline(std::cin, searchCategory);

    std::transform(searchCategory.begin(), searchCategory.end(), searchCategory.begin(), ::tolower);  //<- convert name for case insensitive comparison
    bool found = false;
    for (const auto& entry : Entries) {
        std::string entryCategoryLower = entry.category;
        std::transform(entryCategoryLower.begin(), entryCategoryLower.end(), entryCategoryLower.begin(), ::tolower);

        if (entryCategoryLower == searchCategory) {
            entry.displayInfo();
            found = true;
        }
    }

    if (!found) {
        std::cout << "\nNo entries found for the category \"" << searchCategory << "\".\n";
    }
}



// Function to sum the prices of all entries in the list
void sumPrices(const std::vector<entry>& Entries) {
    if (Entries.empty()) {
        std::cout << "\nThe list is empty. No entries to sum.\n";
    }
    else {
        int totalCost = 0;
        for (const auto& entry : Entries) {
            totalCost += entry.cost;
        }
        std::cout << "\nThe total sum of prices is: " << totalCost << "\n";
    }
}

// Function to save entries to a file
void saveEntriesToFile(const std::vector<entry>& Entries, const std::string& filename) {
    std::ofstream outFile(filename);

    if (!outFile) {
        std::cerr << "Failed to open the file for saving.\n";
        return;
    }

    for (const auto& entry : Entries) {
        outFile << entry.cost << "\n"
            << entry.category << "\n"            //<- Save each entry's information to the file
            << entry.description << "\n"
            << entry.creationDate << "\n";
    }

    outFile.close();
    std::cout << "\nEntries saved to file.\n";
}

// Function to load entries from a file
void loadEntriesFromFile(std::vector<entry>& Entries, const std::string& filename) {
    std::ifstream inFile(filename);

    if (!inFile) {
        std::cout << "No file found, starting with an empty list.\n";
        return;
    }

    entry newEntry;
    while (inFile) {
        inFile >> newEntry.cost;
        inFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //<- Skip the newline after the cost

        std::getline(inFile, newEntry.category);       
        std::getline(inFile, newEntry.description);    
        std::getline(inFile, newEntry.creationDate);   

        // Check if we reached the end of file unexpectedly
        if (inFile.eof()) break;

        Entries.push_back(newEntry);
    }

    inFile.close();
    std::cout << "\nEntries loaded from file.\n";
}




entry createEntry() {
    entry entry; 
    while (true) {
        std::cout << "Enter cost: ";
        std::cin >> entry.cost;

        if (std::cin.fail() || entry.cost < 0) {     //<- // Check if the input is valid (a number)
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //<- Ignore the rest of the invalid input
            std::cout << "Invalid input. Please enter a valid age (positive number).\n";
        }
        else {
            break; // <-Exit the loop if valid input is given
        }
    }
    std::cout << "Enter category: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, entry.category);


    std::cout << "Enter description: ";
    std::getline(std::cin, entry.description);

    // Capture the current date and time
    entry.creationDate = getCurrentDateTime();

    return entry;
}


int main()
{
    std::vector<entry> entries;
    int choice;
    std::string filename = "entries.txt";  //<- File to store/load entries

    // Load entries from file at the start of the program
    loadEntriesFromFile(entries, filename);

    while (true) {
        // Display menu options
        std::cout << "\n--- Menu ---\n";
        std::cout << "1. Add a New Entry\n";
        std::cout << "2. View List of Entries (Sorted By Dates) \n";
        std::cout << "3. Sum Prices of All Entries\n";
        std::cout << "4. Search for Entries by Category\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice (1-5): ";
        std::cin >> choice;

        // Validate menu choice input
        if (std::cin.fail() || choice < 1 || choice > 5) {
            std::cin.clear(); // <- Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter a number between 1 and 4.\n";
            continue;
        }

        // Perform action based on input
        switch (choice) {
        case 1:
            entries.push_back(createEntry());
            break;
        case 2:
            displayEntries(entries);
            break;
        case 3:
            sumPrices(entries);
            break;
        case 4:
            searchByCategory(entries);
            break;
        case 5:
            // Save entries to file before exiting
            saveEntriesToFile(entries, filename);
            std::cout << "\nGoodbye!\n";
            return 0;

        }
    }
}
