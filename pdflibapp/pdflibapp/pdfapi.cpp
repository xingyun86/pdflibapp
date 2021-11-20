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

int PdfApi::generate_pdf_1()
{
		//1.初始化文档对象
		HPDF_Doc pdf;
		pdf = HPDF_New(error_handler, this);
		if (!pdf)
		{
			printf("错误:创建PDF失败！\n");
			return (-1);
		}
		if (setjmp(m_env))
		{
			HPDF_Free(pdf);
			printf("错误:生成PDF过程中遇见错误！\n");
			return (-1);
		}

		//2.设置文档对象/
		//set compression mode 
		HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);
		//set page mode to use outlines
		HPDF_SetPageMode(pdf, HPDF_PAGE_MODE_USE_OUTLINE);

		//3.创建Page对象
		HPDF_Page page;
		page = HPDF_AddPage(pdf);
		HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
		///HPDF_PAGE_SIZE_A4  595*841(in pixel)
		///HPDF_PAGE_LANDSCAPE A4倒方向 ；HPDF_PAGE_PORTRAIT 正向方向

		///3.1输出字符串//
		std::string str_Showtxt;
		HPDF_Page_BeginText(page);
		///使用条件：当前graphics mode为HPDF_GMODE_PAGE_DESCRIPTION,函数将当前模式改为HPDF_GMODE_TEXT_OBJECT
		///开始text对象，设置当前text的位置到点（0，0）
		HPDF_Font font;
		/* get default font */
		HPDF_UseCNSFonts(pdf);
		HPDF_UseCNSEncodings(pdf);
		font = HPDF_GetFont(pdf, "SimSun", "GB-EUC-H");///SimSun宋体、SimHei黑体

		///显示报告标题
		HPDF_Page_SetFontAndSize(page, font, 24);//注：字体显示的起始位置指字体左下角的位置
		HPDF_Page_MoveTextPos(page, HPDF_Page_GetWidth(page) / 4, HPDF_Page_GetHeight(page) - 60);
		// 使用条件：graphics mode of the page is in HPDF_GMODE_TEXT_OBJECT
		// 当前位置（x1, y1）, HPDF_Page_MoveTextPos(page, x, y)移动后的位置（x1 + x, y1 + y）
		str_Showtxt = "xxx检测结果";
		HPDF_Page_ShowText(page, str_Showtxt.c_str());///显示字符串方法1

		time_t tt = time(NULL);
		char szTimeAttr[128] = { 0 };
		strftime(szTimeAttr, sizeof(szTimeAttr) / sizeof(*szTimeAttr), "%Y-%m-%d,%H:%M:%S", localtime(&tt));

		///显示测试时间
		HPDF_Page_SetFontAndSize(page, font, 12);
		HPDF_Page_TextOut(page, HPDF_Page_GetWidth(page) * 3 / 4, HPDF_Page_GetHeight(page) - 80, szTimeAttr);///显示字符串方法2

		HPDF_Page_EndText(page);
		///将graphics mode 改为:HPDF_GMODE_PAGE_DESCRIPTION
		///必须有HPDF_Page_BeginText (page);HPDF_Page_EndText (page);否则保存PDF失败。


		///3.2 画线/
		///画线4步骤：设置、起点、LineTo、Stroke
		int i = 0;
		///*3.2.1 Draw horizontal lines. */
		///设置线宽
		HPDF_Page_SetLineWidth(page, 0.5);
		//起点
		HPDF_Page_MoveTo(page, 50, HPDF_Page_GetHeight(page) - 90);//横线1
		//画线
		HPDF_Page_LineTo(page, HPDF_Page_GetWidth(page) - 50, HPDF_Page_GetHeight(page) - 90);

		///* 3.2.2 Draw vertical lines. */
		HPDF_Page_MoveTo(page, 50, HPDF_Page_GetHeight(page) - 90);//第一根竖线
		HPDF_Page_LineTo(page, 50, HPDF_Page_GetHeight(page) - 495);

		///画线结束
		///change the graphics to HPDF_GMODE_PAGE_DESCRIPION
		HPDF_Page_Stroke(page);

		//4.Save PDF//
		///字符数组拼接字符串的方法：///char fname[256]; strcpy (fname, argv[0]); strcat (fname, ".pdf");
		///文件夹必须是之前创建好的，如果文件夹不存在，不会自动创建
		///路径不存在就创建,PathFileExists检验路径是否有效，有效返回true
		///
		HPDF_SaveToFile(pdf, "test_1.pdf");

		//5.Cleanup

		HPDF_Free(pdf);
		return 0;
}

void
print_grid(HPDF_Doc pdf, HPDF_Page page)
{
    HPDF_REAL height = HPDF_Page_GetHeight(page);
    HPDF_REAL width = HPDF_Page_GetWidth(page);
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_UINT x, y;

    HPDF_Page_SetFontAndSize(page, font, 5);
    HPDF_Page_SetGrayFill(page, 0.5);
    HPDF_Page_SetGrayStroke(page, 0.8);

    /* Draw horizontal lines */
    y = 0;
    while (y < height) {
        if (y % 10 == 0)
            HPDF_Page_SetLineWidth(page, 0.5);
        else {
            if (HPDF_Page_GetLineWidth(page) != 0.25)
                HPDF_Page_SetLineWidth(page, 0.25);
        }

        HPDF_Page_MoveTo(page, 0, y);
        HPDF_Page_LineTo(page, width, y);
        HPDF_Page_Stroke(page);

        if (y % 10 == 0 && y > 0) {
            HPDF_Page_SetGrayStroke(page, 0.5);

            HPDF_Page_MoveTo(page, 0, y);
            HPDF_Page_LineTo(page, 5, y);
            HPDF_Page_Stroke(page);

            HPDF_Page_SetGrayStroke(page, 0.8);
        }

        y += 5;
    }


    /* Draw virtical lines */
    x = 0;
    while (x < width) {
        if (x % 10 == 0)
            HPDF_Page_SetLineWidth(page, 0.5);
        else {
            if (HPDF_Page_GetLineWidth(page) != 0.25)
                HPDF_Page_SetLineWidth(page, 0.25);
        }

        HPDF_Page_MoveTo(page, x, 0);
        HPDF_Page_LineTo(page, x, height);
        HPDF_Page_Stroke(page);

        if (x % 50 == 0 && x > 0) {
            HPDF_Page_SetGrayStroke(page, 0.5);

            HPDF_Page_MoveTo(page, x, 0);
            HPDF_Page_LineTo(page, x, 5);
            HPDF_Page_Stroke(page);

            HPDF_Page_MoveTo(page, x, height);
            HPDF_Page_LineTo(page, x, height - 5);
            HPDF_Page_Stroke(page);

            HPDF_Page_SetGrayStroke(page, 0.8);
        }

        x += 5;
    }

    /* Draw horizontal text */
    y = 0;
    while (y < height) {
        if (y % 10 == 0 && y > 0) {
            char buf[12];

            HPDF_Page_BeginText(page);
            HPDF_Page_MoveTextPos(page, 5, y - 2);
#ifdef __WIN32__
            _snprintf(buf, 12, "%u", y);
#else
            snprintf(buf, 12, "%u", y);
#endif
            HPDF_Page_ShowText(page, buf);
            HPDF_Page_EndText(page);
        }

        y += 5;
    }


    /* Draw virtical text */
    x = 0;
    while (x < width) {
        if (x % 50 == 0 && x > 0) {
            char buf[12];

            HPDF_Page_BeginText(page);
            HPDF_Page_MoveTextPos(page, x, 5);
#ifdef __WIN32__
            _snprintf(buf, 12, "%u", x);
#else
            snprintf(buf, 12, "%u", x);
#endif
            HPDF_Page_ShowText(page, buf);
            HPDF_Page_EndText(page);

            HPDF_Page_BeginText(page);
            HPDF_Page_MoveTextPos(page, x, height - 10);
            HPDF_Page_ShowText(page, buf);
            HPDF_Page_EndText(page);
        }

        x += 5;
    }

    HPDF_Page_SetGrayFill(page, 0);
    HPDF_Page_SetGrayStroke(page, 0);
}

int PdfApi::generate_pdf_2()
{
	HPDF_Doc  pdf;
	HPDF_Page page;

	pdf = HPDF_New(error_handler, NULL);
	if (!pdf) {
		printf("error: cannot create PdfDoc object\n");
		return (-1);
	}

	if (setjmp(m_env)) {
		HPDF_Free(pdf);
		return (-1);
	}

	/* add a new page object. */
	page = HPDF_AddPage(pdf);

	HPDF_Page_SetHeight(page, 600);
	HPDF_Page_SetWidth(page, 400);

	print_grid(pdf, page);


	/* save the document to a file */
	HPDF_SaveToFile(pdf, "test_2.pdf");

	/* clean up */
	HPDF_Free(pdf);

	return 0;
}
void
show_stripe_pattern(HPDF_Page   page,
    HPDF_REAL   x,
    HPDF_REAL   y)
{
    HPDF_UINT iy = 0;

    while (iy < 50) {
        HPDF_Page_SetRGBStroke(page, 0.0, 0.0, 0.5);
        HPDF_Page_SetLineWidth(page, 1);
        HPDF_Page_MoveTo(page, x, y + iy);
        HPDF_Page_LineTo(page, x + HPDF_Page_TextWidth(page, "ABCabc123"),
            y + iy);
        HPDF_Page_Stroke(page);
        iy += 3;
    }

    HPDF_Page_SetLineWidth(page, 2.5);
}


void
show_description(HPDF_Page          page,
    HPDF_REAL          x,
    HPDF_REAL          y,
    const char* text)
{
    float fsize = HPDF_Page_GetCurrentFontSize(page);
    HPDF_Font font = HPDF_Page_GetCurrentFont(page);
    HPDF_RGBColor c = HPDF_Page_GetRGBFill(page);

    HPDF_Page_BeginText(page);
    HPDF_Page_SetRGBFill(page, 0, 0, 0);
    HPDF_Page_SetTextRenderingMode(page, HPDF_FILL);
    HPDF_Page_SetFontAndSize(page, font, 10);
    HPDF_Page_TextOut(page, x, y - 12, text);
    HPDF_Page_EndText(page);

    HPDF_Page_SetFontAndSize(page, font, fsize);
    HPDF_Page_SetRGBFill(page, c.r, c.g, c.b);
}
int PdfApi::generate_pdf_3()
{
    const char* page_title = "Text Demo";

    HPDF_Doc  pdf;
    HPDF_Font font;
    HPDF_Page page;

    const char* samp_text = "abcdefgABCDEFG123!#$%&+-@?";
    const char* samp_text2 = "The quick brown fox jumps over the lazy dog.";
    float tw;
    float fsize;
    int i;
    int len;

    float angle1;
    float angle2;
    float rad1;
    float rad2;

    float ypos;

    pdf = HPDF_New(error_handler, NULL);
    if (!pdf) {
        printf("error: cannot create PdfDoc object\n");
        return 1;
    }

    if (setjmp(m_env)) {
        HPDF_Free(pdf);
        return 1;
    }

    /* set compression mode */
    HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);

    /* create default-font */
    font = HPDF_GetFont(pdf, "Helvetica", NULL);

    /* add a new page object. */
    page = HPDF_AddPage(pdf);

    /* draw grid to the page */
    print_grid(pdf, page);

    /* print the lines of the page.
    HPDF_Page_SetLineWidth (page, 1);
    HPDF_Page_Rectangle (page, 50, 50, HPDF_Page_GetWidth(page) - 100,
                HPDF_Page_GetHeight (page) - 110);
    HPDF_Page_Stroke (page);
    */

    /* print the title of the page (with positioning center). */
    HPDF_Page_SetFontAndSize(page, font, 24);
    tw = HPDF_Page_TextWidth(page, page_title);
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, (HPDF_Page_GetWidth(page) - tw) / 2,
        HPDF_Page_GetHeight(page) - 50, page_title);
    HPDF_Page_EndText(page);

    HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, 60, HPDF_Page_GetHeight(page) - 60);

    /*
     * font size
     */
    fsize = 8;
    while (fsize < 60) {
        char buf[50];
        int len;

        /* set style and size of font. */
        HPDF_Page_SetFontAndSize(page, font, fsize);

        /* set the position of the text. */
        HPDF_Page_MoveTextPos(page, 0, -5 - fsize);

        /* measure the number of characters which included in the page. */
        strcpy(buf, samp_text);
        len = HPDF_Page_MeasureText(page, samp_text,
            HPDF_Page_GetWidth(page) - 120, HPDF_FALSE, NULL);

        /* truncate the text. */
        buf[len] = 0x00;

        HPDF_Page_ShowText(page, buf);

        /* print the description. */
        HPDF_Page_MoveTextPos(page, 0, -10);
        HPDF_Page_SetFontAndSize(page, font, 8);
#ifdef __WIN32__
        _snprintf(buf, 50, "Fontsize=%.0f", fsize);
#else
        snprintf(buf, 50, "Fontsize=%.0f", fsize);
#endif
        HPDF_Page_ShowText(page, buf);

        fsize *= 1.5;
    }

    /*
     * font color
     */
    HPDF_Page_SetFontAndSize(page, font, 8);
    HPDF_Page_MoveTextPos(page, 0, -30);
    HPDF_Page_ShowText(page, "Font color");

    HPDF_Page_SetFontAndSize(page, font, 18);
    HPDF_Page_MoveTextPos(page, 0, -20);
    len = strlen(samp_text);
    for (i = 0; i < len; i++) {
        char buf[2];
        float r = (float)i / (float)len;
        float g = 1 - ((float)i / (float)len);
        buf[0] = samp_text[i];
        buf[1] = 0x00;

        HPDF_Page_SetRGBFill(page, r, g, 0.0);
        HPDF_Page_ShowText(page, buf);
    }
    HPDF_Page_MoveTextPos(page, 0, -25);

    for (i = 0; i < len; i++) {
        char buf[2];
        float r = (float)i / (float)len;
        float b = 1 - ((float)i / (float)len);
        buf[0] = samp_text[i];
        buf[1] = 0x00;

        HPDF_Page_SetRGBFill(page, r, 0.0, b);
        HPDF_Page_ShowText(page, buf);
    }
    HPDF_Page_MoveTextPos(page, 0, -25);

    for (i = 0; i < len; i++) {
        char buf[2];
        float b = (float)i / (float)len;
        float g = 1 - ((float)i / (float)len);
        buf[0] = samp_text[i];
        buf[1] = 0x00;

        HPDF_Page_SetRGBFill(page, 0.0, g, b);
        HPDF_Page_ShowText(page, buf);
    }

    HPDF_Page_EndText(page);

    ypos = 450;

    /*
     * Font rendering mode
     */
    HPDF_Page_SetFontAndSize(page, font, 32);
    HPDF_Page_SetRGBFill(page, 0.5, 0.5, 0.0);
    HPDF_Page_SetLineWidth(page, 1.5);

    /* PDF_FILL */
    show_description(page, 60, ypos,
        "RenderingMode=PDF_FILL");
    HPDF_Page_SetTextRenderingMode(page, HPDF_FILL);
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 60, ypos, "ABCabc123");
    HPDF_Page_EndText(page);

    /* PDF_STROKE */
    show_description(page, 60, ypos - 50,
        "RenderingMode=PDF_STROKE");
    HPDF_Page_SetTextRenderingMode(page, HPDF_STROKE);
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 60, ypos - 50, "ABCabc123");
    HPDF_Page_EndText(page);

    /* PDF_FILL_THEN_STROKE */
    show_description(page, 60, ypos - 100,
        "RenderingMode=PDF_FILL_THEN_STROKE");
    HPDF_Page_SetTextRenderingMode(page, HPDF_FILL_THEN_STROKE);
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 60, ypos - 100, "ABCabc123");
    HPDF_Page_EndText(page);

    /* PDF_FILL_CLIPPING */
    show_description(page, 60, ypos - 150,
        "RenderingMode=PDF_FILL_CLIPPING");
    HPDF_Page_GSave(page);
    HPDF_Page_SetTextRenderingMode(page, HPDF_FILL_CLIPPING);
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 60, ypos - 150, "ABCabc123");
    HPDF_Page_EndText(page);
    show_stripe_pattern(page, 60, ypos - 150);
    HPDF_Page_GRestore(page);

    /* PDF_STROKE_CLIPPING */
    show_description(page, 60, ypos - 200,
        "RenderingMode=PDF_STROKE_CLIPPING");
    HPDF_Page_GSave(page);
    HPDF_Page_SetTextRenderingMode(page, HPDF_STROKE_CLIPPING);
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 60, ypos - 200, "ABCabc123");
    HPDF_Page_EndText(page);
    show_stripe_pattern(page, 60, ypos - 200);
    HPDF_Page_GRestore(page);

    /* PDF_FILL_STROKE_CLIPPING */
    show_description(page, 60, ypos - 250,
        "RenderingMode=PDF_FILL_STROKE_CLIPPING");
    HPDF_Page_GSave(page);
    HPDF_Page_SetTextRenderingMode(page, HPDF_FILL_STROKE_CLIPPING);
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 60, ypos - 250, "ABCabc123");
    HPDF_Page_EndText(page);
    show_stripe_pattern(page, 60, ypos - 250);
    HPDF_Page_GRestore(page);

    /* Reset text attributes */
    HPDF_Page_SetTextRenderingMode(page, HPDF_FILL);
    HPDF_Page_SetRGBFill(page, 0, 0, 0);
    HPDF_Page_SetFontAndSize(page, font, 30);


    /*
     * Rotating text
     */
    angle1 = 30;                   /* A rotation of 30 degrees. */
    rad1 = angle1 / 180 * 3.141592; /* Calcurate the radian value. */

    show_description(page, 320, ypos - 60, "Rotating text");
    HPDF_Page_BeginText(page);
    HPDF_Page_SetTextMatrix(page, cos(rad1), sin(rad1), -sin(rad1), cos(rad1),
        330, ypos - 60);
    HPDF_Page_ShowText(page, "ABCabc123");
    HPDF_Page_EndText(page);


    /*
     * Skewing text.
     */
    show_description(page, 320, ypos - 120, "Skewing text");
    HPDF_Page_BeginText(page);

    angle1 = 10;
    angle2 = 20;
    rad1 = angle1 / 180 * 3.141592;
    rad2 = angle2 / 180 * 3.141592;

    HPDF_Page_SetTextMatrix(page, 1, tan(rad1), tan(rad2), 1, 320, ypos - 120);
    HPDF_Page_ShowText(page, "ABCabc123");
    HPDF_Page_EndText(page);


    /*
     * scaling text (X direction)
     */
    show_description(page, 320, ypos - 175, "Scaling text (X direction)");
    HPDF_Page_BeginText(page);
    HPDF_Page_SetTextMatrix(page, 1.5, 0, 0, 1, 320, ypos - 175);
    HPDF_Page_ShowText(page, "ABCabc12");
    HPDF_Page_EndText(page);


    /*
     * scaling text (Y direction)
     */
    show_description(page, 320, ypos - 250, "Scaling text (Y direction)");
    HPDF_Page_BeginText(page);
    HPDF_Page_SetTextMatrix(page, 1, 0, 0, 2, 320, ypos - 250);
    HPDF_Page_ShowText(page, "ABCabc123");
    HPDF_Page_EndText(page);


    /*
     * char spacing, word spacing
     */

    show_description(page, 60, 140, "char-spacing 0");
    show_description(page, 60, 100, "char-spacing 1.5");
    show_description(page, 60, 60, "char-spacing 1.5, word-spacing 2.5");

    HPDF_Page_SetFontAndSize(page, font, 20);
    HPDF_Page_SetRGBFill(page, 0.1, 0.3, 0.1);

    /* char-spacing 0 */
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 60, 140, samp_text2);
    HPDF_Page_EndText(page);

    /* char-spacing 1.5 */
    HPDF_Page_SetCharSpace(page, 1.5);

    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 60, 100, samp_text2);
    HPDF_Page_EndText(page);

    /* char-spacing 1.5, word-spacing 3.5 */
    HPDF_Page_SetWordSpace(page, 2.5);

    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 60, 60, samp_text2);
    HPDF_Page_EndText(page);

    /* save the document to a file */
    HPDF_SaveToFile(pdf, "test_3.pdf");

    /* clean up */
    HPDF_Free(pdf);

    return 0;
}