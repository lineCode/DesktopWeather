#include "StdAfx.h"
#pragma once
#include <UIlib.h>
#include "urlcoding.h"
#include "curl/curl.h"
#include "json/json.h"
#pragma comment(lib, "libcurl_imp.lib")
using namespace DuiLib;
using namespace std;
#include<iostream>

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#pragma comment(lib, "lib_json_d.lib")
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#pragma comment(lib, "lib_json.lib")
#endif

class CWeatherFrameWnd : public WindowImplBase
{
public:
	Json::Value m_weatherDict;
	Json::Value m_weatherPicPath;
	HANDLE m_hMutex;
	CWeatherFrameWnd()
	{
		m_hMutex = CreateMutex(NULL,FALSE,_T("xiamiweather"));

		if(GetLastError() == ERROR_ALREADY_EXISTS)
		{ //����Ѿ�����ͬ����Mutex��õ��������.
			CloseHandle(m_hMutex);
			exit(0);
		}
	}

	~CWeatherFrameWnd()
	{
		CloseHandle(m_hMutex);
	}

	void initWeatherDict()
	{
		 m_weatherPicPath["big"] = "skin\\weather\\bigicon\\";
		 m_weatherPicPath["small"] = "skin\\weather\\icon\\";
		//m_weatherDict["��"] = "1.png";
		m_weatherDict["�����"] = "1.png";
		m_weatherDict["������"] = "2.png";
		//m_weatherDict["��"] = "3.png";
		m_weatherDict["������"] = "3.png";
		m_weatherDict["������"] = "3.png";
		//m_weatherDict["����"] = "5.png";
		m_weatherDict["���ư���"] = "5.png";
		m_weatherDict["��������"] = "6.png";
		m_weatherDict["����"] = "10.png";//
		m_weatherDict["������"] = "15.png";
		m_weatherDict["��������б���"] = "16.png";
		m_weatherDict["���ѩ"] = "14.png";
		m_weatherDict["С��"] = "8.png";
		m_weatherDict["����"] = "9.png";
		m_weatherDict["����"] = "10.png";
		m_weatherDict["����"] = "11.png";
		m_weatherDict["����"] = "12.png";
		m_weatherDict["�ش���"] = "13.png";
		m_weatherDict["��ѩ"]= "18.png";//
		m_weatherDict["Сѩ"] = "17.png";
		m_weatherDict["��ѩ"] = "18.png";
		m_weatherDict["��ѩ"] = "19.png";
		m_weatherDict["��ѩ"] = "20.png";
		m_weatherDict["��"] = "7.png";//
		m_weatherDict["����"] = "14.png";//
		m_weatherDict["ɳ����"] = "23.png";
		//m_weatherDict["С��ת����"] = "8.png";
		//m_weatherDict["����ת����"] = "9.png";
		//m_weatherDict["����ת����"] = "10.png";
		//m_weatherDict["����ת����"] = "11.png";
		//m_weatherDict["����ת�ش���"] = "12.png";
		//m_weatherDict["Сѩת��ѩ"] = "17.png";
		//m_weatherDict["��ѩת��ѩ"] = "18.png";
		//m_weatherDict["��ѩת��ѩ"] = "19.png";
		m_weatherDict["����"] = "21.png";
		m_weatherDict["��ɳ"] = "22.png";
		m_weatherDict["ǿɳ����"] = "24.png";
		m_weatherDict["��"] = "7.png";//��Щ���Ը�Ϊ�������ļ����
		
	}
	
    virtual LPCTSTR    GetWindowClassName() const   {   return _T("XiamiWeatherFrame");  }
    virtual CDuiString GetSkinFile()                {   return _T("ui.xml");  }
    virtual CDuiString GetSkinFolder()              {   return _T("");  }

	void UpdateWeather(Json::Value weatherinfo)
	{
		const Json::Value results = weatherinfo["results"];
		const Json::Value weather_data = results[0]["weather_data"];
		SetCity(results[0]["currentCity"].asCString());
		SetWeather(weather_data[0]["weather"].asCString());
		SetTemp(weather_data[0]["temperature"].asCString());
		SetWind(weather_data[0]["wind"].asCString());
		SetDate(weather_data[0]["date"].asCString());
		SetPM25(results[0]["pm25"].asCString());
		for(int i=1; i<4; i++)
		{
			SetOtherDayWeather(weather_data[i], i);
		}
	}

	void SetCity(const char* citytext)
	{   
		CControlUI* cityctrl = m_PaintManager.FindControl(_T("City"));
		if(cityctrl)
		{
			cityctrl->SetText(citytext);
		}
	}

	void SetWeatherPic(const char* weatherpic)
	{
		CControlUI* weatherPicCtrl = m_PaintManager.FindControl(_T("WeatherPic"));
		if(weatherPicCtrl)
		{
			weatherPicCtrl->SetBkImage(weatherpic);
		}
	}

	void SetWind(const char* text)
	{
		CControlUI* tempctrl = m_PaintManager.FindControl(_T("Wind"));
		if(tempctrl)
		{
			tempctrl->SetText(text);
		}
	}

	void SetDate(const char* text)
	{
		 
		CControlUI* tempctrl = m_PaintManager.FindControl(_T("curtemp"));
		if(tempctrl)
		{
			tempctrl->SetText(text);
		}
		tempctrl = m_PaintManager.FindControl(_T("date"));
		if(tempctrl)
		{
			string text1 =  strtok((char*)text, " ");
			tempctrl->SetText(text1.c_str());
		}	
	}

	void SetPM25(const char* text)
	{
		CControlUI* tempctrl = m_PaintManager.FindControl(_T("pm25"));
		if(tempctrl)
		{
			int pm = 0;
			string pmtext = text;
			sscanf(text, "%d", &pm);
			if(pm < 51)
			{
				pmtext += "��";
				tempctrl->SetBkColor(0xFF348E25);
			}
			else if(pm < 101)
			{
				pmtext += "��";
				tempctrl->SetBkColor(0xFFAFDB00);
			}
			else if(pm < 151)
			{
				pmtext += "�����Ⱦ";
				tempctrl->SetBkColor(0xFFFFA07A);
			}
			else if(pm < 201)
			{
				pmtext += "�ж���Ⱦ";
				tempctrl->SetBkColor(0xFFAF1010);
			}
			else if(pm < 301)
			{
				pmtext += "�ض���Ⱦ";
				tempctrl->SetBkColor(0xFFEE00EE);
			}
			else
			{
				pmtext += "������Ⱦ";
				tempctrl->SetBkColor(0xFF8B1A1A);
			}

		
			tempctrl->SetText(pmtext.c_str());
		}
	}

	string GetWeatherPicPath(const char* weathertext, bool flag=false)
	{
		string weather = weathertext;
		string picpath = m_weatherDict.get(weather, "").asString();
		if ( "" == picpath)//�����ҵ�
		{
			//�������ܲ��ܲ��г�ת���������ҵ���ת���Ѿ�����ֱ������
			weather = (char*)wcstok((wchar_t*)weather.c_str(), (const wchar_t*)"ת");
			picpath = m_weatherDict.get(weather, "").asString();
			if ( "" == picpath)
			{
				//�����Ҳ����Ǿͼ���ʱ��
				SYSTEMTIME sys; 
				GetLocalTime(&sys);
				if(sys.wHour > 18 || sys.wHour < 7)
					weather += "����";
				else if(sys.wHour > 6)
					weather += "����";
				picpath = m_weatherDict.get(weather, "").asString();
			}
		}
		else//���ҵ���Ҳ������ʱ�俴��
		{		
				SYSTEMTIME sys; 
				GetLocalTime(&sys);
				if(sys.wHour > 18 || sys.wHour < 7 )
					weather += "����";
				else if(sys.wHour > 6)
					weather += "����";
				picpath = m_weatherDict.get(weather, "").asString();
				if ( "" == picpath)
				{
					picpath = m_weatherDict.get(weathertext, "").asString();
				}
		}
		string pictype = "big";
		if(true == flag)
		{
			pictype = "small";
		}
		picpath = m_weatherPicPath.get(pictype, "").asString() + picpath;
		//cout << "picpath��" << picpath << endl;

		return picpath;
	}

	void SetWeather(const char* weathertext)
	{
		CControlUI* tempctrl = m_PaintManager.FindControl(_T("Weather"));
		if(tempctrl)
		{
			tempctrl->SetText(weathertext);
		}
		 
		string picpath = GetWeatherPicPath(weathertext);
		SetWeatherPic(picpath.c_str());
	}

	void SetTemp(const char* temptext)
	{
		CControlUI* tempctrl = m_PaintManager.FindControl(_T("temp"));
		if(tempctrl)
		{
			tempctrl->SetText(temptext);
		}
	}

	void SetOtherDayWeather(Json::Value weatherinfo, int i)
	{
		char buffer[255];
		memset(buffer, 0, 255);
		sprintf(buffer, "Wind%d", i);
		string ctrlname = buffer;
		CControlUI* tempctrl = m_PaintManager.FindControl(ctrlname.c_str());
		if(tempctrl)
		{
			tempctrl->SetText(weatherinfo["wind"].asCString());
		}

		memset(buffer, 0, 255);
		sprintf(buffer, "Weather%d", i);
		ctrlname = buffer;
		tempctrl = m_PaintManager.FindControl(ctrlname.c_str());
		if(tempctrl)
		{
			tempctrl->SetText(weatherinfo["weather"].asCString());
		}

		memset(buffer, 0, 255);
		sprintf(buffer, "temp%d", i);
		ctrlname = buffer;
		tempctrl = m_PaintManager.FindControl(ctrlname.c_str());
		if(tempctrl)
		{
			tempctrl->SetText(weatherinfo["temperature"].asCString());
		}

		memset(buffer, 0, 255);
		sprintf(buffer, "date%d", i);
		ctrlname = buffer;
		tempctrl = m_PaintManager.FindControl(ctrlname.c_str());
		if(tempctrl)
		{
			tempctrl->SetText(weatherinfo["date"].asCString());
		}

		memset(buffer, 0, 255);
		sprintf(buffer, "WeatherPic%d", i);
		ctrlname = buffer;
		tempctrl = m_PaintManager.FindControl(ctrlname.c_str());
		if(tempctrl)
		{
			string picpath = GetWeatherPicPath(weatherinfo["weather"].asCString(), true);
			tempctrl->SetBkImage(picpath.c_str());
		}
	}

	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)// ����˫�����������
    {
		if(WM_NCLBUTTONDBLCLK != uMsg &&  WM_SIZE !=uMsg)
		{
			return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
		}
      
   return 0;
}
};

/**
 * һ��curl���յ����ݣ��ͻ���ô˻ص�����
 * buffer:���ݻ�����ָ��
 * size:���Խ׶����Ƿ���Ϊ1
 * nmemb:(memory block)����˴ν��ܵ��ڴ��ĳ���
 * writerData:�û��Զ����һ������
 */
size_t write_data(char* data, size_t size, size_t nmemb, string* writerData)
{
    if (writerData == NULL)
	{
        return 0;
	}
	int len = size*nmemb;
	writerData->append(data, len);
	return len;
}

string UrlRequestData(string urlstr)
{
	curl_global_init(CURL_GLOBAL_ALL); // ����ȫ�ֳ�ʼ��CURL
    CURL* curl = curl_easy_init(); // ��ʼ��CURL���

    if (NULL == curl)
    {
		::MessageBox(NULL, "��鿴�����Ƿ����","NULL",1);
        return "";
    }
	struct curl_slist *chunk = NULL;  
    if (curl)  
    {  
        cout << curl_version() << endl;  
        chunk = curl_slist_append(chunk, "Accept:text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");  
        chunk = curl_slist_append(chunk, "Accept-Charset:GBK,utf-8;q=0.7,*;q=0.3");  
        chunk = curl_slist_append(chunk, "Accept-Language:zh-TW,zh;q=0.8,en-US;q=0.6,en;q=0.4,zh-CN;q=0.2");  
        //chunk = curl_slist_append(chunk, "Connection:keep-alive");    
        chunk = curl_slist_append(chunk, "Accept-Encoding:gunzip,defate");  
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.64 Safari/537.31");  
        chunk = curl_slist_append(chunk, "Expect:");
	}
    string buffer;
    // ����Ŀ��URL
	

	curl_easy_setopt(curl, CURLOPT_URL, urlstr.c_str());
    //curl_easy_setopt(curl, CURLOPT_URL, "http://api.map.baidu.com/telematics/v3/weather?location=%E5%8C%97%E4%BA%AC&output=json&ak=EAdbf1bd711caf257fb60f507f9b8e4c");
    // ���ý��յ�HTTP������������ʱ���õĻص�����
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    // �����Զ������(�ص������ĵ��ĸ�����)
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	// ����header
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
	//curl_easy_setopt(curl,CURLOPT_USERAGENT,_T("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; SV1; .NET CLR 1.1.4322)Accept-Encoding: gzip")); 
    // ִ��һ��URL����
    CURLcode res = curl_easy_perform(curl);
    // ����ɾ�
    curl_easy_cleanup(curl);
	//Sleep(1000);

	if (buffer.empty())
    {
		//::MessageBox(NULL, "��������ʧ��","��ʾ",1);
		cout << "��������ʧ��" << endl;
		return "";
	}
	
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buffer.c_str(), -1, NULL,0); 
	unsigned short* wszGBK = new unsigned short[len+1];       
	memset(wszGBK, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buffer.c_str(), -1, (LPWSTR)wszGBK, len); 
	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	char *szGBK=new char[len + 1]; 
	memset(szGBK, 0, len + 1); 
	WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)wszGBK, -1, (LPSTR)szGBK, len, NULL,NULL); 				
	buffer = szGBK;
	
	//std::cout << buffer << std::endl;
	return buffer;
}


void WriteDataToFile(char* filepath, const char* buffer)//�����������һ�����
{
	HANDLE hFile;
	DWORD nBytes;

	hFile = ::CreateFile(filepath,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,0,NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		::WriteFile(hFile,buffer, strlen(buffer), &nBytes, NULL);
		CloseHandle(hFile);
	}
}

string ReadDataFromFile(char* filename)
{
	HANDLE pfile;
	static string bufferstr = "";

	pfile = ::CreateFile(filename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);//�����������OpenFile��
	if(pfile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,"�������ļ�ʧ��","��ʾ",MB_OK);
		CloseHandle(pfile);//һ��ע���ں����˳�֮ǰ�Ծ�������ͷš�
		exit(0);
		//return bufferstr;
	}
	DWORD filesize = GetFileSize(pfile,NULL);
	char*buffer = new char[filesize+1];
	DWORD readsize;

	ReadFile(pfile,buffer,filesize,&readsize,NULL);
	buffer[filesize] = 0;
	bufferstr = buffer;
	delete[]buffer;
	CloseHandle(pfile);
	//std::cout << bufferstr << std::endl;
	return bufferstr;
}
string GetCityInfoByIP_Sina()//���˽ӿ�
{
	string retstr = "";
	string buffer = UrlRequestData("http://int.dpool.sina.com.cn/iplookup/iplookup.php?format=js");
	if (buffer == "")
	{
		buffer = ReadDataFromFile(_T("cityinfo-sina.json"));
		if(buffer == "")
			MessageBox(NULL,"�������ļ�ʧ��","��ʾ",MB_OK);
			return retstr;
	}

    static Json::Value root;
    Json::Reader reader;
	bool parsingSuccessful = reader.parse(buffer.replace(0,strlen("var remote_ip_info = "),1,' '), root);
    if(!parsingSuccessful)
    {
		::MessageBox(NULL, "���ݽ�������","��ʾ",1);
		return retstr;
	}

	static const Json::Value status = root["ret"];
	if (1 != status.asInt())
	{
		::MessageBox(NULL, "���س���","��ʾ",1);
		return retstr;
	}

	WriteDataToFile(_T("cityinfo-sina.json"), buffer.c_str());
	return root["city"].asString();
}

string GetCityInfoByIP()//�ٶȽӿ�
{
	string retstr = "";
	string buffer = UrlRequestData("http://api.map.baidu.com/location/ip?ak=EAdbf1bd711caf257fb60f507f9b8e4c");
	if (buffer == "")
	{
		buffer = ReadDataFromFile(_T("cityinfo.json"));
		if(buffer == "")
			return retstr;
	}

    static Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(buffer, root);
    if(!parsingSuccessful)
    {
		::MessageBox(NULL, "���ݽ�������","��ʾ",1);
		return retstr;
	}

	static const Json::Value status = root["status"];
	if (0 != status.asInt())
	{
		::MessageBox(NULL, "APP SNУ�����","��ʾ",1);
		return retstr;
	}

	WriteDataToFile(_T("cityinfo.json"), buffer.c_str());
	return root["content"]["address_detail"]["city"].asString();
}

Json::Value GetWeatherInfoByCity(const char * citystr)
{
    strCoding sC;
	string urlstr = "http://api.map.baidu.com/telematics/v3/weather?output=json&ak=EAdbf1bd711caf257fb60f507f9b8e4c&location=";
	string urlcitystr = sC.UrlUTF8((char*)citystr);
	urlstr += urlcitystr;

	cout << "RequestUrl: " << urlstr << endl;

	string buffer = UrlRequestData(urlstr);
	
	if (buffer == "")
	{
		buffer = ReadDataFromFile(_T("weatherinfo.json"));
		if(buffer == "")
			return NULL;
	}
    static Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(buffer, root);
    if(!parsingSuccessful)
    {
		::MessageBox(NULL, "���ݽ�������","��ʾ",1);
		return NULL;
	}

	static const Json::Value status = root["status"];
	if (true == status.isInt())
	{
		::MessageBox(NULL, "APP SNУ�����","��ʾ",1);
		return NULL;
	}

	static const Json::Value errorcode = root["error"];
	if (0 != errorcode.asInt())
	{
		buffer = ReadDataFromFile(_T("weatherinfo.json"));
		if(buffer == "")
			return NULL;
		Json::Reader reader;
		bool parsingSuccessful = reader.parse(buffer, root);
		if(!parsingSuccessful)
		{
			::MessageBox(NULL, "���ݽ�������","��ʾ",1);
			return NULL;
		}

		static const Json::Value status = root["status"];
		if (true == status.isInt())
		{
			::MessageBox(NULL, "APP SNУ�����","��ʾ",1);
			return NULL;
		}
	}
	
	WriteDataToFile(_T("weatherinfo.json"), buffer.c_str());

    return root;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	#ifdef _DEBUG
		AllocConsole();
		freopen( "CONOUT$", "w+t", stdout );// ����д
		freopen( "CONIN$", "r+t", stdin );// �����
	#else	
		CPaintManagerUI::SetResourceZip(_T("skin.zip"));
		CPaintManagerUI::ReloadSkin();
	#endif

	cout << "By: whitefirer@gmail.com" << endl;
	//static const Json::Value weatherinfo = GetWeatherInfoByCity("����");
	string city = GetCityInfoByIP_Sina();
	if(city == "")
	{
		city == GetCityInfoByIP();
	}
	static const Json::Value weatherinfo = GetWeatherInfoByCity(city.c_str());
	if(weatherinfo == NULL)
	{
		return 0;
	}
	cout << weatherinfo.toStyledString() << endl;

    CPaintManagerUI::SetInstance(hInstance);

    CWeatherFrameWnd WeatherFrame;
	WeatherFrame.initWeatherDict();
	WeatherFrame.Create(NULL, _T("Ϻ������"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
    //duiFrame.CenterWindow();
	WeatherFrame.SetPosAtCornerWindow(2, 2, 2);
	WeatherFrame.UpdateWeather(weatherinfo);
    WeatherFrame.ShowModal();
	
    return 0;
}