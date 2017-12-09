#ifndef ERROR_MESSAGE_H
#define ERROR_MESSAGE_H

#pragma once

// ---------------------------------- includes ----------------------------------

#include <iostream>


namespace Engine {

// ============================= class ErrorMessage =============================

class ErrorMessage
{
public:
	ErrorMessage (const std::string& errString, const std::string fileName, size_t lineNum);
	
	const std::string& Get () const;
	friend std::ostream& operator<< (std::ostream& os, const ErrorMessage& errorMessage);

private:
	const std::string m_errString;
	const std::string m_fileName;
	const size_t m_lineNum;
	
	std::string m_errMessage;
};

}	// namespace Engine

#endif	// ERROR_MESSAGE_H
