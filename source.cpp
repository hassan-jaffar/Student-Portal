#include<iostream>
#include<fstream>
using namespace std;

class Helper
{
public:
	static int StringLength(char* str)
	{
		int count = 0;
		while (str[count] != '\0')            //stops counting when finds null
			count++;
		return count;

	}
	static void StringCopy(char*& dest, char*& src) {
		int i = 0;
		while (src[i] != '\0')              //deep copy till null is found
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';     //placing null at last index
	}
	static char* GetStringFromBuffer(char* str)
	{
		char* string1 = new char[StringLength(str)+1];
		StringCopy(string1, str);                     //re-usability of existing functions

		return string1;
	}
};

class Report
{
private:
	char rollNo[9];            //hard-coding 9 because roll no is always 8 chars and 9th for null
	char* firstName;
	char* lastName;
	int* quizMarks;
	int* assignMarks;
	int totalMarksGained;     //stores total marks for all quizes and assignments
public:
	static int totalStudents;
	static int totalQuiz;
	static int totalAssign;
	static int* totalMarksQuiz;
	static int* totalMarksAssign;
	static int totalMarks;

	Report()
	{
		//initializing all dynamic data
		int i = 0;
		while (i < 9)
		{
			rollNo[i] = '\0';
			i++;
		}
		firstName = lastName = 0;
		quizMarks = assignMarks = 0;
		totalMarksGained = 0;
	}
	
	Report(const Report& rhs)       //copy constructor
	{
		//Helper::StringCopy(rollNo, rhs.rollNo);  //does not work for static array. suitable in prev versions
		int i = 0;
		while (rhs.rollNo[i] != '\0')       //deep copy
		{
			rollNo[i] = rhs.rollNo[i];
			i++;
		}
		rollNo[i] = '\0';
		firstName = Helper::GetStringFromBuffer(rhs.firstName);
		lastName = Helper::GetStringFromBuffer(rhs.lastName);

		quizMarks = new int[Report::totalQuiz];
		assignMarks = new int[Report::totalAssign];

		for (int i = 0; i < Report::totalQuiz; i++)
			quizMarks[i] = rhs.quizMarks[i];
		for (int i = 0; i < Report::totalAssign; i++)
			assignMarks[i] = rhs.assignMarks[i];

		totalMarksGained = rhs.totalMarksGained;
	}

	const Report& operator=(const Report& rhs)      //assignment operator
	{
		if (&rhs != this)       //to avoid extra processing
		{
			if (firstName != 0)           //de allocations
				delete firstName;
			if (lastName != 0)
				delete lastName;
			if (quizMarks != 0)
				delete quizMarks;
			if (assignMarks != 0)
				delete assignMarks;

			int i = 0;
			while (rhs.rollNo[i] != '\0')        //re allocations
			{
				rollNo[i] = rhs.rollNo[i];
				i++;
			}
			rollNo[i] = '\0';
			firstName = Helper::GetStringFromBuffer(rhs.firstName);
			lastName = Helper::GetStringFromBuffer(rhs.lastName);

			quizMarks = new int[Report::totalQuiz];
			assignMarks = new int[Report::totalAssign];

			for (int i = 0; i < Report::totalQuiz; i++)
				quizMarks[i] = rhs.quizMarks[i];
			for (int i = 0; i < Report::totalAssign; i++)
				assignMarks[i] = rhs.assignMarks[i];

			totalMarksGained = rhs.totalMarksGained;
		}
		return *this;       //returns contents of this after copying rhs into this
	}

	~Report()
	{
		delete firstName;
		delete lastName;
		delete quizMarks;
		delete assignMarks;
	}

	void ReadDataFromFile(ifstream& fin)
	{
		char temp[20];

		fin >> rollNo;

		//reads data in temp buffer, then allocates required memory on heap and deep copies there

		fin >> temp;
		firstName = Helper::GetStringFromBuffer(temp);

		fin >> temp;
		lastName = Helper::GetStringFromBuffer(temp);

		quizMarks = new int[totalQuiz];        //dynamic allocation after reading required size
		assignMarks = new int[totalAssign];

		for (int i = 0; i < totalQuiz; i++)
		{
			fin >> quizMarks[i];
			totalMarksGained += quizMarks[i];
		}

		for (int i = 0; i < totalAssign; i++)
		{
			fin >> assignMarks[i];
			totalMarksGained += assignMarks[i];
		}

	}

	void PrintListView()
	{
		cout << rollNo << "\t" << firstName << " " << lastName << "\t\t";
		for (int i = 0; i < totalQuiz; i++)
			cout << quizMarks[i] << "\t";
		cout << "\t";
		for (int i = 0; i < totalAssign; i++)
			cout << assignMarks[i] << "\t";
		cout << "\t" << totalMarksGained << endl;
	}

	void PrintDetailView()
	{
		cout << "\tSTUDENT INFORMATION:\n";
		cout << "Roll No.\t" << rollNo << endl;
		cout << "Name: \t" << firstName << " " << lastName << endl;
		cout << "Quizzes Marks:\n";
		for (int i = 0; i < totalQuiz; i++)
			cout << "\tQ" << i + 1 << ":\t" << quizMarks[i] << "/" << totalMarksQuiz[i] << endl;
		cout << "Assignments Marks:\n";
		for (int i = 0; i < totalAssign; i++)
			cout << "\tA" << i + 1 << ":\t" << assignMarks[i] << "/" << totalMarksAssign[i] << endl;
		cout << "Total:\t" << totalMarksGained << "/" << totalMarks << endl << endl;
	}

	static Report* Sorting(Report* list, int size)
	{
		int i = 0, j = 0;
		Report temp;
		bool swaps = true;
		i = size - 1;
		while (i > 0 && swaps == true)    //checks if any swap was made in previous iteration and all indexes checked
		{
			swaps = false;
			for (j = 0; j < i; j++)
			{
				if (list[j].totalMarksGained > list[j + 1].totalMarksGained)  //comapres total marks of this with next index
				{
					temp = list[j + 1];       //swapping and swap flag marked
					list[j + 1] = list[j];
					list[j] = temp;
					swaps = true;
				}
			}
			i--;
		}
		return list;     //returns the same list after sorting
	}

	static int Searching(Report* list, int size, const char* key)
	{
		int index = 0;
		bool found = false;
		while (found == false && index < size)    //checks if key not found already and index within size of list
		{
			for (int j = 0; j < 9; j++)
			{
				found = true;
				if (list[index].rollNo[j] != key[j])    //checks every index of key and list's value's roll no
				{
					found = false;
					break;           //breaks the checking even if one mismatch found. avoids extra processing
				}
			}
			if (found == true)       //returns the index where key matches
				return index;
			index++;
		}
		return -1;           //in case no match found in the whole list
	}

	friend void PrintStats(Report* list);
};


//initializing static variables in global space
int Report::totalStudents = 0;
int Report::totalQuiz = 0;
int Report::totalAssign = 0;
int* Report::totalMarksQuiz = 0;
int* Report::totalMarksAssign = 0;
int Report::totalMarks = 0;

//friend functions
void PrintStats(Report* list)
{
	int* quizMax = new int[Report::totalQuiz];
	int* quizMin = new int[Report::totalQuiz];
	int* quizTotal = new int[Report::totalQuiz];
	int* assignMax = new int[Report::totalAssign];
	int* assignMin = new int[Report::totalAssign];
	int* assignTotal = new int[Report::totalAssign];

	//initialization
	for (int i = 0; i < Report::totalQuiz; i++)
	{
		quizMin[i] = list[0].quizMarks[i];     //just assumes first index value as minimum
		quizMax[i] = 0;
		quizTotal[i] = 0;
	}
	for (int i = 0; i < Report::totalAssign; i++)
	{
		assignMin[i] = list[0].assignMarks[i];     //not zero because no mark gained is less than zero
		assignMax[i] = 0;
		assignTotal[i] = 0;
	}

	//checks for min and max values and adds to the total marks for all quizes and assigments
	for (int i = 0; i < Report::totalStudents; i++)   //loops through all students
	{
		for (int j = 0; j < Report::totalQuiz; j++)   //loops through all quizes for every student
		{
			if (list[i].quizMarks[j] < quizMin[j])
				quizMin[j] = list[i].quizMarks[j];
			if (list[i].quizMarks[j] > quizMax[j])
				quizMax[j] = list[i].quizMarks[j];
			quizTotal[j] += list[i].quizMarks[j];
		}
		for (int j = 0; j < Report::totalAssign; j++)   //loops through all assignments for every student
		{
			if (list[i].assignMarks[j] < assignMin[j])
				assignMin[j] = list[i].assignMarks[j];
			if (list[i].assignMarks[j] > assignMax[j])
				assignMax[j] = list[i].assignMarks[j];
			assignTotal[j] += list[i].assignMarks[j];
		}
	}

	//printing all calculated data with headers
	cout << "\n\n\t-------------------------Class Statistics-------------------------\n\n\n";
	cout << "\t\t";
	for (int i = 0; i < Report::totalQuiz; i++)
		cout << "Q" << i + 1 << "\t";
	cout << "\t";
	for (int i = 0; i < Report::totalAssign; i++)
		cout << "A" << i + 1 << "\t";

	cout << "\nTotal:\t\t";
	for (int i = 0; i < Report::totalQuiz; i++)
		cout << Report::totalMarksQuiz[i] << "\t";
	cout << "\t";
	for (int i = 0; i < Report::totalAssign; i++)
		cout << Report::totalMarksAssign[i] << "\t";

	cout << "\nMaximum:\t";
	for (int i = 0; i < Report::totalQuiz; i++)
		cout << quizMax[i] << "\t";
	cout << "\t";
	for (int i = 0; i < Report::totalAssign; i++)
		cout << assignMax[i] << "\t";

	cout << "\nMinimum:\t";
	for (int i = 0; i < Report::totalQuiz; i++)
		cout << quizMin[i] << "\t";
	cout << "\t";
	for (int i = 0; i < Report::totalAssign; i++)
		cout << assignMin[i] << "\t";

	cout << "\nAverage:\t";         //calculates average before printing to save heap memory by keeping it there
	for (int i = 0; i < Report::totalQuiz; i++)
		cout << (quizTotal[i] / Report::totalStudents) << "\t";
	cout << "\t";
	for (int i = 0; i < Report::totalAssign; i++)
		cout << (assignTotal[i] / Report::totalStudents) << "\t";

	cout << endl << endl;

	//de-allocating memory used for this function
	delete[] quizMax;
	delete[] quizMin;
	delete[] quizTotal;
	delete[] assignMax;
	delete[] assignMin;
	delete[] assignTotal;
}

//global functions
Report* ReadDataFromFile(const char* fileName)
{
	//global function that reads static data
	ifstream fin;

	fin.open(fileName);
	if (fin.is_open())
	{
		fin >> Report::totalStudents;
		fin >> Report::totalQuiz;
		fin >> Report::totalAssign;

		Report::totalMarksQuiz = new int[Report::totalQuiz];   //dynamic allocation after reading size of data
		Report::totalMarksAssign = new int[Report::totalAssign];

		for (int i = 0; i < Report::totalQuiz; i++)
		{
			fin >> Report::totalMarksQuiz[i];
			Report::totalMarks += Report::totalMarksQuiz[i];
		}
			
		for (int i = 0; i < Report::totalAssign; i++)
		{
			fin >> Report::totalMarksAssign[i];
			Report::totalMarks += Report::totalMarksAssign[i];
		}

		Report* studentList = new Report[Report::totalStudents];     //creating objects for every student

		for(int i = 0; i < Report::totalStudents; i++)
			studentList[i].ReadDataFromFile(fin);      //reading data for every student in respective objects

		//calling read function of class for every student

		return studentList;        //returns the object's list after reading all data
	}
	else
	{
		return 0;
	}
}

void Printing(Report* studentList)
{
	cout << "Roll No.\tName\t\t\t";
	for (int i = 0; i < Report::totalQuiz; i++)
		cout << "Q" << i + 1 << "\t";
	cout << "\t";
	for (int i = 0; i < Report::totalAssign; i++)
		cout << "A" << i + 1 << "\t";
	cout << "\t" << "Total" << endl;
	for (int i = 0; i < 3 * Report::totalStudents; i++)
		cout << "-";
	cout << endl;                          //headers printed

	{
		for (int i = 0; i < Report::totalStudents; i++)
		{
			studentList[i].PrintListView();
		}
	}

	cout << "\n##########################################################################################\n";
}

void PrintSearchResult(Report* list, int index)
{
	//saves excessive repetition of this code in case of multiple searches
	cout << "\n##########################################################################################\n\n";
	if (index == -1)
		cout << "\nStudent Not Found\n\n";
	else
		list[index].PrintDetailView();
	cout << "\n#########################################################################################\n\n";
}

void DeleteStaticData()
{
	delete[] Report::totalMarksQuiz;
	delete[] Report::totalMarksAssign;
}

void main()
{
	Report* studentList = ReadDataFromFile("gradesheet.txt");

	if (studentList != 0)      //checks if null pointer i.e error with reading from file
	{
		cout << "\nStudent List:\n\n";
		Printing(studentList);        //just printing data as it was read

		studentList = Report::Sorting(studentList, Report::totalStudents);

		cout << "\nStudent List After Sorting in Ascending Order:\n\n";
		Printing(studentList);

		char key[9] = "15L-1234";         //hard-coding
		int answer = Report::Searching(studentList, Report::totalStudents, key);
		PrintSearchResult(studentList, answer);

		char key2[9] = "15L-4023";        //hard-coding
		answer = Report::Searching(studentList, Report::totalStudents, key2);
		PrintSearchResult(studentList, answer);

		PrintStats(studentList);

		DeleteStaticData();

		delete[] studentList;
	}
}