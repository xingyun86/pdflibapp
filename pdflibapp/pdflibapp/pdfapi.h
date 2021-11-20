// pdfapi.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <vector>

#include <setjmp.h>
#include <hpdf.h>

// TODO: Reference additional headers your program requires here.
class PdfApi {
public:
    jmp_buf m_env = { 0 };
    static void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void* user_data);
public:
    int generate_pdf();
    int generate_pdf_1();
    int generate_pdf_2();
    int generate_pdf_3();
};