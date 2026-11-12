#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Student {
    int studentId;
    char name[40];
    int age;
    char gender[8];      
    char contact[12];    
};

struct Teacher {
    int teacherId;
    char name[40];
    char department[30];
    int experience;      
    char course[10];
};
struct Attendance {
    int studentId;
    int teacherId;
    char date[12];      
    char status[8];      
};
struct MarkRecord {
    int studentId;
    int teacherId;
    char component[12];  
    int marks;
};




int main()
{
	char header[100];
	
    ifstream input;
    
    Student stud[100];
    
    Teacher teach[10];
    
    Attendance attend[100];
    
    MarkRecord marks[100];
    
    input.open("students.txt");

    if(input.fail())
    {
        cout << "Opening failed." << endl;
        
    }


    for(int i=0; i < 100; i++)
    {
    
        input>>stud[i].studentId;
        
        input.ignore();

        input.getline(stud[i].name,40,',');
        
		input>>stud[i].age;
		
        input.ignore();

        input.getline(stud[i].gender, 8 , ',');
        

        input.getline(stud[i].contact, 12, ',');
       
        
    }

    input.close();

    input.open("teachers.txt");
    
    if(input.fail())
    {
        cout << "Opening failed." << endl;
        
    }
	
    for (int i=0; i<10; i++)
    {
    	input>>teach[i].teacherId;
    	
		input.getline(teach[i].name, 40, ',');

		input.getline(teach[i].department, 30, ',');
		
		input>>teach[i].experience;
		
		input.ignore();
		
		input.getline(teach[i].course, 10);
		
	}
	
	input.close();
	
	input.open("attendance.txt");
	
	if(input.fail())
    {
        cout << "Opening failed." << endl;
        
    }
    
    input.getline(header, 100);
    
    for(int i=0;i<100;i++)
    {
    	input>>attend[i].studentId;
    	
    	input.ignore();
    	
    	input>>attend[i].teacherId;
    	
    	input.ignore();
    	
    	input.getline(attend[i].date, 100, ',');
    	
    	input.getline(attend[i].status, 8);
	}
	
	input.close();
	
	input.open("marks.txt"); 

	for(int i=0;i<100;i++)
	{
		input>>marks[i].studentId;
		
	}
	
	
	
    
    

    return 0;
}
