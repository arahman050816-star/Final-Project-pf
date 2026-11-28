void cleanStudents()
{
    ifstream input("students.txt");
    ofstream output("temp.txt");

    if(!input || !output)
    {
        cout << "Student file opening failed!" << endl;
        return;
    }

    Student s;
    int valid = 0, invalid = 0;

    while(input >> s.studentId)
    {
        input.ignore();

        input.getline(s.name, 40, ',');

        input >> s.age;
        input.ignore();

        input.getline(s.gender, 8, ',');

        input.getline(s.contact, 12);

        if(input &&
           s.studentId > 0 &&
           strlen(s.name) > 0 &&
           s.age >= 16 && s.age <= 40 &&
           (strcmp(s.gender, "Male") == 0 ||
            strcmp(s.gender, "Female") == 0) &&
           strlen(s.contact) == 11 &&
           s.contact[0] == '0' &&
           s.contact[1] == '3')
        {
            int correct = 1;

            for(int i = 0; i < 11; i++)
            {
                if(s.contact[i] < '0' || s.contact[i] > '9')
                {
                    correct = 0;
                }
            }

            if(correct == 1)
            {
                output << s.studentId << ","
                       << s.name << ","
                       << s.age << ","
                       << s.gender << ","
                       << s.contact << endl;

                valid++;
            }
            else
            {
                invalid++;
            }
        }
        else
        {
            invalid++;
        }
    }

    input.close();
    output.close();

    remove("students.txt");
    rename("temp.txt", "students.txt");

    cout << "\nStudent File Cleaning Completed!";
    cout << "\nValid Records: " << valid;
    cout << "\nInvalid Records: " << invalid << endl;
}


void cleanTeachers()
{
    ifstream input("teachers.txt");
    ofstream output("temp.txt");

    if(!input || !output)
    {
        cout << "Teacher file opening failed!" << endl;
        return;
    }

    Teacher t;
    int valid = 0, invalid = 0;

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
           t.experience >= 0 &&
           t.experience <= 45 &&
           strlen(t.course) > 0)
        {
            output << t.teacherId << ","
                   << t.name << ","
                   << t.department << ","
                   << t.experience << ","
                   << t.course << endl;

            valid++;
        }
        else
        {
            invalid++;
        }
    }

    input.close();
    output.close();

    remove("teachers.txt");
    rename("temp.txt", "teachers.txt");

    cout << "\nTeacher File Cleaning Completed!";
    cout << "\nValid Records: " << valid;
    cout << "\nInvalid Records: " << invalid << endl;
}




void cleanAttendance()
{
    ifstream input("attendance.txt");
    ofstream output("temp.txt");

    if(!input || !output)
    {
        cout << "Attendance file opening failed!" << endl;
        return;
    }

    string header;
    getline(input, header);

    output << "StudentID,TeacherID,Date,Status" << endl;

    Attendance a;
    int valid = 0, invalid = 0;

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

            valid++;
        }
        else
        {
            invalid++;
        }
    }

    input.close();
    output.close();

    remove("attendance.txt");
    rename("temp.txt", "attendance.txt");

    cout << "\nAttendance File Cleaning Completed!";
    cout << "\nValid Records: " << valid;
    cout << "\nInvalid Records: " << invalid << endl;
}




void cleanMarks()
{
    ifstream input("marks.txt");
    ofstream output("temp.txt");

    if(!input || !output)
    {
        cout << "Marks file opening failed!" << endl;
        return;
    }

    char header[100];
    input.getline(header, 100);

    output << "StudentID,Subject,Component,Marks" << endl;

    MarkRecord m;
    int valid = 0, invalid = 0;

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
           m.marks >= 0 &&
           m.marks <= 100)
        {
            output << m.studentId << ","
                   << m.subject << ","
                   << m.component << ","
                   << m.marks << endl;

            valid++;
        }
        else
        {
            invalid++;
        }
    }

    input.close();
    output.close();

    remove("marks.txt");
    rename("temp.txt", "marks.txt");

    cout << "\nMarks File Cleaning Completed!";
    cout << "\nValid Records: " << valid;
    cout << "\nInvalid Records: " << invalid << endl;
}
