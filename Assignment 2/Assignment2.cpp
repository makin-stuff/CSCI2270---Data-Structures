//Priyanka Makin, 06/20/2017
//CSCI2270 - Data Structures, Assignment 2
//This code reads in a text file of unknown size and stores the words
//in a dynamically allocated array. The code is written to keep track 
//of the top n words, total number of unique words, total number of
//words, and number of array doublings to get to the end on the word
//document. 
//Not sure if I got this working 100%.

#include <iostream>
#include <fstream>
#include <stdlib.h>							//for stoi
#include "WordAnalysis.h"
using namespace std;


int main(){

	char *filename = new char[50];			//cin filename, needs to be an array of chars
	string nstring;							//cin how many words to print at end
	cin>>filename;
	cin>>nstring;
	int num = stoi(nstring);				//change string input to int

	WordAnalysis book(num); 

	if(book.readDataFile(filename)){
		book.printCommonWords(num);
		book.printResult(num);
	} else {
		cout << "Can't open file" << endl;
	}
}

//doubles the array and adds the "current" word to the array
void doubleArrayAndAdd(string current){
	originalSize = originalSize*2;
	word *doubleWords = new word[originalSize];
	for(int i = 0; i < originalSize; i++){
		doubleWords[i] = words[i];
	}
	doubleWords[originalSize] = current;
	delete []words;
	words = doubleWords;

	timesDoubled++;
}

//checks if the current word is on the list of common words
bool checkIfCommonWord(string currentWord){
	//array of common words
	string commonWords[50];
	commonWords[0] = "the";
	commonWords[1] = "be";
	commonWords[2] = "to";
	commonWords[3] = "of";
	commonWords[4] = "and";
	commonWords[5] = "a";
	commonWords[6] = "in";
	commonWords[7] = "that";
	commonWords[8] = "have";
	commonWords[9] = "i";
	commonWords[10] = "it";
	commonWords[11] = "for";
	commonWords[12] = "not";
	commonWords[13] = "on";
	commonWords[14] = "with";
	commonWords[15] = "he";
	commonWords[16] = "as";
	commonWords[17] = "you";
	commonWords[18] = "do";
	commonWords[19] = "at";
	commonWords[20] = "this";
	commonWords[21] = "but";
	commonWords[22] = "his";
	commonWords[23] = "by";
	commonWords[24] = "from";
	commonWords[25] = "they";
	commonWords[26] = "we";
	commonWords[27] = "say";
	commonWords[28] = "her";
	commonWords[29] = "she";
	commonWords[30] = "or";
	commonWords[31] = "an";
	commonWords[32] = "will";
	commonWords[33] = "my";
	commonWords[34] = "one";
	commonWords[35] = "all";
	commonWords[36] = "would";
	commonWords[37] = "there";
	commonWords[38] = "their";
	commonWords[39] = "what";
	commonWords[40] = "so";
	commonWords[41] = "up";
	commonWords[42] = "out";
	commonWords[43] = "if";
	commonWords[44] = "about";
	commonWords[45] = "who";
	commonWords[46] = "get";
	commonWords[47] = "which";
	commonWords[48] = "go";
	commonWords[49] = "me";

	bool common = false;
	for(int i = 0; i < 50; i++){
		if(currentWord == common[i]){
			common = true;
			break;
		}
	}
	return common;
}

//sorts the words in the words array in order of decreasing occurence
void sortData(){

	//I'm using insertion sort because it is the simplest method
	for(int i = 0; i < index; i++){
		int x = arr[i].count;
		int j = i;

		while(j > 0 && words[j-1].count < x){
			words[j].count = words[j-1].count;
			j = j - 1;
		}

		words[j].count = v;
	}
}

//opens the data file and reads in the data
bool readDataFile(char* file){
	bool open = true;
	ifstream textFile;
	textFile.open(file);
	if(textFile.is_open()){

		string book;
		int originalSize = 100;
		// word *words = new word[originalSize];

		wordCount = 0;

		while(textFile >> book){
			//first, check to see if the word is NOT a common word
			if(checkIfCommonWord(book) == false){

				//first item in the book
				if(wordCount == 0){
					words[0].w = book;
					words[0].count = 1;
				}



				bool found = false;
				//check to see if we are at a word we have found before
				for(int i = 0; i < wordCount; i++){
					if(book == words[i].w){
						words[j].count++;
						found = true;
						break;
					}
				}
				//if we find a unique word
				if(found == false){
					words[index].w = book;
					words[index].count = 1;
					index++;
				}
				wordCount++;
			}

			//check the size of the array
			if(index + 1 == originalSize){
				//double the array
				doubleArrayAndAdd(book);
				originalSize = originalSize*2
				wordCount = wordCount*2;
			}
		}
	} else {
		open = false;
	}
	return open;
}

int getWordCount(){
	return wordCount;
}

int getUniqueWordCount(){
	return index;
}

int getArrayDoubling(){
	return timesDoubled;
}

void printCommonWords(int num){
	sortData();
	for(int i = 0; i < num; i++){
		cout << words[i].count + " - " + words[i].w <<endl;
	}

}

void printResult(int){
	int numDoubled = getArrayDoubling();
	int numUnique = getUniqueWordCount();
	int numWord = getWordCount();

	cout << "#" << endl;
	cout << "Array doubled: " << numDoubled << endl;
	cout << "#" << endl;
	cout << "Unique non-common words: " << numUnique << endl;
	cout << "#" << endl;
	cout << "Total non-common words: " << numWord << endl;
}

WordAnalysis(int num){
	timesDoubled = 0;
	word *words = new word[originalSize];			//not sure if this line is correct
	wordCount = 0;
	index = 0;
}
