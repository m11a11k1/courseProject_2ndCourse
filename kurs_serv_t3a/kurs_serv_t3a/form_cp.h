#pragma once
#include "stdafx.h"
	static struct hostent *selfaddr,*remoteHost;
	static struct in_addr addr={0};
	static int m=0;
	static char *actual_ip=new char [16];
	static char *szMessage=new char [1024];
#include <string.h>

namespace kurs_serv_t3a {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Сводка для form_cp
	/// </summary>
	public ref class form_cp : public System::Windows::Forms::Form
	{
	public:
		form_cp(void)
		{
			InitializeComponent();
			listBox1->SelectionMode = SelectionMode::MultiExtended;

		    // Shutdown the painting of the ListBox as items are added.
			listBox1->BeginUpdate();
				WSADATA wsd;
			if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
			{
				MessageBox::Show("error_wsadata(winsock)");
				exit(1);
				WSACleanup();
			}
			char **first_network_ip_array=new char* [256];
			FILE *network;
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
			for(j=1;j<size_ip_array;j++)
			{
				addr.s_addr = inet_addr(first_network_ip_array[j]);
				remoteHost=gethostbyaddr((char *)&addr,4,AF_INET);
				if (remoteHost)
					if (remoteHost[0].h_name)
					{
						char *vvd=new char [100];
						vvd=strcat(first_network_ip_array[j],"   ");
						vvd=strcat(vvd,remoteHost[0].h_name);
						listBox1->Items->Add( gcnew String(vvd));
					}
			}
	   listBox1->EndUpdate();
			//
			//TODO: добавьте код конструктора
			//
		}
		int run_mode(SOCKET sClient,int m)
		{
		int ret;

		if (m==1)
		{
			fflush(stdin);
			//тут ввод текста сообщения кароч
			ret = send(sClient, szMessage, strlen(szMessage), 0);
			if (ret == SOCKET_ERROR)
			{
				MessageBox::Show( "send failed");
			}
		}

		if (m==2)
		{
			szMessage="*shutdown";
			ret = send(sClient, szMessage, strlen(szMessage), 0);
			if (ret == SOCKET_ERROR)
			{
				MessageBox::Show("send failed");
			}
			else
				MessageBox::Show("send completed");
		}

		if (m==3)
		{
			szMessage="*reboot";
			ret = send(sClient, szMessage, strlen(szMessage), 0);
			if (ret == SOCKET_ERROR)
			{
				MessageBox::Show("send failed");
			}
			else
				MessageBox::Show("send completed");
		}

		if (m==0)
		{
			szMessage="*get";
			ret = send(sClient, szMessage, strlen(szMessage), 0);
			if (ret == SOCKET_ERROR)
			{
				MessageBox::Show("send failed");
			}
			else
				MessageBox::Show("send completed");
		}
	return 0;
}

void NetThread()
{
	SOCKET	sClient;
	char	szBuffer[1024];
	int	ret, i=0;
	struct sockaddr_in server;
	struct hostent	*host = NULL;
	char szServerName[50]; 
	char szMessage[1024]; 
	strcpy(szMessage, "get");
	strcpy(szServerName,actual_ip);
	int y=0;
	while(szServerName[y]>0) y++;
	szServerName[y]='\0';
	// Создание сокета
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET)
	{
		MessageBox::Show("Can't create socket");
		return ;
	}
//	else cout <<"socket created"<<endl;
	// Заполнение структуры с адресом сервера и номером порта
	server.sin_family = AF_INET;
	server.sin_port = htons(2282);
	server.sin_addr.s_addr = inet_addr(szServerName);

	// Соединение с сервером
	if (connect(sClient, (struct sockaddr *)&server, 
		sizeof(server)) == SOCKET_ERROR)
		{
			MessageBox::Show("connect failed");
			return ;
		}
//	else cout <<"connected"<<endl;
	// Отправка и прием данных
		run_mode(sClient,m);
		char	szRecvBuff[1024];
		ret = recv(sClient, szRecvBuff, 1024, 0);
		while (ret == SOCKET_ERROR)
		{
			Sleep(100);
//			cout<< "recv failed"<<endl;
		}
		MessageBox::Show(gcnew String(szRecvBuff));

	closesocket(sClient);
}
	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~form_cp()
		{
			if (components)
			{
				delete components;
			}
			WSACleanup();
		}
	private: System::Windows::Forms::ListBox^  listBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::TextBox^  textBox1;
private: System::Windows::Forms::Button^  button3;
private: System::Windows::Forms::TextBox^  textBox2;
private: System::Windows::Forms::Button^  button4;
private: System::Windows::Forms::HelpProvider^  helpProvider1;

	protected: 

	private:
		/// <summary>
		/// Требуется переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Обязательный метод для поддержки конструктора - не изменяйте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->helpProvider1 = (gcnew System::Windows::Forms::HelpProvider());
			this->SuspendLayout();
			// 
			// listBox1
			// 
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Location = System::Drawing::Point(13, 13);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(194, 238);
			this->listBox1->TabIndex = 0;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(246, 13);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(154, 33);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Shutdown";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &form_cp::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(246, 80);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(154, 37);
			this->button2->TabIndex = 2;
			this->button2->Text = L"Reboot";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &form_cp::button2_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(246, 159);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(180, 20);
			this->textBox1->TabIndex = 3;
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(432, 160);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(88, 19);
			this->button3->TabIndex = 4;
			this->button3->Text = L"Send Message";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &form_cp::button3_Click);
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(246, 231);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(180, 20);
			this->textBox2->TabIndex = 5;
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(432, 229);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(95, 21);
			this->button4->TabIndex = 6;
			this->button4->Text = L"Send Command";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &form_cp::button4_Click);
			// 
			// helpProvider1
			// 
			this->helpProvider1->SetShowHelp( this, true );
			this->helpProvider1->HelpNamespace = "bc7.chm";
			//
			// form_cp
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(532, 273);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->listBox1);
			this->Name = L"form_cp";
			this->Text = L"Control Panel";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 m=2;
			 String^ ptr=form_cp::listBox1->SelectedItem->ToString();
			 int k=ptr->IndexOf(' ');
			 ptr=ptr->Remove(k);

			 array<wchar_t>^ A =ptr-> ToCharArray();
			 for (int i = 0; i <k; i++)
			 actual_ip[i] = A[i];

			 form_cp^ cp2=gcnew form_cp;
			 Thread^ myThread2 = gcnew Thread(gcnew ThreadStart(cp2,&form_cp::NetThread));
			 myThread2->Start();
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 m=3;
			 String^ ptr=form_cp::listBox1->SelectedItem->ToString();
			 int k=ptr->IndexOf(' ');
			 ptr=ptr->Remove(k);

			 array<wchar_t>^ A =ptr-> ToCharArray();
			 for (int i = 0; i <k; i++)
			 actual_ip[i] = A[i];

			 form_cp^ cp2=gcnew form_cp;
			 Thread^ myThread2 = gcnew Thread(gcnew ThreadStart(cp2,&form_cp::NetThread));
			 myThread2->Start();
		 }

private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 m=1;
			 String^ ptr=form_cp::listBox1->SelectedItem->ToString();
			 String^ ptr1=textBox1->ToString();
			 array<wchar_t>^ B =ptr1-> ToCharArray();
			 int k=ptr->IndexOf(' ');
			 ptr=ptr->Remove(k);
			 size_t sizeInBytes = (textBox1->Text->Length+1);
			 array<wchar_t>^ A =ptr-> ToCharArray();
			 for (int i = 0; i <k; i++)
			 actual_ip[i] = A[i];
			 for (int i = 0; i<sizeInBytes; i++)
			 szMessage[i] = B[35+i];
			 szMessage[sizeInBytes] ='\0';
			 form_cp^ cp2=gcnew form_cp;
			 Thread^ myThread2 = gcnew Thread(gcnew ThreadStart(cp2,&form_cp::NetThread));
			 myThread2->Start();
			 textBox1->Clear();
		 }
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 m=1;
			 String^ ptr=form_cp::listBox1->SelectedItem->ToString();
			 int k=ptr->IndexOf(' ');
			 ptr=ptr->Remove(k);
			 size_t sizeInBytes = (textBox2->Text->Length+1);
			 array<wchar_t>^ A =ptr-> ToCharArray();
			 for (int i = 0; i <k; i++)
			 actual_ip[i] = A[i];

			 String^ ptr1=textBox2->ToString();
			 A =ptr1-> ToCharArray();
			 szMessage[0]='*';
			 for (int i = 1; i<sizeInBytes+1; i++)
				 szMessage[i] = A[i+34];
			 szMessage[sizeInBytes+1] ='\0';
			 form_cp^ cp2=gcnew form_cp;
			 Thread^ myThread2 = gcnew Thread(gcnew ThreadStart(cp2,&form_cp::NetThread));
			 myThread2->Start();
			 textBox2->Clear();
		 }
};
}
