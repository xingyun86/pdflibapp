// pdfapi.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <pdfapi.h>

// TODO: Reference additional headers your program requires here.

void PdfApi::error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void* user_data)
{
    printf("ERROR: error_no=%04X, detail_no=%u/n", (HPDF_UINT)error_no, (HPDF_UINT)detail_no);
    longjmp(((PdfApi*)user_data)->m_env, 1);
}

int PdfApi::generate_pdf()
{
    HPDF_Doc pdf;
    HPDF_Page page;
    HPDF_Destination dest;
    HPDF_Font font;
    HPDF_Image hpdfImage;

    pdf = HPDF_New(error_handler, this);
    if (!pdf) 
    {
        printf("ERROR: cannot create pdf object./n");
        return (-1);
    }

    if (setjmp(m_env))
    {
        HPDF_Free(pdf);
        return (-1);
    }

    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

    font = HPDF_GetFont(pdf, "Helvetica", NULL);

    std::vector<std::string> vImgList = {
         "1.png",
         "2.png"
    };

    for (auto it : vImgList)
    {
        //add a new page object.
        page = HPDF_AddPage(pdf);

        dest = HPDF_Page_CreateDestination(page);
        HPDF_Destination_SetXYZ(dest, 0, HPDF_Page_GetHeight(page), 1);
        HPDF_SetOpenAction(pdf, dest);
        HPDF_Page_SetFontAndSize(page, font, 12);

        HPDF_Page_SetRGBFill(page, 1.0, 0, 0);
        //HPDF_Page_MoveTo(page, 100, 100);
        //HPDF_Page_LineTo(page, 100, 180);

        HPDF_SetPageMode(pdf, HPDF_PAGE_MODE_FULL_SCREEN);
        hpdfImage = HPDF_LoadPngImageFromFile(pdf, it.c_str());

        if (hpdfImage)
        {
            HPDF_Page_DrawImage(page, hpdfImage, 0, 0, HPDF_Image_GetWidth(hpdfImage), HPDF_Image_GetHeight(hpdfImage));
        }
    }

    //save the document to a fil
    HPDF_SaveToFile(pdf, "test.pdf");
    //clean up
    HPDF_Free(pdf);

    return 0;
}