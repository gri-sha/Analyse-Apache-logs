/*************************************************************************
    début                : 22/01/2025
    copyright            : (C) 2025 par Claire Prevost, Grigory Savchenko, Andreea-Critiana Vlad, Elise Bachet
    e-mail               :  claire.prevost@insa-lyon.fr, grigory.savchenko@insa-lyon.fr, andreea-cristiana.vlad@insa-lyon.fr, elise.bachet@insa-lyon
*************************************************************************/

//---------- Interface de la classe <Graph> (fichier graph.h) ----------------
#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <set>
#include <string>
#include <vector>
using namespace std;

struct PageInfo
{
    unordered_map<string, pair<int, vector<int>>> counts;
    int total;

    PageInfo(const string origin, const int time)
    {
        counts[origin] = {1, {time}};
        total = 1;
    }

    bool exists(const string &key)
    {
        if (counts.find(key) == counts.end())
            return false;
        return true;
    }
};



class Graph
{
public:
    void addVisit(const string dest, const string origin, const int time);
    bool exists(const string &key) const;
    vector<string, int> getTop(int n, int h = -1, bool exclude = false) const;

    friend ostream &operator<<(ostream &out, Graph &graph);

    //il y a moyen de rassembler les deux fonctions selon si hourFilter = -1
    bool FilterLogTime(int hourFilter); // logs filtrés selon heure et extension
    bool FilterLogType();  // logs filtrés selon extension
    void displayTopDocuments(int n = 10);    // affiche les 10 pages les plus visitées 

    void createDotFile(string fileName, int h = -1, bool exclude = false) const; // crée un document dot


protected:
    unordered_map<string, PageInfo *> table;
    set<string> all_vertices;
    set<string> all_html;
};
#endif
