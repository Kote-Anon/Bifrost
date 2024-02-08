#include <iostream>
#include <string>
#include <vector>
#include "colors.h"
#include <strsafe.h>
#include <tchar.h>
#include "handleServer.h"

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

char getInput() {
#ifdef _WIN32
    return _getch();
#else
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    if (ch == '\n' || ch == '\r') {
        std::cout << std::endl; // Manually add a newline when Enter is pressed
    }

    return ch;
#endif
}

void processCommand(const std::string& command) {
    // Add logic to process commands
    if (command == ".help") {
        std::cout << "\n";
        std::cout << "-----------\n";
        std::cout << "Available commands:\n";
        std::cout << ".help - Show this help message\n";
        std::cout << ".server start - starts creating server socket\n";
        std::cout << ".cmd2 - Execute Command 2\n";
        std::cout << ".exit - Exit the program\n";
        std::cout << "-----------\n";
    }
    else if (command == ".server start") {
        std::cout << "\n";
        handleServer();
    }
    else if (command == ".cmd2") {
        std::cout << "\n" << "Executing Command 2...\n";
        // Add functionality for Command 2
    }
    else if (command == ".exit") {
        std::cout << "\n" << "Exiting...\n";
        exit(0);
    }
    else if (command.empty()) {
        std::cout << "\n";
        return;
    }
    else {
        std::cout << " " << "\n";
        std::cout << "Unknown command. Please enter valid command." << "\n";
        std::cout << "Enter command .help for displaying list of commands" << "\n";
    }
}

int main() {
    std::vector<char> userInputBuffer;

    TCHAR szOldTitle[MAX_PATH];
    TCHAR szNewTitle[MAX_PATH];

    // Save current console title.

    if (GetConsoleTitle(szOldTitle, MAX_PATH))
    {
        // Build new console title string.

        StringCchPrintf(szNewTitle, MAX_PATH, TEXT("B I F R O S T"), szOldTitle);

        // Set console title to new title
        if (!SetConsoleTitle(szNewTitle))
        {
            return 1;
        }
        else
        {
        }
    }

    std::cout << Bright_White << R"(

    |~\  | | / / |~\   /~\    /     /|\   
    |  > | |/ /  |  \ /   \  /     / | \  
    |_/  | | /   |  / \   / /____ /  |  \ 
    |~\  | |/    |_/   \ /      /    |    
    |  > | |     | \   / \     /     |    
    |_/  | |     |  \ /   \   /      |    
                                 
)" << RESET;
    std::cout << "\n";

    while (true) {
        std::cout << Bright_Blue << "~>" << RESET;
        for (char ch : userInputBuffer) {
            std::cout << ch;
        }

        // Get user input
        char ch;
        do {
            ch = getInput();
            if (ch == '\b' && !userInputBuffer.empty()) {
                userInputBuffer.pop_back();  // Handle backspace
                std::cout << "\b \b"; // Move cursor back and overwrite the character with a space
            }
            else if (ch != '\n' && ch != '\r') {
                userInputBuffer.push_back(ch);
                std::cout << ch;
            }
        } while (ch != '\n' && ch != '\r');

        std::string userInput(userInputBuffer.begin(), userInputBuffer.end());

        // Process userInput as a command
        processCommand(userInput);

        userInputBuffer.clear();
    }

    return 0;
}