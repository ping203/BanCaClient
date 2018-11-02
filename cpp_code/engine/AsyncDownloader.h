#pragma once
#include "cocos2d.h"
#include <string>
#include "network/HttpClient.h"

USING_NS_CC;
class AsyncDownloader : public Ref
{
public:
	AsyncDownloader(void);
	~AsyncDownloader(void);

	static AsyncDownloader *create(std::string url, std::string filePath, std::function<void(int ret, std::string path)> callback);
	void initDownload(std::string url, std::string filePath, std::function<void(int ret, std::string path)> callback);
	void startDownload();
	void httpCallback(network::HttpClient* client, network::HttpResponse* response);
	void setCallback(std::function<void(int ret, std::string path)> callback);

public:
	std::string _url;
	std::string _filename;
	bool _dowloading;
	bool _need_quit;
	void progressDownloaded();
	std::function<void(int ret, std::string path)> _callback;
};


class HttpMultipart : public Ref
{
public:
	HttpMultipart(std::string url, std::function<void(std::string data)> _callback);
	~HttpMultipart();

	void setTargetResponseHttp(CCObject *target, network::SEL_HttpResponse pSelector);

	void addFormPart(std::string paramName, std::string value);
	void addFilePart(std::string paramName, std::string fileName, const char* data, unsigned long lengthData);
	void addImage(std::string paramName, std::string fileName, std::string path);

	void executeAsyncTask();

	static HttpMultipart *create(std::string url, std::function<void(std::string data)> _callback);
	void setCallback(std::function<void(std::string data)> callback) { this->callback = callback; }
protected:
	network::HttpRequest *cRequest;
	std::string _url;
	std::string baseData;
	std::function<void(std::string data)> callback;
	void httpCallback(network::HttpClient* client, network::HttpResponse* response);
};
