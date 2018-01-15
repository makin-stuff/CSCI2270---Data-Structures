//Priyanka Makin, 07/04/2017
//CSCI2270 - Data Structures, Assignment 4
//User enters words and sentences at the command line to add words
//to the queue. We can also dequeue items and print the queue.


#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
using namespace std;


class Queue{
    public:
        Queue(int);
        ~Queue();
        void enqueue(std::string);
        std::string dequeue();
        void printQueue();
        bool queueIsFull();
        bool queueIsEmpty();

    protected:

    private:
        int queueSize;
        int queueHead;
        int queueTail;
        int queueCount;
        std::string *arrayQueue;
};


int main(int argc, char *argv[]){

    //Starting menu
    cout << "======Main Menu=====" << endl;
    cout << "1. Enqueue word" << endl;
    cout << "2. Dequeue word" << endl;
    cout << "3. Print queue" << endl;
    cout << "4. Enqueue sentence" << endl;
    cout << "5. Quit" << endl;

    //create queue
    int n = atoi(argv[1]);
    Queue qs(n);
    int menuChoice;
    cin >> menuChoice;

    while(menuChoice < 6){
        if(menuChoice == 5){
            cout << "Goodbye!" << endl;
            return 0;
        }
        if(menuChoice == 4){
            string sentence, word;
            cout << "sentence: ";
            cin.ignore();
           getline(cin, sentence);
           cout << sentence << endl;
            istringstream ss(sentence);
            while(ss >> word){
                qs.enqueue(word);
            }
        }
        if(menuChoice == 3){
            qs.printQueue();
        }
        if(menuChoice == 2){
            qs.dequeue();
        }
        if(menuChoice == 1){
            string enqueueWord;
            cin >> enqueueWord;
            qs.enqueue(enqueueWord);
        }

        //Starting Menu
        cout << "======Main Menu=====" << endl;
        cout << "1. Enqueue word" << endl;
        cout << "2. Dequeue word" << endl;
        cout << "3. Print queue" << endl;
        cout << "4. Enqueue sentence" << endl;
        cout << "5. Quit" << endl;

        cin >> menuChoice;
    }
    return 0;
}


//constructor
Queue::Queue(int qs){
    queueSize = qs;
    arrayQueue = new string[queueSize];
    queueHead = 0;
    queueTail = 0;
    queueCount = 0;
}

//destructor
Queue::~Queue(){
    //cout << "Goodbye!" << endl;
}

void Queue::enqueue(string word){

    //check that queue is not full
    if(!queueIsFull()){
        //add new word to array
       arrayQueue[queueTail] = word;
       queueCount++;

       //move tail
       if(queueTail == (queueSize - 1)){
            queueTail = 0;
       } else {
            queueTail++;
       }

        cout << "E: " << word << endl;
        cout << "H: " << queueHead << endl;
        cout << "T: " << queueTail << endl;
    } else {
        cout << "Queue is full" << endl;
    }
}

string Queue::dequeue(){

    //define the word to be deleted
    string word = arrayQueue[queueHead];

    //check to see if the queue is not empty
    if(!queueIsEmpty()){

        //delete the word
        arrayQueue[queueHead] = "";
        queueCount--;

        //move the head
        if(queueHead == (queueSize - 1)){
            queueHead = 0;
        } else {
            queueHead++;
        }

        cout << "H: " << queueHead << endl;
        cout << "T: " << queueTail << endl;
        cout << "word: " << word << endl;
    } else {
        cout << "Queue is empty" << endl;
    }
    return word;
}

void Queue::printQueue(){
    //create temp pointer
    int temp = queueHead;

    if(queueCount == 0){
        cout << "Empty" << endl;
    } else {

        //iterate through and print what is in the queue
        for(int i = 0; i < queueCount; i++){

            //if temp is at the "end" of the queue, have it wrap around to the front
            if(temp == queueSize){
                temp = 0;
            } else {
                cout << temp << ": " << arrayQueue[temp] << endl;
                temp++;
            }
        }
    }
}

bool Queue::queueIsFull(){
    if(queueCount == queueSize){
        return true;
    } else {
        return false;
    }
}

bool Queue::queueIsEmpty(){
    if(queueCount == 0){
        return true;
    } else {
        return false;
    }
}
