/*************************************************************************
                         
                             -------------------
    début                : 22/01
    copyright            : (C) $YEAR$ par Claire Prevost, Grisha Savchenko, Andreea-Cristiana Vlad, Elise Bachet
    e-mail               : claire.prevost@insa-lyon.fr, grigory.savchenko@insa-lyon.fr, andreea-cristiana.vlad@insa-lyon.fr, elise.bachet@insa-lyon.fr
*************************************************************************/

//---------- Réalisation de la classe <FileHandler> (fichier FileHandler.cpp) ------------

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <sstream>
# include "FileHandler.h"
using namespace std;




bool FileHandler :: readLine( ifstream& fichier ) {

    logStruct log;           // déclarer la structure de données log de la ligne en entrée
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

bool FileHandler :: readDocument() {

    ifstream fichier(fileName, ios::in);    

    if (!fichier.is_open()) {
        cerr << "Erreur : Impossible d'ouvrir le fichier.\n";
        return false;
    }

    logStruct line;
    while (readLine(fichier)) {}
    return true;
}


ostream & operator << (ostream & out, FileHandler & file)
{
    for (int i=0 ; i < file.logHistory.size() ; ++i)
    {
        cout << "log " << i << " : " << endl;
        cout << "\tIP Address: " << file.logHistory[i].ipAddress << endl;
        cout << "\tIdentity: " << file.logHistory[i].identity << endl;
        cout << "\tUser: " << file.logHistory[i].user << endl;
        cout << "\tDate Time: " << file.logHistory[i].dateTime << endl;
        cout << "\tHTTP Method: " << file.logHistory[i].httpMethod << endl;
        cout << "\tResource: " << file.logHistory[i].resource << endl;
        cout << "\tHTTP Version: " << file.logHistory[i].httpVersion << endl;
        cout << "\tHTTP Status Code: " << file.logHistory[i].httpStatusCode << endl;
        cout << "\tResponse Size: " << file.logHistory[i].responseSize << endl;
        cout << "\tReferer: " << file.logHistory[i].referer << endl;
        cout << "\tUser Agent: " << file.logHistory[i].userAgent << endl;
    }
    
    return out;
}




int main() 
{
    FileHandler myFileHandler("../exemple.log");
    myFileHandler.readDocument();
    cout << myFileHandler << endl;
    return 0;

}

    


