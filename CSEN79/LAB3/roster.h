/*
 * Austin Nguyen Caroline Tapia
 * anguyen10@scu.edu, ctapia@scu.edu
 * CSEN 79 Lab Sample Code
 * Description: Header File for the lab. Mostly the same from Lab3, and given by TA
 */
#ifndef ROSTER_H
#define ROSTER_H

// Class declaration for roster
namespace csen79 {
	// base data for the roster
	class Student {
	public:
		using ID_t = unsigned int;	// really should be a 7-digit unsigned int
		Student(ID_t n, std::string fn, std::string ln);
		friend std::ostream& operator<<(std::ostream& os, const Student &);

		ID_t getID() const {return id;};
		std::string getFirstName() const { return firstName; }
		std::string getLastName() const { return lastName; }

		Student() : id(0), firstName(""), lastName("") {}
	
	private:
		ID_t id;
		std::string firstName;
		std::string lastName;
	};

	class Roster {
	public:
		using T = Student;
		Roster(); 		//Default Constructor

		void delRoster();
		bool insert(T& rec);
		void erase(Student::ID_t id);

		T* begin(void);
		T* end(void);
		T* next(T* current);
		int getSize() const;
	private:
		T* students;
		int size;
		int capacity;
		void resize();
		static const int STARTING_CAPACITY = 10;
	};
}
#endif // ROSTER_H
