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

// ---------- FUNCTION PROTOTYPES ----------

bool adminLogin();

bool readStudentLine(char line[], Student &s);
bool readTeacherLine(char line[], Teacher &t);
bool readAttendanceLine(char line[], Attendance &a);
bool readMarkLine(char line[], MarkRecord &m);

void cleanStudents();
void cleanTeachers();
void cleanAttendance();
void cleanMarks();

Student* loadStudents(int &count);
Teacher* loadTeachers(int &count);
Attendance* loadAttendance(int &count);
MarkRecord* loadMarks(int &count);

void saveStudents(Student *stud, int count);
void saveTeachers(Teacher *teach, int count);
void saveAttendance(Attendance *attend, int count);
void saveMarks(MarkRecord *marks, int count);

Student* findStudentById(Student *stud, int count, int id);
Teacher* findTeacherById(Teacher *teach, int count, int id);

void addStudent(Student *&stud, int &count);
void updateStudent(Student *stud, int count);
void deleteStudent(Student *&stud, int &count);
void viewStudents(Student *stud, int count);

void addTeacher(Teacher *&teach, int &count);
void updateTeacher(Teacher *teach, int count);
void deleteTeacher(Teacher *&teach, int &count);
void viewTeachers(Teacher *teach, int count);

void removeRecordsOfStudent(int id);

void addAttendance(Attendance *&attend, int &count,
                   Student *stud, int studentCount,
                   Teacher *teach, int teacherCount);
void viewAttendance(Attendance *attend, int count);
void updateAttendance(Attendance *attend, int count);
void searchAttendance(Attendance *attend, int count);

void addMarks(MarkRecord *&marks, int &count,
              Student *stud, int studentCount);
void viewMarks(MarkRecord *marks, int count);
void updateMarks(MarkRecord *marks, int count);
void searchMarks(MarkRecord *marks, int count);

// ---------- CSV LINE READING ----------

bool readStudentLine(char line[], Student &s)
{
    return sscanf(line, "%d,%39[^,],%d,%7[^,],%11s",
                  &s.studentId, s.name, &s.age,
                  s.gender, s.contact) == 5;
}

bool readTeacherLine(char line[], Teacher &t)
{
    return sscanf(line, "%d,%39[^,],%29[^,],%d,%29[^\n]",
                  &t.teacherId, t.name, t.department,
                  &t.experience, t.course) == 5;
}

bool readAttendanceLine(char line[], Attendance &a)
{
    return sscanf(line, "%d,%d,%11[^,],%7s",
                  &a.studentId, &a.teacherId,
                  a.date, a.status) == 4;
}

bool readMarkLine(char line[], MarkRecord &m)
{
    return sscanf(line, "%d,%9[^,],%11[^,],%d",
                  &m.studentId, m.subject,
                  m.component, &m.marks) == 4;
}

// ---------- ADMIN LOGIN ----------

bool adminLogin()
{
    char password[20];
    const char correctPassword[] = "aa1234";
    int attempts = 3;

    while(attempts > 0)
    {
        cout << "\nEnter Admin Password: ";
        cin.width(20);
        cin >> password;

        if(strcmp(password, correctPassword) == 0)
        {
            cout << "\nLogin Successful!\n";
            return true;
        }

        attempts--;
        cout << "\nIncorrect Password!";
        cout << "\nRemaining Attempts: " << attempts << endl;
    }

    cout << "\nAccess Denied!\n";
    return false;
}

// ---------- FILE CLEANING ----------

void cleanStudents()
{
    ifstream input("students.txt");
    if(!input)
    {
        cout << "\nStudent file not found. A new file will be created.\n";
        ofstream create("students.txt");
        return;
    }

    ofstream output("temp_students.txt");
    char line[200];
    Student s;
    int valid = 0, invalid = 0;

    while(input.getline(line, 200))
    {
        if(readStudentLine(line, s) &&
           s.studentId > 0 &&
           strlen(s.name) > 0 &&
           s.age >= 16 && s.age <= 40 &&
           (strcmp(s.gender, "Male") == 0 ||
            strcmp(s.gender, "Female") == 0) &&
           strlen(s.contact) == 11 &&
           s.contact[0] == '0' && s.contact[1] == '3')
        {
            bool digits = true;
            for(int i = 0; i < 11; i++)
                if(s.contact[i] < '0' || s.contact[i] > '9')
                    digits = false;

            if(digits)
            {
                output << s.studentId << "," << s.name << ","
                       << s.age << "," << s.gender << ","
                       << s.contact << endl;
                valid++;
            }
            else invalid++;
        }
        else invalid++;
    }

    input.close();
    output.close();
    remove("students.txt");
    rename("temp_students.txt", "students.txt");

    cout << "\nStudent File Clearing Completed!";
    cout << "\nValid Records: " << valid;
    cout << "\nInvalid Records: " << invalid << endl;
}

void cleanTeachers()
{
    ifstream input("teachers.txt");
    if(!input)
    {
        cout << "\nTeacher file not found. A new file will be created.\n";
        ofstream create("teachers.txt");
        return;
    }

    ofstream output("temp_teachers.txt");
    char line[250];
    Teacher t;
    int valid = 0, invalid = 0;

    while(input.getline(line, 250))
    {
        if(readTeacherLine(line, t) &&
           t.teacherId > 0 &&
           strlen(t.name) > 0 &&
           strlen(t.department) > 0 &&
           t.experience >= 0 && t.experience <= 45 &&
           strlen(t.course) > 0)
        {
            output << t.teacherId << "," << t.name << ","
                   << t.department << "," << t.experience
                   << "," << t.course << endl;
            valid++;
        }
        else invalid++;
    }

    input.close();
    output.close();
    remove("teachers.txt");
    rename("temp_teachers.txt", "teachers.txt");

    cout << "\nTeacher File Clearing Completed!";
    cout << "\nValid Records: " << valid;
    cout << "\nInvalid Records: " << invalid << endl;
}

void cleanAttendance()
{
    ifstream input("attendance.txt");
    if(!input)
    {
        cout << "\nAttendance file not found. A new file will be created.\n";
        ofstream create("attendance.txt");
        create << "StudentID,TeacherID,Date,Status\n";
        return;
    }

    ofstream output("temp_attendance.txt");
    char line[150];
    Attendance a;
    int valid = 0, invalid = 0;

    output << "StudentID,TeacherID,Date,Status\n";

    while(input.getline(line, 150))
    {
        if(strncmp(line, "StudentID", 9) == 0)
            continue;

        if(readAttendanceLine(line, a) &&
           a.studentId > 0 && a.teacherId > 0 &&
           strlen(a.date) == 10 &&
           (strcmp(a.status, "Present") == 0 ||
            strcmp(a.status, "Absent") == 0))
        {
            output << a.studentId << "," << a.teacherId << ","
                   << a.date << "," << a.status << endl;
            valid++;
        }
        else invalid++;
    }

    input.close();
    output.close();
    remove("attendance.txt");
    rename("temp_attendance.txt", "attendance.txt");

    cout << "\nAttendance File Clearing Completed!";
    cout << "\nValid Records: " << valid;
    cout << "\nInvalid Records: " << invalid << endl;
}

void cleanMarks()
{
    ifstream input("marks.txt");
    if(!input)
    {
        cout << "\nMarks file not found. A new file will be created.\n";
        ofstream create("marks.txt");
        create << "StudentID,Subject,Component,Marks\n";
        return;
    }

    ofstream output("temp_marks.txt");
    char line[150];
    MarkRecord m;
    int valid = 0, invalid = 0;

    output << "StudentID,Subject,Component,Marks\n";

    while(input.getline(line, 150))
    {
        if(strncmp(line, "StudentID", 9) == 0)
            continue;

        if(readMarkLine(line, m) &&
           m.studentId > 0 &&
           strlen(m.subject) > 0 &&
           strlen(m.component) > 0 &&
           m.marks >= 0 && m.marks <= 100)
        {
            output << m.studentId << "," << m.subject << ","
                   << m.component << "," << m.marks << endl;
            valid++;
        }
        else invalid++;
    }

    input.close();
    output.close();
    remove("marks.txt");
    rename("temp_marks.txt", "marks.txt");

    cout << "\nMarks File Clearing Completed!";
    cout << "\nValid Records: " << valid;
    cout << "\nInvalid Records: " << invalid << endl;
}

// ---------- LOAD FUNCTIONS ----------

Student* loadStudents(int &count)
{
    count = 0;
    ifstream input("students.txt");
    if(!input) return NULL;

    char line[200];
    Student s;

    while(input.getline(line, 200))
        if(readStudentLine(line, s)) count++;

    input.close();

    if(count == 0) return NULL;

    Student *data = new Student[count];
    input.open("students.txt");
    int i = 0;

    while(input.getline(line, 200) && i < count)
        if(readStudentLine(line, data[i])) i++;

    input.close();
    count = i;
    return data;
}

Teacher* loadTeachers(int &count)
{
    count = 0;
    ifstream input("teachers.txt");
    if(!input) return NULL;

    char line[250];
    Teacher t;

    while(input.getline(line, 250))
        if(readTeacherLine(line, t)) count++;

    input.close();

    if(count == 0) return NULL;

    Teacher *data = new Teacher[count];
    input.open("teachers.txt");
    int i = 0;

    while(input.getline(line, 250) && i < count)
        if(readTeacherLine(line, data[i])) i++;

    input.close();
    count = i;
    return data;
}

Attendance* loadAttendance(int &count)
{
    count = 0;
    ifstream input("attendance.txt");
    if(!input) return NULL;

    char line[150];
    Attendance a;

    while(input.getline(line, 150))
        if(readAttendanceLine(line, a)) count++;

    input.close();

    if(count == 0) return NULL;

    Attendance *data = new Attendance[count];
    input.open("attendance.txt");
    int i = 0;

    while(input.getline(line, 150) && i < count)
        if(readAttendanceLine(line, data[i])) i++;

    input.close();
    count = i;
    return data;
}

MarkRecord* loadMarks(int &count)
{
    count = 0;
    ifstream input("marks.txt");
    if(!input) return NULL;

    char line[150];
    MarkRecord m;

    while(input.getline(line, 150))
        if(readMarkLine(line, m)) count++;

    input.close();

    if(count == 0) return NULL;

    MarkRecord *data = new MarkRecord[count];
    input.open("marks.txt");
    int i = 0;

    while(input.getline(line, 150) && i < count)
        if(readMarkLine(line, data[i])) i++;

    input.close();
    count = i;
    return data;
}

// ---------- SAVE FUNCTIONS ----------

void saveStudents(Student *stud, int count)
{
    ofstream output("students.txt");
    for(int i = 0; i < count; i++)
        output << stud[i].studentId << "," << stud[i].name << ","
               << stud[i].age << "," << stud[i].gender << ","
               << stud[i].contact << endl;
}

void saveTeachers(Teacher *teach, int count)
{
    ofstream output("teachers.txt");
    for(int i = 0; i < count; i++)
        output << teach[i].teacherId << "," << teach[i].name << ","
               << teach[i].department << "," << teach[i].experience
               << "," << teach[i].course << endl;
}

void saveAttendance(Attendance *attend, int count)
{
    ofstream output("attendance.txt");
    output << "StudentID,TeacherID,Date,Status\n";
    for(int i = 0; i < count; i++)
        output << attend[i].studentId << "," << attend[i].teacherId
               << "," << attend[i].date << "," << attend[i].status << endl;
}

void saveMarks(MarkRecord *marks, int count)
{
    ofstream output("marks.txt");
    output << "StudentID,Subject,Component,Marks\n";
    for(int i = 0; i < count; i++)
        output << marks[i].studentId << "," << marks[i].subject
               << "," << marks[i].component << "," << marks[i].marks << endl;
}

// ---------- SEARCH FUNCTIONS ----------

Student* findStudentById(Student *stud, int count, int id)
{
    for(int i = 0; i < count; i++)
        if(stud[i].studentId == id) return &stud[i];
    return NULL;
}

Teacher* findTeacherById(Teacher *teach, int count, int id)
{
    for(int i = 0; i < count; i++)
        if(teach[i].teacherId == id) return &teach[i];
    return NULL;
}

// ---------- STUDENT FUNCTIONS ----------

void addStudent(Student *&stud, int &count)
{
    Student s;

    cout << "\nEnter Student ID: ";
    cin >> s.studentId;

    while(cin.fail() || s.studentId <= 0 ||
          findStudentById(stud, count, s.studentId) != NULL)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Enter a new positive and unique ID: ";
        cin >> s.studentId;
    }

    cin.ignore(1000, '\n');

    cout << "Enter Student Name: ";
    cin.getline(s.name, 40);
    while(strlen(s.name) == 0)
    {
        cout << "Name cannot be empty. Enter again: ";
        cin.getline(s.name, 40);
    }

    cout << "Enter Age (16-40): ";
    cin >> s.age;
    while(cin.fail() || s.age < 16 || s.age > 40)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Enter age between 16 and 40: ";
        cin >> s.age;
    }

    cin.ignore(1000, '\n');
    cout << "Enter Gender (Male/Female): ";
    cin.getline(s.gender, 8);
    while(strcmp(s.gender, "Male") != 0 &&
          strcmp(s.gender, "Female") != 0)
    {
        cout << "Enter Male or Female: ";
        cin.getline(s.gender, 8);
    }

    cout << "Enter Contact (11 digits, starts with 03): ";
    cin.getline(s.contact, 12);

    while(strlen(s.contact) != 11 ||
          s.contact[0] != '0' || s.contact[1] != '3')
    {
        cout << "Invalid contact. Enter again: ";
        cin.getline(s.contact, 12);
    }

    for(int i = 0; i < 11; i++)
    {
        while(s.contact[i] < '0' || s.contact[i] > '9')
        {
            cout << "Contact must contain digits only. Enter again: ";
            cin.getline(s.contact, 12);
            i = -1;
            break;
        }
    }

    Student *temp = new Student[count + 1];
    for(int i = 0; i < count; i++) temp[i] = stud[i];
    temp[count] = s;

    delete[] stud;
    stud = temp;
    count++;

    saveStudents(stud, count);
    cout << "\nStudent added successfully!\n";
}

void updateStudent(Student *stud, int count)
{
    if(count == 0)
    {
        cout << "\nNo students available!\n";
        return;
    }

    int id;
    cout << "\nEnter Student ID to update: ";
    cin >> id;

    Student *s = findStudentById(stud, count, id);
    if(s == NULL)
    {
        cout << "\nStudent not found!\n";
        return;
    }

    cin.ignore(1000, '\n');
    cout << "Enter New Name: ";
    cin.getline(s->name, 40);
    while(strlen(s->name) == 0)
    {
        cout << "Name cannot be empty: ";
        cin.getline(s->name, 40);
    }

    cout << "Enter New Age (16-40): ";
    cin >> s->age;
    while(cin.fail() || s->age < 16 || s->age > 40)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Enter age between 16 and 40: ";
        cin >> s->age;
    }

    cin.ignore(1000, '\n');
    cout << "Enter New Gender (Male/Female): ";
    cin.getline(s->gender, 8);
    while(strcmp(s->gender, "Male") != 0 &&
          strcmp(s->gender, "Female") != 0)
    {
        cout << "Enter Male or Female: ";
        cin.getline(s->gender, 8);
    }

    cout << "Enter New Contact: ";
    cin.getline(s->contact, 12);
    while(strlen(s->contact) != 11 ||
          s->contact[0] != '0' || s->contact[1] != '3')
    {
        cout << "Enter 11 digits starting with 03: ";
        cin.getline(s->contact, 12);
    }

    saveStudents(stud, count);
    cout << "\nStudent updated successfully!\n";
}

void removeRecordsOfStudent(int id)
{
    // Attendance
    ifstream inA("attendance.txt");
    if(inA)
    {
        ofstream outA("temp_attendance.txt");
        char line[150];
        Attendance a;
        outA << "StudentID,TeacherID,Date,Status\n";

        while(inA.getline(line, 150))
        {
            if(readAttendanceLine(line, a) && a.studentId != id)
                outA << a.studentId << "," << a.teacherId << ","
                     << a.date << "," << a.status << endl;
        }

        inA.close();
        outA.close();
        remove("attendance.txt");
        rename("temp_attendance.txt", "attendance.txt");
    }

    // Marks
    ifstream inM("marks.txt");
    if(inM)
    {
        ofstream outM("temp_marks.txt");
        char line[150];
        MarkRecord m;
        outM << "StudentID,Subject,Component,Marks\n";

        while(inM.getline(line, 150))
        {
            if(readMarkLine(line, m) && m.studentId != id)
                outM << m.studentId << "," << m.subject << ","
                     << m.component << "," << m.marks << endl;
        }

        inM.close();
        outM.close();
        remove("marks.txt");
        rename("temp_marks.txt", "marks.txt");
    }
}

void deleteStudent(Student *&stud, int &count)
{
    if(count == 0)
    {
        cout << "\nNo students available!\n";
        return;
    }

    int id;
    cout << "\nEnter Student ID to delete: ";
    cin >> id;

    int position = -1;
    for(int i = 0; i < count; i++)
        if(stud[i].studentId == id) position = i;

    if(position == -1)
    {
        cout << "\nStudent not found!\n";
        return;
    }

    char confirm;
    cout << "Confirm deletion (Y/N): ";
    cin >> confirm;
    if(confirm != 'Y' && confirm != 'y')
    {
        cout << "\nDeletion cancelled.\n";
        return;
    }

    Student *temp = NULL;
    if(count > 1)
    {
        temp = new Student[count - 1];
        int j = 0;
        for(int i = 0; i < count; i++)
            if(i != position) temp[j++] = stud[i];
    }

    delete[] stud;
    stud = temp;
    count--;

    saveStudents(stud, count);
    removeRecordsOfStudent(id);
    cout << "\nStudent deleted successfully!\n";
}

void viewStudents(Student *stud, int count)
{
    if(count == 0)
    {
        cout << "\nNo students available!\n";
        return;
    }

    cout << "\n========== STUDENT DETAILS ==========\n";
    for(int i = 0; i < count; i++)
    {
        cout << "\nStudent ID: " << stud[i].studentId
             << "\nName: " << stud[i].name
             << "\nAge: " << stud[i].age
             << "\nGender: " << stud[i].gender
             << "\nContact: " << stud[i].contact << "\n";
    }
}

// ---------- TEACHER FUNCTIONS ----------

void addTeacher(Teacher *&teach, int &count)
{
    Teacher t;

    cout << "\nEnter Teacher ID: ";
    cin >> t.teacherId;
    while(cin.fail() || t.teacherId <= 0 ||
          findTeacherById(teach, count, t.teacherId) != NULL)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Enter a new positive and unique ID: ";
        cin >> t.teacherId;
    }

    cin.ignore(1000, '\n');
    cout << "Enter Teacher Name: ";
    cin.getline(t.name, 40);
    while(strlen(t.name) == 0)
    {
        cout << "Name cannot be empty: ";
        cin.getline(t.name, 40);
    }

    cout << "Enter Department: ";
    cin.getline(t.department, 30);
    while(strlen(t.department) == 0)
    {
        cout << "Department cannot be empty: ";
        cin.getline(t.department, 30);
    }

    cout << "Enter Experience (0-45): ";
    cin >> t.experience;
    while(cin.fail() || t.experience < 0 || t.experience > 45)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Enter experience from 0 to 45: ";
        cin >> t.experience;
    }

    cin.ignore(1000, '\n');
    cout << "Enter Course: ";
    cin.getline(t.course, 30);
    while(strlen(t.course) == 0)
    {
        cout << "Course cannot be empty: ";
        cin.getline(t.course, 30);
    }

    Teacher *temp = new Teacher[count + 1];
    for(int i = 0; i < count; i++) temp[i] = teach[i];
    temp[count] = t;

    delete[] teach;
    teach = temp;
    count++;

    saveTeachers(teach, count);
    cout << "\nTeacher added successfully!\n";
}

void updateTeacher(Teacher *teach, int count)
{
    if(count == 0)
    {
        cout << "\nNo teachers available!\n";
        return;
    }

    int id;
    cout << "\nEnter Teacher ID to update: ";
    cin >> id;

    Teacher *t = findTeacherById(teach, count, id);
    if(t == NULL)
    {
        cout << "\nTeacher not found!\n";
        return;
    }

    cin.ignore(1000, '\n');
    cout << "Enter New Name: ";
    cin.getline(t->name, 40);
    while(strlen(t->name) == 0)
    {
        cout << "Name cannot be empty: ";
        cin.getline(t->name, 40);
    }

    cout << "Enter New Department: ";
    cin.getline(t->department, 30);
    while(strlen(t->department) == 0)
    {
        cout << "Department cannot be empty: ";
        cin.getline(t->department, 30);
    }

    cout << "Enter New Experience (0-45): ";
    cin >> t->experience;
    while(cin.fail() || t->experience < 0 || t->experience > 45)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Enter experience from 0 to 45: ";
        cin >> t->experience;
    }

    cin.ignore(1000, '\n');
    cout << "Enter New Course: ";
    cin.getline(t->course, 30);
    while(strlen(t->course) == 0)
    {
        cout << "Course cannot be empty: ";
        cin.getline(t->course, 30);
    }

    saveTeachers(teach, count);
    cout << "\nTeacher updated successfully!\n";
}

void deleteTeacher(Teacher *&teach, int &count)
{
    if(count == 0)
    {
        cout << "\nNo teachers available!\n";
        return;
    }

    int id, position = -1;
    cout << "\nEnter Teacher ID to delete: ";
    cin >> id;

    for(int i = 0; i < count; i++)
        if(teach[i].teacherId == id) position = i;

    if(position == -1)
    {
        cout << "\nTeacher not found!\n";
        return;
    }

    char confirm;
    cout << "Confirm deletion (Y/N): ";
    cin >> confirm;
    if(confirm != 'Y' && confirm != 'y')
    {
        cout << "\nDeletion cancelled.\n";
        return;
    }

    Teacher *temp = NULL;
    if(count > 1)
    {
        temp = new Teacher[count - 1];
        int j = 0;
        for(int i = 0; i < count; i++)
            if(i != position) temp[j++] = teach[i];
    }

    delete[] teach;
    teach = temp;
    count--;
    saveTeachers(teach, count);

    cout << "\nTeacher deleted successfully!\n";
}

void viewTeachers(Teacher *teach, int count)
{
    if(count == 0)
    {
        cout << "\nNo teachers available!\n";
        return;
    }

    cout << "\n========== TEACHER DETAILS ==========\n";
    for(int i = 0; i < count; i++)
    {
        cout << "\nTeacher ID: " << teach[i].teacherId
             << "\nName: " << teach[i].name
             << "\nDepartment: " << teach[i].department
             << "\nExperience: " << teach[i].experience
             << "\nCourse: " << teach[i].course << "\n";
    }
}

// ---------- ATTENDANCE FUNCTIONS ----------

void addAttendance(Attendance *&attend, int &count,
                   Student *stud, int studentCount,
                   Teacher *teach, int teacherCount)
{
    Attendance a;

    if(studentCount == 0 || teacherCount == 0)
    {
        cout << "\nAdd students and teachers first!\n";
        return;
    }

    cout << "\nEnter Student ID: ";
    cin >> a.studentId;
    if(findStudentById(stud, studentCount, a.studentId) == NULL)
    {
        cout << "\nStudent not found!\n";
        return;
    }

    cout << "Enter Teacher ID: ";
    cin >> a.teacherId;
    if(findTeacherById(teach, teacherCount, a.teacherId) == NULL)
    {
        cout << "\nTeacher not found!\n";
        return;
    }

    cout << "Enter Date (YYYY-MM-DD): ";
    cin >> a.date;
    if(strlen(a.date) != 10)
    {
        cout << "\nInvalid date format!\n";
        return;
    }

    cout << "Enter Status (Present/Absent): ";
    cin >> a.status;
    if(strcmp(a.status, "Present") != 0 &&
       strcmp(a.status, "Absent") != 0)
    {
        cout << "\nInvalid status!\n";
        return;
    }

    for(int i = 0; i < count; i++)
    {
        if(attend[i].studentId == a.studentId &&
           attend[i].teacherId == a.teacherId &&
           strcmp(attend[i].date, a.date) == 0)
        {
            cout << "\nThis attendance record already exists!\n";
            return;
        }
    }

    Attendance *temp = new Attendance[count + 1];
    for(int i = 0; i < count; i++) temp[i] = attend[i];
    temp[count] = a;

    delete[] attend;
    attend = temp;
    count++;

    saveAttendance(attend, count);
    cout << "\nAttendance added successfully!\n";
}

void viewAttendance(Attendance *attend, int count)
{
    if(count == 0)
    {
        cout << "\nNo attendance records found!\n";
        return;
    }

    cout << "\n========== ATTENDANCE RECORDS ==========\n";
    for(int i = 0; i < count; i++)
    {
        cout << "\nStudent ID: " << attend[i].studentId
             << "\nTeacher ID: " << attend[i].teacherId
             << "\nDate: " << attend[i].date
             << "\nStatus: " << attend[i].status << "\n";
    }
}

void updateAttendance(Attendance *attend, int count)
{
    if(count == 0)
    {
        cout << "\nNo attendance records available!\n";
        return;
    }

    int studentId, teacherId;
    char date[12], status[8];

    cout << "\nEnter Student ID: ";
    cin >> studentId;
    cout << "Enter Teacher ID: ";
    cin >> teacherId;
    cout << "Enter Date (YYYY-MM-DD): ";
    cin >> date;

    for(int i = 0; i < count; i++)
    {
        if(attend[i].studentId == studentId &&
           attend[i].teacherId == teacherId &&
           strcmp(attend[i].date, date) == 0)
        {
            cout << "Current Status: " << attend[i].status;
            cout << "\nEnter New Status (Present/Absent): ";
            cin >> status;

            if(strcmp(status, "Present") != 0 &&
               strcmp(status, "Absent") != 0)
            {
                cout << "\nInvalid status!\n";
                return;
            }

            strcpy(attend[i].status, status);
            saveAttendance(attend, count);
            cout << "\nAttendance updated successfully!\n";
            return;
        }
    }

    cout << "\nAttendance record not found!\n";
}

void searchAttendance(Attendance *attend, int count)
{
    int id, found = 0;
    cout << "\nEnter Student ID to search: ";
    cin >> id;

    for(int i = 0; i < count; i++)
    {
        if(attend[i].studentId == id)
        {
            found = 1;
            cout << "\nStudent ID: " << attend[i].studentId
                 << "\nTeacher ID: " << attend[i].teacherId
                 << "\nDate: " << attend[i].date
                 << "\nStatus: " << attend[i].status << "\n";
        }
    }

    if(!found) cout << "\nAttendance record not found!\n";
}

// ---------- MARKS FUNCTIONS ----------

void addMarks(MarkRecord *&marks, int &count,
              Student *stud, int studentCount)
{
    MarkRecord m;

    if(studentCount == 0)
    {
        cout << "\nAdd students first!\n";
        return;
    }

    cout << "\nEnter Student ID: ";
    cin >> m.studentId;
    if(findStudentById(stud, studentCount, m.studentId) == NULL)
    {
        cout << "\nStudent not found!\n";
        return;
    }

    cout << "Enter Subject (max 9 characters, no spaces): ";
    cin.width(10);
    cin >> m.subject;

    cout << "Enter Component (max 11 characters, no spaces): ";
    cin.width(12);
    cin >> m.component;

    cout << "Enter Marks (0-100): ";
    cin >> m.marks;
    if(cin.fail() || m.marks < 0 || m.marks > 100)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid marks!\n";
        return;
    }

    for(int i = 0; i < count; i++)
    {
        if(marks[i].studentId == m.studentId &&
           strcmp(marks[i].subject, m.subject) == 0 &&
           strcmp(marks[i].component, m.component) == 0)
        {
            cout << "\nThis marks record already exists!\n";
            return;
        }
    }

    MarkRecord *temp = new MarkRecord[count + 1];
    for(int i = 0; i < count; i++) temp[i] = marks[i];
    temp[count] = m;

    delete[] marks;
    marks = temp;
    count++;

    saveMarks(marks, count);
    cout << "\nMarks added successfully!\n";
}

void viewMarks(MarkRecord *marks, int count)
{
    if(count == 0)
    {
        cout << "\nNo marks records found!\n";
        return;
    }

    cout << "\n========== STUDENT MARKS ==========\n";
    for(int i = 0; i < count; i++)
    {
        cout << "\nStudent ID: " << marks[i].studentId
             << "\nSubject: " << marks[i].subject
             << "\nComponent: " << marks[i].component
             << "\nMarks: " << marks[i].marks << "\n";
    }
}

void updateMarks(MarkRecord *marks, int count)
{
    if(count == 0)
    {
        cout << "\nNo marks records available!\n";
        return;
    }

    int id, newMarks;
    char subject[10], component[12];

    cout << "\nEnter Student ID: ";
    cin >> id;
    cout << "Enter Subject: ";
    cin.width(10);
    cin >> subject;
    cout << "Enter Component: ";
    cin.width(12);
    cin >> component;

    for(int i = 0; i < count; i++)
    {
        if(marks[i].studentId == id &&
           strcmp(marks[i].subject, subject) == 0 &&
           strcmp(marks[i].component, component) == 0)
        {
            cout << "Current Marks: " << marks[i].marks;
            cout << "\nEnter New Marks (0-100): ";
            cin >> newMarks;

            if(cin.fail() || newMarks < 0 || newMarks > 100)
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "\nInvalid marks!\n";
                return;
            }

            marks[i].marks = newMarks;
            saveMarks(marks, count);
            cout << "\nMarks updated successfully!\n";
            return;
        }
    }

    cout << "\nMarks record not found!\n";
}

void searchMarks(MarkRecord *marks, int count)
{
    int id, found = 0;
    cout << "\nEnter Student ID: ";
    cin >> id;

    for(int i = 0; i < count; i++)
    {
        if(marks[i].studentId == id)
        {
            found = 1;
            cout << "\nStudent ID: " << marks[i].studentId
                 << "\nSubject: " << marks[i].subject
                 << "\nComponent: " << marks[i].component
                 << "\nMarks: " << marks[i].marks << "\n";
        }
    }

    if(!found) cout << "\nMarks record not found!\n";
}

// ---------- MAIN FUNCTION ----------

int main()
{
    if(!adminLogin())
        return 0;

    cleanStudents();
    cleanTeachers();
    cleanAttendance();
    cleanMarks();

    int studentCount = 0;
    int teacherCount = 0;
    int attendanceCount = 0;
    int marksCount = 0;

    Student *stud = loadStudents(studentCount);
    Teacher *teach = loadTeachers(teacherCount);
    Attendance *attend = loadAttendance(attendanceCount);
    MarkRecord *marks = loadMarks(marksCount);

    cout << "\nLoaded Students: " << studentCount;
    cout << "\nLoaded Teachers: " << teacherCount;
    cout << "\nLoaded Attendance: " << attendanceCount;
    cout << "\nLoaded Marks: " << marksCount << endl;

    int choice = 0;

    do
    {
        cout << "\n====================================";
        cout << "\n      UNIVERSITY MANAGEMENT SYSTEM";
        cout << "\n====================================";
        cout << "\n1. Student Management";
        cout << "\n2. Teacher Management";
        cout << "\n3. Attendance Management";
        cout << "\n4. Marks Management";
        cout << "\n5. Search Student";
        cout << "\n6. Search Teacher";
        cout << "\n7. Exit";
        cout << "\nEnter your choice: ";

        if(!(cin >> choice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nPlease enter a number.\n";
            continue;
        }

        if(choice == 1)
        {
            int sub = 0;
            do
            {
                cout << "\n--- STUDENT MANAGEMENT ---";
                cout << "\n1. Add Student";
                cout << "\n2. Update Student";
                cout << "\n3. Delete Student";
                cout << "\n4. View Students";
                cout << "\n5. Back";
                cout << "\nEnter choice: ";

                if(!(cin >> sub))
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\nInvalid input!\n";
                    continue;
                }

                if(sub == 1) addStudent(stud, studentCount);
                else if(sub == 2) updateStudent(stud, studentCount);
                else if(sub == 3)
                {
                    deleteStudent(stud, studentCount);
                    delete[] attend;
                    delete[] marks;
                    attend = loadAttendance(attendanceCount);
                    marks = loadMarks(marksCount);
                }
                else if(sub == 4) viewStudents(stud, studentCount);
                else if(sub == 5) cout << "\nReturning...\n";
                else cout << "\nInvalid choice!\n";

            } while(sub != 5);
        }
        else if(choice == 2)
        {
            int sub = 0;
            do
            {
                cout << "\n--- TEACHER MANAGEMENT ---";
                cout << "\n1. Add Teacher";
                cout << "\n2. Update Teacher";
                cout << "\n3. Delete Teacher";
                cout << "\n4. View Teachers";
                cout << "\n5. Back";
                cout << "\nEnter choice: ";

                if(!(cin >> sub))
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\nInvalid input!\n";
                    continue;
                }

                if(sub == 1) addTeacher(teach, teacherCount);
                else if(sub == 2) updateTeacher(teach, teacherCount);
                else if(sub == 3) deleteTeacher(teach, teacherCount);
                else if(sub == 4) viewTeachers(teach, teacherCount);
                else if(sub == 5) cout << "\nReturning...\n";
                else cout << "\nInvalid choice!\n";

            } while(sub != 5);
        }
        else if(choice == 3)
        {
            int sub = 0;
            do
            {
                cout << "\n--- ATTENDANCE MANAGEMENT ---";
                cout << "\n1. Add Attendance";
                cout << "\n2. View Attendance";
                cout << "\n3. Update Attendance";
                cout << "\n4. Search Attendance";
                cout << "\n5. Back";
                cout << "\nEnter choice: ";

                if(!(cin >> sub))
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\nInvalid input!\n";
                    continue;
                }

                if(sub == 1)
                    addAttendance(attend, attendanceCount, stud, studentCount,
                                  teach, teacherCount);
                else if(sub == 2) viewAttendance(attend, attendanceCount);
                else if(sub == 3) updateAttendance(attend, attendanceCount);
                else if(sub == 4) searchAttendance(attend, attendanceCount);
                else if(sub == 5) cout << "\nReturning...\n";
                else cout << "\nInvalid choice!\n";

            } while(sub != 5);
        }
        else if(choice == 4)
        {
            int sub = 0;
            do
            {
                cout << "\n--- MARKS MANAGEMENT ---";
                cout << "\n1. Add Marks";
                cout << "\n2. View Marks";
                cout << "\n3. Update Marks";
                cout << "\n4. Search Marks";
                cout << "\n5. Back";
                cout << "\nEnter choice: ";

                if(!(cin >> sub))
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\nInvalid input!\n";
                    continue;
                }

                if(sub == 1) addMarks(marks, marksCount, stud, studentCount);
                else if(sub == 2) viewMarks(marks, marksCount);
                else if(sub == 3) updateMarks(marks, marksCount);
                else if(sub == 4) searchMarks(marks, marksCount);
                else if(sub == 5) cout << "\nReturning...\n";
                else cout << "\nInvalid choice!\n";

            } while(sub != 5);
        }
        else if(choice == 5)
        {
            int id;
            cout << "\nEnter Student ID: ";
            cin >> id;
            Student *found = findStudentById(stud, studentCount, id);

            if(found != NULL)
            {
                cout << "\nStudent Found!";
                cout << "\nID: " << found->studentId;
                cout << "\nName: " << found->name;
                cout << "\nAge: " << found->age;
                cout << "\nGender: " << found->gender;
                cout << "\nContact: " << found->contact << endl;
            }
            else cout << "\nStudent not found!\n";
        }
        else if(choice == 6)
        {
            int id;
            cout << "\nEnter Teacher ID: ";
            cin >> id;
            Teacher *found = findTeacherById(teach, teacherCount, id);

            if(found != NULL)
            {
                cout << "\nTeacher Found!";
                cout << "\nID: " << found->teacherId;
                cout << "\nName: " << found->name;
                cout << "\nDepartment: " << found->department;
                cout << "\nExperience: " << found->experience;
                cout << "\nCourse: " << found->course << endl;
            }
            else cout << "\nTeacher not found!\n";
        }
        else if(choice == 7)
        {
            cout << "\nThank you for using University Management System!\n";
        }
        else
        {
            cout << "\nInvalid choice!\n";
        }

    } while(choice != 7);

    delete[] stud;
    delete[] teach;
    delete[] attend;
    delete[] marks;

    return 0;
}
