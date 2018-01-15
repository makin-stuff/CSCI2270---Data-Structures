#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <iostream>
using namespace std;

template<class T>
struct vertex;

///////////////////
///Linked list structs and classes
//////////////////
struct Person{
	string personName;
	Person *next;
	Person *previous;
};

class City{
	public:
		City();
		~City();
		void buildCity();
		void printCity(Person *head);
		void addPerson(Person *head, string newPersonName, string previousName);
		void deletePerson(Person *head, string removePerson);
		Person *head;
	private:
		//Person *head;
		Person *tail;
};


//////////////////
//Graph structs and classes
/////////////////

template<class T>
struct adjVertex{
    vertex<T> *v;
    int weight;
};

template<class T>
struct vertex{
    T name;
    std::vector<adjVertex<T> > adj;
    bool visited = false;
    int districtID;
    bool solved = false;
    vertex<T> *parent;
    int distance = 0;
    Person *next = nullptr;
};

template<class T>
class Graph{
	public:
		Graph();
		~Graph();
		void addEdge(T v1, T v2, int weight);
		void addVertex(T name);
		void displayEdges();
		void breadthFirstTraversal();
		void findShortestPath(std::string startVal, std::string endVal);
		vertex <T> *searchCity(T val);
		
		//Dijkstra and roadtrip not working... delete when done
		vertex <T> *Dijkstra(std::string start, std::string end);
		std::vector<vertex<T> > vertices;
	protected:
	private:
		//std::vector<vertex<T> > vertices;
};

#endif 