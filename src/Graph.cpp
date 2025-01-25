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
#include <iomanip>
#include <set>
#include <algorithm>
using namespace std;

void Graph::addVisit(const string dest, const string origin, const int time)
{
    if (!exists(dest)) {
        table[dest] = new PageInfo(origin, time);
    } else {
        if (!table[dest]->exists(origin))
        {
            table[dest]->counts[origin] = {0, {}};
        }
        table[dest]->counts[origin].first += 1;
        table[dest]->counts[origin].second.push_back(time);
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
        out << "( " << it->second->total << " hits )" << endl;
    }
    return out;
}


bool Graph::FilterLogTime(int hour)
{
    for (auto it = table.begin(); it != table.end();)
    {
        PageInfo *pageInfo = it->second;

        for (auto subIt = pageInfo->counts.begin(); subIt != pageInfo->counts.end();)
        {
            vector<int> &times = subIt->second.second; 

            auto newEnd = remove_if(times.begin(), times.end(), [hour](int time) {return time != hour; });
            int removedCount = distance(newEnd, times.end());
            times.erase(newEnd, times.end());

            auto &count = subIt->second.first;    
            count -= removedCount;

            // Si aucune heure ne reste pour cette origine, supprimer l'entrée
            if (times.empty())
            {
                subIt = pageInfo->counts.erase(subIt);
            }
            else
            {
                ++subIt;
            }
        }

        // Mettre à jour le total pour la page
        pageInfo->total = 0;
        for (const auto &entry : pageInfo->counts)
        {
            pageInfo->total += entry.second.first;
        }

        // Si aucune origine ne reste pour cette URL cible, supprimer l'entrée
        if (pageInfo->counts.empty())
        {
            delete pageInfo; // Libérer la mémoire
            it = table.erase(it);
        }
        else
        {
            ++it;
        }
    }
    return true;
}

bool Graph::FilterLogType()
{
    for (auto it = table.begin(); it != table.end();)
    {
        string pageName = it->first;
        // Vérifier si le nom de la page contient une extension d'image, CSS ou JS
        if (pageName.find(".pgn") != std::string::npos ||
            pageName.find(".jpg") != std::string::npos ||
            pageName.find(".jpeg") != std::string::npos ||
            pageName.find(".css") != std::string::npos ||
            pageName.find(".js") != std::string::npos ||
            pageName.find(".heic") != std::string::npos)
        {
            // Supprimer la page si une extension correspond
            it = table.erase(it);
        }
        else
        {
            ++it;
        }
    }
    return true;
}


void Graph::displayTopDocuments(int n) 
{
    // Vector pour stocker les paires (URL, total)
    vector<pair<string, int>> logTotals;

    for (auto it = table.begin(); it != table.end(); ++it)
    {
        logTotals.push_back({it->first, it->second->total});
    }

    sort(logTotals.begin(), logTotals.end(),
         [](const pair<string, int> &a, const pair<string, int> &b) {
             return a.second > b.second; 
         });

    for (int i = 0; i < n && i < logTotals.size(); ++i)
    {
        cout << logTotals[i].first << " ( " << logTotals[i].second << " hits )" << endl;
    }
}



void Graph::createDotFile(string fileName, int h, bool exclude) const {} // crée un document dot