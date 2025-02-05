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

struct logStruct;

class FileHandler
{
public:
    static Graph *readDocument(const string &fileName,
                               const string &baseURL,
                               const string &ext,
                               bool excludeExtensions,
                               bool filterTime,
                               int hourFilter,
                               int n,
                               bool dashIgnore);

    static string extractDomain(const string &url, const string& base = "");
private:
    static bool readLine(ifstream &ifs, bool dashIgnore = false, logStruct *l = nullptr);
    static int extractHourFromDateTime(const string &date);
    static bool filterType(const string &domain, const string &ext);
};

struct logStruct
{
public:
    friend class FileHandler; 
private:
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

#endif