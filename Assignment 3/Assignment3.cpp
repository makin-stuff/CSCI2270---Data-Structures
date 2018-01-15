//Priyanka Makin, 06/27/2017
//CSCI2270 - Data Structures, Assignment 3
//This code simulates a communication network using a linked list. Each
//node represents a city and a message needs to be sent between them.


#include <string>
//#include "CommunicationNetwork.hpp"
#include <iostream>
#include <fstream>
using namespace std;


struct City{
    std::string cityName;
    std::string message;
    City *next;
    City *previous;

    City(){}; // default constructor

    City(std::string initName, City *initNext, City *initPrevious, std::string initMessage)
    {
        cityName = initName;
        next = initNext;
        previous = initPrevious;
        message = initMessage;
    }

};

class CommunicationNetwork
{
    public:
        CommunicationNetwork();
        ~CommunicationNetwork();
        void addCity(std::string, std::string);
        void buildNetwork();
        void transmitMsg(char *); //this is like a string
        void printNetwork();
        void deleteCity(std::string);
        void deleteNetwork();
    protected:
    private:
        City *head;
        City *tail;
};


int main(){

	//Starting menu
	cout << "======Main Menu======" << endl;
	cout << "1. Build Network" << endl;
	cout << "2. Print Network Path" << endl;
	cout << "3. Transmit Message Coast-To-Coast-To-Coast" << endl;
	cout << "4. Add City" << endl;
	cout << "5. Delete City" << endl;
	cout << "6. Clear Network" << endl;
	cout << "7. Quit" << endl;

	//initialize class
	CommunicationNetwork network;

	//user input options
	int menuChoice;
	cin >> menuChoice;
	char *filename = new char[50];
	string newCity;
	string prevCity;
	string deleteCity;

	while(menuChoice < 8){
        if(menuChoice == 7){
            cout << "Goodbye!" << endl;
            return 0;
        }
		if(menuChoice == 1){
			network.buildNetwork();
		}
		if(menuChoice == 2){
			network.printNetwork();
		}
		if(menuChoice == 3){
			cout << "Enter name of file: " << endl;
			cin >> filename;
			network.transmitMsg(filename);
		}
		if(menuChoice == 4){
			cout << "Enter a city name: " << endl;
            cin >> newCity;
			cout << "Enter a previous city name: " << endl;
			cin >> prevCity;
			network.addCity(newCity, prevCity);
		}
		if(menuChoice == 5){
			cout << "Enter a city name: " << endl;
            cin >> deleteCity;
			network.deleteCity(deleteCity);
		}
		if(menuChoice == 6){
			network.deleteNetwork();
		}

		//Starting menu
		cout << "======Main Menu======" << endl;
		cout << "1. Build Network" << endl;
		cout << "2. Print Network Path" << endl;
		cout << "3. Transmit Message Coast-To-Coast-To-Coast" << endl;
		cout << "4. Add City" << endl;
		cout << "5. Delete City" << endl;
		cout << "6. Clear Network" << endl;
		cout << "7. Quit" << endl;

		cin >> menuChoice;
	}
	return 0;
}

//constructor
//what to do with this??
CommunicationNetwork::CommunicationNetwork(){
    head = nullptr;
    tail = nullptr;
}

//destructor
//what to do with this??
CommunicationNetwork::~CommunicationNetwork(){
    deleteNetwork();
}

//create starting linked list
void CommunicationNetwork::buildNetwork(){

	//start with an array of starting city names
	string cities[6] = {"Miami", "New York", "Chicago", "Seattle", "Denver", "Dallas"};

	//assign currentCity parameters and create the head node
	City *currentCity = new City;
	currentCity->cityName = "Miami";
	currentCity->message = "";
	currentCity->next = nullptr;
	currentCity->previous = nullptr;
	head = currentCity;

	//assign the following cities into the linked list
	for(int i = 1; i < 6; i++){
		City *temp = new City;

		temp->cityName = cities[i];
		temp->message = "";
		temp->next = nullptr;
		temp->previous = currentCity;
		currentCity->next = temp;
		currentCity = currentCity->next;
	}

	//assign the tail
	if(currentCity->next == nullptr){
		tail = currentCity;
	}

	//print the original list
	CommunicationNetwork::printNetwork();
}


void CommunicationNetwork::printNetwork(){

	//starting print statements
	cout << "===CURRENT PATH===" << endl;

	City *temp = head;

	//if the list is not empty
	if(head != nullptr){
		cout << "NULL <- ";

		//loop through and print the city names
		while(temp->next != nullptr){
			cout << temp->cityName << " <-> ";
			temp = temp->next;
		}

		//don't forget to print the last city
		cout << temp->cityName << " -> NULL" << endl;

        //ending print statement
        cout << "==================" << endl;
	}
	else{
        cout << "NULL" << endl;
	}


}


void CommunicationNetwork::transmitMsg(char *filename){

	//if the list is not empty
	if(head != nullptr){
		ifstream message;
		message.open(filename);

		//if the message opens
		if(message.is_open()){

			//Read the message from start to end of network
			string word;
			City *temp = new City;

			while(message >> word){
				temp = head;
				while(temp != nullptr){
					temp->message = word;
					cout << temp->cityName << " received " << temp->message << endl;
					temp = temp->next;
				}
			}

			//Read the message from end to start of network
			while(message >> word){
				temp = tail;
				while(temp != nullptr){
					temp->message = word;
					//cout << temp->cityName << " received " << temp->message << endl;
					temp = temp->previous;
				}
			}


		} else{
			cout << "Can't open message!" << endl;
		}
	} else{
		cout << "Empty network!" << endl;
	}
}

void CommunicationNetwork::addCity(std::string newCityName, std::string previousCityName){

	City *add = new City;
	add->cityName = newCityName;
	add->message = "";

    City *temp = head;
    City *left = nullptr;
    bool found = false;

	//check to see if new city needs to be inserted before the head
	if(previousCityName == "First"){
		add->next = head;
		add->previous = nullptr;
		//reassign head
		head = add;
	}

	else{
        //search for previous (left) city -- algorithm from book
        while(!found && temp != NULL){
            if(temp->cityName == previousCityName){
                found = true;
                left = temp;
            }else{
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
}

void CommunicationNetwork::deleteCity(std::string removeCity){
	//create temp nodes
	City *temp = head;
	City *del = nullptr;
	bool found = false;

    while(!found && temp != NULL){
        if(temp->cityName == removeCity){
            found = true;
            del = temp;
        }else{
            temp = temp->next;
        }
    }

    //head case
    if(del == head){
        head = head->next;
        head->previous = nullptr;
        delete del;
    }
    //middle or tail
    else{
        if(del == tail){
            tail = tail->previous;
            tail->next = nullptr;
            delete del;
        }
        else{
            del->previous->next = del->next;
            del->next->previous = del->previous;
            delete del;
        }
    }
}

void CommunicationNetwork::deleteNetwork(){

	//loop through to delete cities until the whole network is gone
	City *currentCity = head;
	while(currentCity != nullptr){
		City *deleteCity = currentCity;
		currentCity = currentCity->next;
		cout << "deleting " << deleteCity->cityName << endl;
		delete deleteCity;

		//once everything is deleted, make head point to a nullptr
		if(currentCity == nullptr){
			head = currentCity;
		}
	}
}

