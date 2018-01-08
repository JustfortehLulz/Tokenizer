//============================================================================
//
//% Student Name 1: Luke Gair
//% Student 1 #: 30128694
//% Student 1 userid (email): stu1 lgair@sfu.ca
//
//% Student Name 2: Johnston Yang
//% Student 2 #: 301282192
//% Student 2 userid (email): jya121 (jya121@sfu.ca)
//
//% Below, edit to list any people who helped you with the code in this file,
//%      or put 'None' if nobody helped (the two of) you.
//
// Helpers: None_
//
// Also, list any resources beyond the course textbook and the course pages on Piazza
// that you used in making your submission.
//
// Resources: None
//
//%% Instructions:
//% * Put your name(s), student number(s), userid(s) in the above section.
//% * Enter the above information in tokenClasses.cpp  too.
//% * Edit the "Helpers" line and "Resources" line.
//% * Your group name should be "P5_<userid1>_<userid2>" (eg. P5_stu1_stu2)
//% * Form groups as described at:  https://courses.cs.sfu.ca/docs/students
//% * Submit files to courses.cs.sfu.ca
//
// Name        : tokenClassifier.cpp
// Description : Course Project Part 2 (Token Classifier)
// Original portions Copyright (c) 2017 School of Engineering Science, Simon Fraser University
//============================================================================

#include <vector>
#include <sstream>
#include <algorithm>
#include "tokenClasses.hpp"
#include <string>
#include <cctype>
#include <cstdlib>

using namespace std;
using namespace ensc251;
#define WHITE "\r\t\v\f "

std::string delim3[] =  {">>=", "<<="};
std::string delim2[] =  {"+=","-=", "/=", "%=", "<<", ">>", "++", "--"};
std::string delim1andWhite =  WHITE "/()*%:;=+-~?";
std::string white = WHITE;

bool processDigits(std::string line, std::string::size_type &index)
{
	bool DigiFound=false;
	while (index < line.length() && isdigit(line.at(index)))
		{
			index++;
			DigiFound=true;
		}
	return DigiFound;
}
// Construct token objects categorized into appropriate types
// Input: a stream
// Output: a vector containing the list of Token objects properly categorized
std::vector<ensc251::Token*> tokenClassifier(istream& code)
{
    std::vector<ensc251::Token*> tokenObjectPs;
	int LineNum = 0; //keeps track of line number
	int spec = false; //for when there is a spec type
	std::string checktype; //used together with spec
	int pointer = false; //checks if spec type is a pointer
	int isfloat = false; //checks if the check variable is a float

		std::string line;
		for (;getline (code, line);LineNum++)
		{
			if (line.at(0) == '#')
			{
				continue;
			}
			std::string::size_type index2 = 0;
	        auto length = line.length();

			while(index2 < length )
			{
				auto index1 = line.find_first_not_of(white, index2);
				if (index1 == std::string::npos)
				{
					break;
				}
				/// Dealing with int and float consts
				if (isdigit(line.at(index1)) || line.at(index1) == '.')
				{
					index2 = index1;
					// integer consts
					bool preDigitFound = processDigits(line, index2);
					// floats
					if (index2 < length && line.at(index2) == '.')
					{
						index2++;
						bool postDigitFound = processDigits(line, index2);

						if(!preDigitFound && !postDigitFound) // detecting the single "." as an invalid token!
						{
							std::string check= "";
							Token* type = new incorrect(check, LineNum, static_cast<int>(index1));
							tokenObjectPs.push_back(type);
							continue;
						}
						isfloat = true; //when there is a '.' that means the number is going to be a float
					}
					// exponentials
					if (index2 < length && (line.at(index2) == 'e' || line.at(index2) == 'E'))
					{
						if(line.at(index2+1) == '+' || line.at(index2+1) == '-')
							index2+=2;
						else
							index2++;

						if(!processDigits(line, index2)) // invalid float -> roll back ...
						{
							if(line.at(index2-1) == 'e' || line.at(index2-1) == 'E')
								index2--;
							else
								index2-=2;
						}
						isfloat = true; // number will be a float
					}

					// suffix f and F
					if (index2 < length && (line.at(index2) == 'f' || line.at(index2) == 'F')){
						index2++;
						isfloat = true; // number will be a float
					}
					std::string subs = line.substr(index1, index2 - index1); // takes out the number
					float subsrep = stof(subs); //changes the string to a float
					if(isfloat) //checks if its a float
					{
						Token* type = new numeric_const<float>(subsrep,LineNum,static_cast<int>(index1));
						tokenObjectPs.push_back(type);
						isfloat = false; //since the float is already put into a class, set the flag back to false
					}
					else
					{
						Token* type = new numeric_int_const<int>(subsrep,LineNum,static_cast<int>(index1));
						tokenObjectPs.push_back(type); //pushes all other numbers that are not floats
					}
					continue;
				}
				for (auto delim : delim3) { // checks for <<= or >>=
					if (line.substr(index1, 3) == delim) { // finds if the substr contains either <<= or >>=
						if(delim == "<<=" || delim == ">>=")
						{
							Token* type = new int_assignment_operator(delim,LineNum,static_cast<int>(index1));
							tokenObjectPs.push_back(type);
						}
						else
						{ // if its not then it becomes a gen_assignment_operator
							Token* type = new gen_assignment_operator(delim,LineNum,static_cast<int>(index1));
							tokenObjectPs.push_back(type);
						}
						index2 = index1 + 3; // increase by 3 because the delim always has a length of 3
						break;
					}
				}
				if (index2 > index1) continue;

				// the below follows the pattern from the lines above.
				for (auto delim : delim2)
				{
					if (line.substr(index1, 2) == delim)
					{
						if(delim == "%=") //checks for %=
						{
							Token* type = new int_assignment_operator(delim,LineNum,static_cast<int>(index1));
							tokenObjectPs.push_back(type);
						}
						else if(delim == "<<" || delim == ">>") //checks for << or >>
						{
							Token* type = new shift_operator(delim,LineNum,static_cast<int>(index1));
							tokenObjectPs.push_back(type);
						}
						else if(delim == "++" || delim == "--") //checks ++ or --
						{
							Token* type = new postfix_operator(delim,LineNum,static_cast<int>(index1));
							tokenObjectPs.push_back(type);
						}
						else // if its not any of the above delims then it will become a gen_assignment_operator
						{
							Token* type = new gen_assignment_operator(delim,LineNum,static_cast<int>(index1));
							tokenObjectPs.push_back(type);
						}
						index2 = index1 + 2; //increase by two because the delim always has a legnth of two
						break;
					}
				}
				if (index2 > index1) continue;

				if (line[index1] == '"') { // checking for things within double quotation marks
					index2 = index1 + 1;
					while (index2 < length) {
						if (line[index2] == '\\') {
							index2++; // skip over escape character
						}
						else
							if (line.at(index2) == '"') {
								std::string check= line.substr(index1, index2 + 1 - index1); // quotation marks surrounding characters
								Token* type = new ensc251::string(check,LineNum,static_cast<int>(index1));
								tokenObjectPs.push_back(type);
								break;
							}
						index2++;
					}
					if (index2 >= length) { // String is not terminated!
						std::string check= line.substr(index1, index2 - index1);
						Token* type = new incorrect(check,LineNum,static_cast<int>(index1));
						tokenObjectPs.push_back(type);
						break;
					}
					index2++; // non-empty string was pushed on the result vector
					continue;
				}
				if (line[index1] == '\'') { // checking for strings within two single quotation marks
					index2 = index1 + 1;
					if (index2 < length && line[index2] == '\\') {
						index2++; // skip over escape character
					}
					index2++; // skip over character
					if (index2 >= length || line.at(index2) != '\'') // Character constant is not terminated!
					{
						std::string check= "";
						Token* type = new incorrect(check,LineNum,static_cast<int>(index1));
						tokenObjectPs.push_back(type);
					}
					else
					{
						index2++;
						std::string check= line.substr(index1, index2 - index1);
						Token* type = new numeric_int_const<char>(check[1] ,LineNum,static_cast<int>(index1));
						tokenObjectPs.push_back(type);
					}
					continue;
				}
				if(line[index1]=='_' || isalpha(line[index1])) { //checks if at index1 you get a letter or a '_' character
					index2 = index1 + 1;
					while (index2 < length && (line[index2]=='_' || isalnum(line[index2]))) //makes sure that index2 is not past the length of the string
						index2 ++;
					std::string check= line.substr(index1, index2 - index1);
					if(check== "char" || check== "int" || check== "float") //checks for the which type spec it is
					{
						Token* type = new type_spec(check, LineNum, static_cast<int>(index1));
						tokenObjectPs.push_back(type);
						checktype = check;
						spec = true; //when there is a type spec
					}
					else if(check== "errno") //makes this case int_id
					{
						Token* type = new int_id(check,LineNum,static_cast<int>(index1));
						tokenObjectPs.push_back(type);
					}
					else
					{
						if(spec == true)
						{
							if(pointer) // if there is a * before it will turn the token afterwards a pointer_id
							{
								Token* type = new pointer_id(check,LineNum,static_cast<int>(index1));
								tokenObjectPs.push_back(type);
								pointer = false;
							}
							else if(checktype == "char" || checktype == "int")
							{ // if not it will become an int_id
								Token* type = new int_id(check,LineNum,static_cast<int>(index1));
								tokenObjectPs.push_back(type);
							}
							else if(checktype == "float")
							{ //if not it will become a numeric_id
								Token* type = new numeric_id(check,LineNum,static_cast<int>(index1));
								tokenObjectPs.push_back(type);
							}
						}
						else
						{//if it wasnt given anything it defaults to numeric_id
							Token* type = new numeric_id(check, LineNum,static_cast<int>(index1));
						}

					}
					continue;
				}
				index2 = index1 + 1; // we found a 1-character token
				std::string check= line.substr(index1, index2 - index1); // 1 character
				char value = check[0];
				if(isdigit(value)) //checks if that value is a digit
				{
					Token* type = new numeric_int_const<int>(value, LineNum, static_cast<int>(index1));
					tokenObjectPs.push_back(type);
				}
				else if(isalpha(value)) // checks if that value is alpha
				{
					if(spec == true)//checks if there was a char int or float before
					{
						if(checktype == "char" || checktype == "int") //if it was an int or char the token after is an int_id
						{
							Token* type = new int_id(check,LineNum,static_cast<int>(index1));
							tokenObjectPs.push_back(type);
						}
						else if(checktype == "float") //if it was an float the token after is an numeric_id
						{
							Token* type = new numeric_id(check,LineNum,static_cast<int>(index1));
							tokenObjectPs.push_back(type);
						}
					}
					else
					{
						Token* type = new int_id(check, LineNum, static_cast<int>(index1));
						tokenObjectPs.push_back(type);
					}
				}
				else
				{
					if(check== "?") //checks if there is a conditional operator
					{
						Token* type = new conditional_operator(check,LineNum,static_cast<int>(index1));
						tokenObjectPs.push_back(type);
					}
					else if(check== "%") //checks if there is a modulo operator
					{
						Token* type = new mod_operator(check,LineNum,static_cast<int>(index1));
						tokenObjectPs.push_back(type);
					}
					else if(check==  "=") //checks if there is a equal sign
					{
						Token* type = new gen_assignment_operator(check,LineNum,static_cast<int>(index1));
						tokenObjectPs.push_back(type);
					}
					else if(check== ";" || check==  "*" || check== "(" || check== ")" || check== ":") //checks if there is puncuation
					{
						Token* type = new punctuator(check,LineNum,static_cast<int>(index1));
						tokenObjectPs.push_back(type);
						if(check== "*") // if the punctuation happens to be * that means there is a pointer
						{
							pointer = true;
						}
					}
					else if(check== "+" || check== "-") //checks for the additive operators
					{
						Token* type = new additive_operator(check,LineNum,static_cast<int>(index1));
						tokenObjectPs.push_back(type);
					}
					else // everything else would be incorrect
					{
						Token* type = new incorrect(check,LineNum,static_cast<int>(index1));
						tokenObjectPs.push_back(type);
					}

				}

			}
		}
    return tokenObjectPs;
}