/*COPYRIGHT AND PERMISSION NOTICE
*                        __              __             __	 __	  ___   __
*         /    ------   |  |    |     | |  \   //      |  | |  | |     /  \
*        /       /      |__/ __ |     | |__/  // ___   |__/ |__/ |___ /    \
*       /       /       |\      |     | |  \    /__    |\   |\   |    \   \/
*      /____.  /.       | \     |____ | |__/   ___/    | \  | \  |___  \__/\
* R-Lib's RReq | Copyright (c) 2022, Luca Trefz, Luca-Trefz@web.de
* GitHub: https://github.com/Random-typ
* 
* Permission to use, copy, modify, and distribute this software
* for any purpose with or without fee is hereby granted, provided
* that the above copyright notice and this permission notice
* appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __RREQ
#define __RREQ
#include <vector>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#include <wininet.h>
#pragma comment(lib, "Wininet.lib")
#endif
//#include <socketapi.h>

#ifndef __CPP20Enabled
#if _MSVC_LANG > __cplusplus
#if _MSVC_LANG >= 202002L
#define __CPP20Enabled 1
#else
#define __CPP20Enabled 0
#endif // _MSVC_LANG > 202002L
#else
#if __cplusplus >= 202002L
#define __CPP20Enabled 1
#else
#define __CPP20Enabled 0
#endif // _MSVC_LANG > 202002L
#endif // _MSVC_LANG > __cplusplus
#endif // !__CPP20Enabled

namespace RReq {
	class RReq;

	using Error = __int16;
	using URL = std::string;
	using RequestType = const char*;
	using DATA = std::string;

	constexpr RequestType GETRequest = "GET";
	constexpr RequestType POSTRequest = "POST";
	constexpr RequestType HEADRequest = "HEAD";
	constexpr RequestType PUTRequest = "PUT";
	constexpr RequestType DELETERequest = "DELETE";

	enum Errors {
		RequestSuccess = 0,
		OpenInternetFailed = 1,
		OpenUrlFailed = 2,
		DataQueryFailed = 3,
		ReturnDataEmtpy = 4,
		RequestSendFailed = 5
	};


	class RReq
	{
	public:
		RReq();
		~RReq();
		std::string RemoveURIProtokoll(std::string _url);

		std::string GetURIdomain(std::string _url);

		std::string GetURIpath(std::string _url);

#ifdef _WIN32
		Error Win32Req(LPCSTR _url, std::vector<std::string> _headers, bool _https);
		Error Win32Req(URL _url, RequestType _requestType, std::vector<std::string> _headers, DATA _data, bool _https);
#endif
		std::string Data;
	};
}
#endif