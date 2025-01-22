#ifndef GRAPHE_H
#define GRAPHE_H

#include <unordered_map>
#include <set>
#include <string>
#include <vector>
using namespace std;

struct PageInfo
{
    unordered_map<string, int> count;
    int total;
};

class Graphe
{
public:
    void addVisit();
    vector<string, int> getTop(int n, int h=-1, bool exclude = false) const;
    void createFile(int h=-1, bool exclude = false) const;

protected:
    unordered_map<string, PageInfo> table;
    set<string> all_vertices;
    set<string> all_html;
};
#endif