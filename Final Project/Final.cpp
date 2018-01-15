//Priyanka Makin, 07/28/2017
//CSCI2270 - Data Structures, Final Project
//Look at the Read Me for project description.


#include <iostream>
#include "Graph.cpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <string>
#include <stdlib.h>
using namespace std;

void displayMenu(){
    cout << "======Main Menu======" << endl;
    cout << "1. Print city vertices" << endl;
    cout << "2. Find districts" << endl;
    cout << "3. Find shortest path" << endl;
    cout << "4. Populate all the cities" << endl;
    cout << "5. Print the cities and their people" << endl;
    cout << "6. Add city member" << endl;
    cout << "7. Delete city member" << endl;
    cout << "8. Quit" << endl;
}

int main(int argc, char* argv[]){
	cout << "Please specify adjacency matrix to use:" << endl;
	string file;
	cin >> file;
	ifstream myFile;
	myFile.open(file);

	vector <string> cities;
	Graph <string> g;

	while (!myFile.eof()){
		string sentence;
	        
		while (getline(myFile,sentence)){
	        stringstream ss(sentence);
	        string value;
	        getline(ss,value,',');
	            
	        if (value == "cities"){
	            string city;
	                
	            while (getline(ss,city,',')){
	                cities.push_back(city);
	                g.addVertex(city);
				}
				
	            continue;
	        }
	        
	        string fromCity = value;
	        int index = 0;
	        string string_weight;
	        int weight;
	        
	        while (getline(ss,string_weight,',')){
	            weight = stoi(string_weight);
	            string toCity;
	            toCity = cities[index];
	            
				if (weight != -1){
					g.addEdge(fromCity,toCity,weight);
	            }
	            index ++;
	        }
	    }
	}

	bool quit = false;
	int menuChoice;
	City city;

	while(!quit){
		displayMenu();

		cin >> menuChoice;

		switch(menuChoice){
            case 1:{
                g.displayEdges();
                break;
            }

            case 2:{
                g.breadthFirstTraversal();
                break;
            }

            case 3:{
                cin.clear();
                cin.ignore(100, '\n');
                string starting_city;
                string ending_city;
                cout << "Enter a starting city:" << endl;
                getline(cin, starting_city);
                cout << "Enter an ending city:" << endl;
                getline(cin, ending_city);
                g.findShortestPath(starting_city, ending_city);
                break;
            }

            case 4:{
				for(int i = 0; i < g.vertices.size(); i++){
					City newCity;
					newCity.buildCity();
					g.vertices[i].next = newCity.head;
				}
                break;
            }

            case 5:{
				for(int i = 0; i < g.vertices.size(); i++){
					cout << g.vertices[i].name;
					city.printCity(g.vertices[i].next);
				}
                break;
            }

            case 6:{	
            	int index;
            	string name;
            	string prevName;
            	cout << "Enter city index:" << endl;
            	cin >> index;
            	cout << "So, the city getting a new member is " + g.vertices[index].name + "!"<< endl;
            	cout << "Enter the person's name:" << endl;
            	cin >> name;
            	cout << "Enter the previous person's name:" << endl;
            	cin >> prevName;
            	city.addPerson(g.vertices[index].next, name, prevName);
				break;
			}
		
			case 7:{
				int index;
				string delName;
				cout << "Enter city index:" << endl;
				cin >> index;
				cout << "So, the city losing a member is " + g.vertices[index].name + "!" << endl;
				cout << "Enter the person's name:" << endl;
				cin >> delName;
				city.deletePerson(g.vertices[index].next, delName);
				break;
			}
			case 8:{
                quit = true;
                cout << "Goodbye!" << endl;
                break;
            }

            default:{
                cout << "Invalid input" << endl;
                break;
            }
		}
	}
return 0;
}
