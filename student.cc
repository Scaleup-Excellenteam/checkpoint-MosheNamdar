#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    struct Student* _next;
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
void delete_student();
void edit_student();
void search_student();
void candidates_for_departure();
void calculate_average_per_course_per_level();
void export_db_to_file(const char* filename);

//---------main section----------------
int main()
{
    init_db();
    
    display_menu();
    
    return 0;
}

//---------functions-----------------------------
void init_db()
{
    FILE* file = fopen("data1.txt", "r");
    if (!file) {
        fprintf(stderr, "Error opening the file.\n");
        return;
    }

    char first_name[MAX_LEN];
    char last_name[MAX_LEN];
    int cell_number;
    int stage;
    int _class;
    int grades[NUM_OF_COURSES];

    while (fscanf(file, "%s %s %d %d %d", first_name, last_name, &cell_number, &stage, &_class) == 5) {
        for (int i = 0; i < NUM_OF_COURSES; i++) {
            fscanf(file, "%d", &grades[i]);
        }

        insert_student(stage, _class, first_name, last_name, cell_number, grades);
    }

    fclose(file);
}

//-----------------------------------------------
void insert_student(int stage, int _class, const char* first_name, const char* last_name, int cell_number, int* grades)
{
    // Create a new student struct
    struct Student* new_student = (struct Student*)malloc(sizeof(struct Student));
    if (!new_student) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Copy data into the student struct
    strncpy(new_student->first_name, first_name, MAX_LEN - 1);
    new_student->first_name[MAX_LEN - 1] = '\0';

    strncpy(new_student->last_name, last_name, MAX_LEN - 1);
    new_student->last_name[MAX_LEN - 1] = '\0';

    new_student->cell_namber = cell_number;

    for (int i = 0; i < NUM_OF_COURSES; i++) {
        new_student->courses[i] = (char*)malloc(LENGTH * sizeof(char));
        if (!new_student->courses[i]) {
            fprintf(stderr, "Memory allocation failed.\n");
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
    for (int stage = 0; stage < NUM_OF_LEVELS; stage++) {
        for (int _class = 0; _class < NUM_OF_CLASSES; _class++) {
            struct Student* student = s.db[stage][_class];
            if (student) {
                printf("Stage: %d, Class: %d\n", stage + 1, _class + 1);
                printf("Name: %s %s\n", student->first_name, student->last_name);
                printf("Cell Number: %d\n", student->cell_namber);
                printf("Courses: ");
                for (int i = 0; i < NUM_OF_COURSES; i++) {
                    printf("%s ", student->courses[i]);
                }
                printf("\n");
            }
        }
    }
}

//-------------------------------------------
void display_menu()
{
    int choice;
    do {
        printf("---- Menu ----\n");
        printf("1. Admission of a new student\n");
        printf("2. Top ten students in each grade in a particular subject\n");
        printf("3. Students who are candidates for departure\n");
        printf("4. Calculation of average per course per layer\n");
        printf("5. Delete a student\n");
        printf("6. Edit a student's information\n");
        printf("7. Search for a student\n");
        printf("8. Export the database to a file\n"); // Option added for exporting the database
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                admission_new_student();
                break;
            case 2:
                top_ten_students_in_subject();
                break;
            case 3:
                candidates_for_departure();
                break;
            case 4:
                calculate_average_per_course_per_level();
                break;
            case 5:
                delete_student();
                break;
            case 6:
                edit_student(); // Call the edit_student function for editing a student's information
                break;
            case 7:
                search_student();
                break;
            case 8:
                char filename[MAX_LEN];
                printf("Enter the filename to export the database: ");
                scanf("%s", filename);
                export_db_to_file(filename);
                break;
            case 9:
                printf("Exiting the system. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 9);
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

    printf("Enter the first name of the new student: ");
    getchar(); // Ignore the newline character from the previous input
    fgets(first_name, MAX_LEN, stdin);
    first_name[strcspn(first_name, "\n")] = '\0'; // Remove the newline character

    printf("Enter the last name of the new student: ");
    fgets(last_name, MAX_LEN, stdin);
    last_name[strcspn(last_name, "\n")] = '\0'; // Remove the newline character

    printf("Enter the cell number of the new student: ");
    scanf("%d", &cell_number);

    printf("Enter the stage of the new student: ");
    scanf("%d", &stage);

    printf("Enter the class of the new student: ");
    scanf("%d", &_class);

    printf("Enter the grades for %d courses: ", NUM_OF_COURSES);
    for (int i = 0; i < NUM_OF_COURSES; i++) {
        scanf("%d", &grades[i]);
    }

    // Insert the new student into the database
    insert_student(stage, _class, first_name, last_name, cell_number, grades);


    printf("New student added successfully!\n");
}

//--------------------------------------------
void top_ten_students_in_subject()
{
    int subject_index;
    printf("Enter the subject index (0 to %d): ", NUM_OF_COURSES - 1);
    scanf("%d", &subject_index);

    // Create an array to store students and their corresponding average in the subject
    struct StudentWithAverage {
        struct Student* student;
        double average;
    };

    struct StudentWithAverage students_with_average[NUM_OF_LEVELS * NUM_OF_CLASSES];
    int num_students_with_average = 0;

    // Calculate the average score for each student in the given subject
    for (int stage = 0; stage < NUM_OF_LEVELS; stage++) {
        for (int _class = 0; _class < NUM_OF_CLASSES; _class++) {
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
    printf("Top ten students in Subject %d:\n", subject_index);
    for (int i = 0; i < num_students_with_average && i < 10; i++) {
        struct StudentWithAverage student_with_average = students_with_average[i];
        printf("Name: %s %s, Average: %.2f\n", student_with_average.student->first_name, student_with_average.student->last_name, student_with_average.average);
    }
}

//-------------------------------------------
void delete_student()
{
    int stage, _class;
    char first_name[MAX_LEN], last_name[MAX_LEN];

    printf("Enter the stage of the student you want to delete: ");
    scanf("%d", &stage);

    printf("Enter the class of the student you want to delete: ");
    scanf("%d", &_class);

    printf("Enter the first name of the student you want to delete: ");
    getchar(); // Ignore the newline character from the previous input
    fgets(first_name, MAX_LEN, stdin);
    first_name[strcspn(first_name, "\n")] = '\0'; // Remove the newline character

    printf("Enter the last name of the student you want to delete: ");
    fgets(last_name, MAX_LEN, stdin);
    last_name[strcspn(last_name, "\n")] = '\0'; // Remove the newline character

    // Find the student in the database and delete it
    Student* student = s.db[stage - 1][_class - 1];
    if (student) {
        if (strcmp(student->first_name, first_name) == 0 && strcmp(student->last_name, last_name) == 0) {
            // Found the student, now delete it
            for (int i = 0; i < NUM_OF_COURSES; i++) {
                free(student->courses[i]);
            }
            s.db[stage - 1][_class - 1] = nullptr;

            printf("Student deleted successfully!\n");
            return;
        }
    }

    printf("Student not found in the database.\n");
}

//------------------------------------------
void edit_student()
{
    int stage, _class;
    char first_name[MAX_LEN], last_name[MAX_LEN];
    int phone_number;
    int choice;

    printf("Enter the stage of the student you want to edit: ");
    scanf("%d", &stage);

    printf("Enter the class of the student you want to edit: ");
    scanf("%d", &_class);

    printf("Enter the first name of the student you want to edit: ");
    getchar(); // Ignore the newline character from the previous input
    fgets(first_name, MAX_LEN, stdin);
    first_name[strcspn(first_name, "\n")] = '\0'; // Remove the newline character

    printf("Enter the last name of the student you want to edit: ");
    fgets(last_name, MAX_LEN, stdin);
    last_name[strcspn(last_name, "\n")] = '\0'; // Remove the newline character

    // Find the student in the database
    struct Student* student = s.db[stage - 1][_class - 1];
    if (student) {
        if (strcmp(student->first_name, first_name) == 0 && strcmp(student->last_name, last_name) == 0) {
            // Found the student, now let the user choose what information to edit
            printf("Student found:\n");
            printf("Name: %s %s\n", student->first_name, student->last_name);
            printf("Cell Number: %d\n", student->cell_namber);
            printf("Courses: ");
            for (int i = 0; i < NUM_OF_COURSES; i++) {
                printf("%s ", student->courses[i]);
            }
            printf("\n");

            printf("What information do you want to edit?\n");
            printf("1. First Name\n");
            printf("2. Last Name\n");
            printf("3. Cell Number\n");
            printf("4. Courses\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("Enter the new First Name: ");
                    getchar();
                    fgets(first_name, MAX_LEN, stdin);
                    first_name[strcspn(first_name, "\n")] = '\0';
                    strncpy(student->first_name, first_name, MAX_LEN - 1);
                    student->first_name[MAX_LEN - 1] = '\0';
                    break;
                case 2:
                    printf("Enter the new Last Name: ");
                    getchar();
                    fgets(last_name, MAX_LEN, stdin);
                    last_name[strcspn(last_name, "\n")] = '\0';
                    strncpy(student->last_name, last_name, MAX_LEN - 1);
                    student->last_name[MAX_LEN - 1] = '\0';
                    break;
                case 3:
                    printf("Enter the new phone number: ");
                    scanf("%d", &phone_number);
                    student->cell_namber = phone_number;
                    printf("Phone number updated successfully!\n");
                    break;
                case 4:
                    printf("Enter the course index (0 to %d) you want to update the grade: ", NUM_OF_COURSES - 1);
                    int course_index;
                    scanf("%d", &course_index);
                    if (course_index >= 0 && course_index < NUM_OF_COURSES) {
                        printf("Enter the new grade for the course: ");
                        scanf("%s", student->courses[course_index]);
                    } else {
                        printf("Invalid course index.\n");
                    }
                    break;
                default:
                    printf("Invalid choice.\n");
            }

            printf("Student information updated successfully!\n");
            return;
        }
    }

    printf("Student not found in the database.\n");
}

//----------------------------------------------
void search_student()
{
    char first_name[MAX_LEN], last_name[MAX_LEN];

    printf("Enter the first name of the student you want to search: ");
    getchar(); // Ignore the newline character from the previous input
    fgets(first_name, MAX_LEN, stdin);
    first_name[strcspn(first_name, "\n")] = '\0'; // Remove the newline character

    printf("Enter the last name of the student you want to search: ");
    fgets(last_name, MAX_LEN, stdin);
    last_name[strcspn(last_name, "\n")] = '\0'; // Remove the newline character

    // Find the student in the database
    for (int stage = 0; stage < NUM_OF_LEVELS; stage++) {
        for (int _class = 0; _class < NUM_OF_CLASSES; _class++) {
            struct Student* student = s.db[stage][_class];
            if (student) {
                if (strcmp(student->first_name, first_name) == 0 && strcmp(student->last_name, last_name) == 0) {
                    // Found the student, display all the information
                    printf("Student found:\n");
                    printf("Stage: %d, Class: %d\n", stage + 1, _class + 1);
                    printf("Name: %s %s\n", student->first_name, student->last_name);
                    printf("Cell Number: %d\n", student->cell_namber);
                    printf("Courses: ");
                    for (int i = 0; i < NUM_OF_COURSES; i++) {
                        printf("%s ", student->courses[i]);
                    }
                    printf("\n");
                    return;
                }
            }
        }
    }

    printf("Student not found in the database.\n");
}

//-----------------------------------------
void candidates_for_departure()
{
    // Create an array to store students and their corresponding average grades
    struct StudentWithAverage {
        struct Student* student;
        double average;
    };

    struct StudentWithAverage students_with_average[NUM_OF_LEVELS * NUM_OF_CLASSES];
    int num_students_with_average = 0;

    // Calculate the average score for each student
    for (int stage = 0; stage < NUM_OF_LEVELS; stage++) {
        for (int _class = 0; _class < NUM_OF_CLASSES; _class++) {
            struct Student* student = s.db[stage][_class];
            if (student) {
                // Calculate the average grade for the student
                double total = 0.0;
                for (int i = 0; i < NUM_OF_COURSES; i++) {
                    total += atof(student->courses[i]);
                }
                double average = total / NUM_OF_COURSES;

                students_with_average[num_students_with_average].student = student;
                students_with_average[num_students_with_average].average = average;
                num_students_with_average++;
            }
        }
    }

    // Display the students with average grade below 55 as candidates for departure
    printf("Candidates for Departure (Average Grade Below 55):\n");
    for (int i = 0; i < num_students_with_average; i++) {
        struct StudentWithAverage student_with_average = students_with_average[i];
        if (student_with_average.average < 55.0) {
            printf("Name: %s %s, Average: %.2f\n", student_with_average.student->first_name, student_with_average.student->last_name, student_with_average.average);
        }
    }
}

//-------------------------------------------------
void calculate_average_per_course_per_level()
{
    // Create arrays to store the total grades and the number of students for each course in each layer
    int total_grades[NUM_OF_LEVELS][NUM_OF_COURSES] = {0};
    int num_students[NUM_OF_LEVELS][NUM_OF_COURSES] = {0};

    // Calculate the total grades and the number of students for each course in each layer
    for (int stage = 0; stage < NUM_OF_LEVELS; stage++) {
        for (int _class = 0; _class < NUM_OF_CLASSES; _class++) {
            struct Student* student = s.db[stage][_class];
            if (student) {
                for (int i = 0; i < NUM_OF_COURSES; i++) {
                    total_grades[stage][i] += atoi(student->courses[i]);
                    num_students[stage][i]++;
                }
            }
        }
    }

    // Display the average grade for each course in each layer
    printf("Average Grade per Course per Layer:\n");
    for (int stage = 0; stage < NUM_OF_LEVELS; stage++) {
        printf("Layer: %d\n", stage + 1);
        for (int i = 0; i < NUM_OF_COURSES; i++) {
            double average = (double)total_grades[stage][i] / num_students[stage][i];
            printf("Course %d: %.2f\n", i + 1, average);
        }
    }
}

//------------------------------------------
void export_db_to_file(const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error opening the file.\n");
        return;
    }

    for (int stage = 0; stage < NUM_OF_LEVELS; stage++) {
        for (int _class = 0; _class < NUM_OF_CLASSES; _class++) {
            struct Student* student = s.db[stage][_class];
            if (student) {
                fprintf(file, "%s %s %d %d %d", student->first_name, student->last_name, student->cell_namber, stage + 1, _class + 1);
                for (int i = 0; i < NUM_OF_COURSES; i++) {
                    fprintf(file, " %s", student->courses[i]);
                }
                fprintf(file, "\n");
            }
        }
    }

    fclose(file);
    printf("Database exported to '%s' successfully!\n", filename);
}

//----------------------------------------------

