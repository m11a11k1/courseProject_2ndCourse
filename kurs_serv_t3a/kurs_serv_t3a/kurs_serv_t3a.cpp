// kurs_serv_t3a.cpp: ������� ���� �������.

#include "stdafx.h"
#include "Form1.h"
#pragma once
using namespace kurs_serv_t3a;

[STAThreadAttribute]
//[ComVisibleAttribute(true)]


int main(array<System::String ^> ^args)
{
	// ��������� ���������� �������� Windows XP �� �������� �����-���� ��������� ����������
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// �������� �������� ���� � ��� ������
	Application::Run(gcnew Form1());
	return 0;
}
