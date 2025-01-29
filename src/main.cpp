#include "../include/FileHandler.h"
#include "../include/Graph.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <regex>

int main(int argc, char *argv[])
{
    string logFile;
    string dotFile; 
    string baseURL = "intranet-if.insa-lyon.fr";
    bool excludeExtensions = false;
    bool filterTime = false;
    bool generateDotFile = false;
    int hourFilter = -1;
    const regex urlPattern(R"(^((https?|ftp)://)?([a-zA-Z0-9.-]+)\.([a-zA-Z]{2,6})(:[0-9]{1,5})?(\/.*)?$)"); 


    if (argc == 1)
    {
        cerr << "Error : log file missing" << endl;
        return 1;
    }


    bool logFileFound = false;

    for (int i = 1; i < argc; ++i)  
    {
        string arg = argv[i];

        if (arg.size() >= 4 && arg.rfind(".log") == arg.size() - 4)
        {
            logFile = arg;
            logFileFound = true;
            break;  
        }
    }

    if (!logFileFound)
    {
        std::cerr << "Error: log file missing" << std::endl;
        return 1;
    }


    // 2 ou plus arguments => trouver lequel est le fichier .log
    for (int i = 1; i < argc; ++i)
    {
        
        if (string(argv[i]) == "-g")
        {
            if (i+1 < argc && string(argv[i+1]).rfind(".dot") == string(argv[i+1]).size() - 4)
                dotFile = argv[i+1];
            else
            {
                dotFile = "graph.dot"; // nom de dot file par défaut
            }
            generateDotFile = true;
        }


        else if (string(argv[i]) == "-t")
        {
            if (i+1 < argc && isdigit(stoi(argv[i+1])) && stoi(argv[i+1]) > 0 && stoi(argv[i+1]) < 24)
            {
                hourFilter = stoi(argv[i+1]) ; 
                filterTime = true;
            }

            else if (i+1 == argc)
            {
                cerr << "Error : -t option but time missing" << endl;
                return 1;
            }
            else
            {
                cerr << "Error : -t option but time missing" << endl;
                return 1;
            }

        }


        else if (string(argv[i]) == "-s")
        {
            if (i+1 < argc && regex_match(argv[i+1], urlPattern))
            {
                baseURL = string(argv[i+1]) ; 
            }
            
            else if (i+1 == argc)
            {
                cerr << "Error : -s option but URL missing" << endl;
                return 1;
            }
            else 
            {
                cerr << "Error : -s option but not a valid URL" << endl;
                return 1;
            }
        }

        else if (string(argv[i]) == "-e")
        {
            excludeExtensions = true;
        }
        
        // else if (string(argv[i]).rfind(".log") == string(argv[i]).size() - 4)
        // {
        //     // Ne rien faire : on a déjà traité le fichier log
        // }
        // else
        // {
        //     cerr << "Error: Unknown parameter '" << argv[i] << "' in the command line" << std::endl;
        //     return 1;
        // }   

    }

    FileHandler *myFileHandler = new FileHandler(logFile, baseURL);
    Graph *graph = myFileHandler->readDocument(excludeExtensions , filterTime, hourFilter);

    if (generateDotFile)
    {
        cout << "Dot-file " << dotFile << " generated " << endl;
        graph->createDotFile(dotFile);
    }
    graph->displayTopDocuments(); 
    delete graph;
    delete myFileHandler;
    return 0;
}
