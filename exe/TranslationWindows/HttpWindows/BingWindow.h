#pragma once
#include "HttpWindow.h"

class BingWindow : public HttpWindow
{
	wchar_t *appId;
public:
	BingWindow();
	~BingWindow();
	wchar_t *FindTranslatedText(wchar_t* html);
	char* GetLangIdString(Language lang, int src);
	wchar_t *GetTranslationPath(Language src, Language dst, const wchar_t *text);
};
