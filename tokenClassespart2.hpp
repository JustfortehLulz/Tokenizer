//============================================================================
//
//% Student Name 1: student1
//% Student 1 #: 123456781
//% Student 1 userid (email): stu1 (stu1@sfu.ca)
//
//% Student Name 2: student2
//% Student 2 #: 123456782
//% Student 2 userid (email): stu2 (stu2@sfu.ca)
//
//% Below, edit to list any people who helped you with the code in this file,
//%      or put 'None' if nobody helped (the two of) you.
//
// Helpers: _everybody helped us/me with the assignment (list names or put 'None')__
//
// Also, list any resources beyond the course textbook and the course pages on Piazza
// that you used in making your submission.
//
// Resources:  ___________
//
//%% Instructions:
//% * Put your name(s), student number(s), userid(s) in the above section.
//% * Enter the above information in tokenClasses.cpp  too.
//% * Edit the "Helpers" line and "Resources" line.
//% * Your group name should be "P5_<userid1>_<userid2>" (eg. P5_stu1_stu2)
//% * Form groups as described at:  https://courses.cs.sfu.ca/docs/students
//% * Submit files to courses.cs.sfu.ca
//
// Name        : tokenClasses.hpp
// Description : Course Project Part 2 (Token Classifier)
// Original portions Copyright (c) 2017 School of Engineering Science, Simon Fraser University
//============================================================================

#ifndef TOKENCLASSES_HPP_
#define TOKENCLASSES_HPP_

#include <string>
#include <iostream>
#include <typeinfo>

namespace ensc251 {

char *demangle(const char *typeName);

class Token
{	// The Base Token Class
private:
	int lineNumber;
	int indexNumber;
public:
	Token(int lineNum, int indexNum):lineNumber(lineNum),indexNumber(indexNum){}//default constructor w/parameters
	Token() { std::cout << "Warning:  Do not invoke this constructor" << std::endl;}// default constructor without parameters
	void set(int lineNum, int indexNum)
	{
		lineNumber= lineNum;
		indexNumber=indexNum;
	}
	int getLineNumber() const
	{
		return lineNumber;
	}
	int getIndexNumber() const
	{
		return indexNumber;
	}
	 // we will fix this up when we learn about virtual functions.
	friend std::ostream& operator <<(std::ostream& outs, const Token& token)
	{
		token.print(outs);
		return outs;
	}
	void print(std::ostream& str) const { str << demangle(typeid(*this).name()) << " (line: " << lineNumber <<", index: " << indexNumber <<")" << ": "; }
	virtual ~Token() {} // You will learn about virtual soon, but for now, don't delete this line.
};

class StringBasedToken:public Token
{
	private:
		std::string stringValue;
	public:
		StringBasedToken() { std::cout << "Warning:  Do not invoke this constructor(strbasedtoken)" << std::endl; } // make sure this does not get invoked.
		StringBasedToken(const std::string &aStringValue, int lineNum, int indexNum):Token(lineNum,indexNum)
		{
			stringValue= aStringValue;
		}
		const std::string& getStringValue( ) const
		{
			return stringValue;
		} // return the stringValue
		void print(std::ostream& str) const { Token::print(str); str << ": " << stringValue; }
};

class punctuator:public StringBasedToken
{// e.g., ';', '*', '(', ')', ':'
	public:
	punctuator() {std::cout << "Warning:  Do not invoke this constructor(puncToken)" << std::endl; } // make sure this does not get invoked.
	punctuator(const std::string &astringValue, int lineNum, int indexNum):StringBasedToken(astringValue,lineNum,indexNum){}//constructor for punctuator
	// return the punctuation
	//punctuator inherits void print() from the StringBasedToken Parent class
};

class type_spec:public StringBasedToken
{// e.g., 'char' | 'int' | 'float' // flags
	public:
	type_spec() {std::cout <<"Warning: Do not invoke this constructor(typespec)" << std::endl;}
	type_spec(const std::string &astringValue, int lineNum, int indexNum):StringBasedToken(astringValue,lineNum,indexNum){}
	// return the typespec
};
// do not instantiate this class.
class id:public StringBasedToken
{	// identifiers -- example:  sum
	public:
	id() {std::cout <<"Do not invoke this constructor"<< std::endl;}
	id(const std::string &astringValue, int lineNum, int indexNum):StringBasedToken(astringValue,lineNum,indexNum){}
};
class pointer_id:public id
{
	// e.g., char* identifier
	public:
		pointer_id(){}
		pointer_id(const std::string &astringValue, int lineNum, int indexNum):id::id(astringValue,lineNum,indexNum){}
};

class numeric_id:public id
{
	// char, int, and float identifiers
	public:
	numeric_id(){}
	numeric_id(const std::string &astringValue, int lineNum, int indexNum):id(astringValue,lineNum,indexNum){}
};

class int_id:public numeric_id
{
	// char and int identifiers
	public:
	int_id(){}
	int_id(const std::string &astringValue, int lineNum, int indexNum):numeric_id(astringValue,lineNum,indexNum){}
};

class gen_assignment_operator:public StringBasedToken
{// '=' | '/=' | '+=' | '-='
	public:
	gen_assignment_operator(){}
	gen_assignment_operator(const std::string &astringValue, int lineNum, int indexNum):StringBasedToken(astringValue,lineNum,indexNum){}
};

class int_assignment_operator:public StringBasedToken
{
	// '%=' | '<<=' | '>>='
	public:
	int_assignment_operator(){}
	int_assignment_operator(const std::string &astringValue, int lineNum, int indexNum):StringBasedToken(astringValue,lineNum,indexNum){}
};

class conditional_operator:public StringBasedToken
{
	// ?
	public:
	conditional_operator(){}
	conditional_operator(const std::string &astringValue, int lineNum, int indexNum):StringBasedToken(astringValue,lineNum,indexNum){}
};

class shift_operator:public StringBasedToken
{
	// '<<' | '>>'
	public:
	shift_operator(){}
	shift_operator(const std::string &astringValue, int lineNum, int indexNum):StringBasedToken(astringValue,lineNum,indexNum){}
};

class additive_operator:public StringBasedToken
{
	// '+' | '-'
	public:
	additive_operator(){}
	additive_operator(const std::string &astringValue, int lineNum, int indexNum):StringBasedToken(astringValue,lineNum,indexNum){}
};

class div_operator:public StringBasedToken
{
	// '/'
	public:
	div_operator(){}
	div_operator(const std::string &astringValue, int lineNum, int indexNum):StringBasedToken(astringValue,lineNum,indexNum){}
};

class mod_operator:public StringBasedToken
{
	// '%'
	public:
	mod_operator(){}
	mod_operator(const std::string &astringValue, int lineNum, int indexNum):StringBasedToken(astringValue,lineNum,indexNum){}
};

class comp_operator:public StringBasedToken
{
	//  '~'
	public:
	comp_operator(){}
	comp_operator(const std::string &astringValue, int lineNum, int indexNum):StringBasedToken(astringValue,lineNum,indexNum){}
};

class postfix_operator:public StringBasedToken
{
		// '++' | '--'
	public:
	postfix_operator(){}
	postfix_operator(const std::string &astringValue, int lineNum, int indexNum):StringBasedToken(astringValue,lineNum,indexNum){}
};
class string:public StringBasedToken
{
	// e.g., "sample string"
	public:
	string(){}
	string(const std::string &astringValue, int lineNum, int indexNum):StringBasedToken(astringValue,lineNum,indexNum){}
};

class incorrect:public StringBasedToken
{
	private:
		std::string error;
	// e.g., "sample string     --> This is incorrect ...
	//		(quotation mark " is missing at the end)
	public:
		incorrect(){}
		incorrect(const std::string &anERROR, int lineNum, int indexNum):StringBasedToken(anERROR,lineNum,indexNum)
		{
			error=anERROR;
		}
		const std::string get_error() const
		{
			return error;
		}
	/* Fill in the implementation for constructor. Store the unrecognized characters. */
};

template<typename T>
class numeric_const;

template<typename T>
class numeric_int_const;

class constant:public Token
{
public:
	constant() { std::cout << "Warning:  Do not invoke this constructor" << std::endl; }; // make sure this does not get invoked.
	constant(int lineNum, int indexNum): Token(lineNum, indexNum) {};
};

template<typename T>
class numeric_const:public constant
{
	// e.g. 48.5
private:
	T value;
public:
	numeric_const(const T& constValue, int lineNum, int indexNum):constant(lineNum,indexNum)
	{
		value = constValue;
	}
	//Fill in for additional constructors if desired

	const T& getValue() const
	{
		return  value;
	}
	void print(std::ostream& str) const {Token::print(str); str << ": " << value; }
};

template<typename T>
class numeric_int_const:public numeric_const<T>
{
	// e.g. 48 or '0' -- covers just int and char
	public:
	numeric_int_const(){}
	numeric_int_const(const T& constValue, int lineNum, int indexNum): numeric_const<T>(constValue, lineNum, indexNum){}
		//Fill in for additional constructors if desired
};

} // namespace ensc251
#endif /* TOKENCLASSES_HPP_ */