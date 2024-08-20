/*!

TinyXMLCodeGenerator - creates NANDRAD_KeywordList.cpp and class-specific implementation files
This file is based on the SIM-VICUS project. See LICENSE file for details.

Authors: Andreas Nicolai <andreas.nicolai@gmx.net>

Example syntax:

	> TinyXMLCodeGenerator NANDRAD /path/to/NANDRAD/src 0 NANDRAD ncg

*/

#include <iostream>
#include <fstream>
#include <IBK_MessageHandler.h>
#include <IBK_MessageHandlerRegistry.h>

#include "CodeGenerator.h"

const char * const SYNTAX =
		"SYNTAX:  TinyXMLCodeGenerator <namespace> <path/to/src> <generateQtSrc> <prefix> <ncg-dir>\n"
		"         <namespace> is usually UIT (used also to compose file names).\n"
		"         <path/to/<lib>/src> is + separated list of input directories to read the header files from.\n"
		"         Keywordlist-source files are written into the first (or only) source directory.\n"
		"         <generateQtSrc> is 1 when Qt source should be generated, 0 otherwise.\n"
		"         <prefix> is the file prefix cg_<prefix>_KeywordList.cpp.\n"
		"         <ncg-dir> is the path to the directory where cg_xxx.cpp files are written to.\n"
		"         Example: TinyXMLCodeGenerator UIT ~/git/SENSOlog/src 0 UIT cg";

// NOTE: Use 'UIT ../../WLog3/src 0 UIT cg' as arguments during debugging in QtCreator
//       and working directory "/path/to/WLOG3/bin/debug"

// ******* MAIN ********

int main(int argc, char *argv[]) {
	std::cout << "-----------------------------------------------------------" << std::endl;
	std::cout << "TinyXMLCodeGenerator v1.1 - based on IBK-KeywordListCreator" << std::endl;
	std::cout << "-----------------------------------------------------------" << std::endl;

	if (argc != 6) {
		std::cerr << "Invalid syntax." << std::endl;
		std::cerr << argc-1 << " Arguments received" << std::endl;
		for (int i=1; i<argc; ++i)
			std::cerr << "  " << argv[i] << std::endl;
		std::cerr << std::endl;
		std::cerr << SYNTAX << std::endl;

		return EXIT_FAILURE;
	}

	IBK::MessageHandlerRegistry::instance().messageHandler()->setConsoleVerbosityLevel(IBK::VL_STANDARD);

	CodeGenerator cg;
	cg.handleArguments(argv);

	// try to read LICENSE_HEADER file
	IBK::Path p("LICENSE_HEADER");
	std::ifstream licenseFile( p.c_str());
	if (!licenseFile) {
		std::cerr << "Error reading license header file '" << p.str() << "', using generic header." << std::endl;
		std::cout << "--------------------------------------------------------------------------" << std::endl;
	}
	else {
		std::stringstream strm;
		strm << licenseFile.rdbuf();
		cg.m_licenseHeader = strm.str();
		std::cout << "LICENSE_HEADER read successfully" << std::endl;
	}

	// the next function parses the input files and does all the error handling
	if (!cg.parseDirectories())
		return EXIT_FAILURE;  // error messages where already written to file

	std::cout << "--------------------------------------------------------------------------" << std::endl;
	try {
		// generate keyword list if we have at least one keyword extracted
		cg.generateKeywordList();
	} catch (IBK::Exception & ex) {
		ex.writeMsgStackToError();
		std::cerr << "Error generating keywordlist" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "--------------------------------------------------------------------------" << std::endl;
	try {
		cg.generateReadWriteCode();
	} catch (IBK::Exception & ex) {
		ex.writeMsgStackToError();
		std::cerr << "Error generating read/write code" << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "--------------------------------------------------------------------------" << std::endl;

	return EXIT_SUCCESS;
}

