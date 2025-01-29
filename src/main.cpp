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


    if (argc == 0)
    {
        cerr << "Error : log file missing" << endl;
        return 1;
    }

    // dans tous les cas : si pas de fichier .log alors erreur 
    for (int i = 0; i < argc; ++i)
    {
        logFile = argv[i];
        while (logFile.size() < 4 || logFile.rfind(".log") != logFile.size() - 4)
            ++i;

        if (i+1==argc)
        {
            cerr << "Error : log file missing";
            return 1;
        }
    }

    // 2 ou plus arguments => trouver lequel est le fichier .log
    for (int i = 0; i < argc; ++i)
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
            if (i+1 < argc && isdigit(stoi(argv[1+1])) && stoi(argv[i+1]) > 0 && stoi(argv[i+1]) < 24)
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
            if (i+1 < argc && regex_match(argv[i], urlPattern))
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
        
        else 
        {
            cerr << "There is an unknown parameter in the compilation command line" << endl;
            return 1;
        }
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
