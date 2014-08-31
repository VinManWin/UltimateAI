#include "Program.hpp"

#include <string>
#include <sstream>
#include <vector>
#include <exception>
#include <iostream>

#ifdef WINDOWS
#include <windows.h>
#endif

int main(int argc, const char* argv[])
{
	try
	{
		std::vector<std::string> args(argc);
		for (size_t i = 0; i < argc; ++i)
			args[i] = argv[i];

		Program p(args);
		p.run();
	}
	catch (const std::exception& ex)
	{
#ifdef WINDOWS
		std::stringstream s;
		s << "message: \"" << ex.what() << "\"";
		MessageBoxA(NULL, s.str().c_str(), "an exception occured", MB_ICONERROR | MB_OK);
#else
		std::cerr << "exception: " << ex.what() << std::endl;
#endif
	}

	return 0;
}