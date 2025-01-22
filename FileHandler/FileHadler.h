#include <vector>
using namespace std;

enum = {... }; // pour donner un nom plus explicit aux index (autodocument)
class FileHandler
{
	public : 
		lireChamps(); // méthode qui ne lit qu'une ligne du document 
		lireDocument(); // méthode qui lit tout le document (fait appel à lireChamps)
		
	protected :
		Vector logHistory;
};
