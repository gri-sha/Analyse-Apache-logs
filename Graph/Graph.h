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
    unordered_map<string, int> counts;
    int total;

    PageInfo(const string origin) : counts{{origin, 1}}, total(1) {}

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
    void addVisit(const string dest, const string origin);
    bool exists(const string &key) const;
    vector<string, int> getTop(int n, int h = -1, bool exclude = false) const;
    void createFile(int h = -1, bool exclude = false) const;
    friend ostream &operator<<(ostream &out, Graph &graph);

protected:
    unordered_map<string, PageInfo *> table;
    set<string> all_vertices;
    set<string> all_html;
};
#endif
