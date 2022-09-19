#include <iostream>
#include<time.h>
#include"Complexity_Timer.hpp"

//Andy Mee
//atm112@uakron.edu
//4571036

/*The purpose of this program is to study the binary heap. The binary heap currently implemented has the capacity of being either a min or max
heap, depending on how the user wishes to define it. This program will measure execution time of certain behaviors of this heap and compare it to
other, similar data structures.*/

//This will be the main data structure that will be the focus of this project's research
template <typename T>
class BinaryHeap{
T *data; //underlying array for heap
int last;//last postition in the heap
int limit;//the limit in space allocated for the current heap
bool max; //bool variable that determines whether heap is implemented as a max or min


//Private member functions: most of these are variants called by their public counterpart dependent on what the heap is currently
//implemented as. None of these should be allowed to be accessed directly
	void swapNodes(int node1, int node2){ //swaps the elements indexed at node1 and node2 within the underlying array
		if (node1 > last|| node1 < 0 || node2 > last || node2 < 0){ //Should not attempt to swap an element out of range in the array
			std::cout << "Something went wrong: Attempting to swap with an element out of range\n";
			return;
		}
		T temp = data[node1];
		data[node1] = data[node2];
		data[node2] = temp;
		return;
	}

    void siftDownMax(int node){ //start from the top of a tree, swap elements of the parent and highest valued child (currently implemented as a max heap)
		if (node >=last){ //invalid range, return

			return;
		}
		for (T temp = node; !(isALeaf(node)); node = temp){ //loop until you reach the bottom level of the heap
			int left = getLeftChild(node), right = getRightChild(node); //get the left and right children of the current root, the higher of the two will be stored in temp
			if ((data[right] > data[left]) && (!((right >= last)) )){ // make sure the returned right child position is within range.
				temp = right; //sets temp to the position of right if right is higher, pitfalls if not
			}
			else{
			    temp = left;
			}
			if (data[node] > data[temp]){ //If the parent node is higher priority than the largest child
				return; //all done, no further sifting required
			}
			swapNodes(node, temp); //if not, swap the contents
		}
	}

	    void siftDownMin(int node){ //start from the top of a tree, swap elements of the parent and highest valued child (currently implemented as a max heap)
		if (node >=last){ //invalid range, do nothing
			return;
		}
		for (T temp = node; !(isALeaf(node)); node = temp){ //loop until you reach the bottom level of the heap
			int left = getLeftChild(node), right = getRightChild(node); //get the left and right children of the current root
			if ((data[right] < data[left]) && (!(right >= last))){ // make sure the returned right child position is within range.
				temp = right; //sets temp to the position of right if right is higher, pitfalls if not
			}
			else{
			    temp = left;
			}
			if (data[node] < data[temp]){ //If the parent node is higher priority than the largest child
				return; //all done, no further sifting required
			}
			swapNodes(node, temp); //if not, swap the contents
		}
	}

	void siftUpMax(int node){ //starts from the last node of the tree and swaps elements with the current node's parent if it holds a higher value than the parent
		for( ; (node != 0) && (data[node] > data[getParent(node)]); node = getParent(node)){ //while not at the top of the tree and while the current node is node is higher priority than its parent
			swapNodes(node, getParent(node));
		}
	}

	void siftUpMin(int node){ //starts from the last node of the tree and swaps elements with the current node's parent if it holds a higher value than the parent
		for ( ; (node != 0) && (data[node] < data[getParent(node)]); node = getParent(node)){ //while not at the top of the tree and while the current node is node is higher priority than its parent
			swapNodes(node, getParent(node));
		}
	}

	void insertNodeMax(T value){
	if (last >= limit){
	    reallocate(limit * 2);
	    std::cout << "Reallocating space... new limit is: " << limit << '\n';
	}
        int current = last; //Set the "current" index to the last index
        ++last; //increment the last position in the array
        data[current] = value;
        siftUpMax(current); //sift the last element up to its rightful place
        return;
	}

	void insertNodeMin(T value){
	if (last >= limit){
	    reallocate(limit * 2);
	    std::cout << "Reallocating space... new limit is: " << limit << '\n';
	}
        int current = last;//Set the "current" index to the last index
        ++last; //increment the last position in the array
        data[current] = value;
        siftUpMin(current); //sift the last element up to its rightful place
		return;
	}

	T removeMax(){
		if (last <= 0){ //If heap is empty (or somehow below zero)
			std::cout << "Heap is empty, nothing could be removed. Returning null";
			return NULL;
		}
		T root = data[0];
		--last;
		swapNodes(0, last);//Swaps the root and last node in the heap, then decrements the "last" index
		siftDownMax(0); //Sift down from the top
		return root; //returns the (now former) biggest element of the heap
	}

	T removeMin(){
		if (last <= 0){ //If heap is empty (or somehow below zero)
			std::cout << "Heap is empty, nothing could be removed. Returning null";
			return NULL;
		}
		T root = data[0];
		--last; //decrement the heap size
		swapNodes(0, last);//Swaps the root and last node in the heap
		siftDownMin(0); //Sift down from the top
		return root; //returnst the (now former) biggest element of the heap
	}

	void buildMax(){
		for (int i = (getParent(last)); i >= 0; --i){
			siftDownMax(i);
		}
	}

	void buildMin(){
		for (int i = (getParent(last)); i >= 0; --i){
			siftDownMin(i);
		}
	}
public:
	//ctors
	BinaryHeap(){last = 0; limit = 10; data = new T[limit]; max = true;} //default ctr, gives 10 nodes as default
	BinaryHeap(int size){last = 0; limit = size; data = new T[limit]; max = true;} //overloaded ctr that allocates space for "size" amount of nodes
	BinaryHeap(T* array, int size, int maximum, char *htype){ //overloaded ctr with given initializer list,
		data = new T[maximum];
		if (htype == "max"){
		    max = true;
		}
		else
		if (htype == "min"){
		    max = false;
		}
		else{
		    max = true;
		}
		for (int i = 0; i < size; ++i){
            data[i] = array[i];
		}
		//data = array;
		last = size;
		limit = maximum;
		build();
	}
	//dtor
	~BinaryHeap(){delete[] data;}
	//getters
	int getLast(){return last;}
	int getLimit(){return limit;} //there might not be much use for these outside of class implementation, but it's good to have them just in case

	//node access
	int getParent(int node){
		if (node <= 0){ // if 0, there is no parent (root node), return 0, avoids returning a negative value
			return 0;
		}
		return (node - 1) / 2;
	}

	int getLeftChild(int node){
		return ((node * 2) + 1);
	}

	int getRightChild(int node){
		return((node * 2) + 2);
	}

	//member functions


	bool isALeaf(int node){
		return (node > last / 2 - 1)  && (!(node >= last));
	}

    void insertNode(T item){
        if(max == true){
            insertNodeMax(item);
        }
        else{
            insertNodeMin(item);
        }
    }

    T remove(){
        if(max == true){
            return removeMax();
        }
        else{
            return removeMin();
        }
    }

    void build(){
        if(max == true){
            buildMax();
        }
        else
        if (max == false){
            buildMin();
        }
    }

    void reverse(){ //reverses the implementation of the heap (ie, max to min or min to max)
        if(max == true){
            max = false;
            buildMin();
            return;
        }
        if(max == false){
            max = true;
            buildMax();
            return;
        }
    }
	void reallocate(int newSize){
	T *temp = new T[newSize];
	for(int i = 0; i < limit; ++i){
        temp[i] = data[i];
	}
	delete data;
	data = temp;
	limit = newSize;
	}
};

//The first data structure that our binary heap will be tested against
template <class T>

	struct ListNode{

	T value; //Stored value

	ListNode *next; //Link to next node

	};

template <class T>

class LinkedHeap{

private:

ListNode <T> *head; //head node every list will have

public:

LinkedHeap<T>(){ //default ctr
	head = nullptr;
}

~LinkedHeap<T>(){ //destructor

ListNode<T> *nodePtr = head;

ListNode<T> *nextNode;

while(nodePtr != nullptr){

nextNode = nodePtr->next;

delete nodePtr;

nodePtr = nextNode;
}

}

public:

void insertNode(T); //Inserts node within the heap in descending order, assuring the highest priority value is at the head at all times

T removeMax(); //Removes the highest priority element of the heap and returns its value
};

/*NOTICE: The data structure below belongs to Brandon Kulmala, all rights reserved.
    bjk74@uakron.edu
    */
class MinHeap {
public:
    int *ArrA;
    int capacity;
    int heapSize;

    MinHeap(int cap) {
    heapSize = 0;
    capacity = cap;
    ArrA = new int[cap];
    }

    int parent(int i) {
    return (i-1)/2;
    }

    int left(int i) {
    return (2*i+1);
    }

    int right(int i) {
    return (2*i+2);
    }

    void swap(int &x, int &y) {
    int temp = x;
    x = y;
    y = temp;
    }
    bool linearSearch(int val) {
    for (int i = 0; i < heapSize; i++) {
        if(ArrA[i] == val) {
            return 1;
        }
    }
    return 0;
    }

    void printArray() {
    for(int i = 0; i < heapSize; i++) {
        std::cout << ArrA[i] << " ";
    }
    }

    void minHeapify(int i) {
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heapSize && ArrA[l] < ArrA[i]) {
        smallest = l;
    }
    if (r < heapSize && ArrA[l] < ArrA[i]) {
        smallest = r;
    }
    if(smallest != i) {
        swap(ArrA[i], ArrA[smallest]);
        minHeapify(smallest);
    }
    }

    void insertNode(int k) {
    if(heapSize == capacity) {
        return;
    }
    else {
        ++heapSize;
        int i = heapSize - 1;
        ArrA[i] = k;

        while(i != 0 && ArrA[parent(i)] > ArrA[i]) {
            swap(ArrA[i], ArrA[parent(i)]);
            i = parent(i);
        }
    }
    }

    int removeRoot() {
    if (heapSize <= 0) {
        return 0;
    }
    if (heapSize == 1) {
        --heapSize;
        return ArrA[0];
    }
    else {
    int root = ArrA[0];
    ArrA[0] = ArrA[heapSize-1];
    --heapSize;
    minHeapify(0);
    return root;
    }
    }

};
void mainMenu();

void menuDisplay();

void testBinaryVsLinked();

void testTwoBinaryHeaps();

void testNew();

int main()
{
    mainMenu();
    std:: cout << "Ending program... see ya later!";
    return 0;
}


//inserts a node onto the list that falls in order
//pre: the data type being used supports the < comparison, a list is declared
//post: a node is inserted within the list, adjusting node pointers accordingly
template <class T>
void LinkedHeap<T>::insertNode(T item){

    ListNode<T> *newNode = new ListNode<T>; //create a new node

    newNode->value = item; //set its value

    newNode->next = nullptr; //assure it's pointing to no data

    ListNode<T> *nodePtr; // pointer to traverse the list

    ListNode<T> *previous; // pointer to keep track of the previous node

    if (!head){ //Check to see if the list is empty. If it is, set the new node as the head
        head = newNode;
    }

    else{

        nodePtr = head; //start at the head

        previous = nullptr; //there's no previous yet

        do{ //while the nodePtr isnt at the end and until we find the correct spot for the new node

            previous = nodePtr; //shift the previous up

            nodePtr = nodePtr->next; //shift the node pointer over

        } while (nodePtr != nullptr && nodePtr->value > item);

        if(previous == nullptr){ //If the new node is being inserted at the beginning of the list

            head = newNode; //new node is now the head

            newNode->next = nodePtr; //it's next pointer is the next on the list

        }

        else{ //otherwise...

        previous->next = newNode; //update the previous node's link to the new node

        newNode->next = nodePtr; //the new node's next link is where node pointer resides
        }
    }

}


//removes the highest priority value and returns its value
template <class T>
T LinkedHeap<T>::removeMax(){

    if(!head){
        std::cout << "List is empty, nothing to return\n";
        return -1;
    }

    T highest = head->value;

    if(head->next == nullptr){ //if the head is the only element in the linked list
        delete head;
        head = nullptr;
        return highest;
    }

    ListNode<T> *nodePtr = head->next; //pointer for traversal

    delete head;

    head =  nodePtr;

    return highest;

}
void mainMenu(){
bool menuLoop = true;

while (menuLoop){
    int select = 0;
    menuDisplay();
    std::cout << "\nMake your selection now: ";
    std::cin >> select;

    switch(select){
    case 1:
    menuLoop = true;
    testBinaryVsLinked();
    break;
    case 2:
    menuLoop = true;
    testTwoBinaryHeaps();
    break;
    case 3:
    menuLoop = true;
    testNew();
    break;
    default:
    menuLoop = false;


    }
}
}
void menuDisplay(){
std::cout << "Welcome to Andy Mee's project 2: The binary heap\n"
             "1) Test against Linked list heap\n"
             "2) Test against classmates' min heap\n"
             "3) Test new implementation of binary heap (re-heapify as min/max)\n"
             "4) Exit\n";
}

void testBinaryVsLinked(){
srand(time(NULL));
std::cout << "\nPlease enter the size of the problem you wish to test(n = ...): ";
int n = 0;

std::cin >> n;

BinaryHeap<int> bHeap(n);
LinkedHeap<int> lHeap;

timer timer1, timer2, timer3; //timer 1 used for binary heap inner times, 2 used for linked heap inner times, 3 used for outer times of both

timer1.restart();
timer3.restart();
for(int i = 0; i < n; ++i){ // load the binary heap
    bHeap.insertNode(rand() % n);
}
timer1.stop();
timer1.record();

int buffer; //buffer to receive values returned

timer1.restart();
for(int i = 0; i < n; ++i){
    buffer == bHeap.remove();
}
timer1.stop();
timer1.record();
timer3.stop();
timer3.record();

timer2.restart();
timer3.restart();
for(int i = 0; i < n; ++i){
    lHeap.insertNode(rand() % n);
}
timer2.stop();
timer2.record();

timer2.restart();
for(int i = 0; i < n; ++i){
    buffer = lHeap.removeMax();
}
timer2.stop();
timer2.record();
timer3.stop();
timer3.record();

std::cout << "\nFor problem size of " << n << ":\nBinary heap loading: " << timer1.times[0] << " seconds"
             "\nBinary heap unloading: " << timer1.times[1] << " seconds"
             "\nBinary heap total time: " << timer3.times[0] << " seconds"
             "\nLiked heap loading time: " << timer2.times[0] << " seconds"
             "\nLinked heap unloading time: " << timer2.times[1] << " seconds"
             "\nLinked heap total time: " << timer3.times[1] << " seconds\n";
}

void testTwoBinaryHeaps(){
srand(time(NULL));
int n = 0;
std::cout << "\nPlease enter the size of the problem you wish to test(n = ...): ";
std::cin >> n;

BinaryHeap<int> myHeap(n);
MinHeap otherHeap(n);

timer timer1, timer2, timer3; //timer 1 used for individual times for operations on BinaryHeap, 2 used for individual times for operations on MinHeap, 3 used for total times

timer1.restart();
timer3.restart();
for(int i = 0; i < n; ++i){
    myHeap.insertNode(rand() % n);
}
timer1.stop();
timer1.record();

timer1.restart();
int buffer = 0;
for(int i = 0; i < n; ++i){
    buffer = myHeap.remove();
}
timer1.stop();
timer3.stop();
timer1.record();
timer3.record();

timer2.restart();
timer3.restart();
for (int i = 0; i < n; ++i){
    otherHeap.insertNode(rand() % n);
}
timer2.stop();
timer2.record();

timer2.restart();
for (int i = 0; i < n; ++i){
    buffer = otherHeap.removeRoot();
}
timer2.stop();
timer3.stop();
timer2.record();
timer3.record();

std::cout << "BinaryHeap insert time: " << timer1.times[0] << " seconds\n"
             "BinaryHeap remove time: " << timer1.times[1] << " seconds\n"
             "BinaryHeap total time: " << timer3.times[0] << " seconds\n"
             "MinHeap insert time: " << timer2.times[0] << " seconds\n"
             "MinHeap remove time: " << timer2.times[1] << " seconds\n"
             "MinHeap total time: " << timer3.times[1] << " seconds\n";

}

void testNew(){
int * arrPtr = new int[10];
for (int i = 0; i < 10; ++i){
    arrPtr[i] = i + 1;
}
BinaryHeap<int> myHeap(arrPtr, 10, 10, "min");

for(int i =0; i < 10; ++i){
std::cout << myHeap.remove() << '\n';
}

BinaryHeap<int> myHeap2(arrPtr, 10, 10, "min");
myHeap2.reverse();
std::cout << "This should still be in the same reverse order when output: \n";

for (int i = 0; i < 10; ++i){
    std::cout << myHeap2.remove() << '\n';
}
}
