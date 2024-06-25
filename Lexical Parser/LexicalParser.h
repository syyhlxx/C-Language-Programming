
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

#pragma once
#include <iostream>
#include <string>
#include "Node.h"
#include "RList.h"
#include "Tokenizer.h"


using namespace std;
class LexicalParser
{
public:
	LexicalParser::LexicalParser(char* cpInput);
	LexicalParser(void);
	~LexicalParser(void);
	// Call this function to print the parse tree.
	void PrintTree(CNode* pExpPtr, int space);
	// Call this function to determine if the input is a valid expression according to the BNF.
	bool IsExpression(CNode* pToken = NULL);
	// Call this function to print the current expression tree.
	void Print(int space =1);
		// Call this function to populate the list of tokens for processing.
	bool PopulateListOfTokens(string Token);
	// Call this function to analyze the tokens in the input file.
	void AnalyzeTokens();
	
private:

	// String members
	string m_szSymbols;
	string m_szPlusMinus;
	string m_szReallNumbers;
	string m_szCurrentFileName;
	string m_szLowerCaseLetters;
	string m_szUpperCaseLetters;
	string m_szLettersAndNumbers;
	string m_szLetterOrNumberOrSymbol;

	// List objects members
	RList m_lSymbolTable;
	RList m_lListOfTokens;
	RList m_lListOfSymbols;
	RList m_lListOfKeywords;
	RList m_lFileOutputList;
	RList m_lExpressionTree;
	RList m_lListOfValidTokens;

	Tokenizer m_Tokenizer;
	const char *m_aIdentifiers[200]; 
	CNode* m_pCurrentToken;

	// Imported Functions
	// Call this function to create and print the output files.
	void GenerateOutputFiles(void);
	// Call this function to figure out if the token is a keyword.
	bool IsKeyWord(CNode* pToken);
	// Call this function to figure out if the token consists of an integer.
	bool IsInteger(CNode* pToken);
	// Call this function to determine if the character is a letter either uppercase or lowercase.
	bool IsLetter(CNode* pNode, int Index =0);
	// Call this function to determine if a character is a symbol.
	bool IsSymbol(CNode* pToken,int Index = 0, bool bParse = true);
	// Call this function to determine if a token is an identifier.
	bool IsIdentifier(CNode* Token);
	// Call this function to place the symbols in a list object for reference.
	bool PopulateListOfSymbols(void);
	// Call this function to initialize the symbol table data structure with symbols.
	bool PopulateListOfKeyWords(void);
	// Call this function to see if the rest of a potential identifier meets the criteria of letter or number....
	bool IsLetterOrNumber(CNode* pToken, int Index = 0);
	// Call this function to strip out of string valid tokens.
	string ParseToken(int iBegin, int iEnd, int Length, CNode* pToken);
	// Call this function to determine if the token is an "Error Token"
	bool IsError(CNode* pToken);
	//Imported Functions

	// Call this function to see if the expression node qualifies as a program.
	bool IsProgram();
	// Call this function to determine if the expression node qualifies as a compound statement.
	bool IsCompoundStatement(CNode* pToken = NULL);
	// Call this function to determine if the expression node qualifies as a statement list.
	bool IsStatementList(CNode* pToken = NULL);
	// Call this function to see if the input pointer points to a valid factor under the BNF.
	bool IsFactor(CNode* pToken = NULL);
	// Call this function to determine if the input is a valid more expression according to the BNF.
	bool IsMoreExpression(CNode* pToken = NULL);
	// Call this function to determine if the expression node qualifies as more statments.
	bool IsMoreStatements(CNode* pToken = NULL);
	// Call this function to determine if the expression node is a statment.
	bool IsStatement(CNode* pToken = NULL);
	// Call this function to determine if the expression node is an operator.
	bool IsOperator(CNode* pToken = NULL);
	// Call this function to initialize the class data members
	void Initialize(void);
	// Call this function to see if the token has any capital letters in it.
	bool HasAllUpperCaseLetters(const char* PotentialError);
	// Call this function to get the next token.
	void GetToken(void);
	// Call this function to explicitly classify each token.
	bool ClassifyToken(CNode* pToken);
	// Call this function to sort an array of strings.
	void LexicalParser::BubbleSort( const char *array[] , int size );

public:
	// Call this function to print the tokenized input of the list of valid tokens.
	void PrintOutputList(void);
	// Call this function to parse the input program.
	bool ParseProgram(void);
	// Call this function to test the token type of the current token.
	bool TestTokenType(CNode::EnumTokenType eTokenType);
	// Call this function to print the parse tree.
	void LexicalParser::PrintParseFile(CNode* pExpPtr,int space, ofstream& fileStream);
	// Call this function to set the file name of each output file.
	void SetCurrentFileName(string szFileName);
	// Call this function to get the name for each output file.
	string GetCurrentFileName(void);
	// Call this function before creating output file.
	string StringFileNameOfSuffix(string fileName);
private:
	// Call this function to populate the array of identifiers.
	bool PopulateIdentifierArray(void);
};
