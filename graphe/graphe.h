#ifndef GRAPHE_H
#define GRAPHE_H

#include <unordered_map>
#include <set>
#include <string>
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
    void getTop(int n) const;
    void getNodes() const;
protected:
    unordered_map<string, PageInfo> table;
    set<string> all_vertices;
    set<string> all_html;
    set<string> all_img;
    set<string> all_js;
};
#endif