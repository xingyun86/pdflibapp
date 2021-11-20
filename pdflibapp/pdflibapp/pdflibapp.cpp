// pdflibapp.cpp : Defines the entry point for the application.
//

#include <pdflibapp.h>
#include <pdfapi.h>

int main(int argc, char ** argv)
{
	std::cout << "Hello CMake." << std::endl;
	PdfApi().generate_pdf();
	PdfApi().generate_pdf_1();
	PdfApi().generate_pdf_2();
	PdfApi().generate_pdf_3();
	return 0;
}
