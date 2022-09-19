#include <iostream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include"Complexity_Timer.hpp"
//These values were provided with Brandon Kulmala's code
int tableSize = rand()%100;   //Random tablesize, max 100       NOTICE: In Brandon's code, this global variable was constant. In order for me to experiment the way I wanted to, this needed to be modified (to work with his code)
const int prime = 7;                //Prime number to traverse hash

//Andy Mee
//atm112@uakron.edu

/*The purpose of this code is to study the efficiency of a two choice hash table that uses double hashing, as well as compare said hash to that that does not use two choice. */

class TwoChoiceHash{
	//data members
	int* table; //underlying array used for hashing table. NOTICE: for experimentation purposes, the type of data stored is arbitrary. only numeric keys will be stored in our hash table.
	int limit = 0; //number of slots within the hashing table, will be used for hashing functions
	int size = 0; //current number of items within the hashing table

	//private member functions

	//hashing functions
	int hash1(int key){return key % limit;} //the first (and default) hashing function
	int hash2(int key){return (key / limit) % limit;} //second hashing function. Called upon whenever a collision has been come across with the first hashing function / miss upon first hash

	//double hash value
	int doubleValue(int key){
		return 1 + (key % (limit - 1)); //ensures the slots that are probed next are >=1 and < table size. This ensures the hash is traversed instead of just moving back to where
									//it started
	}


public:

	//ctors and dtor

	TwoChoiceHash(){ //default ctor
		limit = 11; //prime number to avoid as many collisions as possible
		table = new int[limit];

		for(int i = 0; i < limit; ++i){ //fill every value in the array with -1
			table[i] = -1; //-1 will be used as sentinals to indicate a position is open within the underlying array. None of the key values will be negative
		}
	}

	TwoChoiceHash(int n){ //overloaded ctor with user choice of hash table size
		limit = n;
		table = new int[limit];

		for(int i = 0; i < limit; ++i){
			table[i] = -1;
		}
	}

	~TwoChoiceHash(){ //dtor
		delete [] table;
	}

	//member functions

	int getSize(){
	return size;
	}
	int getLimit(){
	return limit;
	}

	//simply displays the hash table
	void displayHash(){
		for (int i = 0; i < limit; ++i){
			std::cout << table[i];
			if(i != limit - 1){
				std::cout << " / ";
			}
		}
	}

	void insert(int key){
	    if(size == (limit)){ // do not allow any more insertions after the limit has been reached
	        return;
	    }
		int slot1 = hash1(key);
		int slot2 = hash2(key); //two different choices for key to be inserted.

		if (table[slot1] == key){ //key already exists within the hash table
			return;
		}

		if (table[slot1] == -1){ //If slot 1 is available
			table[slot1] = key;
			++size;
			return;
		}

		if(table[slot2] == key){ //key already exists within the hash table
			return;
		}

		if(table[slot2] == -1){ //if slot 1 wasn't available but slot 2 is
			table[slot2] = key;
			++size;
			return;
		}

		//after this point, two collisions are detected: Collision handling
		int probe1 = doubleValue(slot1), probe2 = doubleValue(slot2) + 1, orig1 = slot1, orig2 = slot2; //double hash values are set for the two initial choices

		bool fullLoop1 = false, fullLoop2 = false;

		while (true){ //should loop until a position is found. Will return when it is. If not, returns when both probes reach their innitial position at least once
			slot1 += probe1;
			if (slot1 >= limit){ //if the new position surpasses the limit of the hash table
				slot1 -= limit; //wind back around
			}

			slot2 += probe2;
			if(slot2 >= limit){
				slot2 -= limit;
			}


			if (table[slot1] == key){
				return;
			}

			if (table[slot1] == -1){
				table[slot1] = key;
				++size;
				return;
			}


			if (table[slot2] == key){
				return;
			}

			if (table[slot2] == -1){
				table[slot2] = key;
				++size;
				return;
			}

			//handling case of infinite loop

			if(slot1 == orig1){
			    fullLoop1 = true;
			}

			if(slot2 == orig2){
			    fullLoop2 = true;
			}

			//Spot could not be found: rehashing for a larger table and trying again
			if(fullLoop1 == true && fullLoop2 == true){
			    std::cout << "Could not find slot to insert, rehashing\n";
			    rehash(limit * 2);
			    insert(key);
			    return;
			}
		}

	}

	int search(int key){ // determines if a key is within the hash table, returns the position of the key if it is,
	                     //returns -1 if it isnt

		//check their initial hash positions

	    int slot1 = hash1(key);
		int slot2 = hash2(key);

		if(table[slot1] == key){
			return slot1;
		}

		if(table[slot2] == key){
			return slot2;
		}

		//Collision occured when inserting this key; find key in other possible positions

		int probe1 = doubleValue(slot1), probe2 = doubleValue(slot2) + 1, orig1 = slot1, orig2 = slot2;

		bool fullLoop1 = false, fullLoop2 = false;

		while(true){ //loops until either key is found or both probes reach their initial position

			slot1 += probe1;
			if (slot1 >= limit){ //if the new position surpasses the limit of the hash table
				slot1 -= limit; //wind back around
			}

			slot2 += probe2;
			if(slot2 >= limit){
				slot2 -= limit;
			}

			if (table[slot1] == key){
				return slot1;
			}

			if(table[slot2] == key){
				return slot2;
			}

			//Handling infinite loop

			if(slot1 == orig1){
			    fullLoop1 = true;
			}

			if(slot2 == orig2){
			    fullLoop2 = true;
			}

			if(fullLoop1 == true && fullLoop2 == true){
			    //std::cout << "Could not find value\n";
			    return -1;
			}
		}
	}

	void deleteKey(int key){
	    if(size == 0){
            return;
	    }
		int i = search(key);
		if(i != -1){
			table[i] = -1;
			--size;
		}
	}

	void rehash(int n){
	    //do nothing if the given table size is smaller
	    if(n <= limit){
            return;
	    }

	    //create temporary array to hold old table
        int *temp = new int[limit];
        for(int i = 0; i < limit; ++i){
        temp[i] = table[i];
        }
        delete[] table;
        table = new int[n];

        //refill table with -1's
        for (int i = 0; i < n; ++i){
            table[i] = -1;
        }
        int oldLimit = limit;
        limit = n;
        for(int i = 0; i < oldLimit; ++i){
            if(temp[i] != -1){
                insert(temp[i]);
            }
        }
        delete[] temp;
	}
};


/* The following code below belongs to Brandon Kulmala. He was kind enough to let me test against his code for the purpose of this research\
    bjk74@uakron.edu*/

class DoubleHash
{
    // Pointer to an array containing buckets
    int *hashTable;
    int curr_size;

public:

    // function to check if hash table is full
    bool isFull()
    {

        // if hash size reaches maximum size
        return (curr_size == tableSize);
    }

    // function to calculate first hash
    int hash1(int key)
    {
        return (key % tableSize);
    }

    // function to calculate second hash
    int hash2(int key)
    {
        return (prime - (key % prime));
    }

    DoubleHash()
    {
        hashTable = new int[tableSize];
        curr_size = 0;
        for (int i=0; i<tableSize; i++)
            hashTable[i] = -1;
    }

    // function to insert key into hash table
    void insertHash(int key)
    {
        // if hash table is full
        if (isFull())
            return;

        // get index from first hash
        int index = hash1(key);

        // if collision occurs
        if (hashTable[index] != -1)
        {
            // get index2 from second hash
            int index2 = hash2(key);
            int i = 1;
            while (1)
            {
                // get newIndex
                int newIndex = (index + i * index2) % tableSize;
                // if no collision occurs, store
                // the key
                if (hashTable[newIndex] == -1)
                {
                    hashTable[newIndex] = key;
                    break;
                }
                i++;
            }
        }
        // if no collision occurs
        else
            hashTable[index] = key;
        curr_size++;
    }
    void eraseHash(int key){
        /* Return if element is not present */
        if(!searchHash(key))
            return;

        int probe = hash1(key), offset = hash2(key);

        while(hashTable[probe] != -1)
            if(hashTable[probe] == key){
                hashTable[probe] = -2;          // mark element as deleted (rather than unvisited(-1)).
                return;
            }
            else
                probe = (probe + offset) % tableSize;

    }
        bool searchHash(int key){
        int probe = hash1(key), offset = hash2(key), initialPos = probe;
        bool firstItr = true;

        while(1){
            if(hashTable[probe] == -1)                   // Stop search if -1 is encountered.
                break;
            else if(hashTable[probe] == key)           // Stop search after finding the element.
                return true;
            else if(probe == initialPos && !firstItr)    // Stop search if one complete traversal of hash table is completed.
                return false;
            else
                probe = ((probe + offset) % tableSize);  // if none of the above cases occur then update the index and check at it.

            firstItr = false;
        }
        return false;
    }
    // function to display the hash table
    void displayHash()
    {
        for (int i = 0; i < tableSize; i++)
        {
            if (hashTable[i] != -1)
                std::cout << i << " --> "
                     << hashTable[i] << std::endl;
            else
                std::cout << i << std::endl;
        }
    }
};

//supporting functions for Brandon's code
    void doubleHashfunc() {
        DoubleHash hashDouble;
        int a[tableSize];
        std::cout << "Creation of array.\n";
        for(int i = 0;i < tableSize;i++) {
            a[i] = rand()%500;
            std::cout << a[i] << "->";
        }
        std::cout << std::endl;
    int n = sizeof(a)/sizeof(a[0]);

    // inserting keys into hash table
    for (int i = 0; i < n; i++) {
        hashDouble.insertHash(a[i]);
    }
    // display the hash Table
    hashDouble.displayHash();
    }

//Menu functions

void displayMenu(){
    std::cout << "Welcome to Andy Mee's data structures project 3: Hash\n"
              << "Please make a selection now: \n"
              << "1) Test two choice hash against single choice\n"
              << "2) Test the space taken up by the hash\n"
              << "Else) Exit\n";
}

void testHashes(){

    std::cout << "\nPlease enter the (initial) size of both hash tables: ";
    int n = 0;
    std::cin >> n;

    tableSize = n;

    TwoChoiceHash myHash(n);
    DoubleHash dHash;
    int *values = new int[n];

    //fill out the set of data
    for(int i = 0; i < n; ++i){
        values[i] = rand() % 1000000000;
    }

    timer timer1, timer2, timer3; //1:myHash insert/delete, 2:dHash insert/delete, 3:total times

    //measure myHash
    timer1.restart();
    timer3.restart();
    for(int i = 0; i < n; ++i){
        myHash.insert(values[i]);
    }

    timer1.stop();
    timer1.record();

    int buffer = 0;
    timer1.restart();
    for(int i = 0; i < n; ++i){
       buffer =  myHash.search(values[i]);
    }
    timer1.stop();
    timer1.record();

    timer1.restart();
    for(int i = 0; i < n; ++i){
        myHash.deleteKey(values[i]);
    }
    timer1.stop();
    timer3.stop();
    timer1.record();
    timer3.record();
    //myHash.displayHash();
    std::cout << "Passed myHash " << '\n';


    //measure dHash
    timer2.restart();
    timer3.restart();
    for(int i = 0; i < n; ++i){
        dHash.insertHash(values[i]);
    }
    timer2.stop();
    timer2.record();

    //dHash.displayHash();

    bool buff = false;
    timer2.restart();
    for(int i = 0; i < n; ++i){
       buff = dHash.searchHash(values[i]);
    }
    timer2.stop();
    timer2.record();

    timer2.restart();
    for(int i = 0; i < n; ++i){
        dHash.eraseHash(values[i]);
    }
    timer2.stop();
    timer3.stop();
    timer2.record();
    timer3.record();

        std::cout << "Passed dHash " << '\n';

    std::cout << "\nmyHash insert time: " << timer1.times[0]
              << "\nmyHash search time: " << timer1.times[1]
              <<"\nmyHash delete time: " << timer1.times[2]
              << "\nmyHash total time: " << timer3.times[0]
              << "\ndHash insert time: " << timer2.times[0]
              << "\ndHash search time: " << timer2.times[1]
              << "\ndHash delete time: " << timer2.times[2]
              << "\ndHash total time: " << timer3.times[1]
              << '\n';

    delete[] values;
}

void testSpace(){
std::cout << "\nEnter the size of the hash table you want, I'll tell you the space it takes up: ";
int n = 0;
std::cin >> n;

TwoChoiceHash myHash(n);

std::cout << "\nWould you like your hash table half full or attempted to be full? (h/f): ";
char choice;
std::cin >> choice;
switch(choice){
case 'h':
    for(int i = 0; i < n / 2; ++i){
            myHash.insert(i);
    }
    break;
default:
    for(int i = 0; i < n; ++i){
        myHash.insert(i);
    }
}
std::cout << "\nThis hash table has as follows:\n"
          << "\nBytes for pointer: " << sizeof(int*)
          << "\nBytes for size and limit values: " << 2 * sizeof(int)
          << "\nBytes for entire table: " << (n * sizeof(int)) + sizeof(int*) + (2 * sizeof(int))
          << "\nAmount of overhead (in bytes): " << sizeof(int) * (myHash.getLimit() - myHash.getSize()) << '\n';
}
void menu(){
    int choice = 0;
    while(true){
        displayMenu();
        std::cin >> choice;
        if(choice == 1){
            testHashes();
        }
        else
        if (choice == 2){
            testSpace();
        }
        else{
            break;
        }
    }
    std::cout << "Ending program... have a good summer!";

}

int main()
{
    menu();
    return 0;
}
