#include "StdAfx.h"
#include ".\lexicalanalyzer.h"
#include "Node.h"
#include "RList.h"
#include <string>
#include <fstream>

using namespace std;

LexicalAnalyzer::LexicalAnalyzer(void)
{
	m_szLetterOrNumberOrSymbol = "abcdefghijklmnopqrstuvwxyz0123456789()<>-+;=,";
	m_szLowerCaseLetters = "abcdefghijklmnopqrstuvwxyz";
	m_szLettersAndNumbers = "abcdefghijklmnopqrstuvwxyz0123456789";
	m_szReallNumbers = "0123456789";
	m_szSymbols = "()<>-+;=,";
	m_szPlusMinus = "-+";

	this->PopulateListOfKeyWords();

}

LexicalAnalyzer::~LexicalAnalyzer(void)
{
}

// Call this function to place the symbols in a list object for reference.
bool LexicalAnalyzer::PopulateListOfSymbols(void)
{
	bool bListOfSymbolsCreated = false;

	CNode* pNode = new CNode("(",0);
	this->m_lListOfSymbols.Add(pNode);

	pNode = new CNode(")",1);
	this->m_lListOfSymbols.Add(pNode);

	pNode = new CNode("<",2);
	this->m_lListOfSymbols.Add(pNode);

	pNode = new CNode(">",3);
	this->m_lListOfSymbols.Add(pNode);

	pNode = new CNode("+",4);
	this->m_lListOfSymbols.Add(pNode);

	pNode = new CNode("-",5);
	this->m_lListOfSymbols.Add(pNode);

	pNode = new CNode(";",6);
	this->m_lListOfSymbols.Add(pNode);

	pNode = new CNode("=",7);
	this->m_lListOfSymbols.Add(pNode);

	if (this->m_lListOfSymbols.GetCount()>0) {
		bListOfSymbolsCreated = true;
	}
	return bListOfSymbolsCreated;
}

// Call this function to initialize the symbol table data structure with symbols.
bool LexicalAnalyzer::PopulateListOfKeyWords(void)
{
	bool bKeyWordTableCreated = false;

	CNode* pNode = new CNode("BEGIN",0);
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
	if (this->m_lListOfKeywords.GetCount()>0) {
		bKeyWordTableCreated = true;
	}

	return bKeyWordTableCreated;
}

// Call this function to populate the list of tokens for processing.
bool LexicalAnalyzer::PopulateListOfTokens(string Token)
{
	bool bTokenListPopulated = false;

	CNode* pNode = new CNode(Token.c_str(),-777);
	this->m_lListOfTokens.Add(pNode);

	return bTokenListPopulated;
}

// Call this function to analyze the tokens in the input file.
void LexicalAnalyzer::AnalyzeTokens()
{
	bool bTokenIdentified = false;
	CNode* pNode = NULL;
	pNode = this->m_lListOfTokens.GetHead();

	for (;pNode;pNode = pNode->GetNext()) {
		
		cout << pNode->GetName() << endl;

		if (this->IsIdentifier(pNode)) {
			bTokenIdentified = true;
		}
		 else if (this->IsKeyWord(pNode)) {
			bTokenIdentified = true;
		}
		 else if (this->IsSymbol(pNode)) {
			bTokenIdentified = true;
		}
		  else if (this->IsInteger(pNode)) {
			bTokenIdentified = true;
		}
		  else if (this->IsError(pNode)) {
			 bTokenIdentified = true;
		 }
	}
}


// Call this function to determine if the character is a letter either uppercase or lowercase.
bool LexicalAnalyzer::IsLetter(CNode* pToken,int iIndex)
{
	bool bIsLetter = false;
	int stringIndex = iIndex;
	char* result = NULL;
		result =strchr(this->m_szLowerCaseLetters.c_str(), pToken->GetName()[stringIndex]);
		if (result!=NULL) {
			bIsLetter = true;
		}
	return bIsLetter;
}

// Call this function to figure out if the token consists of an integer.
bool LexicalAnalyzer::IsInteger(CNode* pToken)
{
	// Declare and initialize local variables
	CNode* pNode = NULL;
	bool bIsInteger = false;
	string szResultString= "";
	string szTokenString = "Integer : ";

	int stringIndex = 0;
	int stringLength = 0;
	int begin = NULL;
	int end = NULL;

	char* result = NULL;

	stringLength = (int)strlen(pToken->GetName());
	result = strchr(this->m_szPlusMinus.c_str(), pToken->GetName()[stringIndex]);

	if (result==NULL)
	result = strchr(this->m_szReallNumbers.c_str(), pToken->GetName()[stringIndex]);
	
	if (result !=NULL){ 
		begin = stringIndex;
		do {
			
			stringIndex++;
		} while(result = strchr(this->m_szReallNumbers.c_str(),pToken->GetName()[stringIndex]));
		end = stringIndex;

	if (end <stringLength) 
	szResultString = this->ParseToken(begin,end,stringLength,pToken);
	else
		szResultString = pToken->GetName();
	
	szTokenString += szResultString;
	pNode = new CNode(szTokenString.c_str(),-777);
	this->m_lFileOutputList.Add(pNode);

	bIsInteger = true;

	}	

	return bIsInteger;
}

// Call this function to figure out if the token is a keyword.
bool LexicalAnalyzer::IsKeyWord(CNode* pToken)
{
	bool bIsKeyWord = false;
	CNode* pNode = NULL;
	char* result = "";
	pNode = this->m_lListOfKeywords.GetNodeByName(pToken->GetName());
	if (pNode && !strcmp(pNode->GetName(),pToken->GetName())) 
		bIsKeyWord = true;
	return bIsKeyWord;
}

// Call this function to determine if a character is a symbol.
bool LexicalAnalyzer::IsSymbol(CNode* pToken, int iIndex)
{
	bool bIsSymbol = false;

	CNode* pNode = NULL;
	char* result = NULL;

	int isDigit = 0;
	int isMinus = 0;
	int stringIndex = iIndex;
	int stringLength = (int)strlen(pToken->GetName());;

	string szIdString = "";
	string szNewString = "";
	string szParseString = "";
	
	// Dispose of case that is a letter or a number.	

		if (result = strchr(")",pToken->GetName()[stringIndex])) {
			// Right Parenthesis
			cout<< "RightParen" << endl;
			szIdString = "RightParen";
			bIsSymbol = true;
		}
		else if (result = strchr("(",pToken->GetName()[stringIndex])) {
			// Left Parenthesis
			cout<< "Left Paren" << endl;
			szIdString = "LeftParen";
			bIsSymbol = true;
		}

		else if (result = strchr(">",pToken->GetName()[stringIndex])) {
			// Less Than
			cout<< "Less Than" << endl;
			szIdString = "Less Than";
			bIsSymbol = true;
		}

		else if (result = strchr("<",pToken->GetName()[stringIndex])) {
			// Greater Than
			cout<< "Greater Than" << endl;
			szIdString = "Greater Than";
			bIsSymbol = true;
		}

		else if (result = strchr("+",pToken->GetName()[stringIndex])) {
			// Plus Sign	
			szIdString = "Plus";
			bIsSymbol = true;
		}
		else if (result = strchr("-",pToken->GetName()[stringIndex])) {
			// Minus Sign
			if(isDigit = isdigit(pToken->GetName()[stringIndex+1])){
			szIdString = "";
			bIsSymbol = false;
			}
			else{
				szIdString = "Minus";
				bIsSymbol = true;
			}

		}

		else if (result = strchr(";",pToken->GetName()[stringIndex])) {
			// Semicolon
			
			szIdString = "Semicolon";
			bIsSymbol = true;
		}

		else if (result = strchr("=",pToken->GetName()[stringIndex])) {
			// Equals Sign
			cout<< "Equals" << endl;
			szIdString = "Equals";
			bIsSymbol = true;
		}
	

	// Regardless of whether first character is a symbol or not, if it's length
	// is not one, we tokenize the rest of the string for reparsing.

	if (bIsSymbol) {

		szParseString = pToken->GetName();
		if (stringLength!= 1) 
			szParseString = this->ParseToken(0,1,stringLength,pToken);

		// Add a node to the file output list for later identification.
		pNode = new CNode(szIdString.c_str(),0);
		this->m_lFileOutputList.Add(pNode);
	}

	return bIsSymbol;
}


// Call this function to determine if a token is an identifier.
bool LexicalAnalyzer::IsIdentifier(CNode* pToken)
{
	//Identifier: [a-z]([a-z0-9])*

	bool bIsIdentifier = false;
	int tokenIndex = 0;
	CNode* pNode = NULL;
	if (this->IsLetter(pToken)) 
		if (this->IsLetterOrNumber(pToken)) 
			bIsIdentifier = true;

	if (bIsIdentifier) {
		pNode = this->m_lSymbolTable.GetNodeByName(pToken->GetName());
		if (!pNode) 
			this->m_lSymbolTable.Add(pToken);
	}

	return bIsIdentifier;
}


// Call this function to see if the rest of a potential identifier meets the criteria of letter or number....
bool LexicalAnalyzer::IsLetterOrNumber(CNode* pToken, int iIndex)
{
	int stringIndex = iIndex;
	int stringLength = 0;
	char* result = NULL;
	bool bIsLetterOrNumber = false;

	stringLength = (int)strlen(pToken->GetName());


	do {
		stringIndex++;
	} while(result = strchr(this->m_szLettersAndNumbers.c_str(),pToken->GetName()[stringIndex]));

	if (result == NULL && stringIndex == stringLength+1) 
		bIsLetterOrNumber = true;

	return bIsLetterOrNumber;
}


string LexicalAnalyzer::ParseToken(int iBegin, int iEnd, int Length, CNode* pToken)
{
	CNode* pNode = NULL;
	string szRemainderString;
	string szValidString;
	string szParseString = pToken->GetName();

	szValidString = szParseString.substr(iBegin,iEnd);
	szRemainderString = szParseString.substr(iEnd,Length-iEnd);
	pNode = new CNode(szRemainderString.c_str(),0);
	this->m_lListOfTokens.Add(pNode);
	
	return szValidString;

}
// Call this function to determine if the token is an "Error Token"
bool LexicalAnalyzer::IsError(CNode* pToken)
{
	bool bIsErrorToken = false;
	bool bContainsNoLetters = false;
	int punctIndex = 0;
	int letterIndex = 0;
	int iAllCaps = 0;
	int iNoLetters = 0;
	int stringLength = (int)strlen(pToken->GetName());
	string szError = "Error : ";
	string szErrorString = pToken->GetName();
	CNode* pNode = NULL;


	// Find out if token is in all uppercase.
	do {
		iAllCaps = isupper(pToken->GetName()[letterIndex]);
			letterIndex++;
	} while(pToken->GetName()[letterIndex]!=NULL && iAllCaps);

	if (iAllCaps) {
		szError += pToken->GetName();
		pNode = new CNode(szError.c_str(),-1);
		bIsErrorToken = true;

	}

	else if (!iAllCaps && letterIndex && letterIndex!=stringLength) {
		if (stringLength!=1) 
			szError += this->ParseToken(0,letterIndex,stringLength, pToken);
		else
			szError+= pToken->GetName();

		pNode = new CNode(szError.c_str(),-1);
		bIsErrorToken = true;
	}
	else if (!iAllCaps && letterIndex && letterIndex==stringLength) {
		if (stringLength!=1) 
			szError += this->ParseToken(0,letterIndex-1,stringLength, pToken);
		else
			szError+= pToken->GetName();

		pNode = new CNode(szError.c_str(),-1);
		bIsErrorToken = true;
	}
	this->m_lFileOutputList.Add(pNode);
	
	return bIsErrorToken;
}

// Call this function to create and print the output files.
void LexicalAnalyzer::GenerateOutputFiles(void)
{
	CNode* pNode  = NULL;
	pNode = this->m_lFileOutputList.GetHead();

	ofstream OutputToken("TokenFile.tok");
	for (;pNode;pNode= pNode->GetNext())
		OutputToken<< pNode->GetName()<<endl;
	OutputToken.close();

	ofstream OutputSymbolTable("SymbolTable.sym");
	pNode = this->m_lSymbolTable.GetHead();

	for (;pNode;pNode= pNode->GetNext()) 
		OutputSymbolTable<< pNode->GetName()<<endl;
	OutputSymbolTable.close();
}
