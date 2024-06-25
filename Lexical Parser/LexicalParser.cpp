/* BNF
<program> ::= 	<compound stmt> EndFile
<compound stmt>	::= BEGIN <stmt list> END
<stmt list>	::= <stmt> <more stmts>	| <null>
<more stmts> ::= ; <stmt list> | <null>
<stmt> ::=	READ <ident> | WRITE <expr> | SET <ident> TO <expr> |	IF <expr> <stmt> | WHILE <expr> <stmt> | <compound stmnt>
<expr> ::=	<factor> <more expr>
<more expr>	::=	<operator> <factor> <more expr> | <null>
<factor> ::= <integer> | <ident> | ( <expr> )
<operator> ::=	+ | - | < | > | = 
*/


#define ArraySize 200
#include "StdAfx.h"
#include ".\lexicalparser.h"
#include "Tokenizer.h"
#include "Node.h"
#include "RList.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>


using namespace std;

LexicalParser::LexicalParser(void)
{
	this->Initialize();
}

LexicalParser::LexicalParser(char* cpInput)
{
	this->m_Tokenizer.SetLine(cpInput);
	this->Initialize();

}

// Call this function to initialize the class data members
void LexicalParser::Initialize(void)
{
	m_szLetterOrNumberOrSymbol = "abcdefghijklmnopqrstuvwxyz0123456789()<>-+;=,";
	m_szLowerCaseLetters = "abcdefghijklmnopqrstuvwxyz";
	m_szUpperCaseLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	m_szLettersAndNumbers = "abcdefghijklmnopqrstuvwxyz0123456789";
	m_szCurrentFileName = "";
	m_szReallNumbers = "0123456789";
	m_szSymbols = "()<>-+;=,";
	m_szPlusMinus = "-+";

	this->PopulateListOfKeyWords();
	this->PopulateListOfSymbols();
}
LexicalParser::~LexicalParser(void)
{
}


// Call this function to see if the input pointer points to a valid factor under the BNF.
// <factor> ::=	<integer> |	<ident> | ( <expr> )
// <integer> ::= 0|1|2|3|4|5|6|7|8|9|0
// <expr> ::=	"(" <factor> <more expr> ")"

bool LexicalParser::IsFactor(CNode* pNode)
{
	bool bIsFactor = false;
	CNode* pRootNode = NULL;
	// Check to see if current token is an integer, identifier or expression within braces. 
	if (this->TestTokenType(CNode::Integer)) {
		pRootNode = new CNode("", 0);
		pRootNode->SetTokenType((CNode::Integer));
		pRootNode->SetId(atoi(this->m_pCurrentToken->GetName()));

		if (pNode) {
			pNode->SetRight(pRootNode);
			this->m_lExpressionTree.Add(pNode);
		}
		else
		this->m_lExpressionTree.Add(pRootNode);
		
		bIsFactor = true; 
	}
	else if (this->TestTokenType(CNode::Identifier)) {
		pRootNode = new CNode("", 0);
		pRootNode->SetTokenType((CNode::EnumTokenType)this->m_pCurrentToken->GetTokenType());
		pRootNode->SetLeft(this->m_lSymbolTable.GetNodeByName(this->m_pCurrentToken->GetName()));
		if (pNode) {
			pNode->SetLeft(pRootNode);
		}
		else if (pNode)
			pNode->SetRight(pRootNode);

		this->m_lExpressionTree.Add(pNode);
		
		bIsFactor = true; 
	}
	else if (!strcmp(this->m_pCurrentToken->GetName(),"LeftParen")){
			this->GetToken();
				if(this->IsExpression(pNode)) 
					if (!strcmp(this->m_pCurrentToken->GetName(),"RightParen"))
							bIsFactor = true; 
	}
	return bIsFactor;
}

// Call this function to determine if the input is a valid more expression according to the BNF.
// <more expr>	::=	<operator> <expr> <more expr> |	<null>
// <operator> ::=	+ | - | < | > | =
// <factor> ::= <integer> |	<ident> | ( <expr> )
bool LexicalParser::IsMoreExpression(CNode* pNode)
{
	bool bIsMoreExpression = true;
	CNode* Factor = NULL;;
	CNode* Expression = NULL;

	/*
				       Simple Case: 

							+ operator
							/\
						 exp  factor
	*/

	// Check to see if first part of string is an operator.
	// Try to match the current token's type with either plus or minus...
//	if (this->m_Tokenizer.MatchTokenType(CNode::Plus) || this->m_Tokenizer.MatchTokenType(CNode::Minus)) {
	if(this->TestTokenType(CNode::Plus) || this->TestTokenType(CNode::Minus) || this->TestTokenType(CNode::GreaterThan)
		||this->TestTokenType(CNode::LessThan)|| this->TestTokenType(CNode::Equals))
	{
		CNode::EnumTokenType Operator = this->m_pCurrentToken->GetTokenType();

		// Create a temporary expression node to build result expression.
		Expression = new CNode("",0);
		Expression->SetTokenType(Operator);
		// Advance to the next token.
		this->GetToken();

		// If it is, check to see if next part of string is an expression which is a factor
		// and a more expression.
		if (IsExpression(Expression)) {
			// If so, continue and check to see if next part of string is a more expression.
			if (IsMoreExpression()) {
				bIsMoreExpression = true;
			}
		}
	}
	else if(!strcmp(this->m_pCurrentToken->GetName(),"ENDKEY"))
	{
		bIsMoreExpression = true;
	}

	return bIsMoreExpression;
}

// Call this function to determine if the input is a valid expression according to the BNF.
// <expr> ::=	<factor> <more expr>
// <factor> ::=	<integer> |	<ident> | ( <expr> )
// <more expr>	::=	<operator> <expr> <more expr> |	<null>

bool LexicalParser::IsExpression(CNode* pNode)
{
	bool bIsExpression = false;
	if (IsFactor(pNode)) {
		this->GetToken();
		if (IsMoreExpression(pNode)) {
			bIsExpression = true;
		}
	}
	return bIsExpression;
}


// Call this function to see if the expression node qualifies as a program.
// <program> ::= <compound stmt> EndFile
bool LexicalParser::IsProgram()
{
	bool bIsProgram = false;
	if (this->IsCompoundStatement()) {
		bIsProgram = true;
	}
	else
		cout<< "Not Valid Program." << endl;
	return bIsProgram;
}

// Call this function to determine if the expression node qualifies as a compound statement.
// <compound stmt>	::= BEGIN <stmt list> END
// <stmt list>	::= <stmt> <more stmts>	| <null>
// <more stmts> ::= ; <stmt list> | <null>
// <stmt> ::=	READ <ident> | WRITE <expr> | SET <ident> TO <expr> | IF <expr> <stmt> | WHILE <expr> <stmt>*/

bool LexicalParser::IsCompoundStatement(CNode* pNode)
{
	bool bIsCompoundStatement = false;
	CNode* pCompoundStatement = NULL;

	if(!strcmp(this->m_pCurrentToken->GetName(),"BEGINKEY"))
	{
		this->GetToken();
		if(this->IsStatementList())
			if(!strcmp(this->m_pCurrentToken->GetName(), "ENDKEY"))
				bIsCompoundStatement= true;
	}

	return bIsCompoundStatement;
}


// Call this function to determine if the expression node qualifies as a statement list.
// <stmt list>	::= <stmt> <more stmts>	| <null>
bool LexicalParser::IsStatementList(CNode* pNode)
{
	bool bIsStatementList = true;
	if(this->IsStatement()){
		if(strcmp(this->m_pCurrentToken->GetName(),"Semicolon"))
			this->GetToken();
		if(this->IsMoreStatements())
		{
			this->GetToken();
			bIsStatementList = true;
		}
	}
	else if(!strcmp(this->m_pCurrentToken->GetName(),"BEGINKEY")){
		this->GetToken();
		bIsStatementList = true;
	}
	return bIsStatementList;
}

// Call this function to determine if the expression node qualifies as more statements.
// <more stmts> ::= ; <stmt list> | <null>
// <stmt list>	::= <stmt> <more stmts>	| <null>
// NULL

bool LexicalParser::IsMoreStatements(CNode* pNode)
{
	CNode* pRootNode = NULL;
	bool bIsMoreStatements = false;
	if(!strcmp(this->m_pCurrentToken->GetName(),"Semicolon")){
		pRootNode = new CNode("",0);
		pRootNode->SetTokenType(CNode::Semicolon);
		this->m_lExpressionTree.Add(pRootNode);
		
		this->GetToken();
		if (this->IsStatementList())
			this->GetToken();
			bIsMoreStatements = true;
	}

	return bIsMoreStatements;
}

// Call this function to determine if the expression node is a statement.
// <stmt> ::=	READ <ident> | WRITE <expr> | SET <ident> TO <expr> |	IF <expr> <stmt> |
// WHILE <expr> <stmt> | <compound stmnt>


/*
		Simple case: 
						/\
					Read  a
*/

bool LexicalParser::IsStatement(CNode* pNode)
{
	bool bIsStatement = false;
	CNode* pRootNode = NULL;
	CNode* pSecondaryNode = NULL;

	if (!strcmp(this->m_pCurrentToken->GetName(),"READKEY")) {
		this->GetToken();
		if (this->TestTokenType(CNode::Identifier)) 
		{
			pRootNode = new CNode("",0);
			pRootNode->SetTokenType(CNode::Read);
			pRootNode->SetLeft(this->m_lSymbolTable.GetNodeByName(this->m_pCurrentToken->GetName()));
			this->m_lExpressionTree.Add(pRootNode);
			/*if (Result) 
			pRootNode->SetPrevious(Result);
			Result = pRootNode;*/
		}
			bIsStatement = true;
	}
	else if (!strcmp(this->m_pCurrentToken->GetName(),"WRITEKEY")) {
			this->GetToken();
			pRootNode = new CNode("",0);
			pRootNode->SetTokenType(CNode::Write); 
			//pRootNode->SetLeft(this->m_lSymbolTable.GetNodeByName(this->m_pCurrentToken->GetName()));
			//this->m_lExpressionTree.Add(pRootNode);
			if (this->IsExpression(pRootNode)) 
				bIsStatement = true;
	}
	else if (!strcmp(this->m_pCurrentToken->GetName(),"SETKEY")) {
		this->GetToken();
		pRootNode = new CNode("",0);
		pRootNode->SetTokenType(CNode::Set); 
		if (this->TestTokenType(CNode::Identifier)) {
			pRootNode->SetLeft(this->m_lSymbolTable.GetNodeByName(this->m_pCurrentToken->GetName()));
				this->m_lExpressionTree.Add(pRootNode);
				this->GetToken();
		}
		if (!strcmp(this->m_pCurrentToken->GetName(),"TOKEY")) {
			pSecondaryNode = new CNode("",0);
			pSecondaryNode->SetTokenType(CNode::To); 
			this->GetToken();
			if (this->IsExpression(pSecondaryNode)) 
				bIsStatement = true;
		}
	}
	else if (!strcmp(this->m_pCurrentToken->GetName(),"IFKEY")) {
		pRootNode = new CNode("",0);
		pRootNode->SetTokenType(CNode::If); 
		this->GetToken();
		if (this->IsExpression(pRootNode))
			if(this->IsStatement(pRootNode))
			bIsStatement = true;
	}
	else if (!strcmp(this->m_pCurrentToken->GetName(),"WHILEKEY")) {
		pRootNode = new CNode("",0);
		pRootNode->SetTokenType(CNode::While); 
		this->GetToken();
		if (this->IsExpression(pRootNode)) 
			//this->GetToken();
			if (this->IsStatement(pRootNode)) 
				bIsStatement = true;
	}
	else if (this->IsCompoundStatement()) {
		bIsStatement = true;
	}

	return bIsStatement;
}

// Call this function to determine if the expression node is an operator.
// <operator> ::=	+ | - | < | > | =
bool LexicalParser::IsOperator(CNode* pNode)
{
	bool bIsIsOperator = false;
	return bIsIsOperator;
}

// Call this function to print the parse tree.
/*void LexicalParser::PrintTree(CNode* exprPtr, int space)
{
	if (exprPtr) {
		//cout << "BEGIN" << endl;
		for (int loop =1; loop <=space; loop++) {
			cout << " ";
			if (exprPtr->m_szKeyword.size()!=0) 
				cout << exprPtr->m_szKeyword << endl;
			if (exprPtr->m_ttOperatorValue &&exprPtr->m_ttOperatorValue == CNode::Integer) {
				cout << "Integer: Value =" << exprPtr->m_iOperand << endl;
			}
			else if (exprPtr->m_ttOperatorValue&&exprPtr->m_ttOperatorValue  == CNode::Plus) {
				cout << "Plus" << endl;
			}
			else if (exprPtr->m_ttOperatorValue&&exprPtr->m_ttOperatorValue  == CNode::Minus) {
				cout << "Minus" << endl;
			}
			PrintTree(exprPtr->m_pExpLeft, space+2);
			PrintTree(exprPtr->m_pExpRight, space+2);
		}
		//cout << "END" << endl;
	}
}*/

// Call this function to print the parse tree.
void LexicalParser::PrintTree(CNode* pExpPtr,int space)
{

		//cout << " ";

	if (pExpPtr) {
		if (pExpPtr->GetName()!=NULL) {
			cout << setw (space);
			cout <<  pExpPtr->GetName() << endl;
		}

		if (pExpPtr->GetTokenType()== CNode::Integer){
			cout << setw (space);
			cout << "Integer: Value =" << pExpPtr->GetId() << endl;
		}

		if (pExpPtr->GetTokenType() == CNode::Plus) {
			cout << setw (space);
			cout << "Plus" << endl;
		}

		else if (pExpPtr->GetTokenType() == CNode::Minus) {
			cout << setw (space);
			cout << "Minus" << endl;
		}
		else if (pExpPtr->GetTokenType() == CNode::Semicolon) {
			cout << setw (space);
			cout << "Semicolon" << endl;
		}
		else if (pExpPtr->GetTokenType() == CNode::Read) {
			cout << setw (space);
			cout << "Read" << endl;
		}
		else if (pExpPtr->GetTokenType() == CNode::Set) {
			cout << setw (space);
			cout << "Set" << endl;
		}
		else if (pExpPtr->GetTokenType() == CNode::To) {
			cout << setw (space);
			cout << "To" << endl;
		}
		else if (pExpPtr->GetTokenType() == CNode::While) {
			cout << setw (space);
			cout << "While" << endl;
		}
		else if (pExpPtr->GetTokenType() == CNode::Write) {
			cout << setw (space);
			cout << "Write" << endl;
		}
		else if (pExpPtr->GetTokenType() == CNode::If) {
			cout << setw (space);
			cout << "If" << endl;
		}
		PrintTree(pExpPtr->GetLeft(), space+2);
		PrintTree(pExpPtr->GetRight(), space +2);
	}
}


// Call this function to print the parse tree.
void LexicalParser::PrintParseFile(CNode* pExpPtr,int space, ofstream& fileStream)
{

	//cout << " ";

	if (pExpPtr) {
		if (pExpPtr->GetName()!=NULL) {
			fileStream << setw(space);
			fileStream << pExpPtr->GetName() << endl;
		}

		if (pExpPtr->GetTokenType()== CNode::Integer) {
			fileStream << setw(space);
			fileStream << "Integer: Value =" << pExpPtr->GetId() << endl;
		}

		if (pExpPtr->GetTokenType() == CNode::Plus) {
			fileStream << setw(space);
			fileStream << "Plus" << endl;
		}

		else if (pExpPtr->GetTokenType() == CNode::Minus) {
			fileStream << setw(space);
			fileStream << "Minus" << endl;
		}
		else if (pExpPtr->GetTokenType() == CNode::Semicolon) {
			fileStream << setw(space);
			fileStream << "Semicolon" << endl;
		}
		else if (pExpPtr->GetTokenType() == CNode::Read) {
			fileStream << setw(space);
			fileStream << "Read" << endl;
		}
		else if (pExpPtr->GetTokenType() == CNode::Set) {
			fileStream << setw(space);
			fileStream << "Set" << endl;
		}
		else if (pExpPtr->GetTokenType() == CNode::To) {
			fileStream << setw(space);
			fileStream << "To" << endl;
		}
		else if (pExpPtr->GetTokenType() == CNode::While) {
			fileStream << setw(space);
			fileStream << "While" << endl;
		}
		else if (pExpPtr->GetTokenType() == CNode::Write) {
			fileStream << setw(space);
			fileStream << "Write" << endl;
		}
		else if (pExpPtr->GetTokenType() == CNode::If) {
			fileStream << setw(space);
			fileStream << "If" << endl;
		}
		PrintParseFile(pExpPtr->GetLeft(), space+2, fileStream);
		PrintParseFile(pExpPtr->GetRight(), space +2, fileStream);
	}
}


// Call this function to print the current expression tree.
void LexicalParser::Print(int space)
{
	CNode* pExpPtr = NULL;
	pExpPtr = this->m_lExpressionTree.GetHead();

	for (;pExpPtr;pExpPtr= pExpPtr->GetNext()) 
		this->PrintTree(pExpPtr,space);
}

	
// Call this function to place the symbols in a list object for reference.
bool LexicalParser::PopulateListOfSymbols(void)
{
	bool bListOfSymbolsCreated = false;
	string line;
	int counter = 0;

   ifstream symbolfile ("symbols.txt");
   if (symbolfile.is_open())
   {
	   while (! symbolfile.eof() )
	   {
		   getline (symbolfile,line);
		   CNode* pNode = new CNode(line.c_str(),counter++);
		   this->m_lListOfSymbols.Add(pNode);
		   //cout << line << endl;
	   }
    symbolfile.close();
  }

  else cout << "Unable to open file"; 

	if (this->m_lListOfSymbols.GetCount()>0) {
		bListOfSymbolsCreated = true;
	}
	return bListOfSymbolsCreated;
}

// Call this function to initialize the symbol table data structure with symbols.
bool LexicalParser::PopulateListOfKeyWords(void)
{
	bool bKeyWordTableCreated = false;
	string line;
	int counter = 0;

   ifstream keywordfile ("keywords.txt");
   if (keywordfile.is_open())
   {
	   while (! keywordfile.eof() )
	   {
		   getline (keywordfile,line);
		   CNode* pNode = new CNode(line.c_str(),counter++);
		   this->m_lListOfKeywords.Add(pNode);
		   //cout << line << endl;
	   }
    keywordfile.close();
  }

  else cout << "Unable to open file"; 

	/*CNode* pNode = new CNode("BEGIN",0);
	this->m_lListOfKeywords.Add(pNode);

	pNode = new CNode("END",0);
	this->m_lListOfKeywords.Add(pNode);

	pNode = new CNode("IF",0);
	this->m_lListOfKeywords.Add(pNode);

	pNode = new CNode("READ",0);
	this->m_lListOfKeywords.Add(pNode);

	pNode = new CNode("SET",0);
	this->m_lListOfKeywords.Add(pNode);

	pNode = new CNode("TO",0);
	this->m_lListOfKeywords.Add(pNode);

	pNode = new CNode("WHILE",0);
	this->m_lListOfKeywords.Add(pNode);

	pNode = new CNode("WRITE",0);
	this->m_lListOfKeywords.Add(pNode);
	*/
	if (this->m_lListOfKeywords.GetCount()>0) {
		bKeyWordTableCreated = true;
	}

	return bKeyWordTableCreated;
}

// Call this function to populate the list of tokens for processing.
bool LexicalParser::PopulateListOfTokens(string Token)
{
	bool bTokenListPopulated = false;

	CNode* pNode = new CNode(Token.c_str(),-777);
	this->m_lListOfTokens.Add(pNode);
	bTokenListPopulated = true;

	return bTokenListPopulated;
}

// Call this function to analyze the tokens in the input file.
void LexicalParser::AnalyzeTokens()
{
	CNode* pNode = NULL;
	pNode = this->m_lListOfTokens.GetHead();
	if (pNode) {
		for (;pNode;pNode = pNode->GetNext()) 
			this->ClassifyToken(pNode);
	}
	//this->PrintOutputList();
}


// Call this function to determine if the character is a letter either uppercase or lowercase.
bool LexicalParser::IsLetter(CNode* pToken,int iIndex)
{
	bool bIsLetter = false;
	int stringIndex = iIndex;
	const char* result = NULL;
		result =strchr(this->m_szLowerCaseLetters.c_str(), pToken->GetName()[stringIndex]);
		if (result!=NULL) {
			bIsLetter = true;
		}
	return bIsLetter;
}

// Call this function to figure out if the token contains  an integer.
bool LexicalParser::IsInteger(CNode* pToken)
{
	// Declare and initialize local variables
	CNode* pNode = NULL;
	bool bIsInteger = false;
	bool bTokenParsed = false;
	string szResultString= "";
	int stringIndex = -1;
	int stringLength = 0;

	const char* result = NULL;

	stringLength = (int)strlen(pToken->GetName());
	result = strchr(this->m_szPlusMinus.c_str(), pToken->GetName()[stringIndex]);
	
	if (result !=NULL)
		 stringIndex++;

		do {		
			stringIndex++;
		} while(result = strchr(this->m_szReallNumbers.c_str(),pToken->GetName()[stringIndex]));

		if (stringIndex >0 && stringIndex!=stringLength+1) {
			szResultString = this->ParseToken(0,stringIndex,stringLength,pToken);
			bIsInteger = true;
			bTokenParsed = true;
		}

		if (stringIndex >0 && stringIndex==stringLength+1) {
			szResultString = pToken->GetName();
			bIsInteger = true;
		}

		// If token is properly identified as an Identifier add it to the list of
		// valid tokens.
		if (bIsInteger) {
			pNode = new CNode(szResultString.c_str(),pToken->GetId());
			pNode->SetTokenType(CNode::Integer);
			this->m_lListOfValidTokens.Add(pNode);
			}
		if (bTokenParsed) 
			this->ClassifyToken(pToken);

	return bIsInteger;
}

// Call this function to figure out if the token is a keyword.
bool LexicalParser::IsKeyWord(CNode* pToken)
{
	bool bIsKeyWord = false;
	bool bTokenParsed = false;
	bool bHasUpperCaseLetters = true;
	int letterIndex = -1;
	int stringLength = 0;
	CNode* pNode = NULL;
	CNode* pKeyToken = NULL;
	const char* result = "";
	string szSuffix = "KEY";
	string szResultString = "";

	stringLength = (int)strlen(pToken->GetName());

	if(stringLength >1)
	{
	do{
		letterIndex++;
		result =strchr(this->m_szUpperCaseLetters.c_str(), pToken->GetName()[letterIndex]);
	}	while(result!= NULL);

	if (letterIndex > stringLength) {
		szResultString = pToken->GetName();
		bHasUpperCaseLetters = true;
	}
	else
	{
	szResultString = this->ParseToken(0,letterIndex,stringLength,pToken);
	}

	pNode = this->m_lListOfKeywords.GetNodeByName(pToken->GetName());
	if (pNode && !strcmp(pNode->GetName(),szResultString.c_str())) 
	{
		// Add the valid token to the list of valid tokens for parsing.
		szSuffix = szResultString + szSuffix; 
		pKeyToken = new CNode(szSuffix.c_str(),pToken->GetId());
		this->m_lListOfValidTokens.Add(pKeyToken);
		bIsKeyWord = true;
	}

	if(bTokenParsed)
		this->ClassifyToken(pToken);
	}
	return bIsKeyWord;
}

// Call this function to determine if a character is a symbol.
bool LexicalParser::IsSymbol(CNode* pToken, int iIndex, bool bParse)
{
	bool bIsSymbol = false;
	CNode::EnumTokenType Operator = CNode::None;
	CNode* pNode = NULL;
	char* result = NULL;

	int isDigit = 0;
	int isMinus = 0;
	int stringIndex = iIndex;
	int stringLength = (int)strlen(pToken->GetName());

	string szIdString = "";
	string szNewString = "";
	string szParseString = "";
	

switch(pToken->GetName()[stringIndex]) {
   case '-':
	   // Minus Sign	
	   Operator = CNode::Minus;
	   szIdString = "Minus";
	   bIsSymbol = true;
	   break;
   case '+':
	   // Plus Sign	
	   Operator = CNode::Plus;
	   szIdString = "Plus";
	   bIsSymbol = true;
	   break;
   case ';':
	   // Semicolon
	   Operator = CNode::Semicolon;
	   szIdString = "Semicolon";
	   bIsSymbol = true;
	   break;
   case '=':
	   // Equals Sign
	   Operator = CNode::Equals;
	   szIdString = "Equals";
	   bIsSymbol = true;
	   break;
   case ')':
	   szIdString = "RightParen";
	   bIsSymbol = true;
	   break;

   case '(':
	   szIdString = "LeftParen";
	   bIsSymbol = true;
	   break;
   case '>':
	   // Less Than
	   Operator = CNode::LessThan;
	   szIdString = "Less Than";
	   bIsSymbol = true;
	   break;

   case '<':
	   // Greater Than
	   Operator = CNode::GreaterThan;
	   szIdString = "Greater Than";
	   bIsSymbol = true;
	   break;
	}

	if (!strcmp(&pToken->GetName()[stringIndex],"-")) {
		// Minus Sign
		Operator = CNode::Minus;
		szIdString = "Minus";
		bIsSymbol = true;
	}

	
	// Regardless of whether first character is a symbol or not, if it's length
	// is not one, we tokenize the rest of the string for reparsing.

		if (bIsSymbol && bParse) {
			szParseString = pToken->GetName();
			if (stringLength!= 1){ 
				szParseString = this->ParseToken(0,1,stringLength,pToken);
				// Add the identified token to the list of valid tokens.
				pNode = new CNode(szIdString.c_str(),0);
				pNode->SetTokenType(Operator);
				//this->m_lFileOutputList.Add(pNode);
				this->m_lListOfValidTokens.Add(pNode);
				this->ClassifyToken(pToken);
			}
			else{
				pNode = new CNode(szIdString.c_str(),0);
				pNode->SetTokenType(Operator);
				//this->m_lFileOutputList.Add(pNode);
				this->m_lListOfValidTokens.Add(pNode);
			}
		}
		else{
			/*szParseString = this->ParseToken(0,1,stringLength,pToken);
			pNode = new CNode(szParseString.c_str(),0);
			this->IsError(pNode);
			this->ClassifyToken(pToken);
			bIsSymbol = true;*/
			
		}

	return bIsSymbol;
}


// Call this function to determine if a token is an identifier.
bool LexicalParser::IsIdentifier(CNode* pToken)
{
	//Identifier: [a-z]([a-z0-9])*

	bool bIsIdentifier = false;
	bool bTokenParsed = false;
	//int tokenIndex = 0;
	int stringLength = 0;
	int stringIndex = 0;
	const char* result = NULL;
	string szResultString = "";
	CNode* pNode = NULL;
	CNode* pSymbolTableNode = NULL;
	CNode* pSearchResult = NULL;

	stringLength = (int)strlen(pToken->GetName());

	result = strchr(this->m_szLowerCaseLetters.c_str(),pToken->GetName()[stringIndex]);

	if (result) {

	do {
		stringIndex++;
	} while(result = strchr(this->m_szLettersAndNumbers.c_str(),pToken->GetName()[stringIndex]));

	if (stringIndex >0 && stringIndex!=stringLength+1) {
	szResultString = this->ParseToken(0,stringIndex,stringLength,pToken);
	bIsIdentifier = true;
	bTokenParsed = true;
	}

	if (stringIndex >0 && stringIndex==stringLength+1) {
		szResultString = pToken->GetName();
		bIsIdentifier = true;
	}

	// If token is properly identified as an Identifier add it to the list of
	// valid tokens.
	 if (bIsIdentifier) {
		 pNode = new CNode(szResultString.c_str(),pToken->GetId());
		 pNode->SetTokenType(CNode::Identifier);
		 pSymbolTableNode = new  CNode(szResultString.c_str(),pToken->GetId());
		 pSymbolTableNode->SetTokenType(CNode::Identifier);
		 this->m_lListOfValidTokens.Add(pNode);

		 // Only add this identifier to the symbol table if it's not already there.
		 pSearchResult = this->m_lSymbolTable.GetNodeByName(pSymbolTableNode->GetName());
		 if (!pSearchResult) {
			 this->m_lSymbolTable.Add(pSymbolTableNode);
			}
		 if (bTokenParsed) 
			 this->ClassifyToken(pToken);
	 }
	}

	return bIsIdentifier;
}


// Call this function to see if the rest of a potential identifier meets the criteria of letter or number....
bool LexicalParser::IsLetterOrNumber(CNode* pToken, int iIndex)
{
	int stringIndex = iIndex;
	int stringLength = 0;
	const char* result = NULL;
	bool bIsLetterOrNumber = false;

	stringLength = (int)strlen(pToken->GetName());

	do {
		stringIndex++;
	} while(result = strchr(this->m_szLettersAndNumbers.c_str(),pToken->GetName()[stringIndex]));

	if (result == NULL && stringIndex == stringLength+1) 
		bIsLetterOrNumber = true;
	else if (stringIndex > 0 ){
		result = strchr(";",pToken->GetName()[stringIndex]);
		if (result) 
		bIsLetterOrNumber = true;
	}

	return bIsLetterOrNumber;
}


// This function is called by other functions that need to identify parts of a token.
// Any part of the token that is not identified will be placed in a substring a put back
// into the list of tokens for re-examination.
string LexicalParser::ParseToken(int iBegin, int iEnd, int Length, CNode* pToken)
{
	CNode* pNode = NULL;
	string szRemainderString;
	string szValidString;
	string szParseString = pToken->GetName();

	szValidString = szParseString.substr(iBegin,iEnd);
	szRemainderString = szParseString.substr(iEnd,Length-iEnd);
	pToken->SetName(szRemainderString.c_str());
	//pNode = new CNode(szRemainderString.c_str(),0);
	//this->m_lListOfTokens.Add(pNode);
	
	return szValidString;

}
// Call this function to determine if the token is an "Error Token"
bool LexicalParser::IsError(CNode* pToken)
{
	bool bIsErrorToken = false;
	int stringLength = (int)strlen(pToken->GetName());
	string szError = "Error : ";
	string szParseString = "";
	CNode* pNode = NULL;


	if (!isalpha(pToken->GetName()[0]) ) {
		szParseString = this->ParseToken(0,1,stringLength,pToken);
			szError+= szParseString;
			pNode = new CNode(szError.c_str(),-1);
			//this->m_lFileOutputList.Add(pNode);
			this->m_lListOfValidTokens.Add(pNode);
			this->ClassifyToken(pToken);
	}
	else
	{
		szError+= pToken->GetName();
		pNode = new CNode(szError.c_str(),-1);
		//this->m_lFileOutputList.Add(pNode);
		this->m_lListOfValidTokens.Add(pNode);
	}
	
	return bIsErrorToken;
}

// Call this function to create and print the output files.
void LexicalParser::GenerateOutputFiles(void)
{
	CNode* pNode  = NULL;
	int indexMax = this->m_lExpressionTree.GetCount();
	int symbolCount = this->m_lSymbolTable.GetCount();
	string szTextSuffix = ".txt";
	string ParseFileName = "";

	string szSymbolSuffix = ".sym";
	string SymbolTableFileName = "";

	if (this->GetCurrentFileName()!= "") {
		 ParseFileName  = this->StringFileNameOfSuffix(this->GetCurrentFileName())+= szTextSuffix; 
		 SymbolTableFileName = this->StringFileNameOfSuffix(this->GetCurrentFileName()) += szSymbolSuffix;
	}

	ofstream OutputToken(ParseFileName.c_str());
	pNode = this->m_lExpressionTree.GetHead();
	
	for (;pNode;pNode= pNode->GetNext())
		this->PrintParseFile(pNode,1,OutputToken);
		OutputToken.close();

	ofstream OutputSymbolTable(SymbolTableFileName.c_str());
	this->PopulateIdentifierArray();

	for (int index =0;index <symbolCount;index++) 
		OutputSymbolTable<< this->m_aIdentifiers[index] <<endl;
		OutputSymbolTable.close();
}



// Call this function to print the tokenized input of the list of valid tokens.
void LexicalParser::PrintOutputList(void)
{
	CNode* pNode = NULL;
	pNode = this->m_lListOfValidTokens.GetHead();
	for(;pNode;pNode = pNode->GetNext())
	{
		cout<< pNode->GetName() << endl;
	}
}

// Call this function to see if the token has any capital letters in it.
bool LexicalParser::HasAllUpperCaseLetters(const char* szPotentialError)
{
	bool bHasUpperCaseLetters = false;
	int letterIndex = 0;
	const char* result = NULL;
	int stringLength = (int)strlen(szPotentialError);

	do{
		result =strchr(this->m_szUpperCaseLetters.c_str(), szPotentialError[letterIndex]);
		letterIndex++;
	}	while(result== NULL);

	if (letterIndex  < stringLength-1) {
		bHasUpperCaseLetters = true;
	}
	else
	{

	}

	return bHasUpperCaseLetters;
}

// Call this function to get the next token.
void LexicalParser::GetToken(void)
{
	if ( strcmp(this->m_pCurrentToken->GetName(),"ENDKEY")) 
	 this->m_pCurrentToken = this->m_pCurrentToken->GetNext();
}

// Call this function to parse the input program.
bool LexicalParser::ParseProgram(void)
{
	bool bProgramParsed = false;

	CNode* pNextToken = NULL;
	// Initialize current token to the first token in the list of
	// valid tokens.
	if (this->m_lListOfValidTokens.GetCount()) {
		this->m_pCurrentToken = this->m_lListOfValidTokens.GetHead();
		if(this->IsProgram()){
			this->Print();
			this->GenerateOutputFiles();
			bProgramParsed = true;
		}
	}
	else
		cout<< "xXxError Empty Token ListxXx";


	return bProgramParsed;
}

// Call this function to explicitly classify each token.
bool LexicalParser::ClassifyToken(CNode* pToken)
{
	bool bTokenIdentified = true;
	if (this->IsIdentifier(pToken)) {
		bTokenIdentified = true;
	}
	else if (this->IsKeyWord(pToken)) {
		bTokenIdentified = true;
	}
	else if (this->IsSymbol(pToken)) {
		bTokenIdentified = true;
	}
	else if (this->IsInteger(pToken)) {
		bTokenIdentified = true;
	}
	else if(this->IsError(pToken)) 
		bTokenIdentified = true;
	return bTokenIdentified;
}

// Call this function to test the token type of the current token.
bool LexicalParser::TestTokenType(CNode::EnumTokenType eTokenType)
{
	bool bMatches = false;
	if(this->m_pCurrentToken)
	if (eTokenType == this->m_pCurrentToken->GetTokenType())
		bMatches = true;
	return bMatches;
}



// Call this function to set the file name of each output file.
void LexicalParser::SetCurrentFileName(string szFileName)
{
	this->m_szCurrentFileName = szFileName;
}

// Call this function to get the name for each output file.
string LexicalParser::GetCurrentFileName(void)
{
	return this->m_szCurrentFileName;
}

// Call this function before creating output file.
string LexicalParser::StringFileNameOfSuffix(string fileName)
{
	int stringLength = 0;
	int index=0;
	string szResult = "";

	stringLength= (int) strlen(fileName.c_str());
	index = (int)fileName.find(".pmm",0);
	szResult = fileName.erase(index,4);

	return szResult;
}
void LexicalParser::BubbleSort( const char *array[] , int size )
{

	int result;
	//Performs a run through number of strings
	for ( int pass = 0; pass < size - 1 ; ++pass ){
		//Runs through each string for compare
		for ( int j = 0; j < size - 1 - pass; ++j ){
			//Perform string compare and return value store as result
			result = strcmp (array[j], array[j+1]);
			//If value is less than 0 then perform swap function to rearrange
			if (result > 0)
				swap ( array[j] , array[j+1] );
		}
	}
}

// Call this function to populate the array of identifiers.
bool LexicalParser::PopulateIdentifierArray(void)
{
	bool bWasPopulated = false;
	CNode* pNode = NULL;
	int symbolCount = this->m_lSymbolTable.GetCount();
	int index = 0;
	if (symbolCount< 200) {
		pNode = this->m_lSymbolTable.GetHead();
		for (;pNode,index < symbolCount; pNode = pNode->GetNext(),index++) {
			this->m_aIdentifiers[index]= pNode->GetName();
		}
	}

	return bWasPopulated;
}
