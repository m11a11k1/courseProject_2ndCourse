#pragma once
#include "stdafx.cpp"

namespace kurs_serv_t3a {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	/// <summary>
	/// Сводка для Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}
		void cp()
		{
			Application::Run(gcnew form_cp());
		}
	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(13, 13);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(157, 60);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Refresh network";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click_1);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(191, 13);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(170, 60);
			this->button2->TabIndex = 1;
			this->button2->Text = L"Control Panel";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(380, 96);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"Form1";
			this->Text = L"Main Window";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click_1(System::Object^  sender, System::EventArgs^  e) 
			 {
				 setlocale(0,"Russian");
				 STARTUPINFO si;
				 PROCESS_INFORMATION pi;

				 ZeroMemory( &si, sizeof(si) );
				 si.cb = sizeof(si);
				 ZeroMemory( &pi, sizeof(pi) );

				 TCHAR czCommandLine[30];
				 _tcscpy( czCommandLine, _T("kurs_serv_t2.exe"));
				 if (!CreateProcess(NULL,czCommandLine,NULL,NULL,false,0,NULL,NULL,&si,&pi)) MessageBox::Show("Файл kurs_serv_t2.exe не найден!");
				 else { TopMost=true; MessageBox::Show("Краткая справка: окно консоли выдает информацию о сети и текущий процесс, впоследствии формируется файл network.txt содержащий ip-адреса всех кто есть в сети, а окончание работы - вывод имен компьютеров подключенных к сети");TopMost=false;}
			 }

	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 Form1^ cp1=gcnew Form1;
				 Thread^ myThread = gcnew Thread(gcnew ThreadStart(cp1,&Form1::cp));
				 myThread->Start();
			 }
	};
	
}

