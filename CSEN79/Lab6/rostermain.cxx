/*
 * Austin Nguyen
 * anguyen10@scu.edu
 * CSEN 79 Lab 6 (3 of 3)
 * Description: Main file to read input file, and calls implementation to run correctly
 */

#include "roster.h"

using namespace std;
using namespace csen79;

// Test code for class roster
// Input file: <CMD> [ID] [FIRST LAST]
// CMD : A | X | L
// ID: 7-digit unsigned integer
// FIRST, LAST: string
int main() {
	Roster r;
	Student::ID_t id;
	string cmd, first, last;

	// Read cmd first, then branch accordingly
	while (cin >> cmd) {
		if (cmd == "A") {               //Insertion
			cin >> id >> first >> last;
			Student initStudent(id, first, last);
			r.insert(initStudent);
		} else if (cmd == "X") {        //Erasing
			cin >> id;
			r.erase(id);
		} else if (cmd == "L") {        //List
			int i = 1;
            for (auto st = r.begin(); st != r.end(); st = r.next(st), i++) {
                cout << i << ": " << st->data << endl;
            }
		} 
	}
	return EXIT_SUCCESS;
}

