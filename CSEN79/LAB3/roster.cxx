/*
 * Austin Nguyen Caroline Tapia
 * anguyen10@scu.edu, ctapia@scu.edu
 * CSEN 79 Lab Sample Code
 * Description: Our implementation of inputting students, erasing students, resizing the 
 * roster, as well as incrementing the program.
 */
#include <string>
#include <iostream>
#include <stdexcept>
#include "roster.h"

namespace csen79 {
	static int idx = 0;
	
	Student::Student(ID_t temp, std::string fn, std::string ln){
		this->id = temp;
		this->firstName = fn;
		this->lastName = ln;
	}


	std::ostream& operator<<(std::ostream& os, const Student &t) {
		os << "ID:" << t.id << " " << "First Name: " << t.firstName << " " << "Last Name: " << t.lastName;
		return os;
	}

	Roster::Roster(): students(nullptr), capacity(STARTING_CAPACITY), size(0){
		students = new T[capacity];
	}

	void Roster::delRoster(){
		delete[] students;
	}

	// state your pre-/post-conditions
	// Precondition: Must have a student to add from the input.txt file
	// Postcondition: If the size of the current roster is larger than capacity,
	// calls resize function to extend the dynamic data structure size 
	bool Roster::insert(T &rec) {		
		if (size >= capacity){
			resize();
		}
		for (int i = 0; i < size; ++i){
			if(students[i].getID() == rec.getID()){
				std::cout << "Student, " <<  rec.getFirstName() << " " <<  rec.getLastName() << " with ID: " << rec.getID() << " has a duplicate." << std::endl;
				return false;
			}
		}
		students[size++] = rec;
		return true;


		
	}

	// PreCondition: Roster cannot be empty
	// PostCondition: Program breaks when roster becomes empty
	void Roster::erase(Student::ID_t id) {
		for (int i = 0; i < size; ++i){
			if (students[i].getID() == id){
				for (int j = i; j < size - 1; ++j){
					students[j] = students[j+1];
				}
				std::cout << "Student, " << students[i].getFirstName() << " " << students[i].getLastName() << "with ID, " << students[i].getID() << " has been removed from the roster." << std::endl;
				--size;
				break;
			}
		}
	}

	// rudimentary iterator
	Roster::T* Roster::begin(void) {
		idx = 0;
		return students;
	}

	Roster::T* Roster::end(void) {

		return students+size;
	}

	// The next element for interation
	Roster::T* Roster::next(T* current) {
		// return nullptr;
		if (idx < size-1){
			++idx;
			return &students[idx];
		}else{
			resize();
			idx++;
			return &students[idx];
		}
	}
	void Roster::resize(){
		capacity *=2;
		T* newRoster = new T[capacity];
		
		for ( int i = 0; i < size; ++i){
			newRoster[i] = students[i];
		}
		delete[] students;
		students = newRoster;
	}
}
