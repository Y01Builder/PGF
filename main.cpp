#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void copyToClipboard(const std::string &text) {
#ifdef _WIN32
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();
        HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
        if (hg) {
            memcpy(GlobalLock(hg), text.c_str(), text.size() + 1);
            GlobalUnlock(hg);
            SetClipboardData(CF_TEXT, hg);
        }
        CloseClipboard();
    }
#else
    FILE *pipe = popen("xclip -selection clipboard", "w");
    if (pipe != nullptr) {
        fwrite(text.c_str(), 1, text.size(), pipe);
        pclose(pipe);
    }
#endif
}

void printColoredString(const std::string &str) {
    for (char ch : str) {
        if (isalpha(ch)) {
            std::cout << ch; // Normal output for letters
        } else if (isdigit(ch)) {
            std::cout << "\033[1;34m" << ch << "\033[0m"; // Blue for numbers
        } else {
            // Orange for special characters
            std::cout << "\033[1;33m" << ch << "\033[0m";
        }
    }
}

std::string generateRandomString(int length) {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789%$€!?+§(){}[]*<>#";
    std::string randomString;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < length; ++i) {
        randomString += characters[std::rand() % characters.size()];
    }

    return randomString;
}

int main(int argc, char *argv[]) {
    // Set the locale to the default system locale
    std::locale::global(std::locale(""));

    int passwordLength = 20; // Default length

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--length") == 0) {
            if (i + 1 < argc) {
                passwordLength = std::atoi(argv[i + 1]);
            } else {
                std::cerr << "Error: Missing argument after -l/--length." << std::endl;
                return 1;
            }
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {

            std::cout << "    ____  ____________ \n";
            std::cout << "   / __ \\/ ____/ ____/\n";
            std::cout << "  / /_/ / / __/ /_    \n";
            std::cout << " / ____/ /_/ / __/    \n";
            std::cout << "/_/    \\____/_/       \n\n";
            std::cout << "Password Generator by Fliese\n\n";
            std::cout << "Usage:\n";
            std::cout << "PGF.exe [-l|--length LENGTH] [-h|--help]\n" << std::endl;
            return 0;
        }
    }

    std::string password = generateRandomString(passwordLength);

    // Output the colored string
    printColoredString(password);
    std::cout << std::endl;

    // Copy to clipboard
    copyToClipboard(password);

    std::cout << "Password copied to clipboard." << std::endl;
    return 0;
}
