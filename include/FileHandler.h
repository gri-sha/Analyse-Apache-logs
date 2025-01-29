/*************************************************************************
    début                : 22/01/2025
    copyright            : (C) 2025 par Claire Prevost, Grigory Savchenko, Andreea-Cristiana Vlad, Elise Bachet
    e-mail               : claire.prevost@insa-lyon.fr, grigory.savchenko@insa-lyon.fr, andreea-cristiana.vlad@insa-lyon.fr, elise.bachet@insa-lyon.fr
*************************************************************************/

//---------- Interface de la classe <FileHandler> (fichier FileHandler.h) ------------
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

//------------------------------------------------------ Include personnel
#include "Graph.h"
//-------------------------------------------------------- Include système
#include <string>
#include <vector>
using namespace std;

struct logStruct
{
    string ipAddress;      // Adresse IP du client
    string identity;       // Identité (souvent '-')
    string user;           // Utilisateur authentifié (souvent '-')
    string dateTime;       // Date et heure du log
    string httpMethod;     // Méthode HTTP (GET, POST, etc.)
    string resource;       // Ressource demandée (ex: /page2.html)
    string httpVersion;    // Version du protocole HTTP (ex: HTTP/1.1)
    string httpStatusCode; // Code de statut HTTP (ex: 200)
    string responseSize;   // Taille de la réponse en octets
    string referer;        // Référant (URL précédente)
    string userAgent;      // Agent utilisateur (navigateur)
};

class FileHandler
{
public:
    FileHandler(const string &fileName, const string &baseURL) : fileName(fileName), baseURL(baseURL) {}
    virtual ~FileHandler();

    Graph* readDocument(bool excludeExtensions=false, bool filterTime=false, int hourFilter = -1, int n = -1, bool dashIgnore = false);
    bool readLine(ifstream &fichier, bool dashIgnore = false, logStruct* l=NULL);

    static string extractDomain(const string url);
    int extractHourFromDateTime(const string &date) const;
    bool filterType(string domain) const;


protected:
    string fileName;
    const string baseURL = "intranet-if.insa-lyon.fr";
};

#endif