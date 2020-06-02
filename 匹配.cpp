#include <fstream>   
#include <string>  
#include <iostream>  
#include <streambuf>
#include <sstream>
#include <map>
#include <vector>
using namespace std;

bool StringtoInt(string,int&);
int main(int argc, char* argv[])
{
	ios::sync_with_stdio(false), cin.tie(nullptr);
	string name;
	if (argc == 1)
	{
		cout << "请输入文件名！";
		cin >> name;
		name += ".csv";
	}
	else
		name = argv[1];
	map<string, int> biao;
	vector<string> fenlei;
	ifstream iFile2("分类.csv", ios::in);
	string fl;
	while (getline(iFile2, fl))
	{
		fenlei.push_back(fl);
	}
	iFile2.close();
	std::ifstream iFile(name, std::ios::in);
	string z_title, token, m_countent, fenxi;
	const int num = 2;
	int image_int = 0, order_int = 0, script_int = 0, err_int = 0;
	string str_now;
	while (getline(iFile, m_countent))
	{
		static bool is_huanhang = false;        //是否在换行里面
		static bool is_new = true;              //是否新单元格
		static bool is_check = false;           //是否校验
		bool is_in = false;
		bool is_break = false;                  //是否退出
		bool is_show = false;                   //是否输出
		static int num1 = 0;                    //循环校验
		std::stringstream sstr(m_countent);
		if (m_countent == "" && is_check)
		{
			++num1;
			continue;
		}

		while (std::getline(sstr, token, ','))
		{
			/*cout << num1 << "  " << token << endl;
			continue;*/
			static int m_num = 0;
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
					is_check = false;
				}
				else
				{
					is_huanhang = false;
					is_check = true;
				}
			}
			if (token == "" && !is_check)
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
				//cout << num1 << "  " << str << endl;            //str
				if (num1 == 1)
				{
					if (!StringtoInt(str, m_num))
					{
						system("pause");
						return -1;
					}
				}
				else
					str_now = str;
				++num1;
				if (num == num1)
				{
					num1 = 0;
					is_show = true;
				}
				str = "";
				is_check = false;
			}
			if (is_show)
			{
				for (auto& f : fenlei)
				{
					int k = 0;
					if (str_now.find(f) != string::npos)
					{
						try {
							k = biao.at(f);
						}
						catch (std::out_of_range&) {
							;
						}
						m_num += k;
						str_now = f;
						is_in = true;
						break;
					}
				}
				if (!is_in)
				{
					try {
						int k = biao.at(str_now);
						m_num += k;
					}
					catch (std::out_of_range&) {
						;
					}
				}
				biao[str_now] = m_num;
				is_show = false;
			}
		}
	}
	iFile.close();
	ofstream oFile("返回.csv",ios::out);
	for (auto b = biao.begin(); b != biao.end(); ++b)
		oFile << b->first << "," << b->second << endl;
	oFile.close();
	char ch;
	cout << "生成成功，是否打开文件？";
	std::cin >> ch;
	if (ch == 'y')
		system("start 返回.csv");
	return 0;
}

bool StringtoInt(string str,int &x)
{
	try {
		x = stoi(str);
		return true;
	}
	catch (std::invalid_argument&) {
		// if no conversion could be performed
		cout << "无法转化，可能存在英文逗号" << endl;         //文字
	}
	catch (std::out_of_range&) {
		// if the converted value would fall out of the range of the result type
		// or if the underlying function (std::strtol or std::strtoull) sets errno
		// to ERANGE.
		cout << "Out of range" << endl;             //超出大小
	}
	catch (...) {
		// everything else
		cout << "Something else" << endl;
	}
	return false;
}