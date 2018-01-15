//Priyanka Makin, 06/15/2017
//CSCI2270 - Data Structures, Assignment 1
//Code reads in messageBoard.txt and matches up items for sale with 
//items wanted.
//Referred to code from wwww.cplusplus.com

#include <iostream>
#include <fstream>			//stream class to read from files
#include <stdlib.h>			//stoi
using namespace std;


struct item{
	string type;
	int price;
	bool status;			//0 = for sale, 1 = wanted
};

void deleteArrayElement(item A[], int index, int numElements);

int main(){
	
	//user inputs name of file and file opens
	string filename;
	cin>>filename;
	ifstream messageBoard;
	messageBoard.open(filename);

	int maxSize = 100;
	item list[maxSize];
	int countItems = 0;

	//counters
	int searchCounter = 0;
	int shiftCounter = 0; 

	if(messageBoard.is_open()){

		//reading and parsing through the .txt file and storing "items" in array "list"
		string newLine;
		string restLine;
		string status;

		while(getline(messageBoard, newLine)){			//Recall: filename.txt lines look like this "chicken, for sale, 60"

			//First, find and store item type
			list[countItems].type = newLine.substr(0, newLine.find(","));		//.find finds the FIRST occurrence
			restLine = newLine.substr(newLine.find(","), newLine.length());
			restLine = restLine.erase(0,2);				//delete ", " to make restLine = "for sale, 60" 

			//Find and store item status
			status = restLine.substr(0, restLine.find(","));
			if(status == "for sale"){
				list[countItems].status = false;
			}
			if(status == "wanted"){
				list[countItems].status = true;
			}

			//Find and store item price
			restLine = restLine.substr(restLine.find(","), restLine.length());
			restLine = restLine.erase(0, 2);			//delete ", " to make restLine = "60"
			list[countItems].price = stoi(restLine);	//converts the string "60" to int 60

			//Move on to the next line and iterate to next item in array "list"
			countItems++;
		}


		//Now, go through the "list" array and look for matches. Unmatched items go in their own array
		int sizeNotMatched = countItems;
		item notMatchedList[sizeNotMatched];

		//iterates through "list"
		for(int i = 0; i < countItems; i++){

			bool found = 0;			//0 = no match found, 1 = match found

			//item read
			cout << "item read " << list[i].type << " cost " << list[i].price << endl;

			//iterates through "notMatchedList"
			for(int j = 0; j < countItems; j++){

				//MATCH FOUND:
				//has the items match already been found?
				//are the items of the same type?
				//is the item in "list" for sale?
				//is the item in "notMatchedList" wanted?
				//do the prices agree?
				if(found == 0 && notMatchedList[j].type == list[i].type && list[i].status == 0 && notMatchedList[j].status == 1 && list[i].price <= notMatchedList[j].price){
					
					//print the match
					cout << list[i].type << " " << list[i].price << endl;
					
					//erase element and shift the array
					found = 1;
					deleteArrayElement(notMatchedList, j, countItems);
					sizeNotMatched--;

					//counters
					//searched through the array j-times, +1 because j startes at zero
					searchCounter = searchCounter + j + 1;
					//shifted through the size of the not matched array up until j
					shiftCounter = shiftCounter + sizeNotMatched - j;
				}

				//MATCH FOUND:
				//has the items match already been found?
				//are the items of the same type?
				//is the item in "list" wanted?
				//is the item in "notMatchedList" for sale?
				//do the prices agree?
				if(found == 0 && notMatchedList[j].type == list[i].type && list[i].status == 1 && notMatchedList[j].status == 0 && list[i].price >= notMatchedList[j].price){

					//print the match
					cout << list[i].type << " " << notMatchedList[j].price << endl;

					//erase the element and shift the array
					found = 1;
					deleteArrayElement(notMatchedList, j, countItems);
					sizeNotMatched--;

					//counters
					searchCounter = searchCounter + j + 1;
					shiftCounter = shiftCounter + sizeNotMatched - j;
				}
			}

			//if no match found, add the item to the end of the "noMatchedList" array
			if(found == 0){
				notMatchedList[i] = list[i];

				searchCounter = searchCounter + sizeNotMatched;			//don't need to shift anything
			}
		}

		cout << "#" << endl;

		for(int k = 0; k < sizeNotMatched - 1; k++){
			cout << notMatchedList[k].type << endl;
		}
		cout << sizeNotMatched << endl;

		//delimiter
		cout << "#" << endl;

	} else {
		cout << "Unable to open file" << endl;		//prints if .txt file can't open
	}

	//print out the counts
	cout << "file read:" << countItems << endl;
	cout << "search array:" << searchCounter << endl;
	cout << "shift array:" << shiftCounter << endl;

	return 0;
}


//function straight from the textbook
void deleteArrayElement(item A[], int index, int numElements){
	for(int x = index; x < numElements - 1; x++){
		A[x] = A[x + 1];
	}
	numElements = numElements - 1;
}