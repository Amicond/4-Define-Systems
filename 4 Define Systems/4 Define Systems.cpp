// Define Systems.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

const string sys="sys";
const string input="input";
const string res="results";
const string loop="l";
const string n_loop="nl";
const string n_single="ns";
const string math_res="results";
const string delim="\\";
const string math_delim="\\\\";
string root_path;

vector<string> my_points;
const int J1=1;
vector<int> good_nl[5];//с запасом: 0+5=5, 4+5=9 - до 9 пор€дка
void define_term(ofstream &math_out,string &point,int type,int order,int subOrder,int &Total,ofstream &out,int term_amount[][3],int step,int plaquet_type)
{
	ostringstream fname,tmpStr;
	ifstream cur_f;
	string MatrixStr,temp;
	int size;

	for(int j=1;j<=term_amount[subOrder][type];j++)
	{
		if((subOrder==order)&&(type==1)&&(order>=5)&&(find(good_nl[order-5].begin(),good_nl[order-5].end(),j)==good_nl[order-5].end()))
			continue;
		
		
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//temp test
		/*if(!((order==7)&&(subOrder==6)&&(j==1)))
			continue;
		if(type!=1)
			continue;*/
		//end temp test
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		Total++;
		if(Total%step==0)
		{
			out.close();
			fname.str("");
			fname<<res<<delim<<point<<delim<<"!_"<<order<<"_results_J2="<<point<<"("<<Total/step<<").txt";
			//добавл€ем в авто файл
			math_out<<"nb1= NotebookOpen[StringJoin[{NotebookDirectory[], \""<<"!_"<<order<<"_results_J2="<<point<<"("<<Total/step<<").txt"<<"\"}]];\n";
			math_out<<"NotebookEvaluate[nb1];\n";
			math_out<<"NotebookClose[nb1];\n";
			//конец добавки
			out.open(fname.str(),ios::out);
		}
		//cout<<i<<" "<<j<<" Start\n";
		fname.str("");
		if(type==0)
		{
			fname<<root_path<<"results_"<<loop<<delim<<order<<"ord"<<delim<<order<<"_"<<subOrder<<"_"<<j<<"_res_"<<loop<<"_clean.txt";
			temp=loop;
		}
		if(type==1)
		{
			fname<<root_path<<"results_"<<n_loop<<delim<<order<<"ord"<<delim<<order<<"_"<<subOrder<<"_"<<j<<"_res_"<<n_loop<<"_clean.txt";
			temp=n_loop;
		}
		if(type==2)
		{
			fname<<root_path<<"results_"<<n_single<<delim<<order<<"ord"<<delim<<order<<"_"<<subOrder<<"_"<<j<<"_res_"<<n_single<<"_clean.txt";
			temp=n_single;
		}
		cur_f.open(fname.str(),ios::in);
		cur_f>>size;
		tmpStr.str("");
		tmpStr<<"res"<<temp<<"$"<<order<<"$"<<subOrder<<"$"<<j;
		out<<tmpStr.str()<<"=";
		while(!cur_f.eof())
		{
			getline(cur_f,MatrixStr);
			if(MatrixStr.length()>0)
				out<<MatrixStr;
		}
		out<<";\n";
		out<<"Eff"<<tmpStr.str()<<"=Expand[Chop[Solve[Sys";
		switch(size)
		{
			case 4:	  temp="2"; break;
			case 8:   temp="3"; break;
			case 16:  temp="4"; break;
			case 32:  temp="5"; break;
			case 64:  temp="6"; break;
			case 128: temp="7"; break;
			case 256: temp="8"; break;
		}
		out<<temp<<"=="<<tmpStr.str()<<"/.{J1->1,J2->"<<point<<"},Var"<<temp<<"]]];\n";
		out<<"tt = OpenWrite[\"D:"<<math_delim<<math_res<<math_delim<<point<<math_delim<<tmpStr.str()<<"_"<<point<<".txt\"];\n";
		out<<"If[Length[Eff"<<tmpStr.str()<<"]==0,Write[tt,\""<<order<<" "<<subOrder<<" "<<j<<"\"];];\n";
		out<<"For[i = 1, i <= Length[Eff"<<tmpStr.str()<<"[[1]]], i++,\n";
		out<<"	 WriteString[tt,";
		out<<"ScientificForm[Eff"<<tmpStr.str()<<"[[1, i]], ";
			out<<"ExponentFunction -> (If[-100 < # < 10, Null, #] &)], \"\\n\"]";
		out<<"];\n";
		out<<"Close[tt];\n\n\n";
		cur_f.close();
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*cout<<"remove temp\n";
	int uu;
	cin>>uu;
	if(uu!=1)
		return 0;*/
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////


	ifstream conf("config.txt",ios::in);
	string temp;

	ofstream out;
	ofstream math_out;
	
	ifstream cur_f;
	ostringstream fname,tmpStr;
	string MatrixStr;
	
	//string InputDir="input_data_ns\\";
	int order,subOrder,cur_amount,cur_amount_nl,size,mode;
	int amount,term_amount[10][3];//с запасом
	
	getline(conf,temp);
	getline(conf,root_path);
	getline(conf,temp);
	conf>>amount;
	getline(conf,temp);
	getline(conf,temp);
	conf>>order;
	getline(conf,temp);
	getline(conf,temp);
	conf>>mode;
	getline(conf,temp);
	getline(conf,temp);
	
	for(int i=2;i<=order;i++)
	{
		conf>>subOrder>>term_amount[i][0]>>term_amount[i][1]>>term_amount[i][2];
	}
	for(int i=5;i<=order;i++)
	{
		fname.str("");
		fname<<i<<"_good_nl.txt";
		int cur;
		ifstream in(fname.str(),ios::in);
		while(!in.eof())
		{
			in>>cur;
			good_nl[i-5].push_back(cur);
		}
		in.close();
	}
	conf.close();
	ifstream my_points_f("points.txt",ios::in);
	for(int jj=1;jj<=amount;jj++)
	{
		getline(my_points_f,temp);
		my_points.push_back(temp);
	}
	my_points_f.close();

	int Total=0;
	ofstream out_sys;

	//ƒќƒ≈Ћј“№!!!!!!!!!!!!!!!!!!!
	//пока просо копирует файлы, в идеале должно записывать разные файлы в один

	//определ€ем систему дл€ текущего пор€дка
	//for(int i=2;i<order;i++)
	//{
	//	fname.str("");
	//	fname<<res<<delim<<"!_"<<i<<"_res_sys.txt";
	//	out_sys.open(fname.str(),ios::out);
	//	fname.str("");
	//	fname.clear();
	//	fname<<sys<<delim<<"sys"<<i<<".txt";
	//	ifstream sysIn(fname.str(),ios::in);
	//	string sss;
	//	while(!sysIn.eof())
	//	{
	//		getline(sysIn,sss);
	//		out_sys<<sss<<"\n";
	//	}
	//	out_sys.close();
	//	//////////////////////////////////////////////////////////////////////////
	//	cout<<"Sys ok\n";
	//	//////////////////////////////////////////////////////////////////////////
	//	sysIn.close();
	//}

	int step=50;//количество слагаемых через которое надо начинать новый файл
	for(int jj=0;jj<amount;jj++)//перебираем все точки
	{
		fname.str("");
		fname<<res<<delim<<my_points[jj]<<delim<<order<<"_"<<my_points[jj]<<"general_math.txt";
		math_out.open(fname.str(),ios::out);
		fname.str("");
		fname<<sys<<delim<<"sys"<<order<<".txt";
		ifstream sysIn(fname.str(),ios::in);
		while(!sysIn.eof())
		{
			getline(sysIn,temp);
			if(temp.length()>0)
				math_out<<temp<<"\n";
		}
		sysIn.close();
		if(mode==1)//full mode
			subOrder=2;
		else
			subOrder=order;
		for(;subOrder<=order;subOrder++)
		{
			Total=0;
			if(subOrder<6)
				step=50;
			else if(subOrder>=6)
				step=20;
			//else
			//	step=5;
			fname.str("");
			fname<<res<<delim<<my_points[jj]<<delim<<"!_"<<subOrder<<"_results_J2="<<my_points[jj]<<"("<<Total/step<<").txt";
			//добавл€ем в авто файл
			
			math_out<<"nb1= NotebookOpen[StringJoin[{NotebookDirectory[], \"!_"<<subOrder<<"_results_J2="<<my_points[jj]<<"("<<Total/step<<").txt"<<"\"}]];\n";
			math_out<<"NotebookEvaluate[nb1];\n";
			math_out<<"NotebookClose[nb1];\n";
			//конец добавки
			out.open(fname.str(),ios::out);

			
			for(int i=2;i<=subOrder;i++)
			{			
				for (int ll = 0; ll < 3; ll++)
				{
					cout << my_points[jj] << " " << i << " l\n";
					define_term(math_out, my_points[jj], 0, subOrder, i, Total, out, term_amount, step,ll);

					//все тоже самое, только дл€ nl матриц
					cout << my_points[jj] << " " << i << " nl\n";
					define_term(math_out, my_points[jj], 1, subOrder, i, Total, out, term_amount, step,ll);


					//все тоже самое, только дл€ n_single матриц
					cout << my_points[jj] << " " << i << " ns\n";
					define_term(math_out, my_points[jj], 2, subOrder, i, Total, out, term_amount, step,ll);
				}

			}
			out.close();
		}
		math_out.close();
	}
	return 0;
}

