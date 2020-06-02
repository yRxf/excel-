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
		cout << "请输入文件名！";
		std::cin >> name;
		name += ".csv";
	}
	else
		name = argv[1];
	HWND hw = FindWindow("ConsoleWindowClass", NULL);							//获取窗口句柄
	std::ifstream iFile(name, std::ios::in);
	string z_title, token, m_countent, fenxi, fenlei;
	string m_url("http://132.108.207.12:8071/imconsole/uploads/");
	int num = 0;
	int image_int = 0, order_int = 0, script_int = 0, err_int = 0;
	bool is_all = false;
	std::vector<int> must_num;
	std::map<string, std::vector<string>> pip_res
	{
		//{"hd\\fdx_pl",{"知识库页面加载异常","网络/系统/升级问题"}},
		{"dyd\\tmbxl99ydyd_7zb\\11.png",{"客户订单跟下单人信息不一致","数据问题"}},
		{"天猫不退订",{"套餐互斥","数据问题"}},
		{"订单收费金额不一致",{"订单收费金额不一致","数据问题"}},
		{"根据查询条件未找到客户信息",{"根据查询条件未找到客户信息","数据问题"}},
		{"合伙人登录失败",{"网络延迟","网络/系统/升级问题"}},
		{"image\\\\bss\\\\14.png",{"网络延迟","网络/系统/升级问题"}},
		{"dyd\\tmbxl99ydyd_7zb\\04.png",{"网络延迟","网络/系统/升级问题"}}
	};
	std::set<string> my_script_id;
	std::ifstream iFile3("卡单匹配.csv", std::ios::in);
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
	std::ifstream iFile4("匹配脚本ID.csv", std::ios::in);
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
	while (getline(sstr, token, ','))		//获取需要输出的信息
	{
		if(token=="SCRIPT_ID"|| token == "ORDER_ID" || token == "ERR_DESC"|| token == "ERR_URL" || token == "异常链接" || token == "订单id" || token == "脚本id" || token == "异常详情")
			must_num.push_back(num);
		if (token == "ERR_URL" || token == "异常链接")
			image_int = num;
		else if (token == "ORDER_ID" || token == "订单id")
			order_int = num;
		else if (token == "SCRIPT_ID" || token == "脚本id")
			script_int = num;
		else if (token == "ERR_DESC" || token == "异常详情")
			err_int = num;
		else if (num == 0 && token == "否")
			is_all = true;
		++num;
	}
	std::ofstream oFile;
	oFile.open("卡单输出.csv", std::ios::out);
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

			if (token.find("\"") == 0 && token.find_last_of("\"") == 0)		//第一个是引号且找不到别的引号，判断为换行引号,也有可能为换行引号结束
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
			oFile << m_order_id << "," << image_url << "," << "知识库页面加载异常" << "," << "网络/系统/升级问题" << endl;
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
		SetWindowPos(hw, HWND_TOPMOST, 850, 200, 0, 0, SWP_NOSIZE || SW_SHOW);		//设置窗口
		SetForegroundWindow(hw);	
		SetFocus(hw);
		Sleep(400);
		while (hw != GetForegroundWindow())											//判断当前焦点
		{
			Sleep(200);
			SetForegroundWindow(hw);
			SetFocus(hw);
		}//获取焦点
		cout << endl;
		if (cz_next != "下")
			std::cin >> uuru;
		if (uuru != "q")
		{
			if (uuru == "下")
				cz_next = uuru;
			if (cz_next == "下")
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
			if (uuru != "c" && uuru != "同" && uuru != "是")
			{
				fenxi = uuru;
				std::cin >> fenlei;
			}
			if (fenlei == "数据" || fenlei == "其他")
				fenlei += "问题";
			else if (fenlei == "网络")
				fenlei = "网络/系统/升级问题";
			else if (fenlei == "管理")
				fenlei = "管理/操作不规范";
			else if (fenlei == "脚本")
				fenlei = "脚本/程序问题";
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
	cout << "生成成功，是否打开文件？";
	std::cin >> ch;
	if (ch == 'y')
		system("start 卡单输出.csv");
	return 0;
}