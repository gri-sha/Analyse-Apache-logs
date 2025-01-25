#include "../include/FileHandler.h"
#include "../include/Graph.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        cerr << "No file entered." << endl;
        return 1;
    }

    // Variables pour les options
    string logFile;
    string dotFile; 
    bool excludeExtensions = false;
    int filterHour = -1; 


    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i];
        if (arg == "-g") 
        {
            if (i + 1 < argc)
            {
                dotFile = argv[++i]; 
            }
            else
            {
                cerr << "Error: Missing filename for -g option." << endl;
                return 1;
            }
        }
        else if (arg == "-e") 
        {
            excludeExtensions = true;
        }
        else if (arg == "-t") 
        {
            if (i + 1 < argc)
            {
                filterHour = stoi(argv[++i]); // Convertir l'heure en entier
                if (filterHour < 0 || filterHour > 23)
                {
                    cerr << "Error: Invalid hour. Provide an hour between 0 and 23." << endl;
                    return 1;
                }
            }
            else
            {
                cerr << "Error: Missing hour for -t option." << endl;
                return 1;
            }
        }
        else
        {
            logFile = arg; // Le fichier log est le dernier argument non optionnel
        }
    }

    FileHandler *myFileHandler = new FileHandler(logFile);
    myFileHandler->readDocument();
    Graph *graph = myFileHandler->createGraph();


    if (filterHour != -1)
    {
        graph->FilterLogTime(filterHour);
        cout << "Warning : only hits between " << filterHour << "h and " << filterHour+1 << "h have been taken into account." << endl;
    }
    if (excludeExtensions)
    {
        graph->FilterLogType();
    }

    if (!dotFile.empty())
    {
        cout << "Dot-file " << dotFile << "  generated" << dotFile << endl;
        graph->createDotFile(dotFile);
        delete graph;
    }
    
    
    graph->displayTopDocuments(); // les dix pages les + visitées

    return 0;
}
