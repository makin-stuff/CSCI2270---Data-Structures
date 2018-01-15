//Priyanka Makin, 07/12/2017
//CSCI2270 - Data Structures, Assignment 5
//This code reads in Assignment5Movies.txt and places the movies into
//a binary search tree by movie title. From there, the user can find
//a movie, rent a movie, print the inventory, delete a movie, and count
//all the movies in the tree.


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;


struct movie{
    int rank;
    string title;
    int year;
    int quantity;
    movie *parent;
    movie *leftChild;
    movie *rightChild;

    movie(){};

    movie(int _rank, string _title, int _year, int _quantity){
        rank = _rank;
        title = _title;
        year = _year;
        quantity = _quantity;
        parent = nullptr;
        leftChild = nullptr;
        rightChild = nullptr;
    }
};

class movieTree{
    public:
        movieTree();
        ~movieTree();
        void findMovie(string title);
        void rentMovie(string title);
        void printMovieInventory();
        void deleteMovieNode(string title);
        int countMovieNodes();
        void addMovieNode(int rank, string title, int year, int quantity);

    protected:

    private:
        void deleteAll(movie *node);
        void printMovieInventory(movie *node);
        void countMovieNodes(movie *node, int *c);
        movie* search(movie *found, string title);
        movie* treeMinimum(movie *node);
        movie* root;
};


void displayMenu(){
    cout << "======Main Menu======" << endl;
    cout << "1. Find a movie" << endl;
    cout << "2. Rent a movie" << endl;
    cout << "3. Print the inventory" << endl;
    cout << "4. Delete a movie" << endl;
    cout << "5. Count the movies" << endl;
    cout << "6. Quit" << endl;
}

int main(int argc, char* argv[]){

    movieTree tree;
    string line;

    ifstream movieFile;
    movieFile.open(argv[1]);

    //read in text file line by line
    while(getline(movieFile, line)){
        //assign values from text file to variables
        stringstream ss(line);

        string rank;
        getline(ss, rank, ',');

        string title;
        getline(ss, title, ',');

        string year;
        getline(ss, year, ',');

        string quantity;
        getline(ss, quantity, ',');

        //create the movie node
        tree.addMovieNode(stoi(rank), title, stoi(year), stoi(quantity));
    }

    //display menu and get user's menu choice
    bool quit = false;
    int menuChoice;

    while(!quit){
        displayMenu();

        cin >> menuChoice;

        switch(menuChoice){
            case 1:{
                string title;
                cout << "Enter title:" << endl;
                //cin >> title;
                cin.ignore();
                getline(cin, title);
                tree.findMovie(title);
                break;
            }

            case 2:{
                string title;
                cout << "Enter title:" << endl;
                //cin >> title;
                cin.ignore();
                getline(cin, title);
                tree.rentMovie(title);
                break;
            }

            case 3:{
                tree.printMovieInventory();
                break;
            }

            case 4:{
                string title;
                cout << "Enter title:" << endl;
                //cin >> title;
                cin.ignore();
                getline(cin, title);
                tree.deleteMovieNode(title);
                break;
            }

            case 5:{
                tree.countMovieNodes();
                break;
            }

            case 6:{
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


//constructor
movieTree::movieTree(){
    root = nullptr;
}

//destructor
movieTree::~movieTree(){
    deleteAll(root);
}

void movieTree::findMovie(string title){
    //find the movie
    movie *foundMovie = search(root, title);

    if(foundMovie == nullptr){
        cout << "Movie not found." << endl;
        return;
    } else {
        cout << "Movie Info:" << endl;
        cout << "===========" << endl;
        cout << "Ranking:" << foundMovie->rank << endl;
        cout << "Title:" << foundMovie->title << endl;
        cout << "Year:" << foundMovie->year << endl;
        cout << "Quantity:" << foundMovie->quantity << endl;
    }
}

void movieTree::rentMovie(string title){

    //find the movie
    movie *found = search(root, title);

    if(found == nullptr){
        cout << "Movie not found." << endl;
        return;
    } else {

        found->quantity--;
        cout << "Movie has been rented." << endl;

        cout << "Movie Info:" << endl;
        cout << "===========" << endl;
        cout << "Ranking:" << found->rank << endl;
        cout << "Title:" << found->title << endl;
        cout << "Year:" << found->year << endl;
        cout << "Quantity:" << found->quantity << endl;

        if(found->quantity == 0){
            deleteMovieNode(title);
        }
    }
}

void movieTree::printMovieInventory(){
    if(root == nullptr){
        cout << "Movie inventory is empty" << endl;
    } else {
        printMovieInventory(root);
    }
}

void movieTree::deleteMovieNode(string title){
    //find movie to be deleted
    movie *del = search(root, title);
    if(del == nullptr){
        cout << "Movie not found." << endl;
    }

    else{

        //CASE 1: Node has no children
        if(del->leftChild == nullptr && del->rightChild == nullptr){

            //assign nullptr to either left or right child of node above del
            if(del->parent->leftChild == del){
                del->parent->leftChild = nullptr;
            } else {
                del->parent->rightChild = nullptr;
            }
            delete del;
        }

        //CASE 2: Node has only 1 child
        //node to be deleted has only a left child
        else if(del->rightChild == nullptr){
            //if del is a left child
            if(del->parent->leftChild == del){
                del->parent->leftChild = del->leftChild;
                del->leftChild->parent = del->parent;
            }
            //else del is a right child
            else {
                del->parent->rightChild = del->leftChild;
                del->leftChild->parent = del->parent;
            }
            delete del;
        }
        //node to be deleted has only a right child
        else if(del->leftChild == nullptr){
            //if del is a left child
            if(del->parent->leftChild == del){
                del->parent->leftChild = del->rightChild;
                del->rightChild->parent = del->parent;
            }
            //else del is a right child
            else {
                del->parent->rightChild = del->rightChild;
                del->rightChild->parent = del->parent;
            }
            delete del;
        }

        //CASE 3: Node has 2 children
        else{
            movie *min = treeMinimum(del);

            if(min->rightChild != nullptr){
                min->rightChild->parent = min->parent;
                min->parent->leftChild = min->rightChild;
            } else {
                min->parent->leftChild = nullptr;
            }
            delete min;
        }
    }
}

int movieTree::countMovieNodes(){
    int count = 0;
    int *c = &count;
    countMovieNodes(root, c);

    cout << "Tree contains: " << count << " movies." << endl;
    return count;
}

void movieTree::addMovieNode(int rank, string title, int year, int quantity){

    //create new movie node
    movie *node = new movie;
    node->rank = rank;
    node->title = title;
    node->year = year;
    node->quantity = quantity;
    node->leftChild = nullptr;
    node->rightChild = nullptr;

    //if first node make it the root
    if(root == nullptr){
        node->parent = nullptr;
        root = node;
        return;
    }
    else{

        movie *currentMovie = search(root, title);

        if(currentMovie == nullptr){
            currentMovie = root;

            while(currentMovie != nullptr){

                if(currentMovie->title > title){

                    if(currentMovie->leftChild == nullptr){
                        currentMovie->leftChild = node;
                        node->parent = currentMovie;
                        return;
                    }
                    else {
                        currentMovie = currentMovie->leftChild;
                    }

                } else {

                    if(currentMovie->rightChild == nullptr){
                        currentMovie->rightChild = node;
                        node->parent = currentMovie;
                        return;
                    } else {
                        currentMovie = currentMovie->rightChild;
                    }
                }
            }
        }
    }
}

void movieTree::deleteAll(movie *node){
    if(node == nullptr){
        return;
    } else {
        if(node->leftChild != nullptr){
            deleteAll(node->leftChild);
        }
        if(node->rightChild != nullptr){
            deleteAll(node->rightChild);
        }

        cout << "Deleting: " << node->title << endl;
        delete node;
    }
    return;
}


//HELPER FUNCTIONS
void movieTree::printMovieInventory(movie *node){
    if(node == nullptr){
        return;
    } else {
        printMovieInventory(node->leftChild);
        cout << "Movie: " << node->title << " " << node->quantity << endl;
        printMovieInventory(node->rightChild);
    }
}

void movieTree::countMovieNodes(movie *node, int *c){
    if(node == nullptr && node == root){
        *c = 0;
        return;
    } else if(node == nullptr){
        return;
    } else {
        countMovieNodes(node->leftChild, c);
        countMovieNodes(node->rightChild, c);
        *c += 1;
    }
}

movie* movieTree::search(movie *found, string title){
    //movie *found = root;

    if(found != nullptr){
        if(found->title == title)
            return found;
        else if(found->title > title)
            return search(found->leftChild, title);
        else
            return search(found->rightChild, title);
    } else {
        return nullptr;
    }
}

movie* movieTree::treeMinimum(movie *node){
    if(node == nullptr){
        return(0);
    }
    else {
        movie *minRight = node->rightChild;

        while(minRight->leftChild != nullptr){
            minRight = minRight->leftChild;
        }

        node->title = minRight->title;
        return minRight;
    }
}
