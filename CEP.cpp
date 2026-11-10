#include<iostream>
#include<cstring>
#include<fstream>
struct Student {
    int studentId;
    char name[40];
    int age;
    char gender[8];      
    char contact[12];    
};

using namespace std;
int main()
{
	string line[100];
	ifstream input;
	Student stud[100];
	input.open("students.txt");
	if(input.fail())
	{
		cout<<"openning is failed."<<endl;
	}
	for(int i=0;i<10000;i++)
	{
	
		input.getline(line[i],',');
	}	
	cout<<line;
	
}
