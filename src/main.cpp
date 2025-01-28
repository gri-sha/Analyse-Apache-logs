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

    string logFile;
    string dotFile; 
    bool excludeExtensions = false;
    bool filterTime = false;
    bool generatedDotFile = false;
    int hourFilter = -1; 


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
            generatedDotFile = true;
        }
        else if (arg == "-e") 
        {
            excludeExtensions = true;
        }
        else if (arg == "-t") 
        {
            if (i + 1 < argc)
            {
                hourFilter = stoi(argv[++i]); // Convertir l'heure en entier
                if (hourFilter < 0 || hourFilter > 23)
                {
                    cerr << "Error: Invalid hour. Provide an hour between 0 and 23." << endl;
                    return 1;
                }
                filterTime = true;
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
    Graph *graph = myFileHandler->createGraph( excludeExtensions , filterTime, hourFilter);

    if (generatedDotFile)
    {
        if (dotFile.empty())
        {
            cerr << "Pas de nom renseigné pour le fichier dot" << endl;
            return 1;
        }
        cout << "Dot-file " << dotFile << " generated " << dotFile << endl;
        graph->createDotFile(dotFile);
    }
    graph->displayTopDocuments(); 
    delete graph;
    return 0;
}
