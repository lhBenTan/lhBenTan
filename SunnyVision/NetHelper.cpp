#include <iostream>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")		// 套接字编程需要的库文件

constexpr auto MULIT_IP = "239.255.255.250";	// 组播组的IP地址
constexpr auto MULIT_PORT = 37020;				// 组播端口

/// 获取本地IP
char* get_local_ip() {
	char hostname[100];

	WSADATA wsadata;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsadata))   //初始化
	{
		MessageBox(NULL, "初始化网络环境失败!", "ERROR", MB_OK);
		return NULL;
	}
	gethostname(hostname, 100);
	auto phost = gethostbyname(hostname);

	char *pIP = inet_ntoa(*(in_addr *)phost->h_addr_list[0]);	//将32位IP转化为字符串IP
	WSACleanup();												//释放Winsock API

	return pIP;
}

/// 搜索海康设备并返回IP地址
int _stdcall HikDevDetect(char* dstIP, int &length)
{
	std::string LOCAL_IP;
	// 获取本地IP
	LOCAL_IP = get_local_ip();

	WSADATA WSAData;
	if (0 != WSAStartup(MAKEWORD(2, 2), &WSAData))
	{
		MessageBox(NULL, "初始化网络环境失败!", "ERROR", MB_OK);
		return -1;
	}

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == sock)
	{
		MessageBox(NULL, "初始化Socket失败!", "ERROR", MB_OK);
		return -1;
	}

	int arg = 1000;
	auto err = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&arg, sizeof(arg));
	if (0 > err)
	{
		MessageBox(NULL, "Set timeout failed.", "ERROR", MB_OK);
		closesocket(sock);
		return -1;
	}

	arg = 1;
	err = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&arg, sizeof(arg));
	if (0 > err)
	{
		MessageBox(NULL, "Reusing ADDR failed.", "ERROR", MB_OK);
		closesocket(sock);
		return -1;
	}

	arg = 0;
	err = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&arg, sizeof(arg));
	if (err < 0)
	{
		MessageBox(NULL, "Set loop failed.", "ERROR", MB_OK);
		closesocket(sock);
		return -1;
	}

	sockaddr_in local_addr;
	memset(&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.S_un.S_addr = inet_addr(LOCAL_IP.c_str());
	local_addr.sin_port = htons(MULIT_PORT);
	err = bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr));
	if (0 > err)
	{
		MessageBox(NULL, "Bind err.", "ERROR", MB_OK);
		closesocket(sock);
		return -1;
	}

	ip_mreq multiCast;
	multiCast.imr_interface.S_un.S_addr = inet_addr(LOCAL_IP.c_str());
	multiCast.imr_multiaddr.S_un.S_addr = inet_addr(MULIT_IP);
	err = setsockopt(sock, IPPROTO_IP, 12, (char *)&multiCast, sizeof(multiCast));
	if (0 > err)
	{
		MessageBox(NULL, "ADD MEMBERSHIP err.", "ERROR", MB_OK);
		closesocket(sock);
		return -1;
	}

	char req[256];
	GUID guid;
	CoCreateGuid(&guid);
	sprintf_s(req,
		"<?xml version=\"1.0\" encoding=\"utf-8\"?><Probe><Uuid>%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X</Uuid><Types>inquiry</Types></Probe>",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);

	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(MULIT_IP);
	saddr.sin_port = htons(MULIT_PORT);
	err = sendto(sock, req, strlen(req), 0, (sockaddr *)&saddr, sizeof(saddr));
	if (0 > err)
	{
		MessageBox(NULL, "Send to err.", "ERROR", MB_OK);
		closesocket(sock);
		return -1;
	}
	Sleep(10);

	int addrLen = sizeof(saddr);
	char buff[4096];
	memset(buff, 0, 4096);

	// 在socket 2.0版本中需要执行2次
	int len = recvfrom(sock, buff, 4096, 0, (sockaddr *)&saddr, &addrLen);
	len = recvfrom(sock, buff, 4096, 0, (sockaddr *)&saddr, &addrLen);

	closesocket(sock);

	std::string ip;
	if (0 < len)
	{
		int a, b;
		std::string msg;

		// OutputDebugStringA(buff);

		msg = buff;

		a = msg.rfind("<IPv4Address>") + std::strlen("<IPv4Address>");
		b = msg.find("</IPv4Address>") - a;
		ip = msg.substr(a, b);
		OutputDebugStringA(ip.c_str());
	}

	std::copy(ip.begin(), ip.end(), dstIP);
	length = ip.length();

	return 0;
}