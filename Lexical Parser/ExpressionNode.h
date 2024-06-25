/* BNF
<program> ::= 	<compound stmt> EndFile
<compound stmt>	::= BEGIN <stmt list> END
<stmt list>	::= <stmt> <more stmts>	| <null>
<more stmts> ::= ; <stmt list> | <null>
<stmt> ::=	READ <ident> | WRITE <expr> | SET <ident> TO <expr> |	IF <expr> <stmt> | WHILE <expr> <stmt>
<expr> ::=	<factor> <more expr>
<more expr>	::=	<operator> <expr> <more expr> |	<null>
<factor> ::=	<integer> |	<ident> | ( <expr> )
<operator> ::=	+ | - | < | > | = 
*/
#pragma once
#include <string>
#include "Node.h"
#include "RList.h"
#include "Tokenizer.h"

using namespace std;


/*		_ _ _ _ _ _ _ _
		|  |   |   |   |
		|L*| I | T | R*|	
		|__| __|___|_ _|
*/
class ExpressionNode 
{
public:
	ExpressionNode();
	~ExpressionNode(void);

	CNode::EnumTokenType m_ttOperatorValue;
	CNode * m_pExpLeft;
	CNode * m_pExpRight;
	int m_iOperand;
	string m_szKeyword;

	// Call this function to set the operand in an Expression Node.
	void SetOperand(int iOperand);
	// Call this function to set the keyword property of the expression node.
	void SetKeyword(const char* szKeyword);
	// Call this function to set the token type of a new expression node.
	//bool SetTokenType(CNode::EnumTokenType NewType);
};
