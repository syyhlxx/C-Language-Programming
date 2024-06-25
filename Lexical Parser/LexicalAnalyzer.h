
#include <fstream>
#include <string>
#include "RList.h"
#include "Node.h"
#pragma once

using namespace std;
class LexicalAnalyzer
{

public:
	LexicalAnalyzer(void);
	~LexicalAnalyzer(void);

private:
	// List objects members
	RList m_lListOfKeywords;
	RList m_lListOfTokens;
	RList m_lSymbolTable;
	RList m_lListOfSymbols;
	RList m_lFileOutputList;

	// String members
	string m_szLowerCaseLetters;
	string m_szUpperCaseLetter;
	string m_szReallNumbers;
	string m_szSymbols;
	string m_szPlusMinus;
	string m_szLettersAndNumbers;
	string m_szLetterOrNumberOrSymbol;


public:
	// Call this function to analyze the tokens in the input file.
	void AnalyzeTokens();
	// Call this function to create and print the output files.
	void GenerateOutputFiles(void);
	// Call this function to populate the list of tokens for processing.
	bool PopulateListOfTokens(string Token);

private:
	// Call this function to figure out if the token is a keyword.
	bool IsKeyWord(CNode* pToken);
	// Call this function to figure out if the token consists of an integer.
	bool IsInteger(CNode* pToken);
	// Call this function to initialize the symbol table data structure with symbols.
	bool PopulateListOfKeyWords(void);
	// Call this function to determine if the character is a letter either uppercase or lowercase.
	bool IsLetter(CNode* pNode, int Index =0);
	// Call this function to determine if a character is a symbol.
	bool IsSymbol(CNode* pToken,int Index = 0);
	// Call this function to determine if a token is an identifier.
	bool IsIdentifier(CNode* Token);
	// Call this function to place the symbols in a list object for reference.
	bool PopulateListOfSymbols(void);
	// Call this function to see if the rest of a potential identifier meets the criteria of letter or number....
	bool IsLetterOrNumber(CNode* pToken, int Index = 0);
	// Call this function to strip out of string valid tokens.
	string ParseToken(int iBegin, int iEnd, int Length, CNode* pToken);
	// Call this function to determine if the token is an "Error Token"
	bool IsError(CNode* pToken);
	
};
