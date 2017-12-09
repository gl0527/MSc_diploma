#include "ErrorMessage.h"

#include <sstream>


namespace Engine {

ErrorMessage::ErrorMessage (const std::string& errString, const std::string fileName, size_t lineNum)
	: m_errString (errString),
	m_fileName (fileName),
	m_lineNum (lineNum)
{
	std::stringstream ss;
	ss << "\"" << m_errString << "\"" << " in file \"" << m_fileName << "\" at line " << m_lineNum;
	m_errMessage = ss.str ();
}


const std::string& ErrorMessage::Get () const
{
	return m_errMessage;
}


std::ostream& operator<< (std::ostream& os, const ErrorMessage& errorMessage)
{
	os << errorMessage.Get ();

	return os;
}

}	// namespace Engine