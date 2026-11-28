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














void addTeacher(Teacher *&teach, int &count)
{
    Teacher t;

    cout << "\nEnter Teacher ID: ";
    cin >> t.teacherId;
    for(int i = 0; i < count; i++)
    {
        if(teach[i].teacherId == t.teacherId)
        {
            cout << "\nTeacher ID already exists!\n";
            return;
        }
    }

    cin.ignore(1000, '\n');

    cout << "Enter Name: ";
    cin.getline(t.name, 40);

    cout << "Enter Department: ";
    cin.getline(t.department, 30);

    cout << "Enter Experience: ";
    cin >> t.experience;

    cin.ignore(1000, '\n');

    cout << "Enter Course: ";
    cin.getline(t.course, 30);
    Teacher *temp = new Teacher[count + 1];

    for(int i = 0; i < count; i++)
    {
        temp[i] = teach[i];
    }

    temp[count] = t;

    delete[] teach;

    teach = temp;
    count++;

    saveTeachers(teach, count);

    cout << "\nTeacher added successfully!\n";
}
void updateTeacher(Teacher *teach, int count)
{
    int id;
    int position = -1;

    cout << "\nEnter Teacher ID to update: ";
    cin >> id;
    for(int i = 0; i < count; i++)
    {
        if(teach[i].teacherId == id)
        {
            position = i;
            break;
        }
    }

    if(position == -1)
    {
        cout << "\nTeacher not found!\n";
        return;
    }

    cin.ignore(1000, '\n');

    cout << "Enter New Name: ";
    cin.getline(teach[position].name, 40);

    cout << "Enter New Department: ";
    cin.getline(teach[position].department, 30);

    cout << "Enter New Experience: ";
    cin >> teach[position].experience;

    cin.ignore(1000, '\n');

    cout << "Enter New Course: ";
    cin.getline(teach[position].course, 30);

    saveTeachers(teach, count);

    cout << "\nTeacher updated successfully!\n";
}

void deleteTeacher(Teacher *&teach, int &count)
{
    int id;
    int position = -1;

    cout << "\nEnter Teacher ID to delete: ";
    cin >> id;
    for(int i = 0; i < count; i++)
    {
        if(teach[i].teacherId == id)
        {
            position = i;
            break;
        }
    }
    if(position == -1)
    {
        cout << "\nTeacher not found!\n";
        return;
    }
    Teacher *temp = NULL;
    if(count > 1)
    {
        temp = new Teacher[count - 1];

        int j = 0;

        for(int i = 0; i < count; i++)
        {
            if(i != position)
            {
                temp[j] = teach[i];
                j++;
            }
        }
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

