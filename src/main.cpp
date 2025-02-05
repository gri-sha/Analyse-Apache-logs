#include "../include/FileHandler.h"
#include "../include/Graph.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <regex>

bool isInteger(const string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    string logFile; 
    string baseURL = "intranet-if.insa-lyon.fr";

    bool excludeExtensions = false;
    string ext = ".html";

    int hourFilter = -1;
    bool filterTime = false;

    string dotFile = "graph.dot";
    bool generateDotFile = false;

    const regex urlPattern(R"(^((https?|ftp)://)?([a-zA-Z0-9.-]+)\.([a-zA-Z]{2,6})(:[0-9]{1,5})?(\/.*)?$)"); 

    if (argc == 1)
    {
        cerr << "Error : log file missing" << endl;
        return 1;
    }

    int logArgPos = -1;

    for (int i = 1; i < argc; ++i)  
    {
        string arg = argv[i];

        if (arg.size() >= 4 && arg.rfind(".log") == arg.size() - 4)
        {
            logFile = arg;
            logArgPos = i;
            break;  
        }
    }

    if (logArgPos < 0)
    {
        std::cerr << "Error: log file missing" << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i)
    {
        if (i == logArgPos)
            continue;
        
        if (string(argv[i]) == "-g")
        {
            if (i+1 < argc && string(argv[i+1]).rfind(".dot") == string(argv[i+1]).size() - 4 && string(argv[i+1]).length() > 4)
                dotFile = argv[i+1];
            generateDotFile = true;
        }


        else if (string(argv[i]) == "-t")
        {
            if (i+1 < argc)
                {
                    string nextArg = argv[i + 1];

                    if (isInteger(nextArg)) 
                    { 
                    int value = stoi(nextArg);
                    if (value >= 0 && value < 24) 
                    {
                        hourFilter = value;
                        filterTime = true;
                    } else {
                        cerr << "Error: The time must be between 0 and 23." << endl;
                        return 1;
                    }
                } else {
                    cerr << "Error: -t option must be followed by a valid integer." << endl;
                    return 1;
                }
            } else {
                cerr << "Error: -t option but no time provided." << endl;
                return 1;
            }
        } 


        else if (string(argv[i]) == "-s")
        {
            if (i+1 < argc && regex_match(argv[i+1], urlPattern))
            {
                baseURL = FileHandler::extractDomain(string(argv[i+1]));
            }
            else 
            {
                cerr << "Error : -s option but a valid URL is missing" << endl;
                return 1;
            }
        }

        else if (string(argv[i]) == "-e")
        {
            excludeExtensions = true;
            
            if (i+1 < argc && argv[i+1][0]=='.')
            {
                ext = string(argv[i+1]);

            }
        }
        
        else
        {
            cerr << "Error: Unknown parameter '" << argv[i] << "' in the command line" << std::endl;
            return 1;
        }   

    }

    cout << "fileName: " << logFile << endl;
    cout << "base: " << baseURL << endl;

    cout << "Extention: " << ext << endl;
    cout << "Exclude: " << excludeExtensions << endl;

    cout << "filterTime: " << filterTime << endl;
    cout << "Hour: " << hourFilter << endl;

    Graph *graph = FileHandler::readDocument(logFile, baseURL, ext, excludeExtensions , filterTime, hourFilter, -1, false);

    if (generateDotFile)
    {
        cout << "Dot-file " << dotFile << " generated " << endl;
        graph->createDotFile(dotFile);
    }
    graph->displayTopDocuments(); 
    delete graph;
    return 0;
}
