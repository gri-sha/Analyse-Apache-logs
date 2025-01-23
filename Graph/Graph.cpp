/*************************************************************************
    début                : 22/01/2025
    copyright            : (C) 2025 par Claire Prevost, Grigory Savchenko, Andreea-Cristiana Vlad, Elise Bachet
    e-mail               : claire.prevost@insa-lyon.fr, grigory.savchenko@insa-lyon.fr, andreea-cristiana.vlad@insa-lyon.fr, elise.bachet@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la classe <Graphe> (fichier FileHandler.cpp) ------------
#include "Graph.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <iomanip>
#include <set>
using namespace std;

void Graph::addVisit(const string dest, const string origin)
{
    if (!exists(dest)) {
        table[dest] = new PageInfo(origin);
    } else {
        if (!table[dest]->exists(origin))
        {
            table[dest]->counts[origin] = 0;
        }
        table[dest]->counts[origin] += 1;
        table[dest]->total += 1;
    }
}

bool Graph::exists(const string &key) const
{
    if (table.find(key) == table.end())
    {
        return false;
    }
    return true;
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
