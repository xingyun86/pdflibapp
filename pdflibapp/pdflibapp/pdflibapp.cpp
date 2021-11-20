// pdflibapp.cpp : Defines the entry point for the application.
//

#include <pdflibapp.h>
#include <pdfapi.h>

int main(int argc, char ** argv)
{
	std::cout << "Hello CMake." << std::endl;
	PdfApi().generate_pdf();
	return 0;
}
