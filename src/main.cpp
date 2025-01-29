#include "../include/FileHandler.h"
#include "../include/Graph.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

int main(int argc, char *argv[])
{
    // TODO: 
    // missing -s argumrent 
    // choose a default server 
    if (argc <= 1) {
        cerr << "No file entered." << endl;
        return 1;
    }

    string logFile;
    string dotFile; 
    bool excludeExtensions = false;
    bool filterTime = false;
    bool generateDotFile = false;
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
            // TODO: Not proper implementation
            // for example: analog file.log -g -e
            // it should generate a file with default name and exclude every content except .html
            // hint: check f starts with '-' or not

            // else
            // {
            //     cerr << "Error: Missing filename for -g option." << endl;
            //     return 1;
            // }
            generateDotFile = true;
        }
        else if (arg == "-e") 
        {
            excludeExtensions = true;
        }
        else if (arg == "-t") 
        {
            if (i + 1 < argc)
            {
                hourFilter = stoi(argv[++i]);
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
            logFile = arg;
        }
    }

    FileHandler *myFileHandler = new FileHandler(logFile);
    myFileHandler->readDocument();
    Graph *graph = myFileHandler->createGraph( excludeExtensions , filterTime, hourFilter);

    if (generateDotFile)
    {
        // if (dotFile.empty())
        // {
        //     cerr << "Pas de nom renseigné pour le fichier dot" << endl;
        //     return 1;
        // }
        cout << "Dot-file " << dotFile << " generated " << dotFile << endl;
        graph->createDotFile(dotFile);
    }
    graph->displayTopDocuments(); 
    delete graph;
    return 0;
}
