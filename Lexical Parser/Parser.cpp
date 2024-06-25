// Parser.cpp : Defines the entry point for the console application.

/* BNF
<program> ::= 	<compound stmt> EndFile
<compound stmt>	::= BEGIN <stmt list> END
<stmt list>	::= <stmt> <more stmts>	| <null>
<more stmts> ::= ; <stmt list> | <null>
<stmt> ::=	READ <ident> | WRITE <expr> | SET ident TO <expr> |	IF <expr> <stmt> | WHILE <expr> <stmt>
<expr> ::=	<factor> <more expr>
<more expr>	::=	<operator> <expr> <more expr> |	<null>
<factor> ::=	<integer> |	<ident> | ( <expr> )
<operator> ::=	+ | - | < | > | = 
*/

//#include "stdafx.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "LexicalParser.h"
//#include "LexicalAnalyzer.h"

using namespace std;

string GetInputFile()
{
	int stringLength = 0;
	string szInputFileName = "";
	string szExtension = "";

	cout<< "Please Enter Input File Name..."<< endl;

	cin >> szInputFileName;
	stringLength = (int)szInputFileName.length();
	szExtension = szInputFileName.substr(stringLength-3,stringLength-1);
	if (!szExtension.compare("pmm")) {
	}
	else
		szInputFileName = "Illegal File Name";


	return szInputFileName;
}

int main()
{
	// Declare and initialize local variables.
	int answer = 0;
	int iFileStatus = -1;
	//cout << ">>";
	string inputFileName = "";
	// Create the LexicalParser object.
	LexicalParser Lex;

	/* Get the input file from the user.
	inputFileName = GetInputFile();

	//Determine if the file has the proper extension.
	if (!inputFileName.compare("Illegal File Name")) {
		cout<< "Parsing Not Complete..."<< endl;
		cout<< "Illegal file name..."<< endl;
	}
	// Determine if file exists.
	else if (!inputFileName.compare("")) {
		cout<< "Parsing Not Complete..."<< endl;
		cout<< "File does not exist..."<< endl;
	}

	// If file is valid create a file stream object.
	ifstream fileContainer (inputFileName.c_str());

	LexParser.SetCurrentFileName(inputFileName);

	// Populate the LexicalParser with all the tokens from
	// the file stream.
	if (fileContainer){
		while (fileContainer >> inputFileName) {
			LexParser.PopulateListOfTokens(inputFileName);
		}
		LexParser.AnalyzeTokens();
		if (LexParser.ParseProgram()) {
			cout << "Parsing Complete." << endl;
		}
	}
	else
		cout <<"Parsing Not Complete. \n";*/

	cin >> inputFileName;
	return 0;
}

