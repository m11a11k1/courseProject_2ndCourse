#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <locale>
#include <windows.h>
using namespace std;

struct hostent *selfaddr,*remoteHost;
struct in_addr addr={0};

char **self_ip_define(char *host_name);
int ping_one(char *);
char *delete_last_three(char *);
int get_hostnames(char **target_ips,int array_size);
DWORD threadproc1(LPVOID args);
DWORD threadproc2(LPVOID args);
char **first_network_ip_array;
char **second_network_ip_array;
char **self_ip_array;
char *self_ip_wo3,*self_ip_wo3_1, last3[4];
FILE *network;

int main()
{
	setlocale(0,"Russian");
	char *self_network_name=new char [20];
	self_ip_wo3=new char [15];
	self_ip_wo3_1=new char [15];
	int res=0;
	network=fopen("network.txt","w");
	first_network_ip_array=new char* [256];
	HANDLE *pings = new HANDLE[10];
	DWORD t;
	for (int i=1;i<256;i++)
		first_network_ip_array[i]=new char [16];
	second_network_ip_array=new char* [256];
	for (int i=1;i<256;i++)
		second_network_ip_array[i]=new char [16];

	WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
	gethostname(self_network_name,20);
	self_ip_array=self_ip_define(self_network_name);
	//cout << self_ip_array[0]<<endl;
	//cout << self_ip_array[1]<<endl;

	fprintf(network,"%s\n","127.0.0.1");
	if(!self_ip_array[0]||!strcmp(self_ip_array[0],"127.0.0.1")) {MessageBox(0,L"Вы не подключены ни к 1 сети",L"Houston, we have a problem",MB_OK); return 0;}
	//cout<<ping_one(self_ip_array[0])<<endl;
	if(ping_one(self_ip_array[1]) != -1){self_ip_array[1]=nullptr;}
	self_ip_wo3=delete_last_three(self_ip_array[0]);

	
	
	if (self_ip_array[1])
	{
		self_ip_wo3_1=delete_last_three(self_ip_array[1]);
		for (int i=0;i<5;i++) 
			{
				pings[i]=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)threadproc2,(LPVOID)(i*50),0,&t);
				pings[i+5]=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)threadproc1,(LPVOID)(i*50),0,&t);
		    }
		WaitForMultipleObjects(10,pings,TRUE,INFINITE);
	}
	else
	{
		for (int i=0;i<5;i++) pings[i]=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)threadproc1,(LPVOID)(i*50),0,&t);
		WaitForMultipleObjects(5,pings,TRUE,INFINITE);
	}

	
	fclose(network);
	for (int i=1;i<256;i++)
		delete first_network_ip_array[i];
	delete first_network_ip_array;
	for (int i=1;i<256;i++)
		delete second_network_ip_array[i];
	delete second_network_ip_array;
																							
	first_network_ip_array=new char* [256];
	for (int i=1;i<256;i++)
		first_network_ip_array[i]=new char [16];
	int size_ip_array=0,j=1;

	network=fopen("network.txt","r");
	while (fscanf(network,"%s",first_network_ip_array[j]))
	{
		j++;
		if (first_network_ip_array[j-1][0]!='1'&&first_network_ip_array[j-1][0]!='2')
			break;
	}
	size_ip_array=j-1;
	get_hostnames(first_network_ip_array,size_ip_array);
																							
	WSACleanup();
	return 0;
}

char **self_ip_define(char *host_name)
{

    //-----------------------------------------
    // Declare and initialize variables
   
    int iResult;

    DWORD dwError;
    int i = 0;

    struct hostent *remoteHost;
    struct in_addr addr;

    char **pAlias;
	char **self_ip_array=(char **)calloc(2,sizeof(char *));
	self_ip_array[0]=(char *)calloc(16,sizeof(char ));
	self_ip_array[1]=(char *)calloc(16,sizeof(char ));
	char *self_ip_array0=(char *)calloc(16,sizeof(char ));

    printf("Calling gethostbyname with %s\n", host_name);
    remoteHost = gethostbyname(host_name);
    
    if (remoteHost == NULL) {
        dwError = WSAGetLastError();
        if (dwError != 0) {
            if (dwError == WSAHOST_NOT_FOUND) {
                printf("Host not found\n");
                return nullptr;
            } else if (dwError == WSANO_DATA) {
                printf("No data record found\n");
                return nullptr;
            } else {
                printf("Function failed with error: %ld\n", dwError);
                return nullptr;
            }
        }
    } else {
        printf("Function returned:\n");
        printf("\tOfficial name: %s\n", remoteHost->h_name);
        for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias++) {
            printf("\tAlternate name #%d: %s\n", ++i, *pAlias);
        }
        printf("\tAddress type: ");
        switch (remoteHost->h_addrtype) {
        case AF_INET:
            printf("AF_INET\n");
            break;
        case AF_NETBIOS:
            printf("AF_NETBIOS\n");
            break;
        default:
            printf(" %d\n", remoteHost->h_addrtype);
            break;
        }
        printf("\tAddress length: %d\n", remoteHost->h_length);

        i = 0;
        if (remoteHost->h_addrtype == AF_INET)
        {
            while (remoteHost->h_addr_list[i] != 0) {
                addr.s_addr = *(u_long *) remoteHost->h_addr_list[i++];
                printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
				strcpy(self_ip_array0,inet_ntoa(addr));
				strcpy(self_ip_array[i-1],self_ip_array0);

            }
        }
        else if (remoteHost->h_addrtype == AF_NETBIOS)
        {   
            printf("NETBIOS address was returned\n");
        }   
    }
    return self_ip_array;
}

int  ping_one(char *argv)  
{
    
    HANDLE hIcmpFile;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[32] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;
    
    ipaddr = inet_addr(argv);

    hIcmpFile = IcmpCreateFile();

    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (VOID*) malloc(ReplySize);
    
    dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), 
        NULL, ReplyBuffer, ReplySize, 200);
    if (dwRetVal != 0) 
		return -1;
    else
		return 1;
    return 0;
}    

char *delete_last_three(char *ip_addr)
{
	for (int i=strlen(ip_addr);i>5;i--)
		if (ip_addr[i]=='.') 
		{
			ip_addr[i+1]=0;
			return ip_addr;
		}
	return nullptr;
}

int get_hostnames(char **target_ips,int array_size)
{
	for (int i=1;i<array_size;i++)
	{
		addr.s_addr = inet_addr(target_ips[i]);
		if(!gethostbyaddr((char *)&addr,4,AF_INET)){cout<<"left from network"<<endl; break;}
		else remoteHost=gethostbyaddr((char *)&addr,4,AF_INET);
		if (remoteHost[0].h_name)
			cout << target_ips[i]<<"    "<<remoteHost[0].h_name<<endl;
		else
			cout << target_ips[i]<<"    kakoi-to noname"<<endl;
	}
	return 0;
}

DWORD threadproc1(LPVOID args)
{
	int i=(int)args+1;
	int n = i+50;
	for (;i<n;i++)
	{
		strcpy(first_network_ip_array[i],self_ip_wo3);
		first_network_ip_array[i]=strcat(first_network_ip_array[i],itoa(i,last3,10));
		if(ping_one(first_network_ip_array[i])==-1) 
		{
			cout <<"exist "<<first_network_ip_array[i]<<endl;
			fprintf(network,"%s\n",first_network_ip_array[i]);
		}
		else
		{
			cout <<"not exist "<<first_network_ip_array[i]<<endl;
		}
	}
	return 0;
}

DWORD threadproc2(LPVOID args)
{
	int i=(int)args+1;
	int n=i+50;
	for (;i<n;i++)
		{
			strcpy(second_network_ip_array[i],self_ip_wo3_1);
			second_network_ip_array[i]=strcat(second_network_ip_array[i],itoa(i,last3,10));
			if(ping_one(second_network_ip_array[i])==-1) 
			{
				cout <<"exist "<<second_network_ip_array[i]<<endl;
				fprintf(network,"%s\n",second_network_ip_array[i]);
			}
			else
			{
				cout <<"not exist "<<second_network_ip_array[i]<<endl;
			}
		}
	return 0;
}