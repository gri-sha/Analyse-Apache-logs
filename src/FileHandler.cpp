/*************************************************************************
    début                : 22/01/2025
    copyright            : (C) 2025 par Claire Prevost, Grigory Savchenko, Andreea-Cristiana Vlad, Elise Bachet
    e-mail               : claire.prevost@insa-lyon.fr, grigory.savchenko@insa-lyon.fr, andreea-cristiana.vlad@insa-lyon.fr, elise.bachet@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la classe <FileHandler> (fichier FileHandler.cpp) ------------

#include "../include/FileHandler.h"
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <sstream>
using namespace std;

bool FileHandler::readLine(ifstream &fichier, bool dashIgnore)
{
    logStruct log;
    string useless;

    if (!getline(fichier, log.ipAddress, ' ') ||
        !getline(fichier, log.identity, ' ') ||
        !getline(fichier, log.user, ' '))
        return false;

    if (!getline(fichier, useless, '[') ||
        !getline(fichier, log.dateTime, ']'))
        return false;

    if (!getline(fichier, useless, '"') ||
        !getline(fichier, log.httpMethod, ' ') ||
        !getline(fichier, log.resource, ' ') ||
        !getline(fichier, log.httpVersion, '"'))
        return false;

    if (!getline(fichier, useless, ' ') ||
        !getline(fichier, log.httpStatusCode, ' ') ||
        !getline(fichier, log.responseSize, ' '))
        return false;

    if (!getline(fichier, useless, '"') ||
        !getline(fichier, log.referer, '"') ||
        !getline(fichier, useless, '"') ||
        !getline(fichier, log.userAgent, '"') ) //||
        return false;

    if (fichier.peek()== '\n') 
    {
        fichier.get();
    }

    if (dashIgnore && (log.resource == "-" || log.referer == "-"))
    {
#ifdef SETTING
        cout << "False identifiers, log skipped." << endl;
#endif
        return false;
    }

    logHistory.push_back(log);
    return true;
}

bool FileHandler::readDocument(int n, bool dashIgnore)
{
    int count = 0;
    ifstream fichier(fileName, ios::in);
    if (!fichier.is_open())
    {
        cerr << "Error: Impossible to open the file." << endl;
        return false;
    }

    if (n >= 1)
    {
        for (int i = 0; i < n; ++i)
        {
            readLine(fichier, dashIgnore);
        }
    }
    else
    {
        while (fichier)
        {
            if (!readLine(fichier, dashIgnore))
            {
#ifdef SETTING
                cout << "Line skipped." << endl;
#endif
                continue;
            }
            ++count;
        }
    }
    return true;
}

ostream &operator<<(ostream &out, FileHandler &handler)
{
    for (int i = 0; i < handler.logHistory.size(); ++i)
    {
        cout << "log " << i + 1 << " : " << endl;
        cout << "\tIP Address: " << handler.logHistory[i].ipAddress << endl;
        cout << "\tIdentity: " << handler.logHistory[i].identity << endl;
        cout << "\tUser: " << handler.logHistory[i].user << endl;
        cout << "\tDate Time: " << handler.logHistory[i].dateTime << endl;
        cout << "\tHTTP Method: " << handler.logHistory[i].httpMethod << endl;
        cout << "\tResource: " << handler.logHistory[i].resource << endl;
        cout << "\tHTTP Version: " << handler.logHistory[i].httpVersion << endl;
        cout << "\tHTTP Status Code: " << handler.logHistory[i].httpStatusCode << endl;
        cout << "\tResponse Size: " << handler.logHistory[i].responseSize << endl;
        cout << "\tReferer: " << handler.logHistory[i].referer << endl;
        cout << "\tUser Agent: " << handler.logHistory[i].userAgent << endl;
    }
    return out;
}

string FileHandler::extractDomain(const string url)
{
    if (url == "-")
        return url;

    size_t http_adress = url.find("://");
    if (http_adress == string::npos)
        return "-";

    http_adress += 3;

    size_t start = url.find('/', http_adress);
    if (start == string::npos)
    {
        start = url.length();
    }

    size_t end = url.length();


    string domain = url.substr(start, end - start);
    if (domain == "") domain = "/"; // je ne suis pas sure pour ca 
    // if (domain.find("www.") == 0)
    // {
    //     domain = domain.substr(4);
    // }

    return domain;
}


int FileHandler::extractHourFromDateTime(const string &date) const 
{
    size_t colonPos = date.find(':');
    if (colonPos != string::npos)
    {
        string hourStr = date.substr(colonPos + 1, 2);
        return stoi(hourStr); 
    }
    return -1;
}

bool FileHandler::filterType ( string domain ) const
{
    if (domain.find(".pgn") != string::npos ||
        domain.find(".jpg") != string::npos ||
        domain.find(".jpeg") != string::npos ||
        domain.find(".css") != string::npos ||
        domain.find(".js") != string::npos ||
        domain.find(".heic") != string::npos)
    { return true; }
    return false;
}


Graph *FileHandler::createGraph( bool excludeExtensions , bool filterTime, int hourFilter) const 
// ne pas stocker si heure pas correcte !!!!!!!!!!!!!!!
// donc ne pas faire un vectuer de int pour l'heure 
// autant selectionner au plus juste
{
    Graph *graph = new Graph();
    for (int i = 0; i < logHistory.size(); ++i)
    {
        string domainReferer = extractDomain(logHistory[i].referer);
        string domainResource = logHistory[i].resource;

        if (excludeExtensions) 
        {
            if (filterType ( domainResource ))
            {
                continue; // on veut ignorer cette entrée
            }
        }
        
        if (filterTime) 
        {
            int hourLog = extractHourFromDateTime(logHistory[i].dateTime);
            if (hourLog != hourFilter)
            {
                continue; // on veut ignorer cette entrée
            }
        }
        graph->addVisit(domainResource, domainReferer);
    }
    return graph;
} 

