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
    int teacherId;
    char component[12];
    int marks;
};

int main()
{
    Student *stud = new Student[100]();
    Teacher *teach = new Teacher[100]();
    Attendance *attend = new Attendance[100]();
    MarkRecord *marks = new MarkRecord[100]();

    int studentCount = 0;
    int teacherCount = 0;
    int attendanceCount = 0;
    int marksCount = 0;

    // STUDENT FILE CLEARING

    ifstream input("students.txt");
    ofstream output("temp.txt");

    Student s = {};

    int validCount = 0;
    int invalidCount = 0;

    if(input.fail() || output.fail())
    {
        cout << "Student file opening failed." << endl;

        delete[] stud;
        delete[] teach;
        delete[] attend;
        delete[] marks;

        return 1;
    }

    for(int i = 0; i < 100 && input >> s.studentId; i++)
    {
        input.ignore();

        input.getline(s.name, 40, ',');

        input >> s.age;
        input.ignore();

        input.getline(s.gender, 8, ',');

        input.getline(s.contact, 12);

        int validContact = 1;

        for(int j = 0; j < 11; j++)
        {
            if(s.contact[j] < '0' || s.contact[j] > '9')
            {
                validContact = 0;
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

    cout << "Student File Clearing Completed!" << endl;
    cout << "Valid Records: " << validCount << endl;
    cout << "Invalid Records: " << invalidCount << endl;


    // STUDENT FILE READING

    input.open("students.txt");

    for(int i = 0; i < 100 && input >> stud[i].studentId; i++)
    {
        input.ignore();

        input.getline(stud[i].name, 40, ',');

        input >> stud[i].age;
        input.ignore();

        input.getline(stud[i].gender, 8, ',');

        input.getline(stud[i].contact, 12);

        studentCount++;
    }

    input.close();


    // TEACHER FILE READING

    input.open("teachers.txt");

    for(int i = 0; i < 100 && input >> teach[i].teacherId; i++)
    {
        input.ignore();

        input.getline(teach[i].name, 40, ',');

        input.getline(teach[i].department, 30, ',');

        input >> teach[i].experience;
        input.ignore();

        input.getline(teach[i].course, 30);

        teacherCount++;
    }

    input.close();


    // ATTENDANCE FILE READING

    input.open("attendance.txt");

    for(int i = 0; i < 100 && input >> attend[i].studentId; i++)
    {
        input.ignore();

        input >> attend[i].teacherId;
        input.ignore();

        input.getline(attend[i].date, 12, ',');

        input.getline(attend[i].status, 8);

        attendanceCount++;
    }

    input.close();


    // MARKS FILE READING

    input.open("marks.txt");

    for(int i = 0; i < 100 && input >> marks[i].studentId; i++)
    {
        input.ignore();

        input >> marks[i].teacherId;
        input.ignore();

        input.getline(marks[i].component, 12, ',');

        input >> marks[i].marks;

        marksCount++;
    }

    input.close();


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
             << marks[i].teacherId << " "
             << marks[i].component << " "
             << marks[i].marks << endl;
    }


    // RELEASE DYNAMIC MEMORY

    delete[] stud;
    delete[] teach;
    delete[] attend;
    delete[] marks;

    return 0;
}
