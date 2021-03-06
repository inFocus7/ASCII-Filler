#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int rowAmt = 0, columnAmt = 0; //To be set after reading in the graphic/file. Global to access easily throughout code.

//Out of Bounds?
bool oob(int currow, int curcol)
{
	if (currow > rowAmt - 1 || currow < 0 || curcol > columnAmt - 1 || curcol < 0)
		return true;
	return false;
}

void paintBucket(char ** graphicPos, int row, int column, char symbol)
{
	//Base Case
	if (oob(row, column) || graphicPos[row][column] != ' ')
		return;
	//Recursion
	else
	{
		//Check Top
		paintBucket(graphicPos, row - 1, column, symbol);
		//Check Left
		paintBucket(graphicPos, row, column - 1, symbol);
		//FILL
		graphicPos[row][column] = symbol; //Takes care of area (since if you don't fill here, you end up with an inf-loop at some point)
		//Check Bottom
		paintBucket(graphicPos, row + 1, column, symbol);
		//Check Right
		paintBucket(graphicPos, row, column + 1, symbol);

	}

}

void printList(int rowAmt, int columnAmt, char ** graphicPos)
{
	for (int i = 0; i < rowAmt; i++)
	{
		for (int j = 0; j < columnAmt; j++)
		{
			cout << graphicPos[i][j];
		}
		cout << endl;
	}
}

int main()
{
	/* Purpose
	Write a Fill function/program using a 2D array of symbols/characters and recursion. */

	//Variables
	char **graphicPos, choice, end, symbolFill;

	graphicPos = new char*[rowAmt];
	for (int i = 0; i < rowAmt; ++i)
		graphicPos[i] = new char[columnAmt];

	ifstream graphicIn;
	ofstream graphicOut;
	string graphicName, line;
	int posX, posY, rowFill, columnFill;

	//Program
BEGIN:
	do
	{

		cout << "Select from the options: (L)oad art. (F)ill. (S)ave art." << endl;
		cout << "Choice? ";
		cin >> choice;
		if (choice == 'L')
		{
			//Deletes previous 2d array image to add ability to load in new file/image.
			for (int i = 0; i < rowAmt; i++)
				delete[] graphicPos[i];
			delete[] graphicPos;
			// ^Doesn't work yet...

			//1) Load File
			cout << "Loading..." << endl << "Filename: ";
			cin >> graphicName;
			graphicIn.open(graphicName);

			if (!graphicIn)
			{
				cout << "Unknown file. Please try again." << endl;
				goto BEGIN;
			}

			//2) Put File into 2D array
			//2a) Get length & width by counting lines and longest char width/sideways
			//Line/Row counter
			while (getline(graphicIn, line))
			{
				if (line.length() > columnAmt)
					columnAmt = line.length();

				rowAmt++;
			}
			graphicIn.close(); //Since going through file again starts at eof due to getline, i believe.
			cout << "Number of Rows: " << rowAmt << endl;
			cout << "Longest char width: " << columnAmt << endl;
			//2b) Make 2D array to that length*width
			graphicPos = new char*[rowAmt];
			for (int i = 0; i < rowAmt; ++i)
				graphicPos[i] = new char[columnAmt];
			//2c) Input chars into array
			graphicIn.open(graphicName);

			int leftover = 0;
			for (int i = 0; getline(graphicIn, line); i++)
			{
				for (int j = 0; j < columnAmt; j++)
				{
					if (line.length() < columnAmt && j >= line.length()) //>= since at linelength, it read newline. doing this just to have spaces in replacement of NULL chars (due to newline)
					{
						graphicPos[i][j] = ' ';
					}
					else
					{
						graphicPos[i][j] = line[j];
					}
				}
			}

			//print
			printList(rowAmt, columnAmt, graphicPos);

			//3) Close File
			cout << "Done." << endl;
			graphicIn.close();

			goto BEGIN;
		}
		else if (choice == 'F')
		{
			//1) Choose position to start fill
			cout << "Cell to begin Fill:" << endl;
			cout << "Cell Row: ";
			cin >> rowFill;
			cout << "Cell Column: ";
			cin >> columnFill;
			cout << "Symbol: ";
			cin >> symbolFill;
			//2) Fill
			cout << "Filling..." << endl;
			paintBucket(graphicPos, rowFill, columnFill, symbolFill);
			cout << "Done." << endl;
			printList(rowAmt, columnAmt, graphicPos);
		}
		else if (choice == 'S')
		{
			//1) Use ofstream instance for saving onto output
			cout << "Saving..." << endl;
			graphicOut.open(graphicName);
			//2) Save onto file
			for (int i = 0; i < rowAmt; i++)
			{
				for (int j = 0; j < columnAmt; j++)
				{
					graphicOut << graphicPos[i][j];
				}
				graphicOut << '\n';
			}
			// 3) Close ofstream
			graphicOut.close();
			cout << "Done." << endl;
		}
		else
		{
			cout << "Please input 'L', 'F', or 'S'" << endl;
			goto BEGIN;
		}

		cout << "Would you like to quit? ";
		cin >> end;
		system("cls");
		printList(rowAmt, columnAmt, graphicPos); //added after taking screenshots just to show art again if you choose to not quit.
	} while (end != 'Y' && end != 'y');

	return 0;
}