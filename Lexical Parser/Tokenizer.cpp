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

#include "StdAfx.h"
#include ".\tokenizer.h"
#include "Node.h"
#include "string.h"
#include <iostream>

using namespace std;

Tokenizer::Tokenizer(void)
{
}

Tokenizer::~Tokenizer(void)
{
}

// Use this function to iterate through an array of characters.
void Tokenizer::SetLine(char* cpInput)
{
	this->m_cpCurrentCharacter = this->m_caLine;
	while ((*this->m_cpCurrentCharacter++ == *cpInput++) != '\0') {
		this->m_cpCurrentCharacter = this->m_caLine;
	}
}

// Call this function to return the current token.
CNode* Tokenizer::GetCurrentToken(void)
{
	return this->m_pCurrentToken;
}

// Call this function to see if it matches a current token.
bool Tokenizer::MatchTokenType(CNode::EnumTokenType Test)
{
	bool bTokenMatches = false;
	if ( Test == this->m_pCurrentToken->GetTokenType()) {
		bTokenMatches = true;
		//getToken();
	} 
	return bTokenMatches;
}

// Call this function to determine if a token matches the current token.
bool Tokenizer::Test(CNode::EnumTokenType TestType)
{
	bool bIsTypeTestedFor = false;
	return(TestType == this->m_pCurrentToken->GetTokenType());
	return bIsTypeTestedFor;
}

// Call this function to identify the next token.
void Tokenizer::GetToken(void)
{
	if (*this->m_cpCurrentCharacter == '+') {
		this->m_pCurrentToken = new CNode("Plus", -777);
	}
	else if (*this->m_cpCurrentCharacter == '-') {
		this->m_pCurrentToken = new CNode("Minus",-777);
	}
	else if (*this->m_cpCurrentCharacter >= '0' && *this->m_cpCurrentCharacter <= '9') {
		this->m_pCurrentToken = new CNode("Integer",*this->m_cpCurrentCharacter - '0');
	}
	else if (*this->m_cpCurrentCharacter == '\0') {
		this->m_pCurrentToken = new CNode("End",-777);
	}
	else  {
		this->m_pCurrentToken = new CNode("Error",-777);
	}
}



// Call this function to print out the value of the current token.
void Tokenizer::PrintToken(void)
{
		if (this->m_pCurrentToken->GetTokenType() == CNode::Error) {
			cout << "Error" << endl;
		}
		else if (this->m_pCurrentToken->GetTokenType() == CNode::Integer) {
			cout << "Integer: Value =" << this->m_pCurrentToken->GetId()<< endl;
		}
		else if (this->m_pCurrentToken->GetTokenType() == CNode::Plus) {
			cout << "Plus" << endl;
		}
		else if (this->m_pCurrentToken->GetTokenType() == CNode::Minus) {
			cout << "Minus" << endl;
		}

		else if (this->m_pCurrentToken->GetTokenType() == CNode::End) {
			cout << "End" << endl;
		}
}
