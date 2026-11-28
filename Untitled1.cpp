#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>

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

bool adminLogin();
void cleanStudents();
void cleanTeachers();
void cleanAttendance();
void cleanMarks();

Student* loadStudents(int &count);
Teacher* loadTeachers(int &count);

void saveStudents(Student *stud, int count);
void saveTeachers(Teacher *teach, int count);

Student* findStudentById(Student *stud, int count, int id);

void addStudent(Student *&stud, int &count);
void updateStudent(Student *stud, int count);
void deleteStudent(Student *&stud, int &count);
void viewStudents(Student *stud, int count);

void addTeacher(Teacher *&teach, int &count);
void updateTeacher(Teacher *teach, int count);
void deleteTeacher(Teacher *&teach, int &count);
void viewTeachers(Teacher *teach, int count);

void removeRecordsOfStudent(int id);

void markAttendance(Student *stud, int studentCount, Teacher *teach, int teacherCount);
void viewAttendance();
int attendancePercent(int studentId, int teacherId);
void viewAttendanceSummary(Student *stud, int count);

void viewMarks(Student *stud, int count);
void updateMarks(Student *stud, int studentCount, Teacher *teach, int teacherCount);
void deleteMarks();
void quizStatsBySubject();

void searchStudentByIdOrName(Student *stud, int count);
void searchTeacherByDepartment(Teacher *teach, int count);
void sortTeachersByExperience(Teacher *teach, int count);
void topThreeStudents(Student *stud, int studCount);

int attendancePercent(int studentId, int teacherId)
{
    ifstream input("attendance.txt");
    if (!input) return 0;

    char first[100];
    if (input.peek() == 'S')
        input.getline(first, 100);

    Attendance a;
    int total = 0, present = 0;

    while (input >> a.studentId)
    {
        input.ignore();
        input >> a.teacherId;
        input.ignore();
        input.getline(a.date, 12, ',');
        input.getline(a.status, 8);

        if (a.studentId == studentId && a.teacherId == teacherId)
        {
            total++;
            if (strcmp(a.status, "Present") == 0)
                present++;
        }
    }

    input.close();
    if (total == 0) return 0;
    return (present * 100) / total;
}

void viewAttendanceSummary(Student *stud, int count)
{
    if (count == 0)
    {
        cout << "\nNo students available!\n";
        return;
    }

    int studentId;
    cout << "\nEnter Student ID for attendance summary: ";
    cin >> studentId;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid ID!\n";
        return;
    }

    Student *s = findStudentById(stud, count, studentId);
    if (s == NULL)
    {
        cout << "\nStudent not found!\n";
        return;
    }

    ifstream input("attendance.txt");
    if (!input)
    {
        cout << "\nAttendance file not found!\n";
        return;
    }

    char first[100];
    if (input.peek() == 'S')
        input.getline(first, 100);

    Attendance a;
    int total = 0, present = 0;

    while (input >> a.studentId)
    {
        input.ignore();
        input >> a.teacherId;
        input.ignore();
        input.getline(a.date, 12, ',');
        input.getline(a.status, 8);

        if (a.studentId == studentId)
        {
            total++;
            if (strcmp(a.status, "Present") == 0)
                present++;
        }
    }

    input.close();

    int percent = (total == 0) ? 0 : (present * 100) / total;

    cout << "\n========== ATTENDANCE SUMMARY ==========\n";
    cout << "Student ID: " << s->studentId << "\n";
    cout << "Name: " << s->name << "\n";
    cout << "Present Classes: " << present << "\n";
    cout << "Total Classes: " << total << "\n";
    cout << "Attendance Percentage: " << percent << "%\n";
    cout << "Status: " << (percent < 75 ? "Short attendance" : "Regular") << "\n";
}

void markAttendance(Student *stud, int studentCount, Teacher *teach, int teacherCount)
{
    Attendance a;
    bool good;

    cout << "\nEnter Student ID: ";
    cin >> a.studentId;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid ID!\n";
        return;
    }
    if (findStudentById(stud, studentCount, a.studentId) == NULL)
    {
        cout << "\nStudent ID not found!\n";
        return;
    }

    cout << "Enter Teacher ID: ";
    cin >> a.teacherId;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid ID!\n";
        return;
    }

    good = false;
    for (int i = 0; i < teacherCount; i++)
    {
        if (teach[i].teacherId == a.teacherId)
            good = true;
    }
    if (!good)
    {
        cout << "\nTeacher ID not found!\n";
        return;
    }

    cin.ignore(1000, '\n');
    do
    {
        cout << "Enter Date (YYYY-MM-DD): ";
        cin.getline(a.date, 12);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            a.date[0] = '\0';
        }

        good = (strlen(a.date) == 10);
        for (int k = 0; good && k < 10; k++)
        {
            if (k == 4 || k == 7)
            {
                if (a.date[k] != '-')
                    good = false;
            }
            else if (a.date[k] < '0' || a.date[k] > '9')
                good = false;
        }
        if (good)
        {
            int year = (a.date[0] - '0') * 1000 + (a.date[1] - '0') * 100 + (a.date[2] - '0') * 10 + (a.date[3] - '0');
            int month = (a.date[5] - '0') * 10 + (a.date[6] - '0');
            int day = (a.date[8] - '0') * 10 + (a.date[9] - '0');
            int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
                days[1] = 29;
            if (year < 1900 || month < 1 || month > 12 || day < 1 || day > days[month - 1])
                good = false;
        }
        if (!good)
            cout << "Invalid date.\n";
    } while (!good);

    bool existed = true;
    ifstream check("attendance.txt");
    if (!check)
        existed = false;
    else
    {
        char first[100];
        if (check.peek() == 'S')
            check.getline(first, 100);

        Attendance b;
        while (check >> b.studentId)
        {
            check.ignore();
            check >> b.teacherId;
            check.ignore();
            check.getline(b.date, 12, ',');
            check.getline(b.status, 8);

            if (b.studentId == a.studentId && b.teacherId == a.teacherId && strcmp(b.date, a.date) == 0)
            {
                cout << "\nAttendance already recorded for this class!\n";
                check.close();
                return;
            }
        }
        check.close();
    }

    do
    {
        cout << "Enter Status (Present/Absent): ";
        cin.getline(a.status, 8);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            a.status[0] = '\0';
        }
        good = (strcmp(a.status, "Present") == 0 || strcmp(a.status, "Absent") == 0);
        if (!good)
            cout << "Enter Present or Absent.\n";
    } while (!good);

    ofstream output("attendance.txt", ios::app);
    if (!existed)
        output << "StudentID,TeacherID,Date,Status\n";
    output << a.studentId << "," << a.teacherId << "," << a.date << "," << a.status << endl;
    output.close();

    cout << "\nAttendance recorded successfully!\n";
}

void viewAttendance()
{
    ifstream input("attendance.txt");
    if (!input)
    {
        cout << "\nAttendance file not found!\n";
        return;
    }

    char first[100];
    if (input.peek() == 'S')
        input.getline(first, 100);

    Attendance a;
    int total = 0;

    cout << "\n========== ATTENDANCE ==========\n";
    while (input >> a.studentId)
    {
        input.ignore();
        input >> a.teacherId;
        input.ignore();
        input.getline(a.date, 12, ',');
        input.getline(a.status, 8);

        cout << "Student ID: " << a.studentId
             << " | Teacher ID: " << a.teacherId
             << " | Date: " << a.date
             << " | Status: " << a.status << "\n";
        total++;
    }

    input.close();
    if (total == 0)
        cout << "No attendance records available!\n";
}

void viewMarks(Student *stud, int count)
{
    if (count == 0)
    {
        cout << "\nNo students available!\n";
        return;
    }

    int id;
    cout << "\nEnter Student ID for marks sheet: ";
    cin >> id;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid ID!\n";
        return;
    }

    Student *s = findStudentById(stud, count, id);
    if (s == NULL)
    {
        cout << "\nStudent not found!\n";
        return;
    }

    ifstream input("marks.txt");
    if (!input)
    {
        cout << "\nMarks file not found!\n";
        return;
    }

    char first[100];
    if (input.peek() == 'S')
        input.getline(first, 100);

    MarkRecord m;
    int total = 0, records = 0;

    cout << "\n========== MARKS SHEET ==========\n";
    cout << "Student ID: " << s->studentId << " | Name: " << s->name << "\n";
    while (input >> m.studentId)
    {
        input.ignore();
        input.getline(m.subject, 10, ',');
        input.getline(m.component, 12, ',');
        input >> m.marks;
        input.ignore(1000, '\n');

        if (m.studentId == id)
        {
            cout << "Subject: " << m.subject
                 << " | Component: " << m.component
                 << " | Marks: " << m.marks << "\n";
            total += m.marks;
            records++;
        }
    }

    input.close();
    if (records == 0)
        cout << "No marks found for this student!\n";
    else
        cout << "Total Marks: " << total << "\n";
}

void updateMarks(Student *stud, int studentCount, Teacher *teach, int teacherCount)
{
    int id;
    char subject[10];

    cout << "\nEnter Student ID: ";
    cin >> id;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid ID!\n";
        return;
    }
    if (findStudentById(stud, studentCount, id) == NULL)
    {
        cout << "\nStudent not found!\n";
        return;
    }

    cout << "Enter Subject: ";
    cin.width(10);
    cin >> subject;
    cin.ignore(1000, '\n');

    bool good = false;
    for (int i = 0; i < teacherCount; i++)
    {
        if (strcmp(teach[i].course, subject) == 0)
            good = true;
    }
    if (!good)
    {
        cout << "\nSubject does not match any teacher's course!\n";
        return;
    }

    ifstream input("marks.txt");
    if (!input)
    {
        cout << "\nMarks file not found!\n";
        return;
    }

    char first[100];
    if (input.peek() == 'S')
        input.getline(first, 100);

    MarkRecord m;
    bool found = false;
    while (input >> m.studentId)
    {
        input.ignore();
        input.getline(m.subject, 10, ',');
        input.getline(m.component, 12, ',');
        input >> m.marks;
        input.ignore(1000, '\n');

        if (m.studentId == id && strcmp(m.subject, subject) == 0 && strcmp(m.component, "Quiz1") == 0)
            found = true;
    }
    input.close();

    if (!found)
    {
        cout << "\nQuiz1 marks record not found for this student and subject!\n";
        return;
    }

    int newMarks;
    cout << "Enter New Quiz1 Marks (0-100): ";
    cin >> newMarks;
    while (cin.fail() || newMarks < 0 || newMarks > 100)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Enter marks between 0 and 100: ";
        cin >> newMarks;
    }

    ifstream again("marks.txt");
    ofstream output("temp_marks.txt");
    output << "StudentID,Subject,Component,Marks\n";

    if (again.peek() == 'S')
        again.getline(first, 100);

    while (again >> m.studentId)
    {
        again.ignore();
        again.getline(m.subject, 10, ',');
        again.getline(m.component, 12, ',');
        again >> m.marks;
        again.ignore(1000, '\n');

        if (m.studentId == id && strcmp(m.subject, subject) == 0 && strcmp(m.component, "Quiz1") == 0)
            m.marks = newMarks;

        output << m.studentId << "," << m.subject << ","
               << m.component << "," << m.marks << endl;
    }

    again.close();
    output.close();
    remove("marks.txt");
    rename("temp_marks.txt", "marks.txt");
    cout << "\nMarks updated!\n";
}

void deleteMarks()
{
    int id;
    char subject[10], component[12];

    cout << "\nEnter Student ID: ";
    cin >> id;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid ID!\n";
        return;
    }

    cout << "Enter Subject: ";
    cin.width(10);
    cin >> subject;
    cin.ignore(1000, '\n');

    cout << "Enter Component (e.g., Quiz1): ";
    cin.width(12);
    cin >> component;
    cin.ignore(1000, '\n');

    ifstream input("marks.txt");
    if (!input)
    {
        cout << "\nMarks file not found!\n";
        return;
    }

    ofstream output("temp_marks.txt");
    output << "StudentID,Subject,Component,Marks\n";

    char first[100];
    if (input.peek() == 'S')
        input.getline(first, 100);

    MarkRecord m;
    bool found = false;
    while (input >> m.studentId)
    {
        input.ignore();
        input.getline(m.subject, 10, ',');
        input.getline(m.component, 12, ',');
        input >> m.marks;
        input.ignore(1000, '\n');

        if (m.studentId == id && strcmp(m.subject, subject) == 0 && strcmp(m.component, component) == 0)
            found = true;
        else
            output << m.studentId << "," << m.subject << ","
                   << m.component << "," << m.marks << endl;
    }

    input.close();
    output.close();

    if (!found)
    {
        remove("temp_marks.txt");
        cout << "\nMarks record not found!\n";
        return;
    }

    remove("marks.txt");
    rename("temp_marks.txt", "marks.txt");
    cout << "\nMarks record deleted successfully!\n";
}

void quizStatsBySubject()
{
    char subject[10], component[12];
    cout << "\nEnter Subject (e.g., PF, ICT, CAL): ";
    cin.width(10);
    cin >> subject;
    cin.ignore(1000, '\n');

    cout << "Enter Quiz Component (e.g., Quiz1): ";
    cin.width(12);
    cin >> component;
    cin.ignore(1000, '\n');

    ifstream input("marks.txt");
    if (!input)
    {
        cout << "\nMarks file not found!\n";
        return;
    }

    char first[100];
    if (input.peek() == 'S')
        input.getline(first, 100);

    MarkRecord board[100];
    int boardCount = 0;
    int matched = 0;

    MarkRecord m;
    while (input >> m.studentId)
    {
        input.ignore();
        input.getline(m.subject, 10, ',');
        input.getline(m.component, 12, ',');
        input >> m.marks;
        input.ignore(1000, '\n');

        if (strcmp(m.subject, subject) == 0 && strcmp(m.component, component) == 0)
        {
            matched++;
            if (boardCount < 100)
                board[boardCount++] = m;
        }
    }
    input.close();

    if (boardCount == 0)
    {
        cout << "\nNo records found for " << subject << " - " << component << "!\n";
        return;
    }

    int sum = 0;
    int highest = board[0].marks;
    int lowest = board[0].marks;

    for (int i = 0; i < boardCount; i++)
    {
        sum += board[i].marks;
        if (board[i].marks > highest) highest = board[i].marks;
        if (board[i].marks < lowest) lowest = board[i].marks;
    }

    float average = (float)sum / boardCount;

    cout << "\n========== " << subject << " (" << component << ") STATS ==========\n";
    cout << "Records Evaluated: " << boardCount << "\n";
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << "Average Marks: " << average << "\n";
    cout.unsetf(ios::fixed);
    cout << "Highest Marks: " << highest << "\n";
    cout << "Lowest Marks: " << lowest << "\n";

    if (matched > boardCount)
        cout << "Note: only the first " << boardCount << " of " << matched << " records were evaluated.\n";
}

void searchStudentByIdOrName(Student *stud, int count)
{
    if (count == 0)
    {
        cout << "\nNo students available!\n";
        return;
    }

    int option;
    cout << "\nSearch Student by:\n1. ID\n2. Exact Name\nEnter choice: ";
    cin >> option;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid choice!\n";
        return;
    }

    if (option == 1)
    {
        int id;
        cout << "Enter Student ID: ";
        cin >> id;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid ID!\n";
            return;
        }

        Student *found = findStudentById(stud, count, id);
        if (found != NULL)
        {
            cout << "\nStudent Found!\n";
            cout << "ID: " << found->studentId
                 << "\nName: " << found->name
                 << "\nAge: " << found->age
                 << "\nGender: " << found->gender
                 << "\nContact: " << found->contact << "\n";
        }
        else
            cout << "\nStudent not found!\n";
    }
    else if (option == 2)
    {
        char targetName[40];
        cin.ignore(1000, '\n');
        cout << "Enter Exact Student Name: ";
        cin.getline(targetName, 40);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            targetName[0] = '\0';
        }

        bool foundAny = false;
        for (int i = 0; i < count; i++)
        {
            Student *ptr = stud + i;
            if (strcmp(ptr->name, targetName) == 0)
            {
                foundAny = true;
                cout << "\nStudent Found!\n";
                cout << "ID: " << ptr->studentId
                     << "\nName: " << ptr->name
                     << "\nAge: " << ptr->age
                     << "\nGender: " << ptr->gender
                     << "\nContact: " << ptr->contact << "\n";
            }
        }

        if (!foundAny)
            cout << "\nNo student with that exact name found!\n";
    }
    else
        cout << "\nInvalid search option!\n";
}

void searchTeacherByDepartment(Teacher *teach, int count)
{
    if (count == 0)
    {
        cout << "\nNo teachers available!\n";
        return;
    }

    char dept[30];
    cin.ignore(1000, '\n');
    cout << "\nEnter Department Name: ";
    cin.getline(dept, 30);
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        dept[0] = '\0';
    }

    bool foundAny = false;
    for (int i = 0; i < count; i++)
    {
        Teacher *ptr = teach + i;
        if (strcmp(ptr->department, dept) == 0)
        {
            foundAny = true;
            cout << "\nTeacher ID: " << ptr->teacherId
                 << "\nName: " << ptr->name
                 << "\nDepartment: " << ptr->department
                 << "\nExperience: " << ptr->experience
                 << "\nCourse: " << ptr->course << "\n";
        }
    }

    if (!foundAny)
        cout << "\nNo teachers found in this department!\n";
}

void sortTeachersByExperience(Teacher *teach, int count)
{
    if (count <= 1)
    {
        cout << "\nNot enough teachers to sort.\n";
        return;
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            Teacher *a = teach + j;
            Teacher *b = teach + j + 1;
            if (a->experience < b->experience)
            {
                Teacher temp = *a;
                *a = *b;
                *b = temp;
            }
        }
    }

    cout << "\nTeachers sorted by experience (highest first)!\n";
}

void topThreeStudents(Student *stud, int studCount)
{
    if (studCount == 0 || stud == NULL)
    {
        cout << "\nNo students available!\n";
        return;
    }

    ifstream input("marks.txt");
    if (!input)
    {
        cout << "\nMarks file not found!\n";
        return;
    }

    int *totals = new int[studCount];
    int *order = new int[studCount];

    for (int i = 0; i < studCount; i++)
    {
        totals[i] = 0;
        order[i] = i;
    }

    char first[100];
    if (input.peek() == 'S')
        input.getline(first, 100);

    MarkRecord m;
    while (input >> m.studentId)
    {
        input.ignore();
        input.getline(m.subject, 10, ',');
        input.getline(m.component, 12, ',');
        input >> m.marks;
        input.ignore(1000, '\n');

        for (int j = 0; j < studCount; j++)
        {
            if (stud[j].studentId == m.studentId)
            {
                totals[j] += m.marks;
                break;
            }
        }
    }
    input.close();

    for (int i = 0; i < studCount - 1; i++)
    {
        for (int j = 0; j < studCount - i - 1; j++)
        {
            if (totals[order[j]] < totals[order[j + 1]])
            {
                int tempIdx = order[j];
                order[j] = order[j + 1];
                order[j + 1] = tempIdx;
            }
        }
    }

    cout << "\n========== TOP 3 STUDENTS (CUMULATIVE MARKS) ==========\n";
    int limit = (studCount < 3) ? studCount : 3;

    for (int rank = 0; rank < limit; rank++)
    {
        int idx = order[rank];
        cout << (rank + 1) << ". ID: " << stud[idx].studentId
             << " | Name: " << stud[idx].name
             << " | Total Marks: " << totals[idx];
        if (rank == 0)
            cout << "  <-- LEADING";
        cout << "\n";
    }

    delete[] totals;
    delete[] order;
}

Student* loadStudents(int &count)
{
    count = 0;
    ifstream input("students.txt");
    if (!input) return NULL;

    Student s;
    while (input >> s.studentId)
    {
        input.ignore();
        input.getline(s.name, 40, ',');
        input >> s.age;
        input.ignore();
        input.getline(s.gender, 8, ',');
        input.getline(s.contact, 12);
        count++;
    }

    input.clear();
    input.seekg(0);

    if (count == 0) return NULL;

    Student *data = new Student[count];
    int i = 0;
    while (i < count && (input >> data[i].studentId))
    {
        input.ignore();
        input.getline(data[i].name, 40, ',');
        input >> data[i].age;
        input.ignore();
        input.getline(data[i].gender, 8, ',');
        input.getline(data[i].contact, 12);
        i++;
    }

    input.close();
    count = i;
    return data;
}

Teacher* loadTeachers(int &count)
{
    count = 0;
    ifstream input("teachers.txt");
    if (!input) return NULL;

    Teacher t;
    while (input >> t.teacherId)
    {
        input.ignore();
        input.getline(t.name, 40, ',');
        input.getline(t.department, 30, ',');
        input >> t.experience;
        input.ignore();
        input.getline(t.course, 30);
        count++;
    }

    input.clear();
    input.seekg(0);

    if (count == 0) return NULL;

    Teacher *data = new Teacher[count];
    int i = 0;
    while (i < count && (input >> data[i].teacherId))
    {
        input.ignore();
        input.getline(data[i].name, 40, ',');
        input.getline(data[i].department, 30, ',');
        input >> data[i].experience;
        input.ignore();
        input.getline(data[i].course, 30);
        i++;
    }

    input.close();
    count = i;
    return data;
}

void saveStudents(Student *stud, int count)
{
    ofstream output("students.txt");
    for (int i = 0; i < count; i++)
        output << stud[i].studentId << "," << stud[i].name << ","
               << stud[i].age << "," << stud[i].gender << ","
               << stud[i].contact << endl;
}

void saveTeachers(Teacher *teach, int count)
{
    ofstream output("teachers.txt");
    for (int i = 0; i < count; i++)
        output << teach[i].teacherId << "," << teach[i].name << ","
               << teach[i].department << "," << teach[i].experience
               << "," << teach[i].course << endl;
}

Student* findStudentById(Student *stud, int count, int id)
{
    for (int i = 0; i < count; i++)
        if (stud[i].studentId == id) return &stud[i];
    return NULL;
}

void addStudent(Student *&stud, int &count)
{
    Student s;
    bool good;

    cout << "\nEnter Student ID: ";
    cin >> s.studentId;

    while (cin.fail() || s.studentId <= 0 ||
           findStudentById(stud, count, s.studentId) != NULL)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Enter a new positive and unique ID: ";
        cin >> s.studentId;
    }

    cin.ignore(1000, '\n');
    do
    {
        cout << "Enter Student Name: ";
        cin.getline(s.name, 40);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            s.name[0] = '\0';
        }
        good = (strlen(s.name) > 0 && s.name[0] != ' ');
        for (int k = 0; good && s.name[k] != '\0'; k++)
        {
            if (!((s.name[k] >= 'A' && s.name[k] <= 'Z') ||
                  (s.name[k] >= 'a' && s.name[k] <= 'z') || s.name[k] == ' '))
                good = false;
        }
        if (!good)
            cout << "Name must contain letters and spaces only.\n";
    } while (!good);

    cout << "Enter Age (16-40): ";
    cin >> s.age;
    while (cin.fail() || s.age < 16 || s.age > 40)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Enter age between 16 and 40: ";
        cin >> s.age;
    }

    cin.ignore(1000, '\n');
    do
    {
        cout << "Enter Gender (Male/Female): ";
        cin.getline(s.gender, 8);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            s.gender[0] = '\0';
        }
        good = (strcmp(s.gender, "Male") == 0 || strcmp(s.gender, "Female") == 0);
        if (!good)
            cout << "Enter Male or Female.\n";
    } while (!good);

    do
    {
        cout << "Enter Contact (11 digits, starts with 03): ";
        cin.getline(s.contact, 12);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            s.contact[0] = '\0';
        }
        good = (strlen(s.contact) == 11 && s.contact[0] == '0' && s.contact[1] == '3');
        for (int k = 0; good && k < 11; k++)
        {
            if (s.contact[k] < '0' || s.contact[k] > '9')
                good = false;
        }
        if (!good)
            cout << "Invalid contact.\n";
    } while (!good);

    Student *temp = new Student[count + 1];
    for (int i = 0; i < count; i++) temp[i] = stud[i];
    temp[count] = s;

    delete[] stud;
    stud = temp;
    count++;

    saveStudents(stud, count);
    cout << "\nStudent added successfully!\n";
}

void updateStudent(Student *stud, int count)
{
    if (count == 0)
    {
        cout << "\nNo students available!\n";
        return;
    }

    int id;
    bool good;
    cout << "\nEnter Student ID to update: ";
    cin >> id;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid ID!\n";
        return;
    }

    Student *s = findStudentById(stud, count, id);
    if (s == NULL)
    {
        cout << "\nStudent not found!\n";
        return;
    }

    cin.ignore(1000, '\n');
    do
    {
        cout << "Enter New Name: ";
        cin.getline(s->name, 40);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            s->name[0] = '\0';
        }
        good = (strlen(s->name) > 0 && s->name[0] != ' ');
        for (int k = 0; good && s->name[k] != '\0'; k++)
        {
            if (!((s->name[k] >= 'A' && s->name[k] <= 'Z') ||
                  (s->name[k] >= 'a' && s->name[k] <= 'z') || s->name[k] == ' '))
                good = false;
        }
        if (!good)
            cout << "Name must contain letters and spaces only.\n";
    } while (!good);

    cout << "Enter New Age (16-40): ";
    cin >> s->age;
    while (cin.fail() || s->age < 16 || s->age > 40)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Enter age between 16 and 40: ";
        cin >> s->age;
    }

    cin.ignore(1000, '\n');
    do
    {
        cout << "Enter New Gender (Male/Female): ";
        cin.getline(s->gender, 8);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            s->gender[0] = '\0';
        }
        good = (strcmp(s->gender, "Male") == 0 || strcmp(s->gender, "Female") == 0);
        if (!good)
            cout << "Enter Male or Female.\n";
    } while (!good);

    do
    {
        cout << "Enter New Contact (11 digits, starts with 03): ";
        cin.getline(s->contact, 12);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            s->contact[0] = '\0';
        }
        good = (strlen(s->contact) == 11 && s->contact[0] == '0' && s->contact[1] == '3');
        for (int k = 0; good && k < 11; k++)
        {
            if (s->contact[k] < '0' || s->contact[k] > '9')
                good = false;
        }
        if (!good)
            cout << "Invalid contact.\n";
    } while (!good);

    saveStudents(stud, count);
    cout << "\nStudent updated successfully!\n";
}

void removeRecordsOfStudent(int id)
{
    ifstream inA("attendance.txt");
    if (inA)
    {
        ofstream outA("temp_attendance.txt");
        outA << "StudentID,TeacherID,Date,Status\n";

        char first[100];
        if (inA.peek() == 'S')
            inA.getline(first, 100);

        Attendance a;
        while (inA >> a.studentId)
        {
            inA.ignore();
            inA >> a.teacherId;
            inA.ignore();
            inA.getline(a.date, 12, ',');
            inA.getline(a.status, 8);

            if (a.studentId != id)
                outA << a.studentId << "," << a.teacherId << ","
                     << a.date << "," << a.status << endl;
        }

        inA.close();
        outA.close();
        remove("attendance.txt");
        rename("temp_attendance.txt", "attendance.txt");
    }

    ifstream inM("marks.txt");
    if (inM)
    {
        ofstream outM("temp_marks.txt");
        outM << "StudentID,Subject,Component,Marks\n";

        char first[100];
        if (inM.peek() == 'S')
            inM.getline(first, 100);

        MarkRecord m;
        while (inM >> m.studentId)
        {
            inM.ignore();
            inM.getline(m.subject, 10, ',');
            inM.getline(m.component, 12, ',');
            inM >> m.marks;
            inM.ignore(1000, '\n');

            if (m.studentId != id)
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
    if (count == 0 || stud == NULL)
    {
        cout << "\nNo students available!\n";
        return;
    }

    int id;
    cout << "\nEnter Student ID to delete: ";
    cin >> id;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid ID!\n";
        return;
    }

    int pos = -1;
    for (int i = 0; i < count; i++)
    {
        if (stud[i].studentId == id)
        {
            pos = i;
            break;
        }
    }

    if (pos == -1)
    {
        cout << "\nStudent not found!\n";
        return;
    }

    Student *temp = NULL;
    if (count > 1)
    {
        temp = new Student[count - 1];
        int j = 0;
        for (int i = 0; i < count; i++)
            if (i != pos) temp[j++] = stud[i];
    }

    delete[] stud;
    stud = temp;
    count--;

    saveStudents(stud, count);
    removeRecordsOfStudent(id);
    cout << "\nStudent and related records deleted successfully!\n";
}

void viewStudents(Student *stud, int count)
{
    if (count == 0 || stud == NULL)
    {
        cout << "\nNo students available!\n";
        return;
    }

    cout << "\n========== STUDENTS ==========\n";
    for (int i = 0; i < count; i++)
    {
        cout << "ID: " << stud[i].studentId
             << " | Name: " << stud[i].name
             << " | Age: " << stud[i].age
             << " | Gender: " << stud[i].gender
             << " | Contact: " << stud[i].contact << "\n";
    }
}

void addTeacher(Teacher *&teach, int &count)
{
    Teacher t;
    bool good;

    cout << "\nEnter Teacher ID: ";
    cin >> t.teacherId;
    good = false;
    while (!good)
    {
        good = !cin.fail() && t.teacherId > 0;
        for (int i = 0; good && i < count; i++)
        {
            if (teach[i].teacherId == t.teacherId)
                good = false;
        }
        if (!good)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Enter a positive and unique Teacher ID: ";
            cin >> t.teacherId;
        }
    }

    cin.ignore(1000, '\n');
    do
    {
        cout << "Enter Teacher Name: ";
        cin.getline(t.name, 40);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            t.name[0] = '\0';
        }
        good = (strlen(t.name) > 0 && t.name[0] != ' ');
        for (int k = 0; good && t.name[k] != '\0'; k++)
        {
            if (!((t.name[k] >= 'A' && t.name[k] <= 'Z') ||
                  (t.name[k] >= 'a' && t.name[k] <= 'z') || t.name[k] == ' '))
                good = false;
        }
        if (!good)
            cout << "Name must contain letters and spaces only.\n";
    } while (!good);

    do
    {
        cout << "Enter Department: ";
        cin.getline(t.department, 30);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            t.department[0] = '\0';
        }
        good = (strlen(t.department) > 0 && t.department[0] != ' ');
        for (int k = 0; good && t.department[k] != '\0'; k++)
        {
            if (!((t.department[k] >= 'A' && t.department[k] <= 'Z') ||
                  (t.department[k] >= 'a' && t.department[k] <= 'z') || t.department[k] == ' '))
                good = false;
        }
        if (!good)
            cout << "Department must contain letters and spaces only.\n";
    } while (!good);

    cout << "Enter Experience (years, 0-45): ";
    cin >> t.experience;
    while (cin.fail() || t.experience < 0 || t.experience > 45)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Enter experience between 0 and 45: ";
        cin >> t.experience;
    }

    cin.ignore(1000, '\n');
    do
    {
        cout << "Enter Course (e.g., PF): ";
        cin.getline(t.course, 30);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            t.course[0] = '\0';
        }
        good = (strlen(t.course) > 0 && strlen(t.course) <= 9);
        for (int k = 0; good && t.course[k] != '\0'; k++)
        {
            if (!((t.course[k] >= 'A' && t.course[k] <= 'Z') ||
                  (t.course[k] >= 'a' && t.course[k] <= 'z') ||
                  (t.course[k] >= '0' && t.course[k] <= '9')))
                good = false;
        }
        if (!good)
            cout << "Course must be 1-9 letters/digits.\n";
    } while (!good);

    Teacher *temp = new Teacher[count + 1];
    for (int i = 0; i < count; i++) temp[i] = teach[i];
    temp[count] = t;

    delete[] teach;
    teach = temp;
    count++;

    saveTeachers(teach, count);
    cout << "\nTeacher added successfully!\n";
}

void updateTeacher(Teacher *teach, int count)
{
    if (count == 0 || teach == NULL)
    {
        cout << "\nNo teachers available!\n";
        return;
    }

    int id;
    bool good;
    cout << "\nEnter Teacher ID to update: ";
    cin >> id;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid ID!\n";
        return;
    }

    Teacher *t = NULL;
    for (int i = 0; i < count; i++)
    {
        if (teach[i].teacherId == id)
        {
            t = teach + i;
            break;
        }
    }
    if (t == NULL)
    {
        cout << "\nTeacher not found!\n";
        return;
    }

    cin.ignore(1000, '\n');
    do
    {
        cout << "Enter New Name: ";
        cin.getline(t->name, 40);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            t->name[0] = '\0';
        }
        good = (strlen(t->name) > 0 && t->name[0] != ' ');
        for (int k = 0; good && t->name[k] != '\0'; k++)
        {
            if (!((t->name[k] >= 'A' && t->name[k] <= 'Z') ||
                  (t->name[k] >= 'a' && t->name[k] <= 'z') || t->name[k] == ' '))
                good = false;
        }
        if (!good)
            cout << "Name must contain letters and spaces only.\n";
    } while (!good);

    do
    {
        cout << "Enter New Department: ";
        cin.getline(t->department, 30);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            t->department[0] = '\0';
        }
        good = (strlen(t->department) > 0 && t->department[0] != ' ');
        for (int k = 0; good && t->department[k] != '\0'; k++)
        {
            if (!((t->department[k] >= 'A' && t->department[k] <= 'Z') ||
                  (t->department[k] >= 'a' && t->department[k] <= 'z') || t->department[k] == ' '))
                good = false;
        }
        if (!good)
            cout << "Department must contain letters and spaces only.\n";
    } while (!good);

    cout << "Enter New Experience (0-45): ";
    cin >> t->experience;
    while (cin.fail() || t->experience < 0 || t->experience > 45)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Enter experience between 0 and 45: ";
        cin >> t->experience;
    }

    cin.ignore(1000, '\n');
    do
    {
        cout << "Enter New Course: ";
        cin.getline(t->course, 30);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            t->course[0] = '\0';
        }
        good = (strlen(t->course) > 0 && strlen(t->course) <= 9);
        for (int k = 0; good && t->course[k] != '\0'; k++)
        {
            if (!((t->course[k] >= 'A' && t->course[k] <= 'Z') ||
                  (t->course[k] >= 'a' && t->course[k] <= 'z') ||
                  (t->course[k] >= '0' && t->course[k] <= '9')))
                good = false;
        }
        if (!good)
            cout << "Course must be 1-9 letters/digits.\n";
    } while (!good);

    saveTeachers(teach, count);
    cout << "\nTeacher updated successfully!\n";
}

void deleteTeacher(Teacher *&teach, int &count)
{
    if (count == 0 || teach == NULL)
    {
        cout << "\nNo teachers available!\n";
        return;
    }

    int id;
    cout << "\nEnter Teacher ID to delete: ";
    cin >> id;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nInvalid ID!\n";
        return;
    }

    int pos = -1;
    for (int i = 0; i < count; i++)
    {
        if (teach[i].teacherId == id)
        {
            pos = i;
            break;
        }
    }

    if (pos == -1)
    {
        cout << "\nTeacher not found!\n";
        return;
    }

    Teacher *temp = NULL;
    if (count > 1)
    {
        temp = new Teacher[count - 1];
        int j = 0;
        for (int i = 0; i < count; i++)
            if (i != pos) temp[j++] = teach[i];
    }

    delete[] teach;
    teach = temp;
    count--;

    saveTeachers(teach, count);
    cout << "\nTeacher deleted successfully!\n";
}

void viewTeachers(Teacher *teach, int count)
{
    if (count == 0 || teach == NULL)
    {
        cout << "\nNo teachers available!\n";
        return;
    }

    cout << "\n========== TEACHERS ==========\n";
    for (int i = 0; i < count; i++)
    {
        cout << "ID: " << teach[i].teacherId
             << " | Name: " << teach[i].name
             << " | Dept: " << teach[i].department
             << " | Exp: " << teach[i].experience
             << " | Course: " << teach[i].course << "\n";
    }
}

int main()
{
    if (!adminLogin())
        return 0;

    cleanStudents();
    cleanTeachers();
    cleanAttendance();
    cleanMarks();

    int studentCount = 0, teacherCount = 0;
    Student *stud = loadStudents(studentCount);
    Teacher *teach = loadTeachers(teacherCount);

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
        cout << "\n5. Search Student (ID / Exact Name)";
        cout << "\n6. Search Teacher by Department";
        cout << "\n7. Sort Teachers by Experience";
        cout << "\n8. Top 3 Students (Bonus)";
        cout << "\n9. Exit";
        cout << "\nEnter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = 0;
            cout << "\nInvalid input! Please enter a number.\n";
            continue;
        }

        if (choice == 1)
        {
            int subChoice = 0;
            cout << "\n--- Student Management ---";
            cout << "\n1. Add Student";
            cout << "\n2. Update Student";
            cout << "\n3. Delete Student";
            cout << "\n4. View Students";
            cout << "\nEnter choice: ";

            if (!(cin >> subChoice))
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "\nInvalid choice!\n";
            }
            else if (subChoice == 1) addStudent(stud, studentCount);
            else if (subChoice == 2) updateStudent(stud, studentCount);
            else if (subChoice == 3) deleteStudent(stud, studentCount);
            else if (subChoice == 4) viewStudents(stud, studentCount);
            else cout << "\nInvalid choice!\n";
        }
        else if (choice == 2)
        {
            int subChoice = 0;
            cout << "\n--- Teacher Management ---";
            cout << "\n1. Add Teacher";
            cout << "\n2. Update Teacher";
            cout << "\n3. Delete Teacher";
            cout << "\n4. View Teachers";
            cout << "\nEnter choice: ";

            if (!(cin >> subChoice))
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "\nInvalid choice!\n";
            }
            else if (subChoice == 1) addTeacher(teach, teacherCount);
            else if (subChoice == 2) updateTeacher(teach, teacherCount);
            else if (subChoice == 3) deleteTeacher(teach, teacherCount);
            else if (subChoice == 4) viewTeachers(teach, teacherCount);
            else cout << "\nInvalid choice!\n";
        }
        else if (choice == 3)
        {
            int subChoice = 0;
            cout << "\n--- Attendance Management ---";
            cout << "\n1. Mark Attendance";
            cout << "\n2. View All Attendance";
            cout << "\n3. Student Attendance Summary";
            cout << "\n4. Class Attendance Percentage";
            cout << "\nEnter choice: ";

            if (!(cin >> subChoice))
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "\nInvalid choice!\n";
            }
            else if (subChoice == 1)
                markAttendance(stud, studentCount, teach, teacherCount);
            else if (subChoice == 2)
                viewAttendance();
            else if (subChoice == 3)
                viewAttendanceSummary(stud, studentCount);
            else if (subChoice == 4)
            {
                int sid, tid;
                cout << "Enter Student ID: ";
                cin >> sid;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\nInvalid ID!\n";
                }
                else
                {
                    cout << "Enter Teacher ID: ";
                    cin >> tid;
                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "\nInvalid ID!\n";
                    }
                    else
                        cout << "Attendance Percentage: " << attendancePercent(sid, tid) << "%\n";
                }
            }
            else
                cout << "\nInvalid choice!\n";
        }
        else if (choice == 4)
        {
            int subChoice = 0;
            cout << "\n--- Marks Management ---";
            cout << "\n1. View Student Marks Sheet";
            cout << "\n2. Update Marks";
            cout << "\n3. Delete Marks";
            cout << "\n4. Quiz Stats by Subject (Avg / High / Low)";
            cout << "\nEnter choice: ";

            if (!(cin >> subChoice))
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "\nInvalid choice!\n";
            }
            else if (subChoice == 1)
                viewMarks(stud, studentCount);
            else if (subChoice == 2)
                updateMarks(stud, studentCount, teach, teacherCount);
            else if (subChoice == 3)
                deleteMarks();
            else if (subChoice == 4)
                quizStatsBySubject();
            else
                cout << "\nInvalid choice!\n";
        }
        else if (choice == 5)
        {
            searchStudentByIdOrName(stud, studentCount);
        }
        else if (choice == 6)
        {
            searchTeacherByDepartment(teach, teacherCount);
        }
        else if (choice == 7)
        {
            sortTeachersByExperience(teach, teacherCount);
            viewTeachers(teach, teacherCount);
        }
        else if (choice == 8)
        {
            topThreeStudents(stud, studentCount);
        }
        else if (choice == 9)
        {
            cout << "\nExiting program and freeing memory...\n";
        }
        else
        {
            cout << "\nInvalid choice! Please select 1 to 9.\n";
        }

    } while (choice != 9);

    delete[] stud;
    delete[] teach;

    stud = NULL;
    teach = NULL;

    return 0;
}

bool adminLogin()
{
    char password[20];
    const char correctPassword[] = "pass";
    int attempts = 0;

    while (attempts < 3)
    {
        cout << "\nEnter Admin Password: ";
        cin.width(20);
        cin >> password;
        cin.ignore(1000, '\n');
        if (strcmp(password, correctPassword) == 0)
        {
            cout << "\nLogin Successful!\n";
            return true;
        }
        attempts++;
        cout << "\nIncorrect Password!\n";
    }

    cout << "\nAccess Denied!\n";
    return false;
}

void cleanStudents()
{
    ifstream input("students.txt");
    if (!input)
    {
        cout << "\nStudent file not found.\n";
        return;
    }

    int count = 0;
    char line[250];
    while (!input.eof())
    {
        input.getline(line, 250);
        if (input.fail() && !input.eof())
        {
            input.clear();
            input.ignore(10000, '\n');
        }
        count++;
    }
    input.clear();
    input.seekg(0);

    Student *s = new Student[count];
    int valid = 0, invalid = 0;

    while (!input.eof())
    {
        input.getline(line, 250);
        if (input.fail() && !input.eof())
        {
            input.clear();
            input.ignore(10000, '\n');
            invalid++;
            continue;
        }

        char *f[6];
        int n = 0;
        char *tok = strtok(line, ",\r");
        while (tok != NULL && n < 6)
        {
            f[n++] = tok;
            tok = strtok(NULL, ",\r");
        }
        if (n == 0)
            continue;

        bool ok = (n == 5);
        int numeric[2] = {0, 2};
        for (int q = 0; ok && q < 2; q++)
        {
            int len = strlen(f[numeric[q]]);
            if (len > 9)
                ok = false;
            for (int k = 0; ok && k < len; k++)
            {
                if (f[numeric[q]][k] < '0' || f[numeric[q]][k] > '9')
                    ok = false;
            }
        }

        if (ok)
        {
            if (atoi(f[0]) <= 0 || atoi(f[2]) < 16 || atoi(f[2]) > 40)
                ok = false;
            if (strlen(f[1]) >= 40 || f[1][0] == ' ')
                ok = false;
            for (int k = 0; ok && f[1][k] != '\0'; k++)
            {
                if (!((f[1][k] >= 'A' && f[1][k] <= 'Z') ||
                      (f[1][k] >= 'a' && f[1][k] <= 'z') || f[1][k] == ' '))
                    ok = false;
            }
            if (strcmp(f[3], "Male") != 0 && strcmp(f[3], "Female") != 0)
                ok = false;
            if (strlen(f[4]) != 11 || f[4][0] != '0' || f[4][1] != '3')
                ok = false;
            for (int k = 0; ok && k < 11; k++)
            {
                if (f[4][k] < '0' || f[4][k] > '9')
                    ok = false;
            }
            if (ok && findStudentById(s, valid, atoi(f[0])) != NULL)
                ok = false;
        }

        if (ok)
        {
            s[valid].studentId = atoi(f[0]);
            strcpy(s[valid].name, f[1]);
            s[valid].age = atoi(f[2]);
            strcpy(s[valid].gender, f[3]);
            strcpy(s[valid].contact, f[4]);
            valid++;
        }
        else
            invalid++;
    }
    input.close();

    saveStudents(s, valid);
    delete[] s;

    cout << "\nStudent File Cleaning Completed!";
    cout << "\nValid Records: " << valid;
    cout << "\nInvalid Records: " << invalid << endl;
}

void cleanTeachers()
{
    ifstream input("teachers.txt");
    if (!input)
    {
        cout << "\nTeacher file not found.\n";
        return;
    }

    int count = 0;
    char line[250];
    while (!input.eof())
    {
        input.getline(line, 250);
        if (input.fail() && !input.eof())
        {
            input.clear();
            input.ignore(10000, '\n');
        }
        count++;
    }
    input.clear();
    input.seekg(0);

    Teacher *t = new Teacher[count];
    int valid = 0, invalid = 0;

    while (!input.eof())
    {
        input.getline(line, 250);
        if (input.fail() && !input.eof())
        {
            input.clear();
            input.ignore(10000, '\n');
            invalid++;
            continue;
        }

        char *f[6];
        int n = 0;
        char *tok = strtok(line, ",\r");
        while (tok != NULL && n < 6)
        {
            f[n++] = tok;
            tok = strtok(NULL, ",\r");
        }
        if (n == 0)
            continue;

        bool ok = (n == 5);
        int numeric[2] = {0, 3};
        for (int q = 0; ok && q < 2; q++)
        {
            int len = strlen(f[numeric[q]]);
            if (len > 9)
                ok = false;
            for (int k = 0; ok && k < len; k++)
            {
                if (f[numeric[q]][k] < '0' || f[numeric[q]][k] > '9')
                    ok = false;
            }
        }

        if (ok)
        {
            if (atoi(f[0]) <= 0 || atoi(f[3]) > 45)
                ok = false;
            if (strlen(f[1]) >= 40 || f[1][0] == ' ' || strlen(f[2]) >= 30 || f[2][0] == ' ')
                ok = false;
            for (int k = 0; ok && f[1][k] != '\0'; k++)
            {
                if (!((f[1][k] >= 'A' && f[1][k] <= 'Z') ||
                      (f[1][k] >= 'a' && f[1][k] <= 'z') || f[1][k] == ' '))
                    ok = false;
            }
            for (int k = 0; ok && f[2][k] != '\0'; k++)
            {
                if (!((f[2][k] >= 'A' && f[2][k] <= 'Z') ||
                      (f[2][k] >= 'a' && f[2][k] <= 'z') || f[2][k] == ' '))
                    ok = false;
            }
            if (strlen(f[4]) > 9)
                ok = false;
            for (int k = 0; ok && f[4][k] != '\0'; k++)
            {
                if (!((f[4][k] >= 'A' && f[4][k] <= 'Z') ||
                      (f[4][k] >= 'a' && f[4][k] <= 'z') ||
                      (f[4][k] >= '0' && f[4][k] <= '9')))
                    ok = false;
            }
            for (int k = 0; ok && k < valid; k++)
            {
                if (t[k].teacherId == atoi(f[0]))
                    ok = false;
            }
        }

        if (ok)
        {
            t[valid].teacherId = atoi(f[0]);
            strcpy(t[valid].name, f[1]);
            strcpy(t[valid].department, f[2]);
            t[valid].experience = atoi(f[3]);
            strcpy(t[valid].course, f[4]);
            valid++;
        }
        else
            invalid++;
    }
    input.close();

    saveTeachers(t, valid);
    delete[] t;

    cout << "\nTeacher File Cleaning Completed!";
    cout << "\nValid Records: " << valid;
    cout << "\nInvalid Records: " << invalid << endl;
}

void cleanAttendance()
{
    ifstream input("attendance.txt");
    if (!input)
    {
        cout << "\nAttendance file not found.\n";
        return;
    }

    int count = 0;
    char line[250];
    while (!input.eof())
    {
        input.getline(line, 250);
        if (input.fail() && !input.eof())
        {
            input.clear();
            input.ignore(10000, '\n');
        }
        count++;
    }
    input.clear();
    input.seekg(0);

    int studentCount = 0, teacherCount = 0;
    Student *stud = loadStudents(studentCount);
    Teacher *teach = loadTeachers(teacherCount);

    Attendance *a = new Attendance[count];
    int valid = 0, invalid = 0;
    bool firstLine = true;

    while (!input.eof())
    {
        input.getline(line, 250);
        if (input.fail() && !input.eof())
        {
            input.clear();
            input.ignore(10000, '\n');
            invalid++;
            firstLine = false;
            continue;
        }

        if (firstLine)
        {
            firstLine = false;
            if (line[0] < '0' || line[0] > '9')
                continue;
        }

        char *f[6];
        int n = 0;
        char *tok = strtok(line, ",\r");
        while (tok != NULL && n < 6)
        {
            f[n++] = tok;
            tok = strtok(NULL, ",\r");
        }
        if (n == 0)
            continue;

        bool ok = (n == 4);
        int numeric[2] = {0, 1};
        for (int q = 0; ok && q < 2; q++)
        {
            int len = strlen(f[numeric[q]]);
            if (len > 9)
                ok = false;
            for (int k = 0; ok && k < len; k++)
            {
                if (f[numeric[q]][k] < '0' || f[numeric[q]][k] > '9')
                    ok = false;
            }
        }

        if (ok)
        {
            if (atoi(f[0]) <= 0 || atoi(f[1]) <= 0)
                ok = false;
            if (strcmp(f[3], "Present") != 0 && strcmp(f[3], "Absent") != 0)
                ok = false;
            if (findStudentById(stud, studentCount, atoi(f[0])) == NULL)
                ok = false;

            bool teacherFound = false;
            for (int k = 0; k < teacherCount; k++)
            {
                if (teach[k].teacherId == atoi(f[1]))
                    teacherFound = true;
            }
            if (!teacherFound)
                ok = false;

            if (strlen(f[2]) != 10)
                ok = false;
            for (int k = 0; ok && k < 10; k++)
            {
                if (k == 4 || k == 7)
                {
                    if (f[2][k] != '-')
                        ok = false;
                }
                else if (f[2][k] < '0' || f[2][k] > '9')
                    ok = false;
            }
            if (ok)
            {
                int year = (f[2][0] - '0') * 1000 + (f[2][1] - '0') * 100 + (f[2][2] - '0') * 10 + (f[2][3] - '0');
                int month = (f[2][5] - '0') * 10 + (f[2][6] - '0');
                int day = (f[2][8] - '0') * 10 + (f[2][9] - '0');
                int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
                if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
                    days[1] = 29;
                if (year < 1900 || month < 1 || month > 12 || day < 1 || day > days[month - 1])
                    ok = false;
            }

            for (int k = 0; ok && k < valid; k++)
            {
                if (a[k].studentId == atoi(f[0]) && a[k].teacherId == atoi(f[1]) &&
                    strcmp(a[k].date, f[2]) == 0)
                    ok = false;
            }
        }

        if (ok)
        {
            a[valid].studentId = atoi(f[0]);
            a[valid].teacherId = atoi(f[1]);
            strcpy(a[valid].date, f[2]);
            strcpy(a[valid].status, f[3]);
            valid++;
        }
        else
            invalid++;
    }
    input.close();

    ofstream output("attendance.txt");
    output << "StudentID,TeacherID,Date,Status\n";
    for (int i = 0; i < valid; i++)
        output << a[i].studentId << "," << a[i].teacherId << ","
               << a[i].date << "," << a[i].status << endl;
    output.close();

    delete[] a;
    delete[] stud;
    delete[] teach;

    cout << "\nAttendance File Cleaning Completed!";
    cout << "\nValid Records: " << valid;
    cout << "\nInvalid Records: " << invalid << endl;
}

void cleanMarks()
{
    ifstream input("marks.txt");
    if (!input)
    {
        cout << "\nMarks file not found.\n";
        return;
    }

    int count = 0;
    char line[250];
    while (!input.eof())
    {
        input.getline(line, 250);
        if (input.fail() && !input.eof())
        {
            input.clear();
            input.ignore(10000, '\n');
        }
        count++;
    }
    input.clear();
    input.seekg(0);

    int studentCount = 0, teacherCount = 0;
    Student *stud = loadStudents(studentCount);
    Teacher *teach = loadTeachers(teacherCount);

    MarkRecord *m = new MarkRecord[count];
    int valid = 0, invalid = 0;
    bool firstLine = true;

    while (!input.eof())
    {
        input.getline(line, 250);
        if (input.fail() && !input.eof())
        {
            input.clear();
            input.ignore(10000, '\n');
            invalid++;
            firstLine = false;
            continue;
        }

        if (firstLine)
        {
            firstLine = false;
            if (line[0] < '0' || line[0] > '9')
                continue;
        }

        char *f[6];
        int n = 0;
        char *tok = strtok(line, ",\r");
        while (tok != NULL && n < 6)
        {
            f[n++] = tok;
            tok = strtok(NULL, ",\r");
        }
        if (n == 0)
            continue;

        bool ok = (n == 4);
        int numeric[2] = {0, 3};
        for (int q = 0; ok && q < 2; q++)
        {
            int len = strlen(f[numeric[q]]);
            if (len > 9)
                ok = false;
            for (int k = 0; ok && k < len; k++)
            {
                if (f[numeric[q]][k] < '0' || f[numeric[q]][k] > '9')
                    ok = false;
            }
        }

        if (ok)
        {
            if (atoi(f[0]) <= 0 || atoi(f[3]) > 100)
                ok = false;
            if (strlen(f[1]) > 9 || strlen(f[2]) > 11)
                ok = false;
            for (int k = 0; ok && f[1][k] != '\0'; k++)
            {
                if (!((f[1][k] >= 'A' && f[1][k] <= 'Z') ||
                      (f[1][k] >= 'a' && f[1][k] <= 'z') ||
                      (f[1][k] >= '0' && f[1][k] <= '9')))
                    ok = false;
            }
            for (int k = 0; ok && f[2][k] != '\0'; k++)
            {
                if (!((f[2][k] >= 'A' && f[2][k] <= 'Z') ||
                      (f[2][k] >= 'a' && f[2][k] <= 'z') ||
                      (f[2][k] >= '0' && f[2][k] <= '9')))
                    ok = false;
            }
            if (ok && findStudentById(stud, studentCount, atoi(f[0])) == NULL)
                ok = false;

            bool courseFound = false;
            for (int k = 0; k < teacherCount; k++)
            {
                if (strcmp(teach[k].course, f[1]) == 0)
                    courseFound = true;
            }
            if (!courseFound)
                ok = false;

            for (int k = 0; ok && k < valid; k++)
            {
                if (m[k].studentId == atoi(f[0]) && strcmp(m[k].subject, f[1]) == 0 &&
                    strcmp(m[k].component, f[2]) == 0)
                    ok = false;
            }
        }

        if (ok)
        {
            m[valid].studentId = atoi(f[0]);
            strcpy(m[valid].subject, f[1]);
            strcpy(m[valid].component, f[2]);
            m[valid].marks = atoi(f[3]);
            valid++;
        }
        else
            invalid++;
    }
    input.close();

    ofstream output("marks.txt");
    output << "StudentID,Subject,Component,Marks\n";
    for (int i = 0; i < valid; i++)
        output << m[i].studentId << "," << m[i].subject << ","
               << m[i].component << "," << m[i].marks << endl;
    output.close();

    delete[] m;
    delete[] stud;
    delete[] teach;

    cout << "\nMarks File Cleaning Completed!";
    cout << "\nValid Records: " << valid;
    cout << "\nInvalid Records: " << invalid << endl;
}
