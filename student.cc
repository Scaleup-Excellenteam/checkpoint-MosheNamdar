//-------include section----------
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <new>

//--------using section------------
using std::cout;
using std::endl;
using std::cin;
using std::setw;
using std::nothrow;
using std::cerr;
using std::ifstream;

#define LEN 30
#define LENGTH 10
#define MAX_LEN 20
#define NUM_OF_LEVELS 12
#define NUM_OF_CLASSES 10
#define NUM_OF_COURSES 10


//-------struct section-------------
struct Student {
	char first_name[MAX_LEN];
	char last_name[MAX_LEN];
	int cell_namber;
	char* courses[NUM_OF_COURSES];
	//int grades[NUM_OF_COURSES]
	struct Student * _next;
};

struct School {
	struct Student* db[NUM_OF_LEVELS][NUM_OF_CLASSES];
};


struct School s;

//------prototypes------------------
void init_db();
void insert_student(int stage, int _class, const char* first_name, const char* last_name, int cell_number, int* grades);
void print_db();

//---------main section----------------
int main()
{
	init_db();
	
	print_db();
	
	return 0;
}

//---------functions-----------------------------
void init_db()
{
	ifstream file("data.txt");
    if (!file) {
        cerr << "Error opening the file." << endl;
        return;
    }

    char first_name[MAX_LEN];
    char last_name[MAX_LEN];
    int cell_number;
    int stage;
    int _class;
    int grades[NUM_OF_COURSES];

    while (file >> first_name >> last_name >> cell_number >> stage >> _class) {
        for (int i = 0; i < NUM_OF_COURSES; i++) {
            file >> grades[i];
        }

        insert_student(stage, _class, first_name, last_name, cell_number, grades);
    }

    file.close();
}

//-----------------------------------------------
void insert_student(int stage, int _class, const char* first_name, const char* last_name, int cell_number, int* grades)
{
    // Create a new student struct
    Student* new_student = new (nothrow) Student;
    if (!new_student) {
        cerr << "Memory allocation failed." << endl;
        exit(EXIT_FAILURE);
    }

    // Copy data into the student struct
    strncpy(new_student->first_name, first_name, MAX_LEN - 1);
    new_student->first_name[MAX_LEN - 1] = '\0';

    strncpy(new_student->last_name, last_name, MAX_LEN - 1);
    new_student->last_name[MAX_LEN - 1] = '\0';

    new_student->cell_namber = cell_number;

    for (int i = 0; i < NUM_OF_COURSES; i++) {
        new_student->courses[i] = new (nothrow) char[LENGTH];
        if (!new_student->courses[i]) {
            cerr << "Memory allocation failed." << endl;
            exit(EXIT_FAILURE);
        }
        snprintf(new_student->courses[i], LENGTH, "%d", grades[i]);
    }

    // Insert the student into the database
    s.db[stage - 1][_class - 1] = new_student;
}

//----------------------------------------
void print_db()
{
	for (int stage = 0; stage < NUM_OF_LEVELS; stage++)
	{
        for (int _class = 0; _class < NUM_OF_CLASSES; _class++)
        {
            Student* student = s.db[stage][_class];
            if (student) {
                cout << "Stage: " << stage + 1 << ", Class: " << _class + 1 << endl;
                cout << "Name: " << student->first_name << " " << student->last_name << endl;
                cout << "Cell Number: " << student->cell_namber << endl;
                cout << "Courses: ";
                for (int i = 0; i < NUM_OF_COURSES; i++) {
                    cout << student->courses[i] << " ";
                }
                cout << endl;
            }
        }
    }
}





