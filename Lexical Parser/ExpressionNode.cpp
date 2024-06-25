#include "StdAfx.h"
include "CNode.h"
#include <string>

ExpressionNode::ExpressionNode()
{
	//this->m_ttOperatorValue = CNode::None;
	/*this->m_szKeyword = "";
	this->m_pExpLeft = NULL;
	this->m_pExpRight = NULL;*/
}

ExpressionNode::~ExpressionNode(void)
{
}
// Call this function to set the operand in an Expression Node.
void ExpressionNode::SetOperand(int iOperand)
{
	this->m_iOperand = iOperand;
}

// Call this function to set the keyword property of the expression node.
void ExpressionNode::SetKeyword(const char* cpKeyword)
{
	this->m_szKeyword = cpKeyword;
}

// Call this function to set the token type of a new expression node.
/*bool ExpressionNode::SetTokenType(CNode::EnumTokenType NewType)
{
	bool bWasChanged = false;
	this->m_ttOperatorValue = NewType;
	if ( this->m_ttOperatorValue != NewType)
		bWasChanged = true;
	return true;
}*/
