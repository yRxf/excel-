#include <Windows.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include<conio.h>
using std::cout;
using std::string;
using std::endl;
//void find_cout(string&, string&, const string);
int main(int argc, char* argv[])
{
	std::ios::sync_with_stdio(false), std::cin.tie(nullptr);
	string name;
	if (argc == 1)
	{
		cout << "�������ļ�����";
		std::cin >> name;
		name += ".csv";
	}
	else
		name = argv[1];
	HWND hw = FindWindow("ConsoleWindowClass", NULL);							//��ȡ���ھ��
	std::ifstream iFile(name, std::ios::in);
	string z_title, token, m_countent, fenxi, fenlei;
	string m_url("http://132.108.207.12:8071/imconsole/uploads/");
	int num = 0;
	int image_int = 0, order_int = 0, script_int = 0, err_int = 0;
	bool is_all = false;
	std::vector<int> must_num;
	std::map<string, std::vector<string>> pip_res
	{
		//{"hd\\fdx_pl",{"֪ʶ��ҳ������쳣","����/ϵͳ/��������"}},
		{"dyd\\tmbxl99ydyd_7zb\\11.png",{"�ͻ��������µ�����Ϣ��һ��","��������"}},
		{"��è���˶�",{"�ײͻ���","��������"}},
		{"�����շѽ�һ��",{"�����շѽ�һ��","��������"}},
		{"���ݲ�ѯ����δ�ҵ��ͻ���Ϣ",{"���ݲ�ѯ����δ�ҵ��ͻ���Ϣ","��������"}},
		{"�ϻ��˵�¼ʧ��",{"�����ӳ�","����/ϵͳ/��������"}},
		{"image\\\\bss\\\\14.png",{"�����ӳ�","����/ϵͳ/��������"}},
		{"dyd\\tmbxl99ydyd_7zb\\04.png",{"�����ӳ�","����/ϵͳ/��������"}}
	};
	std::set<string> my_script_id;
	std::ifstream iFile3("����ƥ��.csv", std::ios::in);
	while (getline(iFile3, m_countent))
	{
		int c = 0;
		string a[3];
		if (m_countent == "")
			continue;
		std::stringstream sstr(m_countent);
		while (getline(sstr, token, ','))
		{
			a[c % 3] = token;
			++c;
		}
		std::vector<string> join_in{ a[1],a[2] };
		pip_res[a[0]] = join_in;
	}
	iFile3.close();
	std::ifstream iFile4("ƥ��ű�ID.csv", std::ios::in);
	while (getline(iFile4, m_countent))
	{
		int c = 0;
		if (m_countent == "")
			continue;
		my_script_id.insert(m_countent);
	}
	iFile4.close();
	iFile >> z_title;
	std::stringstream sstr(z_title);
	string image_url, m_order_id, uuru,err,scr_id;
	static string cz_next = "", err_old, scr_id_old;
	while (getline(sstr, token, ','))		//��ȡ��Ҫ�������Ϣ
	{
		if(token=="SCRIPT_ID"|| token == "ORDER_ID" || token == "ERR_DESC"|| token == "ERR_URL" || token == "�쳣����" || token == "����id" || token == "�ű�id" || token == "�쳣����")
			must_num.push_back(num);
		if (token == "ERR_URL" || token == "�쳣����")
			image_int = num;
		else if (token == "ORDER_ID" || token == "����id")
			order_int = num;
		else if (token == "SCRIPT_ID" || token == "�ű�id")
			script_int = num;
		else if (token == "ERR_DESC" || token == "�쳣����")
			err_int = num;
		else if (num == 0 && token == "��")
			is_all = true;
		++num;
	}
	std::ofstream oFile;
	oFile.open("�������.csv", std::ios::out);
	while (getline(iFile, m_countent))
	{
		static bool is_huanhang = false;
		static bool is_new = true;
		static bool is_check = false;
		bool is_break = false;
		bool is_show = false;
		static int num1 = 0;
		std::stringstream sstr(m_countent);
		if (m_countent == "" && is_check)
		{
			++num1;
			continue;
		}

		while (std::getline(sstr, token, ','))
		{
			is_show = false;
			static string str("");
			token.erase(0, token.find_first_not_of("\t\r\n"));
			token.erase(token.find_last_not_of("\t\r\n") + 1);
			if (token == "" && !is_huanhang)
			{
				is_check = true;
				continue;
			}

			if (token.find("\"") == 0 && token.find_last_of("\"") == 0)		//��һ�����������Ҳ���������ţ��ж�Ϊ��������,Ҳ�п���Ϊ�������Ž���
			{
				token.erase(0, 1);
				if (!is_huanhang)
				{

					is_huanhang = true;
					is_new = false;
				}
				else
				{
					is_huanhang = false;
				}
				is_check = false;
			}
			if (token == "")
			{
				continue;
			}
			if (is_huanhang)
			{
				if (token.back() == '"')
				{
					is_huanhang = false;
					token.erase(token.length() - 1, 1);
					is_check = true;
				}
				if (is_new)
				{
					str += token;
				}
				else
				{
					is_new = true;
					is_check = false;
				}
			}
			else
			{
				is_huanhang = false;
				is_check = true;
			}
			if (str == "")
				str = token;
			if (is_check)
			{
				if (num1 == image_int)
					image_url = str;
				else if (num1 == order_int)
					m_order_id = str;
				else if (num1 == err_int)
					err = str;
				else if (num1 == script_int)
					scr_id = str;
				++num1;
				if (num == num1)
				{
					num1 = 0;
					is_show = true;
				}
				str = "";
				is_check = false;
			}
		}
		/*while (getline(sstr, token, ','))
		{
			token.erase(0, token.find_first_not_of(" \t\r\n\""));
			token.erase(token.find_last_not_of(" \t\r\n\"") + 1);
			if (token == "")
				continue;
			std::vector<int>::iterator it = find(must_num.begin(), must_num.end(), num1);
			if (it != must_num.end())
				cout << token << "   ";
			if (num1 == image_int)
				image_url = token;
			if (num1 == order_int)
				m_order_id = token;
			++num1;
		}*/
		if (!is_all && my_script_id.find(scr_id) == my_script_id.end())
			continue;
		if (num1 != 0|| image_url=="")
			continue;
		if (scr_id == "hd\\fdx_pl")
		{
			oFile << m_order_id << "," << image_url << "," << "֪ʶ��ҳ������쳣" << "," << "����/ϵͳ/��������" << endl;
			continue;
		}
		for (auto it = pip_res.begin(); it != pip_res.end(); ++it)
		{
			if (err.find(it->first) != string::npos)
			{
				oFile << m_order_id << "," << image_url << "," << (it->second)[0] << "," << (it->second)[1] << endl;
				is_break = true;
				break;
			}
		}
		if (is_break)
			continue;
		
		cout << m_order_id << "  " << scr_id << "  " << err << "  " << image_url << endl;
		ShellExecute(NULL, "open", (m_url + image_url + ".jpg").c_str(), NULL, NULL, SW_SHOWNOACTIVATE);
		//ShellExecute(NULL, "open", "www.baidu.com", NULL, NULL, SW_SHOWNOACTIVATE);
		Sleep(600);
		SetWindowPos(hw, HWND_TOPMOST, 850, 200, 0, 0, SWP_NOSIZE || SW_SHOW);		//���ô���
		SetForegroundWindow(hw);	
		SetFocus(hw);
		Sleep(400);
		while (hw != GetForegroundWindow())											//�жϵ�ǰ����
		{
			Sleep(200);
			SetForegroundWindow(hw);
			SetFocus(hw);
		}//��ȡ����
		cout << endl;
		if (cz_next != "��")
			std::cin >> uuru;
		if (uuru != "q")
		{
			if (uuru == "��")
				cz_next = uuru;
			if (cz_next == "��")
			{
				if (scr_id_old == scr_id && err_old == err)
				{
					Sleep(500);
					if (_kbhit())
					{
						std::cin >> uuru;
						cz_next = uuru;
					}
					else
						uuru = "c";
				}
				else
				{
					std::cin >> uuru;
					cz_next = uuru;
				}
					
			}
			if (uuru != "c" && uuru != "ͬ" && uuru != "��")
			{
				fenxi = uuru;
				std::cin >> fenlei;
			}
			if (fenlei == "����" || fenlei == "����")
				fenlei += "����";
			else if (fenlei == "����")
				fenlei = "����/ϵͳ/��������";
			else if (fenlei == "����")
				fenlei = "����/�������淶";
			else if (fenlei == "�ű�")
				fenlei = "�ű�/��������";
			oFile << m_order_id << "," << image_url <<  "," << fenxi << "," << fenlei << endl;
			scr_id_old = scr_id;
			err_old = err;
			m_order_id = "";
		}
		else break;
	}
	oFile.close();
	iFile.close();
	char ch;
	cout << "���ɳɹ����Ƿ���ļ���";
	std::cin >> ch;
	if (ch == 'y')
		system("start �������.csv");
	return 0;
}