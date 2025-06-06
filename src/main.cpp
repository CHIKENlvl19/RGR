#include <iostream>
#include <string>
#include <cstring>
#include "header.h"

using namespace std;

int main(int argc, char* argv[]) {

    if (argc > 1) 
    {
        for (int i = 1; i < argc; i++) 
        {
            if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) 
            {
                showHelp();
                return 0;
            }
            else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) 
            {
                showVersion();
                return 0;
            }
            else if (strcmp(argv[i], "--uninstall") == 0) 
            {
                startUninstall();
                return 0;
            }
        }
    }

    while (true)
    {
        PasswordCheck();
        Greeting();

        string fileName;

        if(FileMethods() == "да")
        {
            fileName = PreparedFiles();
        }
        else
        {
            fileName = createUserFiles();
        }

        bool isShowingKeys = isPrintingKeys();
        crypt(cipherChoice(), fileName, isShowingKeys);

        oneMoreFile();
    }

    return 0;
}