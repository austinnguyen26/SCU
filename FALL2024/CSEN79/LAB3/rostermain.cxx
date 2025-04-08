/*
 * Austin Nguyen Caroline Tapia
 * anguyen10@scu.edu, ctapia@scu.edu
 * CSEN 79 Lab Sample Code
 * Description: Main file to read input file, and calls implementation to run correctly
 */
#include <cstddef>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>
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
		if (cmd == "A") {
			// your code to insert
			cin >> id >> first >> last;
			Student initStudent(id, first, last);
			r.insert(initStudent);
		} else if (cmd == "X") {
			// your code to erase
			cin >> id;
			r.erase(id);
		} else if (cmd == "L") {
			// this should just work, if you did your begin/end/next correctly
			int i = 1;
			for (auto st = r.begin(); st != r.end(); st = r.next(st), i++)
				cout << i << ": " << *st << endl;
		} 
	}
	r.delRoster();
	return EXIT_SUCCESS;
}

