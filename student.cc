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


static struct School s;

//------prototypes------------------
void init_db();
void insert_student(int stage, int _class, const char* first_name, const char* last_name, int cell_number, int* grades);
void print_db();
void display_menu();
void admission_new_student();
void top_ten_students_in_subject();

//---------main section----------------
int main()
{
	init_db();
	
	display_menu();
	
	//print_db();
	
	return 0;
}

//---------functions-----------------------------
void init_db()
{
	ifstream file("data1.txt");
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

//-------------------------------------------
void display_menu()
{
	int choice;
    do {
        cout << "---- Menu ----" << endl;
        cout << "1. Admission of a new student" << endl;
        cout << "2. Top ten students in each grade in a particular subject" << endl;
        cout << "3. Students who are candidates for departure" << endl;
        cout << "4. Calculation of average per course per layer" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                admission_new_student();
                break;
            case 2:
                top_ten_students_in_subject();
                break;
            case 3:
                //candidates_for_departure();
                break;
            case 4:
                //calculate_average_per_course_per_layer();
                break;
            case 5:
                cout << "Exiting the system. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);
}

//---------------------------------------------------
void admission_new_student()
{
    // Gather information for the new student
    char first_name[MAX_LEN];
    char last_name[MAX_LEN];
    int cell_number;
    int stage;
    int _class;
    int grades[NUM_OF_COURSES];

    cout << "Enter the first name of the new student: ";
    cin.ignore(); // Ignore the newline character from the previous input
    cin.getline(first_name, MAX_LEN);

    cout << "Enter the last name of the new student: ";
    cin.getline(last_name, MAX_LEN);

    cout << "Enter the cell number of the new student: ";
    cin >> cell_number;

    cout << "Enter the stage of the new student: ";
    cin >> stage;

    cout << "Enter the class of the new student: ";
    cin >> _class;

    cout << "Enter the grades for " << NUM_OF_COURSES << " courses: ";
    for (int i = 0; i < NUM_OF_COURSES; i++) {
        cin >> grades[i];
    }

    // Insert the new student into the database
    insert_student(stage, _class, first_name, last_name, cell_number, grades);

    cout << "New student added successfully!" << endl;
}

//--------------------------------------------
void top_ten_students_in_subject()
{
    int subject_index;
    cout << "Enter the subject index (0 to " << NUM_OF_COURSES - 1 << "): ";
    cin >> subject_index;

    // Create an array to store students and their corresponding average in the subject
    struct StudentWithAverage {
        struct Student* student;
        double average;
    };

    struct StudentWithAverage students_with_average[NUM_OF_LEVELS * NUM_OF_CLASSES];
    int num_students_with_average = 0;

    // Calculate the average score for each student in the given subject
    for (int stage = 0; stage < NUM_OF_LEVELS; stage++)
    {
        for (int _class = 0; _class < NUM_OF_CLASSES; _class++)
        {
            struct Student* student = s.db[stage][_class];
            if (student) {
                int grade = atoi(student->courses[subject_index]); // Assuming the grades are stored as strings
                if (grade >= 0) {
                    students_with_average[num_students_with_average].student = student;
                    students_with_average[num_students_with_average].average = grade;
                    num_students_with_average++;
                }
            }
        }
    }

    // Sort the students in descending order of average scores (using a simple selection sort)
    for (int i = 0; i < num_students_with_average - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < num_students_with_average; j++) {
            if (students_with_average[j].average > students_with_average[max_idx].average) {
                max_idx = j;
            }
        }
        if (max_idx != i) {
            struct StudentWithAverage temp = students_with_average[i];
            students_with_average[i] = students_with_average[max_idx];
            students_with_average[max_idx] = temp;
        }
    }

    // Display the top ten students for the selected subject
    cout << "Top ten students in Subject " << subject_index << ":" << endl;
    for (int i = 0; i < num_students_with_average && i < 10; i++) {
        struct StudentWithAverage student_with_average = students_with_average[i];
        cout << "Name: " << student_with_average.student->first_name << " " << student_with_average.student->last_name;
        cout << ", Average: " << student_with_average.average << endl;
    }
}







