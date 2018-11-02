#include "AsyncDownloader.h"

#include <stdio.h>
#include <string>

using namespace network;
using namespace std;
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

AsyncDownloader::AsyncDownloader(void) :
	_dowloading(false),_need_quit(false),_callback(nullptr)
{
}


AsyncDownloader::~AsyncDownloader(void)
{
}

AsyncDownloader *AsyncDownloader::create(string url,string filename,std::function<void(int ret,string path)> callback)
{
	AsyncDownloader *async_ = new AsyncDownloader();
	async_->initDownload(url,filename,callback);

	return async_;
}

void AsyncDownloader::initDownload(string url,string filename,std::function<void(int ret,string path)> callback)
{
	_url = url;
	_filename = filename;
	_callback = callback;
}

void AsyncDownloader::setCallback(std::function<void(int ret,string path)> callback)
{
	_callback = callback;
}

void AsyncDownloader::startDownload()
{
	_need_quit = false;
	HttpRequest *request = new HttpRequest();
	request->setUrl(_url.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setTag(_filename.c_str());
	request->setResponseCallback(CC_CALLBACK_2(AsyncDownloader::httpCallback,this));

	HttpClient::getInstance()->setTimeoutForRead(10);
	HttpClient::getInstance()->setTimeoutForRead(10);
	HttpClient::getInstance()->send(request);
	request->release();
	
}

void AsyncDownloader::progressDownloaded()
{

}

void AsyncDownloader::httpCallback(HttpClient* client, HttpResponse* response)
{
	if(!_callback)
	{
		delete this;
		return;
	}
		
	if (!response || !response->isSucceed() || response->getResponseCode() != 200)
	{
		if(_callback)
		{
			_callback(1,"");
		}
		delete this;
		return;
	}

	vector<char> *data =  response->getResponseData();
	string dat = std::string(data->begin(),data->end());
	FILE *fp;
	fp = fopen(_filename.c_str(),"wb");
	if(!fp)
	{
		CCLOG("FUCK !");
	}
	size_t written = fwrite(dat.c_str(), dat.size(), 1, fp);
	fclose(fp);
	if (written > 0)
	{
		if(_callback)
		{
			_callback(0,_filename);
		}
	}
	else
	{
		if(_callback)
		{
			_callback(1,"");
		}
	}
	delete this;
}

//void AsyncDownloader::threadDownload(AsyncDownloader *downloader)
//{
// 	CURL *curl;
// 	FILE *fp;
// 
// 	char *url = (char *)downloader->_url.c_str();
// 	curl = curl_easy_init();
// 	if (curl) {
// 		fp = fopen(downloader->_filename.c_str(),"wb");
// 		curl_easy_setopt(curl, CURLOPT_URL, url);
// 		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
// 		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
// 		curl_easy_setopt (curl, CURLOPT_VERBOSE, 1L);
// 		res = curl_easy_perform(curl);
// 		downloader->_need_quit = true;
// 		curl_easy_cleanup(curl);
// 		fclose(fp);
// 	}
//}


string delimiter = "--";
string boundary = "Hn0968670703nH";
HttpMultipart::HttpMultipart(string url,std::function<void(string data)> _callback)
{
	_url = url;
	callback = _callback;

	cRequest = new HttpRequest();
	cRequest->setUrl(url.c_str());
	cRequest->setRequestType(HttpRequest::Type::POST);
	cRequest->setResponseCallback(CC_CALLBACK_2(HttpMultipart::httpCallback,this));

	vector<string> headers;
	headers.push_back("Connection: Keep-Alive");
	headers.push_back("Content-Type: multipart/form-data; boundary="+boundary);

	cRequest->setHeaders(headers);
	baseData = "";
}

HttpMultipart::~HttpMultipart()
{
	if (cRequest)
	{
		delete cRequest;
	}
	CCLOG("___ DELETE MULTIPART REQUEST ____");
}

void HttpMultipart::addFormPart(string paramName,string value)
{
	baseData += ("\r\n" + delimiter + boundary + "\r\n");
	baseData += ("Content-Type: text/plain\r\n");
	baseData += ("Content-Disposition: form-data; name=\"" + paramName + "\"\r\n");
	baseData += ("\r\n" + value + "\r\n");
}

void HttpMultipart::addFilePart(string paramName,string fileName,const char* data,unsigned long lengthData)
{

	baseData += (delimiter + boundary + "\r\n");
	baseData += ("Content-Disposition: form-data; name=\"" + paramName +  "\"; filename=\"" + fileName + "\"\r\n");
	baseData += ("Content-Type: application/octet-stream\r\n"  );
	baseData += ("Content-Transfer-Encoding: binary\r\n"  );
	baseData += ("\r\n");
	string _dat(data,lengthData);
	baseData += _dat;
	baseData += ("\r\n");
}

void HttpMultipart::addImage(string paramName,string fileName,string path)
{
	ssize_t buff = 0;
	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &buff);

	const char* fileBinary = (const char*)pBuffer;
	addFilePart(paramName,fileName,fileBinary,buff);
}

void HttpMultipart::executeAsyncTask()
{
	baseData +=  (delimiter + boundary + delimiter + "\r\n");
	cRequest->setRequestData(baseData.c_str(),baseData.size());
	HttpClient::getInstance()->setTimeoutForRead(10);
	HttpClient::getInstance()->setTimeoutForRead(10);
	HttpClient::getInstance()->send(cRequest);
	cRequest->release();
	cRequest = NULL;
}

HttpMultipart* HttpMultipart::create(string url,std::function<void(string data)> _callback)
{
	HttpMultipart *ret = new HttpMultipart(url,_callback);
	return ret;
}

void HttpMultipart::httpCallback(HttpClient* client, HttpResponse* response)
{
	if(!callback)
	{
		delete this;
		return;
	}
	if (!response || !response->isSucceed())
	{
		if (callback)
		{
			callback("{\"error\":1}");
		}
		return;
	}
	CCLOG("%s is completed",response->getHttpRequest()->getTag());

	std::vector<char> *buffer = response->getResponseData();
	std::string result(buffer->begin(),buffer->end());

	if (callback)
	{
		callback(result);
	}

	delete this;
}

