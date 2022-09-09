#include "RReq.h"

namespace RReq {
    RReq::RReq()
    {
    }
    RReq::~RReq()
    {
    }

    URL RReq::RemoveURIProtokoll(URL _url) {
#if __CPP20Enabled
        if (_url.starts_with("http"))
#else
        if (_url.substr(0, 4) == "http")
#endif // __CPP20Enabled
            return _url.erase(0, _url.find_first_of("://") + 3);
        return _url;
    }

    URL RReq::GetURIdomain(URL _url) {
        _url = RemoveURIProtokoll(_url);
        return _url.substr(0, _url.find_first_of("/"));
    }

    URL RReq::GetURIpath(URL _url) {
        _url = RemoveURIProtokoll(_url);
        return _url.substr(_url.find_first_of("/") + 1, _url.length());
    }

    Error RReq::Win32Req(URL _url, RequestType _requestType, std::vector<std::string> _headers, DATA _data, bool _https)
    {
        HINTERNET OpenHandle, hResourceHandle, hConnectHandle;

        if (_https)
            OpenHandle = InternetOpenA("HTTPS", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        else
            OpenHandle = InternetOpenA("HTTP", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

        if (!OpenHandle)
            return OpenUrlFailed;

        std::string headers;
        for (const auto& header : _headers)
            headers += header + "\r\n";
        
        hConnectHandle = InternetConnectA(OpenHandle,
            GetURIdomain(_url).c_str(),
            INTERNET_DEFAULT_HTTPS_PORT,
            NULL, NULL, INTERNET_SERVICE_HTTP,
            0, 1);

        if (!hConnectHandle)
        {
            InternetCloseHandle(OpenHandle);
            return false;
        }
        hResourceHandle = HttpOpenRequestA(hConnectHandle,
            _requestType,
            GetURIpath(_url).c_str(),
            NULL, NULL, NULL, INTERNET_FLAG_SECURE | INTERNET_FLAG_KEEP_CONNECTION,
            1);

        if (!hResourceHandle)
        {
            InternetCloseHandle(OpenHandle);
            InternetCloseHandle(hConnectHandle);
            return 1;
        }

        //InternetSetOption(hResourceHandle, INTERNET_OPTION_USERNAME, USERNAME, );
        //InternetSetOption(hResourceHandle, INTERNET_OPTION_PASSWORD, PASSWORD, );

        if (HttpSendRequestA(hResourceHandle, headers.c_str(), headers.size(), &_data[0], _data.size()))
            while (true)
            {
                DWORD size = 0, sizeread = 0;
                std::string data;
                if (!InternetQueryDataAvailable(hResourceHandle, &size, 0, 0))
                    return DataQueryFailed;

                if (!size)
                    break;

                data.resize(size);

                if (!InternetReadFile(hResourceHandle, &data[0], size, &sizeread))
                    return ReturnDataEmtpy;

                if (!sizeread)
                    break;

                Data += data;
            }

        InternetCloseHandle(hResourceHandle);
        InternetCloseHandle(hConnectHandle);
        InternetCloseHandle(OpenHandle);
        return RequestSuccess;
    }
    Error RReq::Win32Req(LPCSTR _url, std::vector<std::string> _headers, bool _https)
	{
        HINTERNET OpenHandl, hConnectHandle;
        
        if (_https)
            OpenHandl = InternetOpenA("HTTPS", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        else
            OpenHandl = InternetOpenA("HTTP", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

        if (!OpenHandl)
            return OpenUrlFailed;

        std::string headers;
        for (const auto &header : _headers)
            headers += header + "\r\n";

        hConnectHandle = InternetOpenUrlA(OpenHandl, _url,headers.c_str(), headers.length(), INTERNET_FLAG_EXISTING_CONNECT, 0);

        if (hConnectHandle == NULL) {
            InternetCloseHandle(OpenHandl);
            return OpenUrlFailed;
        }

        while (true)
        {
            DWORD size = 0, sizeread = 0;
            std::string data;
            if (!InternetQueryDataAvailable(hConnectHandle, &size, 0, 0))
                return DataQueryFailed;

            if (!size)
                break;

            data.resize(size);

            if (!InternetReadFile(hConnectHandle, &data[0], size, &sizeread))
                return ReturnDataEmtpy;

            if (!sizeread)
                break;

            Data += data;
        }

        InternetCloseHandle(OpenHandl);
        InternetCloseHandle(hConnectHandle);
        return RequestSuccess;
	}
}
