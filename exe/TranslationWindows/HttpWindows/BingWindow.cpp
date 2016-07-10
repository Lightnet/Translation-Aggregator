#include <Shared/Shrink.h>
#include "BingWindow.h"

BingWindow::BingWindow() : HttpWindow(L"Bing", L"http://www.microsofttranslator.com/")
{
	host = L"api.microsofttranslator.com";
	path = L"/v2/ajax.svc/TranslateArray2?appId=%%22%s%%22&from=%%22%hs%%22&to=%%22%hs%%22&texts=%%5B%%22%s%%22%%5D";
	requestHeaders = L"";
	appId = NULL;
}
BingWindow::~BingWindow()
{
	free(appId);
}

wchar_t *BingWindow::FindTranslatedText(wchar_t* html)
{
	if (!ParseJSON(html, L"\"TranslatedText\":\"", L"\"TranslatedText\":\""))
		return NULL;
	return html;
}

char* BingWindow::GetLangIdString(Language lang, int src)
{
	switch (lang)
	{
		case LANGUAGE_AUTO:
			return "";
		case LANGUAGE_English:
			return "en";
		case LANGUAGE_Japanese:
			return "ja";

		case LANGUAGE_Hebrew:
			return "he";
		case LANGUAGE_Queretaro_Otomi:
			return "otq";
		case LANGUAGE_Arabic:
			return "ar";
		case LANGUAGE_Hindi:
			return "hi";
		case LANGUAGE_Romanian:
			return "ro";
		case LANGUAGE_Bosnian:
			return "bs-Latn";
		case LANGUAGE_Hmong_Daw:
			return "mww";
		case LANGUAGE_Russian:
			return "ru";
		case LANGUAGE_Bulgarian:
			return "bg";
		case LANGUAGE_Hungarian:
			return "hu";
		case LANGUAGE_Serbian:
			return "sr-Cyrl";
		case LANGUAGE_Catalan:
			return "ca";
		case LANGUAGE_Indonesian:
			return "id";
		case LANGUAGE_Chinese_Simplified:
			return "zh-CHS";
		case LANGUAGE_Italian:
			return "it";
		case LANGUAGE_Slovak:
			return "sk";
		case LANGUAGE_Chinese_Traditional:
			return "zh-CHT";
		case LANGUAGE_Slovenian:
			return "sl";
		case LANGUAGE_Croatian:
			return "hr";
		case LANGUAGE_Klingon:
			return "tlh";
		case LANGUAGE_Spanish:
			return "es";
		case LANGUAGE_Czech:
			return "cs";
		case LANGUAGE_Swedish:
			return "sv";
		case LANGUAGE_Danish:
			return "da";
		case LANGUAGE_Korean:
			return "ko";
		case LANGUAGE_Thai:
			return "th";
		case LANGUAGE_Dutch:
			return "nl";
		case LANGUAGE_Latvian:
			return "lv";
		case LANGUAGE_Turkish:
			return "tr";
		case LANGUAGE_Lithuanian:
			return "lt";
		case LANGUAGE_Ukrainian:
			return "uk";
		case LANGUAGE_Estonian:
			return "et";
		case LANGUAGE_Malay:
			return "ms";
		case LANGUAGE_Urdu:
			return "ur";
		case LANGUAGE_Finnish:
			return "fi";
		case LANGUAGE_Maltese:
			return "mt";
		case LANGUAGE_Vietnamese:
			return "vi";
		case LANGUAGE_French:
			return "fr";
		case LANGUAGE_Norwegian:
			return "no";
		case LANGUAGE_Welsh:
			return "cy";
		case LANGUAGE_German:
			return "de";
		case LANGUAGE_Persian:
			return "fa";
		case LANGUAGE_Yucatec_Maya:
			return "yua";
		case LANGUAGE_Greek:
			return "el";
		case LANGUAGE_Polish:
			return "pl";
		case LANGUAGE_Haitian_Creole:
			return "ht";
		case LANGUAGE_Portuguese:
			return "pt";
		default:
			return 0;
	}
}

char *DownloadFile(HINTERNET hConnect, const wchar_t *url)
{
	char *data = NULL;
	if (HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", url, 0, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0))
	{
		if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
			WinHttpReceiveResponse(hRequest, NULL))
		{
			size_t size = 0;
			for (;;)
			{
				DWORD len;
				if (!WinHttpQueryDataAvailable(hRequest, &len) || !len)
					break;
				char *data2 = (char*)realloc(data, size + len + 1);
				if (!data2)
					break;
				data = data2;
				if (!WinHttpReadData(hRequest, data + size, len, &len) || !len)
					break;
				size += len;
			}
			if (data)
				data[size] = 0;
		}
		WinHttpCloseHandle(hRequest);
	}
	return data;
}

wchar_t *BingWindow::GetTranslationPath(Language src, Language dst, const wchar_t *text)
{
	char *srcString, *dstString;
	if (src == dst || !(srcString = GetLangIdString(src, 1)) || !(dstString = GetLangIdString(dst, 0)))
		return NULL;

	if (!text)
		return _wcsdup(L"");

	if (!appId)
	{
		extern const wchar_t userAgent[];
		if (HINTERNET hSession = WinHttpOpen(userAgent, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0))
		{
			if (HINTERNET hConnect = WinHttpConnect(hSession, L"www.bing.com", INTERNET_DEFAULT_HTTP_PORT, 0))
			{
				if (char *data = DownloadFile(hConnect, L"/translator/"))
				{
					if (char *p = strstr(data, "/LandingPage.js?"))
					{
						char *s, *e;
						for (s = p - 1; s >= data && *s != '\'' && *s != '"'; s--);
						if (s >= data && (e = strchr(p, *s)))
						{
							*e = 0;
							if (wchar_t *url = (wchar_t*)malloc((e - s + 12)*sizeof(wchar_t)))
							{
								wcscpy(url, L"/translator/");
								MultiByteToWideChar(CP_UTF8, 0, s + 1, -1, url + 12, e - s);
								if (char *data = DownloadFile(hConnect, url))
								{
									if (char *p = strstr(data, "rttAppId:"))
										if (char *s = strchr(p + 9, '"'))
											if (char *e = strchr(s + 1, '"'))
												if (appId = (wchar_t*)malloc((e - s)*sizeof(wchar_t)))
												{
													*e = 0;
													MultiByteToWideChar(CP_UTF8, 0, s + 1, -1, appId, e - s);
												}
									free(data);
								}
								free(url);
							}
						}
					}
					free(data);
				}
				WinHttpCloseHandle(hConnect);
			}
			WinHttpCloseHandle(hSession);
		}
		if (!appId)
			appId = _wcsdup(L"");
	}
	size_t count = _snwprintf(NULL, 0, path, appId, srcString, dstString, text) + 1;
	wchar_t *data = (wchar_t*)malloc(count*sizeof(wchar_t));
	if (data)
		_snwprintf(data, count, path, appId, srcString, dstString, text);
	return data;
}
