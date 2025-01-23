#include "./FileHandler/FileHandler.h"
#include "./Graph/Graph.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        cerr << "No file entered." << endl;
        return 1;
    }

    FileHandler *myFileHandler = new FileHandler(argv[1]);
    myFileHandler->readDocument();
    cout << *myFileHandler << endl;
    Graph *graph = myFileHandler->createGraph();
    cout << *graph << endl;
    return 0;
}