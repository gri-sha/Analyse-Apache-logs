/*************************************************************************
                         
                             -------------------
    début                : 22/01
    copyright            : (C) $YEAR$ par Claire Prevost, Grisha Savchenko, Andreea-Cristiana Vlad, Elise Bachet
    e-mail               : claire.prevost@insa-lyon.fr, grigory.savchenko@insa-lyon.fr, andreea-cristiana.vlad@insa-lyon.fr, elise.bachet@insa-lyon.fr
*************************************************************************/

//---------- Interface de la classe <FileHandler> (fichier FileHandler.h) ------------
#if ! defined ( FileHandler_H )
#define FileHandler_H

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <string>
#include <vector>
using namespace std;


struct ApacheLogEntry {
    string ipAddress;         // Adresse IP du client
    string identity;          // Identité (souvent '-')
    string user;              // Utilisateur authentifié (souvent '-')
    string dateTime;          // Date et heure du log
    string httpMethod;        // Méthode HTTP (GET, POST, etc.)
    string resource;          // Ressource demandée (ex: /page2.html)
    string httpVersion;       // Version du protocole HTTP (ex: HTTP/1.1)
    int httpStatusCode;            // Code de statut HTTP (ex: 200)
    int responseSize;              // Taille de la réponse en octets
    string referer;           // Référant (URL précédente)
    string userAgent;         // Agent utilisateur (navigateur)
};

enum LogFieldIndex {
    IP_ADDRESS,  // Index 0 pour l'adresse IP
    IDENTITY,    
    USER,  
    DATE_TIME,
	HTTP_METHOD,
	RESOURCE,
	HTTP_VERSION,
    STATUS_CODE, 
    RESPONSE_SIZE,   
    REFERER,         
    USER_AGENT      
};

class FileHandler {
public:

    FileHandler ( const string& fileName ) : fileName(fileName) {}

    void readDocument ( void ) ;
    string readLog ( void ) ;

    // créer une surcharge de l'opérateur << ? Utile ou pas, je ne sais pas. A faire si temps 

protected:
    string fileName;                 
    vector<string> logHistory; 
    size_t currentLineIndex = 0; 
};

#endif // FILEHANDLER_H
