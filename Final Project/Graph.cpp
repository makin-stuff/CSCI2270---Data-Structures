#include "Graph.h"
#include <vector>
#include <queue>
#include <iostream>
using namespace std;

//LINKED LIST METHODS
//constructor
City::City(){
	head = nullptr;
	tail = nullptr;	
}

//destructor
City::~City(){
	
}

void City::buildCity(){
	
	//start with an array of towns people
	string people[6] = {"Peter", "Paul", "Mary", "Sam", "John", "Alex"};
	
	Person *currentPerson = new Person;
	currentPerson->personName = "Peter";
	currentPerson->next = nullptr;
	currentPerson->previous = nullptr;
	head = currentPerson;
	
	//assign the following townspeople to the linked list
	for(int i = 1; i < 6; i++){
		Person *temp = new Person;
		
		temp->personName = people[i];
		temp->next = nullptr;
		temp->previous = currentPerson;
		currentPerson->next = temp;

		currentPerson = currentPerson->next; 	
	}
	
	//assign the tail
	if(currentPerson->next == nullptr){
		tail = currentPerson;
	}
}

void City::printCity(Person *head){
	Person *temp = head;
	
	//if the list is not empty
	if(head != nullptr){
		cout << " -> ";
		
		//loop through and print the peoples names
		while(temp->next != nullptr){
			cout << temp->personName << " <-> ";
			temp = temp->next;
		}
		
		//dont forget to print the last person
		cout << temp->personName << " -> nullptr" << endl;
	}
	else{
		cout << " -> nullptr" << endl;
	}
}

//can't add person before first person in linked list...
void City::addPerson(Person *head, string newPersonName, string previousName){
	Person *add = new Person;
	add->personName = newPersonName;
	
	Person *temp = head;
	Person *left = nullptr;
	bool found = false;

		while(!found && temp != nullptr){
			if(temp->personName == previousName){
				found = true;
				left = temp;
			} else {
				temp = temp->next;
			}
		}
	
		//tail case
		if(left->next == nullptr){
			left->next = add;
			add->previous = left;
			tail = add;
		}
		
		//anything in the middle
		else{
			left->next->previous = add;
			add->previous = left;
			add->next = left->next;
			left->next = add;
		}
}

void City::deletePerson(Person *head, string removePerson){
	Person *temp = head;
	Person *del = nullptr;
	bool found = false;
	
	while(!found && temp != nullptr){
		if(temp->personName == removePerson){
			found = true;
			del = temp;
		} else {
			temp = temp->next;
		}
	}
	
	//middle or tail
	if(del == tail){
		tail = tail->previous;
		tail->next = nullptr;
		delete del;
	} else {
		del->previous->next = del->next;
		del->next->previous = del->previous;
		delete del;
	}
}


//GRAPH METHODS
//constructor
template<class T>
Graph<T>::Graph(){
	for (int i = 0; i < vertices.size(); ++i) {
		vertices[i].distance = 0;
		vertices[i].solved = false;
		vertices[i].parent = nullptr;
	}
}

//destructor
template<class T>
Graph<T>::~Graph(){

}

template<class T>
void Graph<T>::addEdge(T v1, T v2, int weight){
	for(int i = 0; i < vertices.size(); i++){
		if(vertices[i].name == v1){
			for(int j = 0; j < vertices.size(); j++){
				if(vertices[j].name == v2 && i != j){
					//create new vertex
					adjVertex<T> av;
					av.v = &vertices[j];
					av.weight = weight;
					vertices[i].adj.push_back(av);
				}
			}
		}
	}
}

template<class T>
void Graph<T>::addVertex(T n){
	bool found = false;

	//check to see if the vertex already exists
	for(int i = 0; i < vertices.size(); i++){
		if(vertices[i].name == n){
			found = true;
			cout << vertices[i].name << " found." << endl;
		}
	}

	//if not, create the new one
	if(found == false){
		vertex<T> v;
		v.name = n;
		v.districtID = -1;
		vertices.push_back(v);
	}
}

template<class T>
void Graph<T>::displayEdges(){
  //loop through all vertices
  for (int i = 0; i < vertices.size(); i++){
    cout << vertices[i].name << " --> ";
	
	//and adjacent vertices
    for (int j = 0; j < vertices[i].adj.size(); j++){
      cout << vertices[i].adj[j].v->name;
      if (j != vertices[i].adj.size() - 1) {
        cout << " *** ";
      }
    }
    cout << endl;
  }
}

template<class T>
void Graph<T>::breadthFirstTraversal(){
	vertex<T> *popper_vertex;
	queue<vertex<T>*> my_queue;
	int values = 1;

	for(int i = 0; i < vertices.size(); i++){
		popper_vertex = &vertices[i];

		if(popper_vertex->visited != true){
			popper_vertex->visited = true;
			my_queue.push(popper_vertex);

			while(!my_queue.empty()){
				vertex<T> *n = my_queue.front();
				my_queue.pop();
				n->districtID = values;

				for(int x = 0; x < n->adj.size(); x++){
					if(!n->adj[x].v->visited){
						n->adj[x].v->visited = true;
						my_queue.push(n->adj[x].v);
					}
				}
			}
			values++;
		}
	}
}

template<class T>
void Graph<T>::findShortestPath(string startPoint, string endPoint){
	vertex<T> * v = searchCity(startPoint);
	vertex<T> * end = searchCity(endPoint);
	queue<vertex<T>*> q;
	vector<vertex<T>*> path;

        vertex<T> * n = nullptr;
        if(v == nullptr)
        {
                std::cout << "One or more cities doesn't exist" << std::endl;
                return;
        }

        if(v->districtID != end->districtID)
        {
                std::cout << "No safe path between cities" << std::endl;
                return;
        }

	for(int i = 0; i < vertices.size(); i++)
		vertices[i].visited = false;
	
	q.push(v);
	v->visited = true;
	v->distance = 0;
	v->parent = nullptr;
	int currentDist = 0;
	
	while(!q.empty())
	{
		n = q.front();
		q.pop();
		
		for(int i = 0; i < n->adj.size(); i++)
		{
			if(n->adj[i].v->visited == false)
			{
				q.push(n->adj[i].v);
				n->adj[i].v->distance = n->distance + 1;
				n->adj[i].v->visited = true;
				n->adj[i].v->parent = n;
				if(n->adj[i].v->name == endPoint)
					break;
			}
		}
	}
	
	vertex<T> * v1 = end;
	while(v1->parent != nullptr)
	{
		path.push_back(v1);
		v1 = v1->parent;
	}
	path.push_back(v);
	std::cout << path.size() - 1 << ",";
	for(int i = path.size() - 1; i != -1; i--)
	{
		if(i == 0)
		{
			std::cout << path[i]->name << std::endl;
			return;
		}
		std::cout << path[i]->name << ",";
	}
	return;
}

template<class T>
vertex<T>* Graph<T>::searchCity(T start){
	bool found = false;
	for(int i = 0; i < vertices.size(); i++){

		if(vertices[i].name == start){
			found = true;
			return &vertices[i];
		}
	}

	if(found == false){
		return nullptr;
	}
	return nullptr;
}

template<class T>
vertex<T> *Graph<T>::Dijkstra(string start, string end){
	vertex<T> *s;
	vertex<T> *solvedV = new vertex<T>;
	vertex<T> *parent = new vertex<T>;
	int minDistance = 0;
	int dist = 0;
	vertex<T> *startV = searchCity(start);
	vertex<T> *endV = searchCity(end);

	if(startV == nullptr){
		cout << "One or more cities doesn't exist" << endl;
		return nullptr;
	}
	if(endV == nullptr){
		cout << "One or more cities doesn't exist" << endl;
		return nullptr;
	}

	if(startV->districtID != endV->districtID){
		cout << "No safe path between cities" << endl;
		return nullptr;
	}
//	if(startV->districtID == -1 || endV->districtID == -1){
//		cout << "Please identify the districts before checking distances" << endl;
//		return 0;
//	}

	startV->solved = true;
	startV->distance = 0;

	for(int h = 0; h < vertices.size(); h++){
		vertices[h].solved = false;
	}

	//list of solved vertices
	vector <vertex<T>*> solved;
	solved.push_back(startV);

	while(!endV->solved){
		minDistance = 20000;
		solvedV = nullptr;
		vertex<T>* s = new vertex<T>;

		for(int x = 0; x < solved.size(); x++){

			s = solved[x];
			for(int y = 0; y < s->adj.size(); y++){

				if(!s->adj[y].v->solved){
					dist = s->distance + s->adj[y].weight;

					if(dist < minDistance){
						solvedV = s->adj[y].v;
						minDistance = dist;
						parent = s;
					}
				}
			}
		}

		solvedV->distance = minDistance;
		solvedV->parent = parent;
		solvedV->solved = true;
		solved.push_back(solvedV);
	}

	cout << solvedV->distance << ",";
	vector <string> cities;
	vertex<T> * city;
	city = solvedV;
	cities.push_back(city->name);

	while(city != startV){
		city = city->parent;
		cities.push_back(city->name);
	}

	for(int x = cities.size() - 1; x <= 0; x--){
		if(x == 0)
			cout << cities[x] << endl;
		else
			cout << cities[x] << ",";
	}
	cout << endl;
	return nullptr;
}