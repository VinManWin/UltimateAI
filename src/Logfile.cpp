#include "Logfile.hpp"

#include <sstream>
#include <iomanip>
#include "utility.hpp"

Logfile::Logfile(std::string filename) :
stream(filename)
{
}

void Logfile::write(std::string msg, EntryType et)
{
	stream << getPrefix(et).c_str() << "\t" << getTime() << "\t" << msg.c_str() << std::endl;
}

std::string Logfile::getPrefix(EntryType et) const
{
	std::string result;
	switch (et)
	{
	case Logfile::Error:
		result = "ERROR";
		break;
	case Logfile::Warning:
		result = "WARNING";
		break;
	case Logfile::Info:
		result = "INFO";
		break;
	}

	return result;
}

std::string Logfile::getTime() const
{
	int ms = clock.getElapsedTime().asMilliseconds();

	using namespace std;
	stringstream s;
	s << setfill('0');
	s << setw(2) << divRem(ms, 3600000) << ":";
	s << setw(2) << divRem(ms, 60000) << ":";
	s << setw(2) << divRem(ms, 1000) << ",";
	s << setw(1) << ms;

	return s.str();
}