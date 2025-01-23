/*************************************************************************
    début                : 22/01/2025
    copyright            : (C) 2025 par Claire Prevost, Grigory Savchenko, Andreea-Cristiana Vlad, Elise Bachet
    e-mail               : claire.prevost@insa-lyon.fr, grigory.savchenko@insa-lyon.fr, andreea-cristiana.vlad@insa-lyon.fr, elise.bachet@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la classe <FileHandler> (fichier FileHandler.cpp) ------------

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include "FileHandler.h"
using namespace std;

bool FileHandler::readLine(ifstream &fichier)
{
    logStruct log;
    string useless;

    // Parsing des champs
    if (!getline(fichier, log.ipAddress, ' ') ||
        !getline(fichier, log.identity, ' ') ||
        !getline(fichier, log.user, ' '))
        return false;

    // Extraction de la date et heure entre crochets []
    if (!getline(fichier, useless, '[') ||
        !getline(fichier, log.dateTime, ']'))
        return false;

    // Extraction de la requête HTTP entre guillemets ""
    if (!getline(fichier, useless, '"') ||
        !getline(fichier, log.httpMethod, ' ') ||
        !getline(fichier, log.resource, ' ') ||
        !getline(fichier, log.httpVersion, '"'))
        return false;

    // Extraction des autres champs
    if (!getline(fichier, useless, ' ') ||
        !getline(fichier, log.httpStatusCode, ' ') ||
        !getline(fichier, log.responseSize, ' '))
        return false;

    // Extraction du référent et de l'agent utilisateur
    if (!getline(fichier, useless, '"') ||
        !getline(fichier, log.referer, '"') ||
        !getline(fichier, useless, '"') ||
        !getline(fichier, log.userAgent, '"'))
        return false;

    logHistory.push_back(log);
    return true;
}

bool FileHandler::readDocument(int n)
{
    ifstream fichier(fileName, ios::in);
    if (!fichier.is_open())
    {
        cerr << "Erreur : Impossible to open the file." << endl;
        return false;
    }
    // read all the line until EOF
    if (n >= 1)
    {
        for (int i = 0; i < n; ++i)
        {
            readLine(fichier);
        }
    }
    else
    {
        while (fichier)
        {
            if (!readLine(fichier))
            {
#ifdef SETTING
                cout << "Line skipped." << endl;
#endif
            }
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

Graph *FileHandler::createGraph() const
{
    Graph* graph = new Graph();
    for (int i = 0; i < logHistory.size(); ++i)
    {
        graph->addVisit(logHistory[i].resource, logHistory[i].referer);
    }
    return graph;
}