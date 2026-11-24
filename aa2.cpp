#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
using namespace std;

struct Student
{
    int studentId;
    char name[40];
    int age;
    char gender[8];
    char contact[12];
};

struct Teacher
{
    int teacherId;
    char name[40];
    char department[30];
    int experience;
    char course[30];
};

struct Attendance
{
    int studentId;
    int teacherId;
    char date[12];
    char status[8];
};

struct MarkRecord
{
    int studentId;
    char subject[10];
    char component[12];
    int marks;
};


Student* findStudentById(Student*, int, int);
void addStudent(Student *&stud, int &studentCount);
void updateStudent(Student *stud, int studentCount);
// STUDENT FILE CLEANING

void cleanStudents()
{
    ifstream input("students.txt");
    ofstream output("temp.txt");

    if(!input || !output)
    {
        cout << "Student file opening failed." << endl;
        return;
    }

    Student s;
    int validCount = 0;
    int invalidCount = 0;

    while(input >> s.studentId)
    {
        input.ignore();

        input.getline(s.name, 40, ',');

        input >> s.age;
        input.ignore();

        input.getline(s.gender, 8, ',');

        input.getline(s.contact, 12);

        int validContact = 1;

        if(strlen(s.contact) != 11)
        {
            validContact = 0;
        }
        else
        {
            for(int j = 0; j < 11; j++)
            {
                if(s.contact[j] < '0' || s.contact[j] > '9')
                {
                    validContact = 0;
                }
            }
        }

        if(input &&
           s.studentId > 0 &&
           strlen(s.name) > 0 &&
           s.age >= 16 && s.age <= 40 &&
           (strcmp(s.gender, "Male") == 0 ||
            strcmp(s.gender, "Female") == 0) &&
           strlen(s.contact) == 11 &&
           s.contact[0] == '0' &&
           s.contact[1] == '3' &&
           validContact == 1)
        {
            output << s.studentId << ","
                   << s.name << ","
                   << s.age << ","
                   << s.gender << ","
                   << s.contact << endl;

            validCount++;
        }
        else
        {
            invalidCount++;
        }
    }

    input.close();
    output.close();

    remove("students.txt");
    rename("temp.txt", "students.txt");

    cout << "\nStudent File Clearing Completed!" << endl;
    cout << "Valid Records: " << validCount << endl;
    cout << "Invalid Records: " << invalidCount << endl;
}


// TEACHER FILE CLEANING

void cleanTeachers()
{
    ifstream input("teachers.txt");
    ofstream output("temp.txt");

    if(!input || !output)
    {
        cout << "Teacher file opening failed." << endl;
        return;
    }

    Teacher t;

    int validCount = 0;
    int invalidCount = 0;

    while(input >> t.teacherId)
    {
        input.ignore();

        input.getline(t.name, 40, ',');

        input.getline(t.department, 30, ',');

        input >> t.experience;
        input.ignore();

        input.getline(t.course, 30);

        if(input &&
           t.teacherId > 0 &&
           strlen(t.name) > 0 &&
           strlen(t.department) > 0 &&
           t.experience >= 0 && t.experience <= 45 &&
           strlen(t.course) > 0)
        {
            output << t.teacherId << ","
                   << t.name << ","
                   << t.department << ","
                   << t.experience << ","
                   << t.course << endl;

            validCount++;
        }
        else
        {
            invalidCount++;
        }
    }

    input.close();
    output.close();

    remove("teachers.txt");
    rename("temp.txt", "teachers.txt");

    cout << "\nTeacher File Clearing Completed!" << endl;
    cout << "Valid Records: " << validCount << endl;
    cout << "Invalid Records: " << invalidCount << endl;
}


// ATTENDANCE FILE CLEANING

void cleanAttendance()
{
    ifstream input("attendance.txt");
    ofstream output("temp.txt");

    if(!input || !output)
    {
        cout << "Attendance file opening failed." << endl;
        return;
    }

    string header;
    getline(input, header);

    Attendance a;

    while(input >> a.studentId)
    {
        input.ignore();

        input >> a.teacherId;
        input.ignore();

        input.getline(a.date, 12, ',');

        input.getline(a.status, 8);

        if(input &&
           a.studentId > 0 &&
           a.teacherId > 0 &&
           strlen(a.date) == 10 &&
           (strcmp(a.status, "Present") == 0 ||
            strcmp(a.status, "Absent") == 0))
        {
            output << a.studentId << ","
                   << a.teacherId << ","
                   << a.date << ","
                   << a.status << endl;
        }
    }

    input.close();
    output.close();

    remove("attendance.txt");
    rename("temp.txt", "attendance.txt");

    cout << "\nAttendance File Clearing Completed!" << endl;
}


// MARKS FILE CLEANING

void cleanMarks()
{
    ifstream input("marks.txt");
    ofstream output("temp.txt");

    if(!input || !output)
    {
        cout << "Marks file opening failed." << endl;
        return;
    }

    MarkRecord m;

    while(input >> m.studentId)
    {
        input.ignore();

        input.getline(m.subject, 10, ',');

        input.getline(m.component, 12, ',');

        input >> m.marks;

        if(input &&
           m.studentId > 0 &&
           strlen(m.subject) > 0 &&
           strlen(m.component) > 0 &&
           m.marks >= 0)
        {
            output << m.studentId << ","
                   << m.subject << ","
                   << m.component << ","
                   << m.marks << endl;
        }
    }

    input.close();
    output.close();

    remove("marks.txt");
    rename("temp.txt", "marks.txt");

    cout << "\nMarks File Clearing Completed!" << endl;
}


// STUDENT FILE READING

Student* loadStudents(int &studentCount)
{
    studentCount = 0;

    ifstream input("students.txt");

    if(!input)
    {
        cout << "Student file opening failed." << endl;
        return NULL;
    }

    Student s;

    // Count records

    while(input >> s.studentId)
    {
        input.ignore();

        input.getline(s.name, 40, ',');

        input >> s.age;
        input.ignore();

        input.getline(s.gender, 8, ',');

        input.getline(s.contact, 12);

        if(input)
        {
            studentCount++;
        }
    }

    input.close();

    // Allocate exact memory

    Student *stud = new Student[studentCount]();

    input.open("students.txt");

    // Read records

    for(int i = 0; i < studentCount; i++)
    {
        input >> stud[i].studentId;
        input.ignore();

        input.getline(stud[i].name, 40, ',');

        input >> stud[i].age;
        input.ignore();

        input.getline(stud[i].gender, 8, ',');

        input.getline(stud[i].contact, 12);
    }

    input.close();

    return stud;
}


// TEACHER FILE READING

Teacher* loadTeachers(int &teacherCount)
{
    teacherCount = 0;

    ifstream input("teachers.txt");

    if(!input)
    {
        cout << "Teacher file opening failed." << endl;
        return NULL;
    }

    Teacher t;

    // Count records

    while(input >> t.teacherId)
    {
        input.ignore();

        input.getline(t.name, 40, ',');

        input.getline(t.department, 30, ',');

        input >> t.experience;
        input.ignore();

        input.getline(t.course, 30);

        if(input)
        {
            teacherCount++;
        }
    }

    input.close();

    // Allocate exact memory

    Teacher *teach = new Teacher[teacherCount]();

    input.open("teachers.txt");

    // Read records

    for(int i = 0; i < teacherCount; i++)
    {
        input >> teach[i].teacherId;
        input.ignore();

        input.getline(teach[i].name, 40, ',');

        input.getline(teach[i].department, 30, ',');

        input >> teach[i].experience;
        input.ignore();

        input.getline(teach[i].course, 30);
    }

    input.close();

    return teach;
}


// SAVE STUDENTS

void saveStudents(Student *stud, int studentCount)
{
    ofstream output("students.txt");

    for(int i = 0; i < studentCount; i++)
    {
        output << stud[i].studentId << ","
               << stud[i].name << ","
               << stud[i].age << ","
               << stud[i].gender << ","
               << stud[i].contact << endl;
    }

    output.close();
}

// ADD STUDENT

void addStudent(Student *&stud, int &studentCount)
{
    Student s;

    // Get Student ID
    cout << "\nEnter Student ID: ";
    cin >> s.studentId;

    // Check positive ID
    while(s.studentId <= 0)
    {
        cout << "Invalid ID. Enter a positive ID: ";
        cin >> s.studentId;
    }

    // Check duplicate ID
    for(int i = 0; i < studentCount; i++)
    {
        while(stud[i].studentId == s.studentId)
        {
            cout << "This ID already exists. Enter another ID: ";
            cin >> s.studentId;

            i = -1;
        }
    }

    cin.ignore(1000, '\n');

    // Get Student Name
    cout << "Enter Student Name: ";
    cin.getline(s.name, 40);

    while(strlen(s.name) == 0)
    {
        cout << "Name cannot be empty. Enter again: ";
        cin.getline(s.name, 40);
    }

    // Get Age
    cout << "Enter Age: ";
    cin >> s.age;

    while(s.age < 16 || s.age > 40)
    {
        cout << "Age must be between 16 and 40. Enter again: ";
        cin >> s.age;
    }

    cin.ignore(1000, '\n');

    // Get Gender
    cout << "Enter Gender (Male/Female): ";
    cin.getline(s.gender, 8);

    while(strcmp(s.gender, "Male") != 0 &&
          strcmp(s.gender, "Female") != 0)
    {
        cout << "Enter Male or Female: ";
        cin.getline(s.gender, 8);
    }

    // Get Contact
    cout << "Enter Contact Number: ";
    cin.getline(s.contact, 12);

    int validContact = 1;

    while(true)
    {
        validContact = 1;

        if(strlen(s.contact) != 11)
        {
            validContact = 0;
        }
        else
        {
            if(s.contact[0] != '0' || s.contact[1] != '3')
            {
                validContact = 0;
            }

            for(int i = 0; i < 11; i++)
            {
                if(s.contact[i] < '0' || s.contact[i] > '9')
                {
                    validContact = 0;
                }
            }
        }

        if(validContact == 1)
        {
            break;
        }

        cout << "Invalid contact. Enter 11 digits starting with 03: ";
        cin.getline(s.contact, 12);
    }

    // Create new dynamic array
    Student *newStud = new Student[studentCount + 1];

    // Copy existing students
    for(int i = 0; i < studentCount; i++)
    {
        newStud[i] = stud[i];
    }

    // Add new student
    newStud[studentCount] = s;

    // Delete old memory
    delete[] stud;

    // Update pointer
    stud = newStud;

    // Increase student count
    studentCount++;

    // Save records into file
    saveStudents(stud, studentCount);

    cout << "\nStudent added successfully!" << endl;
}

// UPDATE STUDENT

void updateStudent(Student *stud, int studentCount)
{
    int searchId;

    cout << "\nEnter Student ID to update: ";
    cin >> searchId;

    // Find student
    Student *found = findStudentById(stud, studentCount, searchId);

    if(found == NULL)
    {
        cout << "\nStudent not found!" << endl;
        return;
    }

    cout << "\nStudent Found!" << endl;

    cout << "\nCurrent Details:" << endl;
    cout << "ID: " << found->studentId << endl;
    cout << "Name: " << found->name << endl;
    cout << "Age: " << found->age << endl;
    cout << "Gender: " << found->gender << endl;
    cout << "Contact: " << found->contact << endl;

    cin.ignore(1000, '\n');

    // Update Name
    cout << "\nEnter New Name: ";
    cin.getline(found->name, 40);

    while(strlen(found->name) == 0)
    {
        cout << "Name cannot be empty. Enter again: ";
        cin.getline(found->name, 40);
    }

    // Update Age
    cout << "Enter New Age: ";
    cin >> found->age;

    while(found->age < 16 || found->age > 40)
    {
        cout << "Age must be between 16 and 40. Enter again: ";
        cin >> found->age;
    }

    cin.ignore(1000, '\n');

    // Update Gender
    cout << "Enter New Gender (Male/Female): ";
    cin.getline(found->gender, 8);

    while(strcmp(found->gender, "Male") != 0 &&
          strcmp(found->gender, "Female") != 0)
    {
        cout << "Enter Male or Female: ";
        cin.getline(found->gender, 8);
    }

    // Update Contact
    cout << "Enter New Contact Number: ";
    cin.getline(found->contact, 12);

    int validContact;

    while(true)
    {
        validContact = 1;

        if(strlen(found->contact) != 11)
        {
            validContact = 0;
        }
        else
        {
            if(found->contact[0] != '0' ||
               found->contact[1] != '3')
            {
                validContact = 0;
            }

            for(int i = 0; i < 11; i++)
            {
                if(found->contact[i] < '0' ||
                   found->contact[i] > '9')
                {
                    validContact = 0;
                }
            }
        }

        if(validContact == 1)
        {
            break;
        }

        cout << "Invalid contact. Enter 11 digits starting with 03: ";
        cin.getline(found->contact, 12);
    }

    // Save updated records
    saveStudents(stud, studentCount);

    cout << "\nStudent details updated successfully!" << endl;
}
// SAVE TEACHERS

void saveTeachers(Teacher *teach, int teacherCount)
{
    ofstream output("teachers.txt");

    for(int i = 0; i < teacherCount; i++)
    {
        output << teach[i].teacherId << ","
               << teach[i].name << ","
               << teach[i].department << ","
               << teach[i].experience << ","
               << teach[i].course << endl;
    }

    output.close();
}



Student* findStudentById(Student *stud, int studentCount, int searchId)
{
    for(int i = 0; i < studentCount; i++)
    {
        if(stud[i].studentId == searchId)
        {
            return &stud[i];
        }
    }

    return NULL;
}

// MAIN FUNCTION

int main()
{
    // Clean all files

    cleanStudents();
    cleanTeachers();
    cleanAttendance();
    cleanMarks();

    int studentCount = 0;
    int teacherCount = 0;
    int attendanceCount = 0;
    int marksCount = 0;


    // Load students and teachers

    Student *stud = loadStudents(studentCount);

    Teacher *teach = loadTeachers(teacherCount);


 updateStudent(stud, studentCount);

addStudent(stud, studentCount);

    // Attendance and Marks memory

    Attendance *attend = new Attendance[100]();

    MarkRecord *marks = new MarkRecord[100]();


    // ATTENDANCE FILE READING

    ifstream input("attendance.txt");

    if(input)
    {
        for(int i = 0; i < 100 && input >> attend[i].studentId; i++)
        {
            input.ignore();

            input >> attend[i].teacherId;
            input.ignore();

            input.getline(attend[i].date, 12, ',');

            input.getline(attend[i].status, 8);

            if(input)
            {
                attendanceCount++;
            }
        }

        input.close();
    }


    // MARKS FILE READING

    input.open("marks.txt");

    if(input)
    {
        for(int i = 0; i < 100 && input >> marks[i].studentId; i++)
        {
            input.ignore();

            input.getline(marks[i].subject, 10, ',');

            input.getline(marks[i].component, 12, ',');

            input >> marks[i].marks;

            if(input)
            {
                marksCount++;
            }
        }

        input.close();
    }


    // DISPLAY STUDENT RECORDS

    cout << "\n----- STUDENT RECORDS -----\n";

    for(int i = 0; i < studentCount; i++)
    {
        cout << stud[i].studentId << " "
             << stud[i].name << " "
             << stud[i].age << " "
             << stud[i].gender << " "
             << stud[i].contact << endl;
    }


    // DISPLAY TEACHER RECORDS

    cout << "\n----- TEACHER RECORDS -----\n";

    for(int i = 0; i < teacherCount; i++)
    {
        cout << teach[i].teacherId << " "
             << teach[i].name << " "
             << teach[i].department << " "
             << teach[i].experience << " "
             << teach[i].course << endl;
    }


    // DISPLAY ATTENDANCE RECORDS

    cout << "\n----- ATTENDANCE RECORDS -----\n";

    for(int i = 0; i < attendanceCount; i++)
    {
        cout << attend[i].studentId << " "
             << attend[i].teacherId << " "
             << attend[i].date << " "
             << attend[i].status << endl;
    }


    // DISPLAY MARKS RECORDS

    cout << "\n----- MARKS RECORDS -----\n";

    for(int i = 0; i < marksCount; i++)
    {
        cout << marks[i].studentId << " "
             << marks[i].subject << " "
             << marks[i].component << " "
             << marks[i].marks << endl;
    }

  int searchId;

cout << "\nEnter Student ID to search: ";
cin >> searchId;

Student *found = findStudentById(stud, studentCount, searchId);

if(found != NULL)
{
    cout << "\nStudent Found!" << endl;
    cout << "ID: " << found->studentId << endl;
    cout << "Name: " << found->name << endl;
    cout << "Age: " << found->age << endl;
    cout << "Gender: " << found->gender << endl;
    cout << "Contact: " << found->contact << endl;
}
else
{
    cout << "\nStudent Not Found!" << endl;
}
    // RELEASE MEMORY

    delete[] stud;
    delete[] teach;
    delete[] attend;
    delete[] marks;

    return 0;
}
