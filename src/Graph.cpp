/*************************************************************************
    début                : 22/01/2025
    copyright            : (C) 2025 par Claire Prevost, Grigory Savchenko, Andreea-Cristiana Vlad, Elise Bachet
    e-mail               : claire.prevost@insa-lyon.fr, grigory.savchenko@insa-lyon.fr, andreea-cristiana.vlad@insa-lyon.fr, elise.bachet@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la classe <Graphe> (fichier FileHandler.cpp) ------------
#include "../include/Graph.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <set>
#include <fstream>
#include <algorithm>
using namespace std;

Graph::~Graph() {};

void Graph::addVisit(const string &dest, const string &origin)
{
    if (table.find(dest) == table.end())
    {
        table[dest] = new PageInfo(origin);
    }
    else
    {
        if (!table[dest]->exists(origin))
        {
            table[dest]->counts[origin] = 0;
        }
        table[dest]->counts[origin] += 1;
        table[dest]->total += 1;
    }
}

ostream& operator << (ostream &out, Graph &graph)
{
    for (auto it = graph.table.begin(); it != graph.table.end(); ++it)
    {
        out << it->first << endl;
        for (auto subIt = it->second->counts.begin(); subIt != it->second->counts.end(); ++subIt)
        {
            out << "├── "  << subIt->first << " : " << subIt->second << std::endl;
        }
        out << "│" << endl;
        out << "└── Total: " << it->second->total << endl;
        out << endl;
    }
    return out;
}

void Graph::displayTopDocuments(int n) const
{
    // Vector pour stocker les paires (URL, total)
    vector<pair<string, int>> logTotals;

    for (auto it = table.begin(); it != table.end(); ++it)
    {
        logTotals.push_back({it->first, it->second->total});
    }

    sort(logTotals.begin(),
         logTotals.end(),
         // lambda function for comparison of pairs
         [](const pair<string, int> &a, const pair<string, int> &b) { return a.second > b.second; });

    for (int i = 0; i < n && i < (static_cast<int>(logTotals.size())); ++i)
    {
        cout << logTotals[i].first << " (" << logTotals[i].second << " hits)" << endl;
    }
}

void Graph::createDotFile(const string &fileName) const
{
    ofstream file(fileName);

    if (file.is_open())
    {
        file << "digraph  {" << endl;
        for (const auto &entry : table)
        {
            file << "  \"" << entry.first << "\";" << endl;
        }
        for (const auto &entry : table)
        {
            const string &arrivalNode = entry.first;

            for (const auto &subEntry : entry.second->counts)
            {
                const string &startNode = subEntry.first;

                file << "  \"" << startNode << "\" -> \"" << arrivalNode << "\" [label=\"" << subEntry.second << "\"];" << endl;
            }
        }
        file << "}" << endl;
    }
    else
    {
        cerr << "Error: Unable to open the file " << fileName << endl;
    }
} 
