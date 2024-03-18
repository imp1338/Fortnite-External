#pragma once

#include<iostream>

class FString
{
public:

	//Operator overloads 
	FString& operator=(const FString& rhs);
	FString& operator+(const FString& rhs);

	friend std::ostream& operator<<(std::ostream&, const FString& str);


private:
	int strlen;
	char* strarr;
	char* c_strarr;

	int c_strlen(const char* c_str);
};
