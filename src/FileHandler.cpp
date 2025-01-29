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
    if (n >= 1)
    {
        for (int i = 0; i < n; ++i)
        {
            readLine(fichier, dashIgnore, log);
        	string domainReferer = extractDomain(log->referer);
        	string domainResource = log->resource;
        	if (excludeExtensions){
            		if (filterType(domainResource)){
                		continue;
            		}
        	}
        	if (filterTime){
            		int hourLog = extractHourFromDateTime(log->dateTime);
            		if (hourLog != hourFilter){
                		continue; // on veut ignorer cette entrée
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
        	if (excludeExtensions){
            		if (filterType(domainResource)){
                		continue;
            		}
        	}
        	if (filterTime){
            		int hourLog = extractHourFromDateTime(log->dateTime);
            		if (hourLog != hourFilter){
                		continue; // on veut ignorer cette entrée
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
    