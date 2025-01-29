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

//---------- déclaration du destructeur
FileHandler::~FileHandler() {}

/* La fonction readLine permettant de lire une ligne du document.
Renvoie True si la ligne est lue correctement, false sinon.
A comme effet de bord de stocker la ligne dans le pointeur donné en entrée.
*/
bool FileHandler::readLine(ifstream &fichier, bool dashIgnore, logStruct* l)
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
    *l=log;
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
Graph * FileHandler::readDocument(bool excludeExtensions, bool filterTime, int hourFilter, int n, bool dashIgnore)
{
    Graph *graph = new Graph();
   
    
    int count = 0;
    ifstream fichier(fileName, ios::in);
    if (!fichier.is_open())
    {
        cerr << "Error: Impossible to open the file." << endl;
        return graph;
    }
    
    logStruct* log=new logStruct;
    if (n >= 1) //Si l'utilisateur spécifie un nombre de lignes
    {
        for (int i = 0; i < n; ++i)
        {
            readLine(fichier, dashIgnore, log);
        	string domainReferer = extractDomain(log->referer);
        	string domainResource = log->resource;
        	if (excludeExtensions){
            		if (filterType(domainResource)){
                		continue; // on ne veut pas ajouter cette entrée au graphe
            		}
        	}
        	if (filterTime){
            		int hourLog = extractHourFromDateTime(log->dateTime);
            		if (hourLog != hourFilter){
                		continue; // on ne veut pas ajouter cette entrée au graphe
            		}
        	}
        	graph->addVisit(domainResource, domainReferer); //si le log correspond à tous les critères spécifiés par l'utilisateur, on l'ajoute au graphe.
   	 } 
   	 return graph;
    }
    else //dans le cas où l'utilisateur ne spécifie pas de nombre fini de lignes -> les traite toutes.
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
        	if (excludeExtensions){
            		if (filterType(domainResource)){
                		continue; // on ne veut pas ajouter cette entrée au graphe
            		}
        	}
        	if (filterTime){
            		int hourLog = extractHourFromDateTime(log->dateTime);
            		if (hourLog != hourFilter){
                		continue; // on ne veut pas ajouter cette entrée au graphe
            		}
        	}
        	graph->addVisit(domainResource, domainReferer); //si le log correspond à tous les critères spécifiés par l'utilisateur, on l'ajoute au graphe.
            ++count;
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
    if (domain == "")
        domain = "/";
    // if (domain.find("www.") == 0)
    // {
    //     domain = domain.substr(4);
    // }

    return domain;
}

/* La fonction extractHourFromDateTime.
données d'entrée :
- la date associée à un log

sortie :
l'heure à laquelle l'utilisateur a consulté cette page
*/
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

/* La fonction filterType.
données d'entrée :
- le domaine sur lequel on réalise un filtrage

sortie :
booléen : false si le type est filtré
*/
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
    