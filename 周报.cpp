#include <fstream>   
#include <string>  
#include <iostream>  
#include <streambuf>   
#include <vector>
#include <sstream>
#include <ctime>
#include <map>
#include <set>
using namespace std;
#pragma warning(disable:4996)
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
	//读取中控清单
	vector<vector<string>> order;
	vector<vector<string>> pipei;
	double sum1 = 0, sum2 = 0;		//统计甩单量、受理量
	map<string, int> itemize_num_success{ pair<string, int>("外呼", 0) };			//按触点统计
	map<string, int> itemize_num{ pair<string, int>("外呼", 0) };
	map<string, int> name_num_success{ pair<string, int>("提速", 0) };				//按名称统计
	map<string, int> name_num_DFP{ pair<string, int>("提速", 0) };
	map<string, int> name_num_LDZ{ pair<string, int>("提速", 0) };
	map<string, int> name_num_WTD{ pair<string, int>("提速", 0) };
	map<string, int> name_num_YQX{ pair<string, int>("提速", 0) };
	map<string, int> name_num_ZCZ{ pair<string, int>("提速", 0) };
	map<string, int> name_num_QT{ pair<string, int>("提速", 0) };
	map<string, int> name_num{ pair<string, int>("提速", 0) };
	set<string> other_id;
	//打开要输出的文件     
	string z_title, token, m_countent;
	int num1 = 0;
	ifstream iFile2("chu.csv", ios::in);
	iFile2 >> z_title;
	num1 = 0;
	stringstream sstr = stringstream(z_title);
	while (getline(sstr, token, ','))
	{
		pipei.push_back(vector<string>{token});
		++num1;
	}
	const int row2 = num1;
	while (getline(iFile2, m_countent))
	{
		num1 = 0;
		if (m_countent == "")
			continue;
		stringstream sstr(m_countent);
		while (getline(sstr, token, ','))
		{
			pipei[num1 % row2].push_back(token);
			++num1;
		}
		while (num1 != row2)
		{
			if (num1 > row2)
				return -1;
			pipei[num1 % row2].push_back("");
			++num1;
		}
	}
	iFile2.close();
	cout << "chu成功" << endl;
	//打开要输出的文件 
	ifstream iFile(name, ios::in);
	num1 = 0;
	int pro_name_num = 0, order_sta_num = 0, script_id = 0;
	iFile >> z_title;
	sstr = stringstream(z_title);
	while (getline(sstr, token, ','))
	{
		order.push_back(vector<string>{token});
		if (token == "商品名称")
			pro_name_num = num1;
		else if (token == "订单状态")
			order_sta_num = num1;
		else if (token == "脚本Id")
			script_id = num1;
		++num1;
	}
	//cout << pro_name_num << ends << order_sta_num;
	//cin.get();
	const int row1 = num1;
	vector<vector<string>> order_success(order);
	vector<vector<string>> order_DFP(order);
	vector<vector<string>> order_LDZ(order);
	vector<vector<string>> order_WTD(order);
	vector<vector<string>> order_YQX(order);
	vector<vector<string>> order_ZCZ(order);
	vector<vector<string>> order_QT(order);
	set<string> order_YGQ;
	while (getline(iFile, m_countent))
	{
		num1 = 0;
		stringstream sstr(m_countent);
		if (m_countent == "")
			continue;
		while (getline(sstr, token, ','))
		{
			order[num1 % row1].push_back(token);
			++num1;
		}
		while (num1 != row1)
		{
			if (num1 > row1)
				return -1;
			order[num1 % row1].push_back("");
			++num1;
		}
	}
	iFile.close();
	cout << "总清单成功" << endl;
	//受理成功
	if (order.size() < 2) return -1;
	/*for (int i = 0; i < 3; ++i) cout << order[1][i];
	return -1;*/
	for (int i = 0; i < order[0].size(); ++i)
	{
		bool is_join = true;
		if (order[pro_name_num][i] == "")
			continue;
		//商品名称是否类chu表里面
		if (order[order_sta_num][i] == "录单完成")
		{
			for (int j = 0; j < row1; ++j)
			{
				order_success[j].push_back(order[j][i]);
			}
		}
		else if (order[order_sta_num][i] == "待分派" || order[order_sta_num][i] == "待录单预处理" || order[order_sta_num][i] == "录单发送中" || order[order_sta_num][i] == "等待录单")
		{
			for (int j = 0; j < row1; ++j)
			{
				order_DFP[j].push_back(order[j][i]);
			}
		}
		else if (order[order_sta_num][i] == "录单中")
		{
			for (int j = 0; j < row1; ++j)
			{
				order_LDZ[j].push_back(order[j][i]);
			}
		}
		else if (order[order_sta_num][i] == "问题单")
		{
			for (int j = 0; j < row1; ++j)
			{
				order_WTD[j].push_back(order[j][i]);
			}
		}
		else if (order[order_sta_num][i] == "已取消")
		{
			for (int j = 0; j < row1; ++j)
			{
				order_YQX[j].push_back(order[j][i]);
			}
		}
		else if (order[order_sta_num][i] == "暂存中")
		{
			for (int j = 0; j < row1; ++j)
			{
				order_ZCZ[j].push_back(order[j][i]);
			}
		}
		else if (order[order_sta_num][i] == "其他")
		{
			for (int j = 0; j < row1; ++j)
			{
				order_QT[j].push_back(order[j][i]);
			}
			order_YGQ.insert(order[0][i]);
		}
		for (auto id : pipei[0])
		{
			if (id == order[pro_name_num][i])
			{
				is_join = false;
				break;
			}
		}
		if (is_join)
		{
			other_id.insert(order[pro_name_num][i]);
			other_id.insert(order[script_id][i]);
		}
	}
	cout << "其他成功" << endl;
	time_t now = time(0);
	tm* ltm = localtime(&now);
	ofstream oFile;
	string file_name = "周报" + to_string(1 + ltm->tm_mon) + to_string(ltm->tm_mday) + ".csv";
	oFile.open(file_name, ios::out);    // 这样就很容易的输出一个需要的excel 文件 
	// pipei：商品名称、分类、触点
	//order：中控id、订单状态、商品名称
	//其他id
	oFile << "甩单量：" << endl << "商品名称" << ",分类" << ",触点" << ",单量" << endl;
	for (int i = 1; i < pipei[0].size(); ++i)
	{

		bool is_join = true;
		int count1 = 0;
		count1 = count(order[pro_name_num].begin(), order[pro_name_num].end(), pipei[0][i]);
		for (auto c = itemize_num.begin(); c != itemize_num.end(); ++c)
		{
			if (c->first == pipei[2][i])
			{
				c->second += count1;
				is_join = false;
				break;
			}
		}
		if (is_join)
			itemize_num.insert(pair<string, int>(pipei[2][i], count1));
		is_join = true;
		for (auto c = name_num.begin(); c != name_num.end(); ++c)
		{
			if (c->first == pipei[1][i])
			{
				c->second += count1;
				is_join = false;
				break;
			}
		}
		if (is_join)
			name_num.insert(pair<string, int>(pipei[1][i], count1));
	}
	oFile << endl << "触点,单量" << endl;
	for (auto c = itemize_num.begin(); c != itemize_num.end(); ++c)
		oFile << c->first << "," << c->second << endl;
	oFile << endl << "分类,单量" << endl;
	for (auto c = name_num.begin(); c != name_num.end(); ++c)
		oFile << c->first << "," << c->second << endl;
	oFile << endl;
	oFile << "受理量：" << endl << "商品名称" << ",分类" << ",触点" << ",单量" << endl;
	for (int i = 1; i < pipei[0].size(); ++i)
	{

		bool is_join = true;
		int count1 = 0;
		count1 = count(order_success[pro_name_num].begin(), order_success[pro_name_num].end(), pipei[0][i]);
		if (count1 != 0)
			oFile << pipei[0][i] << "," << pipei[1][i] << "," << pipei[2][i] << "," << count1 << endl;
		for (auto c = itemize_num_success.begin(); c != itemize_num_success.end(); ++c)
		{
			if (c->first == pipei[2][i])
			{
				c->second += count1;
				is_join = false;
				break;
			}
		}
		if (is_join)
			itemize_num_success.insert(pair<string, int>(pipei[2][i], count1));
		is_join = true;
		for (auto c = name_num_success.begin(); c != name_num_success.end(); ++c)
		{
			if (c->first == pipei[1][i])
			{
				c->second += count1;
				is_join = false;
				break;
			}		
		}
		if (is_join)
			name_num_success.insert(pair<string, int>(pipei[1][i], count1));

		is_join = true;
		count1 = 0;
		count1 = count(order_LDZ[pro_name_num].begin(), order_LDZ[pro_name_num].end(), pipei[0][i]);
		for (auto c = name_num_LDZ.begin(); c != name_num_LDZ.end(); ++c)
		{
			if (c->first == pipei[1][i])
			{
				c->second += count1;
				is_join = false;
				break;
			}				
		}
		if (is_join)
			name_num_LDZ.insert(pair<string, int>(pipei[1][i], count1));

		is_join = true;
		count1 = 0;
		count1 = count(order_DFP[pro_name_num].begin(), order_DFP[pro_name_num].end(), pipei[0][i]);
		for (auto c = name_num_DFP.begin(); c != name_num_DFP.end(); ++c)
		{
			if (c->first == pipei[1][i])
			{
				c->second += count1;
				is_join = false;
				break;
			}
		}
		if (is_join)
			name_num_DFP.insert(pair<string, int>(pipei[1][i], count1));

		is_join = true;
		count1 = 0;
		count1 = count(order_YQX[pro_name_num].begin(), order_YQX[pro_name_num].end(), pipei[0][i]);
		for (auto c = name_num_YQX.begin(); c != name_num_YQX.end(); ++c)
		{
			if (c->first == pipei[1][i])
			{
				c->second += count1;
				is_join = false;
				break;
			}
		}
		if (is_join)
			name_num_YQX.insert(pair<string, int>(pipei[1][i], count1));

		is_join = true;
		count1 = 0;
		count1 = count(order_WTD[pro_name_num].begin(), order_WTD[pro_name_num].end(), pipei[0][i]);
		for (auto c = name_num_WTD.begin(); c != name_num_WTD.end(); ++c)
		{
			if (c->first == pipei[1][i])
			{
				c->second += count1;
				is_join = false;
				break;
			}
		}
		if (is_join)
			name_num_WTD.insert(pair<string, int>(pipei[1][i], count1));

		is_join = true;
		count1 = 0;
		count1 = count(order_ZCZ[pro_name_num].begin(), order_ZCZ[pro_name_num].end(), pipei[0][i]);
		for (auto c = name_num_ZCZ.begin(); c != name_num_ZCZ.end(); ++c)
		{
			if (c->first == pipei[1][i])
			{
				c->second += count1;
				is_join = false;
				break;
			}
		}
		if (is_join)
			name_num_ZCZ.insert(pair<string, int>(pipei[1][i], count1));

		is_join = true;
		count1 = 0;
		count1 = count(order_QT[pro_name_num].begin(), order_QT[pro_name_num].end(), pipei[0][i]);
		for (auto c = name_num_QT.begin(); c != name_num_QT.end(); ++c)
		{
			if (c->first == pipei[1][i])
			{
				c->second += count1;
				is_join = false;
				break;
			}
		}
		if (is_join)
			name_num_QT.insert(pair<string, int>(pipei[1][i], count1));

		is_join = true;
		count1 = 0;
	}
	oFile << endl << "触点,单量" << endl;
	for (auto c = itemize_num_success.begin(); c != itemize_num_success.end(); ++c)
		oFile << c->first << "," << c->second << endl;
	oFile << endl << "录单完成：单量" << endl;
	for (auto ite = name_num_success.begin(); ite != name_num_success.end(); ++ite)
		oFile << ite->first << "," << ite->second << endl;
	oFile << endl << "已取消：,单量" << endl;
	for (auto ite = name_num_YQX.begin(); ite != name_num_YQX.end(); ++ite)
		oFile << ite->first << "," << ite->second << endl;
	oFile << endl << "暂存中：,单量" << endl;
	for (auto ite = name_num_ZCZ.begin(); ite != name_num_ZCZ.end(); ++ite)
		oFile << ite->first << "," << ite->second << endl;
	oFile << endl << "待分派：,单量" << endl;
	for (auto ite = name_num_DFP.begin(); ite != name_num_DFP.end(); ++ite)
		oFile << ite->first << "," << ite->second << endl;
	oFile << endl << "录单中：,单量" << endl;
	for (auto ite = name_num_LDZ.begin(); ite != name_num_LDZ.end(); ++ite)
		oFile << ite->first << "," << ite->second << endl;
	oFile << endl << "其他：,单量" << endl;
	for (auto ite = name_num_QT.begin(); ite != name_num_QT.end(); ++ite)
		oFile << ite->first << "," << ite->second << endl;
	oFile << endl << "问题单：,单量" << endl;
	for (auto ite = name_num_WTD.begin(); ite != name_num_WTD.end(); ++ite)
		oFile << ite->first << "," << ite->second << endl;
	oFile << endl << "已挂起？：" << endl;
	for (auto ite = order_YGQ.begin(); ite != order_YGQ.end(); ++ite)
		oFile << *ite << endl;
	oFile << endl << "其他商品名称：" << endl;
	for (auto ite = other_id.begin(); ite != other_id.end(); ++ite)
		oFile << *ite << endl;
	oFile.close();
	cout << "生成成功" << endl;
	return 0;
}