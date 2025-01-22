/*************************************************************************
                           Catalogue  -  description
                             -------------------
    début                : 22/01
    copyright            : (C) $YEAR$ par Claire Prevost, Grigory Savchenko, Andreea-Critiana Vlad, Elise Bachet
    e-mail               :  claire.prevost@insa-lyon.fr, grigory.savchenko@insa-lyon.fr, andreea-cristiana.vlad@insa-lyon.fr, elise.bachet@insa-lyon 
*************************************************************************/

//---------- Interface de la classe <FileHandler> (fichier FileHandler.h) ----------------
#include <vector>
using namespace std;

enum = {... } // pour donner un nom plus explicit aux index (autodocument)
class FileHandler
{
	public : 
		lireChamps(); // méthode qui ne lit qu'une ligne du document 
		lireDocument(); // méthode qui lit tout le document (fait appel à lireChamps)
		
	protected :
		Vector logHistory;
};
