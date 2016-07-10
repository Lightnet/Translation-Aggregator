#include <Shared/Shrink.h>
#include "BaiduWindow.h"

BaiduWindow::BaiduWindow() : HttpWindow(L"Baidu", L"http://translate.baidu.com/")
{
	host = L"translate.baidu.com";
	path = L"/transcontent";
	postPrefixTemplate = "ie=utf-8&source=txt&from=%s&to=%s&query=%s";
}

wchar_t *BaiduWindow::FindTranslatedText(wchar_t* html)
{
	if (ParseJSON(html, L"{\"dst\":\"", L"{\"dst\":\""))
		return html;
	if (ParseJSON(html, L"\"result\":\"", NULL))
		if (ParseJSON(html, L"\"cont\": {\"", NULL))
			return html;
	return NULL;
}

char *BaiduWindow::GetLangIdString(Language lang, int src)
{
	switch (lang)
	{
		case LANGUAGE_English:
			return "en";
		case LANGUAGE_Japanese:
			return "jp";
		case LANGUAGE_Chinese_Simplified:
			return "zh";
		case LANGUAGE_Thai:
			return "th";
		case LANGUAGE_Portuguese:
			return "pt";
		case LANGUAGE_Spanish:
			return "spa";
		default:
			return 0;
	}
}

char *BaiduWindow::GetTranslationPrefix(Language src, Language dst, const char *text)
{
	if (src == LANGUAGE_Chinese_Traditional)
		src = LANGUAGE_Chinese_Simplified;
	if (dst == LANGUAGE_Chinese_Traditional)
		dst = LANGUAGE_Chinese_Simplified;
	if (src != LANGUAGE_English && dst != LANGUAGE_English &&
		(src != LANGUAGE_Japanese || dst != LANGUAGE_Chinese_Simplified) &&
		(dst != LANGUAGE_Japanese || src != LANGUAGE_Chinese_Simplified))
		return 0;
	return HttpWindow::GetTranslationPrefix(src, dst, text);
}
