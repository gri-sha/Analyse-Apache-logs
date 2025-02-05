/*************************************************************************
    début                : 22/01/2025
    copyright            : (C) 2025 par Claire Prevost, Grigory Savchenko, Andreea-Cristiana Vlad, Elise Bachet
    e-mail               : claire.prevost@insa-lyon.fr, grigory.savchenko@insa-lyon.fr, andreea-cristiana.vlad@insa-lyon.fr, elise.bachet@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la classe <FileHandler> (fichier FileHandler.cpp) ------------

//---------- include personnels
#include "../include/FileHandler.h"

//---------- include système
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <sstream>
using namespace std;

/* La fonction readLine permettant de lire une ligne du document.
Renvoie True si la ligne est lue correctement, false sinon.
A comme effet de bord de stocker la ligne dans le pointeur donné en entrée.
*/
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
        !getline(fichier, log.userAgent, '"'))
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

/* La fonction readDocument permet de lire le document.
données d'entrée :
- excludeExtension désigne le fait d'exclure des extensions ou non, selon ce qu'a spécifié l'utilisateur.
- filterTime est un bool spécifiant si l'utilisatuer désire se concentrer sur une certaine plage horaire.
- hourFilter est un int positif repérant le début de la plage horaire, si il y en a une, et valant -1 sinon.
- n est un ajout optionnel, dans le cas où l'utilisateur ne voudrait traiter qu'un nombre spécifié de lignes.
- dashIgnore est un ajout optionnel, dans le cas où l'utilisateur ne voudrait pas traiter les logs contenant un dash.

Renvoie un graphe contenant les logs et les informations qui nous intéressent, selon les spécifications d'entrées.
*/
Graph *FileHandler::readDocument(const string &fileName, const string &baseURL, const string &ext, bool excludeExtensions, bool filterTime, int hourFilter, int n, bool dashIgnore)
{
    Graph *graph = new Graph();

    ifstream fichier(fileName, ios::in);
    if (!fichier.is_open())
    {
        cerr << "Error: Impossible to open the file." << endl;
        return graph;
    }

    logStruct *log = new logStruct;
    // Si l'utilisateur spécifie un nombre de lignes
    if (n >= 1)
    {
        for (int i = 0; i < n; ++i)
        {
            readLine(fichier, dashIgnore, log);
            string domainReferer = extractDomain(log->referer, baseURL);
            string domainResource = extractDomain(log->resource, baseURL);

            if (excludeExtensions)
            {   
                // we keep the directories and the files with rigth extentions
                if (!filterType(domainResource, ext))
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
            // si le log correspond à tous les critères spécifiés par l'utilisateur, on l'ajoute au graphe.
            graph->addVisit(domainResource, domainReferer);
        }
        return graph;
    }
    // dans le cas où l'utilisateur ne spécifie pas de nombre fini de lignes -> les traite toutes.
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
            string domainResource = extractDomain(log->resource, baseURL);
            string domainReferer = extractDomain(log->referer, baseURL);

            if (excludeExtensions)
            {
                if (!filterType(domainResource, ext))
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
            // si le log correspond à tous les critères spécifiés par l'utilisateur, on l'ajoute au graphe.
            graph->addVisit(domainResource, domainReferer);
            
        }
    }
    delete log;
    return graph;
}

/* La fonction extractDomain.
données d'entrée :
- l'URL d'un site

sortie :
Simplifie l'URL donnée en entrée
*/
string FileHandler::extractDomain(const string& url, const string& base)
{
    if (url == "-")
        return url;

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
        if (base != "")
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

    if (domain == base)
        external_link = false;

    // return the dmain of external_link
    if (external_link)
        return domain;

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
    if (domain.find(base) == string::npos)
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

/* La fonction extractHourFromDateTime.
données d'entrée :
- la date associée à un log

sortie :
l'heure à laquelle l'utilisateur a consulté cette page
*/
int FileHandler::extractHourFromDateTime(const string &date)
{
    size_t colonPos = date.find(':');
    if (colonPos != string::npos)
    {
        string hourStr = date.substr(colonPos + 1, 2);
        return stoi(hourStr);
    }
    return -1;
}

/* La fonction filterType.
données d'entrée :
- le domaine sur lequel on réalise un filtrage

sortie :
booléen : false si le type est filtré
*/
bool FileHandler::filterType(const string& domain, const string& ext)
{
    // if the resource is a directory, we keep it
    if (domain[domain.length()-1] == '/')
    {
        return true;
    }
    // if the resource is not a directory, we filter it
    int n =  ext.length();
    if (domain.length() >= n)
    {
        // compare the last 5 characters with extention
        return domain.compare(domain.length() - n, n, ext) == 0;
    }
    else
    {
        return false;
    }
}