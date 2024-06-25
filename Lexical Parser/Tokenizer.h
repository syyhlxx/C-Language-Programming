#pragma once

#include "Node.h"
#include "RList.h"


class Tokenizer
{
public:
	Tokenizer(void);
	~Tokenizer(void);
private:
	// This is the token currently under consideration.
	CNode* m_pCurrentToken;
	// This is the current character to examine.
	char m_caLine[90];
	// This is the current character to tokenize.
	char* m_cpCurrentCharacter;
public:
	// Use this function to iterate through an array of characters.
	void SetLine(char* cpInput);
	// Call this function to return the current token.
	CNode* GetCurrentToken(void);
private:

public:
	// Call this function to see if it matches a current token.
	bool MatchTokenType(CNode::EnumTokenType Test);
	// Call this function to determine which type it is.
	bool Test(CNode::EnumTokenType TestType);
	// Call this function to identify the next token.
	void GetToken(void);
	// Call this function to print out the value of the current token.
	void PrintToken(void);
};
