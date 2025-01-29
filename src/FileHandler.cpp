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

FileHandler::~FileHandler() {}

bool FileHandler::readLine(ifstream &fichier, bool dashIgnore, logStruct *l)
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
        !getline(fichier, log.userAgent, '"')) //||
        return false;

    if (fichier.peek() == '\n')
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
    *l = log;
    return true;
}

Graph *FileHandler::readDocument(bool excludeExtensions, bool filterTime, int hourFilter, int n, bool dashIgnore)
{
    Graph *graph = new Graph();

    int count = 0;
    ifstream fichier(fileName, ios::in);
    if (!fichier.is_open())
    {
        cerr << "Error: Impossible to open the file." << endl;
        return graph;
    }

    logStruct *log = new logStruct;
    if (n >= 1)
    {
        for (int i = 0; i < n; ++i)
        {
            readLine(fichier, dashIgnore, log);
            string domainReferer = extractDomain(log->referer);
            string domainResource =  extractDomain(log->resource);
            if (excludeExtensions)
            {
                if (filterType(domainResource))
                {
                    continue;
                }
            }
            if (filterTime)
            {
                int hourLog = extractHourFromDateTime(log->dateTime);
                if (hourLog != hourFilter)
                {
                    continue;
                }
            }
            graph->addVisit(domainResource, domainReferer);
        }
        return graph;
    }
    else
    {
        while (fichier)
        {
            if (!readLine(fichier, dashIgnore, log))
            {
#ifdef SETTING
                cout << "Line skipped." << endl;
#endif
                continue;
            }
            string domainReferer = extractDomain(log->referer);
            string domainResource = log->resource;
            if (excludeExtensions)
            {
                if (filterType(domainResource))
                {
                    continue;
                }
            }
            if (filterTime)
            {
                int hourLog = extractHourFromDateTime(log->dateTime);
                if (hourLog != hourFilter)
                {
                    continue;
                }
            }
            graph->addVisit(domainResource, domainReferer);
            ++count;
        }
    }
    delete log;
    return graph;
}

string FileHandler::extractDomain(const string url)
{
    bool external_link = true;

    // find the protocol and read after it
    size_t startPos = url.find("://");
    if (startPos != string::npos)
    {
        startPos += 3;
    }
     // if there is no protocol, read from the start
    else
    {
        startPos = 0;
        external_link = false;
    }

    // extract the domain
    size_t endPos = url.find('/', startPos);
    if (endPos == string::npos)
    {
        endPos = url.length();
    }
    string domain = url.substr(startPos, endPos - startPos);

    // remove the www. from the domain if it exists
    if (domain.find("www.") == 0)
        domain = domain.substr(4);

    if (domain == "intranet-if.insa-lyon.fr")
        external_link = false;


    // return the dmain of external_link
    if (external_link)
    {
        // cout << "external : ";
        return domain;
    }

    /* FUTHER TREATMENT FOR INTERNAL LINKS */

    // extract the path
    string path = url.substr(endPos);

    // remove query from the path
    size_t queryPos = path.find('?');
    if (queryPos != string::npos)
    {
        path = path.substr(0, queryPos);
    }

    // remove the url identifier
    size_t urlPos = path.find("url");
    if (queryPos != string::npos)
    {
        path = path.substr(0, urlPos);
    }

    // remove session data from the path
    size_t sessionPos = path.find(';');
    if (sessionPos != string::npos)
    {
        path = path.substr(0, sessionPos);
    }

    string res;
    if (domain.find("intranet-if.insa-lyon.fr") == string::npos)
    {
        res = domain + path;
    }
    else
    {
        external_link = false;
        res = path;
    }

    if (external_link and res[res.size() - 1] == '/')
        res = res.substr(0, res.size() - 1);

    if (res[0] != '/' and !external_link)
        res = "/" + res;

    return res;
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

bool FileHandler::filterType(string domain) const
{
    if (domain.find(".pgn") != string::npos ||
        domain.find(".jpg") != string::npos ||
        domain.find(".jpeg") != string::npos ||
        domain.find(".css") != string::npos ||
        domain.find(".js") != string::npos ||
        domain.find(".heic") != string::npos)
    {
        return true;
    }
    return false;
}
