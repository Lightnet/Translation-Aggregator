#include <Shared/Shrink.h>
#include "GoogleWindow.h"
#include <Shared/StringUtil.h>

GoogleWindow::GoogleWindow() : HttpWindow(L"Google", L"http://translate.google.com/")
{
	host = L"translate.google.com";
	path = L"/translate_a/t";
	postPrefixTemplate = "client=t&sl=%s&tl=%s&ie=UTF-8&oe=UTF-8&tk=%u.%u&q=%s";
	referrer = L"http://translate.google.com/";
	dontEscapeRequest = true;
}

char *GoogleWindow::GetTranslationPrefix(Language src, Language dst, const char *text)
{
	char *srcString, *dstString;
	if (!(srcString = GetLangIdString(src, 1)) || !(dstString = GetLangIdString(dst, 0)) || !strcmp(srcString, dstString))
		return 0;

	if (!text)
		return (char*)1;

	unsigned int b = (unsigned int)(_time64(NULL) / 3600), a = b;
	for (const unsigned char *p = (const unsigned char*)text; *p; p++)
	{
		a += *p;
		a += a << 10;
		a ^= a >> 6;
	}
	a += a << 3;
	a ^= a >> 11;
	a += a << 15;
	a %= 1000000;
	b ^= a;

	char *HttpEscapeParamA(const char *src, int len);
	char *text2 = HttpEscapeParamA(text, strlen(text));
	char *out = (char*)malloc(strlen(postPrefixTemplate) + strlen(srcString) + strlen(dstString) + strlen(text2) + 1 + 16);
	sprintf(out, postPrefixTemplate, srcString, dstString, a, b, text2);
	free(text2);
	return out;
}

wchar_t *GoogleWindow::FindTranslatedText(wchar_t* html)
{
	wchar_t *end;
	if ((html = wcsstr(html, L"[[[\"")) && ((end = wcsstr(html += 4, L"\",,[[\"")) || (end = wcsstr(html, L"\",,,,,,[[\""))))
	{
		*end = 0;
		if (end = wcsstr(html, L"]],[[\""))
			*end = 0;
		ParseJSON(html, NULL, L"],[\"", true);
		return html;
	}
	return NULL;
}

char* GoogleWindow::GetLangIdString(Language lang, int src)
{
	if (src && lang == LANGUAGE_Chinese_Traditional)
		lang = LANGUAGE_Chinese_Simplified;
	switch (lang)
	{
		case LANGUAGE_AUTO:
			return "auto";
		case LANGUAGE_English:
			return "en";
		case LANGUAGE_Japanese:
			return "ja";

		case LANGUAGE_Afrikaans:
			return "af";
		case LANGUAGE_Albanian:
			return "sq";
		case LANGUAGE_Arabic:
			return "ar";
		//case LANGUAGE_Armenian:
		//	return "hy";
		//case LANGUAGE_Azerbaijani:
		//	return "az";
		//case LANGUAGE_Basque:
		//	return "eu";
		case LANGUAGE_Belarusian:
			return "be";
		case LANGUAGE_Bengali:
			return "bn";
		case LANGUAGE_Bosnian:
			return "bs";
		case LANGUAGE_Bulgarian:
			return "bg";
		case LANGUAGE_Catalan:
			return "ca";
		//case LANGUAGE_Cebuano:
		//	return "ceb";
		//case LANGUAGE_Chichewa:
		//	return "ny";
		case LANGUAGE_Chinese_Simplified:
			return "zh-CN";
		case LANGUAGE_Chinese_Traditional:
			return "zh-TW";
		case LANGUAGE_Croatian:
			return "hr";
		case LANGUAGE_Czech:
			return "cs";
		case LANGUAGE_Danish:
			return "da";
		case LANGUAGE_Dutch:
			return "nl";
		case LANGUAGE_Esperanto:
			return "eo";
		case LANGUAGE_Estonian:
			return "et";
		case LANGUAGE_Filipino:
			return "tl";
		case LANGUAGE_Finnish:
			return "fi";
		case LANGUAGE_French:
			return "fr";
		case LANGUAGE_Galician:
			return "gl";
		//case LANGUAGE_Georgian:
		//	return "ka";
		case LANGUAGE_German:
			return "de";
		case LANGUAGE_Greek:
			return "el";
		//case LANGUAGE_Gujarati:
		//	return "gu";
		case LANGUAGE_Haitian_Creole:
			return "ht";
		case LANGUAGE_Hausa:
			return "ha";
		case LANGUAGE_Hebrew:
			return "iw";
		case LANGUAGE_Hindi:
			return "hi";
		//case LANGUAGE_Hmong:
		//	return "hmn";
		case LANGUAGE_Hungarian:
			return "hu";
		case LANGUAGE_Icelandic:
			return "is";
		//case LANGUAGE_Igbo:
		//	return "ig";
		case LANGUAGE_Indonesian:
			return "id";
		case LANGUAGE_Irish:
			return "ga";
		case LANGUAGE_Italian:
			return "it";
		//case LANGUAGE_Javanese:
		//	return "jw";
		//case LANGUAGE_Kannada:
		//	return "kn";
		//case LANGUAGE_Kazakh:
		//	return "kk";
		//case LANGUAGE_Khmer:
		//	return "km";
		case LANGUAGE_Korean:
			return "ko";
		//case LANGUAGE_Lao:
		//	return "lo";
		case LANGUAGE_Latin:
			return "la";
		case LANGUAGE_Latvian:
			return "lv";
		case LANGUAGE_Lithuanian:
			return "lt";
		case LANGUAGE_Macedonian:
			return "mk";
		//case LANGUAGE_Malagasy:
		//	return "mg";
		case LANGUAGE_Malay:
			return "ms";
		//case LANGUAGE_Malayalam:
		//	return "ml";
		case LANGUAGE_Maltese:
			return "mt";
		//case LANGUAGE_Maori:
		//	return "mi";
		//case LANGUAGE_Marathi:
		//	return "mr";
		//case LANGUAGE_Mongolian:
		//	return "mn";
		//case LANGUAGE_Myanmar:
		//	return "my";
		//case LANGUAGE_Nepali:
		//	return "ne";
		case LANGUAGE_Norwegian:
			return "no";
		case LANGUAGE_Persian:
			return "fa";
		case LANGUAGE_Polish:
			return "pl";
		case LANGUAGE_Portuguese:
			return "pt";
		//case LANGUAGE_Punjabi:
		//	return "pa";
		case LANGUAGE_Romanian:
			return "ro";
		case LANGUAGE_Russian:
			return "ru";
		case LANGUAGE_Serbian:
			return "sr";
		//case LANGUAGE_Sesotho:
		//	return "st";
		//case LANGUAGE_Sinhala:
		//	return "si";
		case LANGUAGE_Slovak:
			return "sk";
		case LANGUAGE_Slovenian:
			return "sl";
		case LANGUAGE_Somali:
			return "so";
		case LANGUAGE_Spanish:
			return "es";
		//case LANGUAGE_Sundanese:
		//	return "su";
		case LANGUAGE_Swahili:
			return "sw";
		case LANGUAGE_Swedish:
			return "sv";
		//case LANGUAGE_Tajik:
		//	return "tg";
		//case LANGUAGE_Tamil:
		//	return "ta";
		//case LANGUAGE_Telugu:
		//	return "te";
		case LANGUAGE_Thai:
			return "th";
		case LANGUAGE_Turkish:
			return "tr";
		case LANGUAGE_Ukrainian:
			return "uk";
		case LANGUAGE_Urdu:
			return "ur";
		//case LANGUAGE_Uzbek:
		//	return "uz";
		case LANGUAGE_Vietnamese:
			return "vi";
		case LANGUAGE_Welsh:
			return "cy";
		case LANGUAGE_Yiddish:
			return "yi";
		//case LANGUAGE_Yoruba:
		//	return "yo";
		case LANGUAGE_Zulu:
			return "zu";
		default:
			return 0;
	}
}
