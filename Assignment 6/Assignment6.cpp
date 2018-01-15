//Priyanka Makin, 07/18/2017
//CSCI2270 - Data Structures, Assignment 6
//Same as assignment 5 but this time we place movie nodes in a red-
//black tree. This means that as we add and delete nodes, the tree is
//self-balancing. There are various rules to follow that make a tree
//"balanced".


//WORKSSS - i think....
#include <iostream>
//#include "MovieTree.hpp"
#include <fstream>
#include <sstream>
using namespace std;

struct MovieNode{
    int ranking;
    std::string title;
    int year;
    int quantity;
    bool isRed;
    MovieNode *parent;
    MovieNode *leftChild;
    MovieNode *rightChild;

    MovieNode(){};

    MovieNode(int in_ranking, std::string in_title, int in_year, int in_quantity)
    {
        ranking = in_ranking;
        title = in_title;
        year = in_year;
        quantity = in_quantity;
        // Now that we are using nil these NULL's should be overwritten in addMovieNode.
        leftChild = nullptr;
        rightChild = nullptr;
        parent = nullptr;
        isRed = true;
    }

};

class MovieTree
{

public:
    MovieTree();
    virtual ~MovieTree();
    void printMovieInventory();
    int countMovieNodes();
    void deleteMovieNode(std::string title);
    void addMovieNode(int ranking, std::string title, int releaseYear, int quantity);
    void findMovie(std::string title);
    void rentMovie(std::string title);
    bool isValid();
    int countLongestPath();

protected:

private:
    void DeleteAll(MovieNode * node); //use this for the post-order traversal deletion of the tree
    void printMovieInventory(MovieNode * node);
    void rbAddFixup(MovieNode * node); // called after insert to fix tree
    void leftRotate(MovieNode * x); //rotate the tree left with x as the root of the rotation
    void rbDelete(MovieNode * z); //delete a node. Call this from deleteMovieNode, the actual delete functionality happens here.
    void rightRotate(MovieNode * x); //rotate the tree right with x as the root of the rotation
    void rbDeleteFixup(MovieNode * node, MovieNode * nodeParent, bool nodeIsLeft); //called after delete to fix the tree
    void rbTransplant(MovieNode * u, MovieNode * v); //replace node u in tree with node v. Your solution doesn't necessarily need to use this method
    int rbValid(MovieNode * node); //check if the tree is valid, with node as the root of the tree
    int countMovieNodes(MovieNode *node, int *count); //number of unique titles in the tree
    int countLongestPath(MovieNode *node, int *path); //longest path from node to a leaf node in the tree
    MovieNode* searchMovieTree(MovieNode * node, std::string title);

    //added these methods
    MovieNode* getSuccessor(MovieNode * x);
    MovieNode* treeMinimum(MovieNode *x);

    MovieNode *root;
    MovieNode *nil;


};

void displayMenu(){

    cout << "======Main Menu======" << endl;
    cout << "1. Find a movie" << endl;
    cout << "2. Rent a movie" << endl;
    cout << "3. Print the inventory" << endl;
    cout << "4. Delete a movie" << endl;
    cout << "5. Count the movies" << endl;
    cout << "6. Count the longest path" << endl;
    cout << "7. Quit" << endl;
}

int main(int argc, char* argv[]){

    MovieTree tree;
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
                tree.countLongestPath();
                break;
            }

            case 7:{
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


///////////////////////////////////////
///////////PUBLIC METHODS//////////////
///////////////////////////////////////

//constructor
MovieTree::MovieTree(){
    nil = new MovieNode();
    nil->isRed = false;
    nil->leftChild = nil;
    nil->rightChild = nil;
    nil->parent = nil;

    root = nil;
}

//destructor
MovieTree::~MovieTree(){
    DeleteAll(root);
}

void MovieTree::printMovieInventory(){
    if(root == nil){
        cout << "Movie inventory is empty" << endl;
    } else {
        printMovieInventory(root);
    }
}

int MovieTree::countMovieNodes(){
    int count = 0;
    countMovieNodes(root, &count);

    cout << "Tree contains: " << count << " movies." << endl;
    return count;
}

void MovieTree::deleteMovieNode(string title){

    //if tree is empty
    if(root == nil){
        cout << "Empty tree" << endl;
        return;
    }

    //find node to be deleted
    MovieNode *del = searchMovieTree(root, title);

    //if movie can't be found
    if(del == nil){
        cout << "Movie not found." << endl;
        return;
    }
    //else
    rbDelete(del);
}

void MovieTree::addMovieNode(int ranking, string title, int releaseYear, int quantity){

    //create movie node with given parameters
    MovieNode *movie = new MovieNode;
    movie->ranking = ranking;
    movie->title = title;
    movie->year = releaseYear;
    movie->quantity = quantity;
    movie->leftChild = nil;
    movie->rightChild = nil;
    movie->isRed = true;

    //if there are no nodes yet, add this one as the root
    if(root == nil){
        movie->parent = nil;
        root = movie;
        rbAddFixup(movie);          //turns the new node black
    }

    //there is at least one node in the tree
    else{

        MovieNode *currentMovie = searchMovieTree(root, title);

        if(currentMovie == nil){

            //start at the root
            currentMovie = root;
            while(currentMovie != nil){

                //compare node to given title
                //if given title is less than current title, move left until you reach nil
                //then, reassign pointers
                if(currentMovie->title > title){

                    if(currentMovie->leftChild == nil){
                        currentMovie->leftChild = movie;
                        movie->parent = currentMovie;
                        rbAddFixup(movie);
                        break;
                    } else {
                        currentMovie = currentMovie->leftChild;
                    }
                }

                //given title is greater than current title
                //keep moving right until you reach nil
                //then, reassign pointers
                else{
                    if(currentMovie->rightChild == nil){
                        currentMovie->rightChild = movie;
                        movie->parent = currentMovie;
                        rbAddFixup(movie);
                        break;
                    } else {
                        currentMovie = currentMovie->rightChild;
                    }
                }
            }
        }
    }

    if(rbValid(root) == 0){
        cout << "Tree Invalid" << endl;
        exit(0);
    }
}

void MovieTree::findMovie(string title){
    //first, find movie
    MovieNode *found = searchMovieTree(root, title);

    if(found == nil){
        cout << "Movie not found." << endl;
        return;
    } else {
        cout << "Movie Info:" << endl;
        cout << "===========" << endl;
        cout << "Ranking:" << found->ranking << endl;
        cout << "Title:" << found->title << endl;
        cout << "Year:" << found->year << endl;
        cout << "Quantity:" << found->quantity << endl;
    }
}

void MovieTree::rentMovie(string title){
    //first, find movie
    MovieNode *found = searchMovieTree(root, title);

    if(found == nil){
        cout << "Movie not found." << endl;
        return;
    } else {

        //change the quantity
        found->quantity--;
        cout << "Movie has been rented." << endl;
        cout << "Movie Info:" << endl;
        cout << "===========" << endl;
        cout << "Ranking:" << found->ranking << endl;
        cout << "Title:" << found->title << endl;
        cout << "Year:" << found->year << endl;
        cout << "Quantity:" << found->quantity << endl;

        //does the movie need to be deleted from the tree??
        if(found->quantity == 0){
            rbDelete(found);
        }
    }
}

bool MovieTree::isValid(){

    //rbValid returns an int
    int valid = rbValid(root);
    if(valid == 0)
        return false;
    return true;
}

int MovieTree::countLongestPath(){
    int path = 0;
    countLongestPath(root, &path);
    cout << "Longest Path: " << path << endl;
    return 0;
}


///////////////////////////////////////
///////////PRIVATE METHODS/////////////
///////////////////////////////////////

void MovieTree::DeleteAll(MovieNode *node){

    //if empty tree
    if(node == nil){
        return;
    } else {

        //go left and delete all
        if(node->leftChild != nil){
            DeleteAll(node->leftChild);
        }
        //go right and delete all
        if(node->rightChild != nil){
            DeleteAll(node->rightChild);
        }
        cout << "Deleting: " << node->title << endl;
        delete node;
    }
}

void MovieTree::printMovieInventory(MovieNode *node){

    //if tree is empty
    if(node == nil){
        return;
    } else {
        printMovieInventory(node->leftChild);
        cout << "Movie: " << node->title << " " << node->quantity << endl;
        printMovieInventory(node->rightChild);
    }
}

void MovieTree::rbAddFixup(MovieNode *node){

    //if node is root, change to black
    if(node == root){
        node->isRed = false;
        return;
    }

    MovieNode *uncle;

    //if color of node's parent is not black and node is not root
    while(node != root && node->parent->isRed == true){

        //grand parent
        MovieNode *g = node->parent->parent;

        //if node's parent is a left child
        if(g->leftChild == node->parent){

            //assign uncle
            uncle = g->rightChild;

            //if node's uncle is red
            if(uncle->isRed == true){

                //1. change node's parent and uncle color to black
                //2. change grandparent to red
                //3. make node = node's grandparent
                node->parent->isRed = false;
                uncle->isRed = false;
                g->isRed = true;
                node = g;
            }
            //else, uncle is black
            else{
                if(node->parent->rightChild == node){
                    node = node->parent;
                    leftRotate(node);
                }
                node->parent->isRed = false;
                g->isRed = true;
                rightRotate(g);
            }
        }
        //else node's parent is a right child
        else{
            uncle = g->leftChild;

            if(uncle->isRed == true){
                //1. change parent and uncle to black
                //2. change grandparent to red
                //3. make node = node's grandparent
                node->parent->isRed = false;
                uncle->isRed = false;
                g->isRed = true;
                node = g;
            }
            //else uncle is black
            else{
                if(node->parent->leftChild == node){
                    node = node->parent;
                    rightRotate(node);
                }
                node->parent->isRed = false;
                g->isRed = true;
                leftRotate(g);
            }
        }

        root->isRed = false;
    }
}

void MovieTree::leftRotate(MovieNode *x){
    if(x->rightChild == nil){
        return;
    } else {
        MovieNode *xright = x->rightChild;
        x->rightChild = xright->leftChild;

        if(xright->leftChild != nil){
            xright->leftChild->parent = x;
        }

        xright->parent = x->parent;

        if(x->parent == nil){
            root = xright;
        } else {
            if(x == x->parent->leftChild){
                x->parent->leftChild = xright;
            } else {
                x->parent->rightChild = xright;
            }
        }

        xright->leftChild = x;
        x->parent = xright;
    }
}

void MovieTree::rbDelete(MovieNode *z){
    MovieNode *y;

    if(!rbValid(root)){
        cout << "Invalid tree" << endl;
    }

    if(z->leftChild == nil || z->rightChild == nil){
        y = z;
    } else {
        y = getSuccessor(z);
    }

    MovieNode *x;
    if(y->leftChild != nil){
        x = y->leftChild;
    } else {
        x = y->rightChild;
    }

    if(x != nil){
        x->parent = y->parent;
    }

    MovieNode *xParent = y->parent;
    bool yIsLeft = false;

    if(y->parent == nil){
        root = x;
    } else if (y == y->parent->leftChild){
        y->parent->leftChild = x;
        yIsLeft = true;
    } else {
        y->parent->rightChild = x;
        yIsLeft = false;
    }

    if(y !=  z){
        z->title = y->title;
        z->quantity = y->quantity;
        z->ranking = y->ranking;
        z->year = y->year;
    }

    if(y->isRed == false){
        rbDeleteFixup(x, xParent, yIsLeft);
    }
}

void MovieTree::rightRotate(MovieNode *x){
    if(x->leftChild == nil){
        return;
    } else {

        MovieNode *xleft = x->leftChild;
        x->leftChild = xleft->rightChild;

        if(xleft->rightChild != nil){
            xleft->rightChild->parent = x;
        }

        xleft->parent = x->parent;

        if(x->parent == nil){
            root = xleft;
        } else {
            if(x == x->parent->leftChild){
                x->parent->leftChild = xleft;
            } else {
                x->parent->rightChild = xleft;
            }
        }

        xleft->rightChild = x;
        x->parent = xleft;
    }
}

void MovieTree::rbDeleteFixup(MovieNode *node, MovieNode *nodeParent, bool nodeIsLeft){

    while(node != root && node->isRed == false){

        MovieNode *s;
        if(nodeIsLeft){
            s = nodeParent->rightChild;

            if(s->isRed == true){
                s->isRed = false;
                nodeParent->isRed = true;
                leftRotate(nodeParent);
                s = nodeParent->rightChild;
            }

            if(s->leftChild->isRed == false && s->rightChild->isRed == false){
                s->isRed = true;
                node = nodeParent;
                nodeParent = node->parent;
                nodeIsLeft = (node == nodeParent->leftChild);
            } else {

                if(s->rightChild->isRed == false){
                    s->leftChild->isRed = false;
                    s->isRed = true;
                    rightRotate(s);
                    s = nodeParent->rightChild;
                }

                s->isRed = nodeParent->isRed;
                nodeParent->isRed = false;

                if(s->rightChild != nil){
                    s->rightChild->isRed = false;
                }
                leftRotate(nodeParent);
                node = root;
                nodeParent = nil;
            }
        }

        //else if nodeIsLeft == false
        else{
            s = nodeParent->leftChild;

            if(s->isRed == true){
                s->isRed = false;
                nodeParent->isRed = true;
                rightRotate(nodeParent);
                s = nodeParent->leftChild;
            }

            if(s->rightChild->isRed == false && s->leftChild->isRed == false){
                s->isRed = true;
                node = nodeParent;
                nodeParent = node->parent;
                nodeIsLeft = (node == nodeParent->leftChild);
            } else {

                if(s->leftChild->isRed == false){
                    s->rightChild->isRed = false;
                    s->isRed = true;
                    leftRotate(s);
                    s = nodeParent->leftChild;
                }

                s->isRed = nodeParent->isRed;
                nodeParent->isRed = false;

                if(s->leftChild != nil){
                    s->leftChild->isRed = false;
                }
                rightRotate(nodeParent);
                node = root;
                nodeParent = nil;
            }
        }
    }
    node->isRed = false;
}

//decided not to use
void MovieTree::rbTransplant(MovieNode *u, MovieNode *v){

}

int MovieTree::rbValid(MovieNode *node){
    int lh = 0;
    int rh = 0;

    // If we are at a nil node just return 1
    if (node == nil)
        return 1;

    else
    {
        // First check for consecutive red links.
        if (node->isRed)
        {
            if(node->leftChild->isRed || node->rightChild->isRed)
            {
                std::cout << "This tree contains a red violation" << std::endl;
                return 0;
            }
        }

        // Check for valid binary search tree.
        if ((node->leftChild != nil && node->leftChild->title.compare(node->title) > 0) || (node->rightChild != nil && node->rightChild->title.compare(node->title) < 0))
        {
            std::cout << "This tree contains a binary tree violation" << std::endl;
            return 0;
        }

        // Deteremine the height of let and right children.
        lh = rbValid(node->leftChild);
        rh = rbValid(node->rightChild);

        // black height mismatch
        if (lh != 0 && rh != 0 && lh != rh)
        {
            std::cout << "This tree contains a black height violation" << std::endl;
            return 0;
        }

        // If neither height is zero, incrament if it if black.
        if (lh != 0 && rh != 0)
        {
                if (node->isRed)
                    return lh;
                else
                    return lh+1;
        }

        else
            return 0;

    }
}

int MovieTree::countMovieNodes(MovieNode *node, int *count){

    if(node == nil && node == root){
        return 0;
    } else if(node == nil){
        return 0;
    } else {
        countMovieNodes(node->leftChild, count);
        countMovieNodes(node->rightChild, count);
        *count+=1;
    }

    return *count;
}

int MovieTree::countLongestPath(MovieNode *node, int *path){
    if(node == nil){
        return 0;
    } else {

        //find the depth of each subtree
        int LDepth = countLongestPath(node->leftChild, path);
        int RDepth = countLongestPath(node->rightChild, path);

        if(LDepth > RDepth){
            *path = LDepth + 1;
        } else {
            *path = RDepth + 1;
        }
    }
    return *path;
}

MovieNode *MovieTree::searchMovieTree(MovieNode *root, string title){
    MovieNode *currentMovie = root;

    while(currentMovie != nil){
        if(currentMovie->title == title){
            return currentMovie;
        } else {

            if(currentMovie->title > title){
                currentMovie = currentMovie->leftChild;
            } else {
                currentMovie = currentMovie->rightChild;
            }
        }
    }

    return nil;
}

MovieNode *MovieTree::getSuccessor(MovieNode *x){
    if(x->rightChild != nil){
        return treeMinimum(x->rightChild);
    }

    MovieNode *y = x->parent;
    while(y != nil && x == y->rightChild){
        x = y;
        y = y->parent;
    }
    return y;
}

MovieNode *MovieTree::treeMinimum(MovieNode *x){
    while(x->leftChild != nil){
        x = x->leftChild;
    }
    return x;
}
