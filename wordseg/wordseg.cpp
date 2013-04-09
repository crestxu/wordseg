// wordseg.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <sstream> 
#define MAX_LINE 5000
int d=5;
struct chinese_word{
	wstring str;
	int feq;  //频次
	map<wstring,int> left_map;
	map<wstring,int> right_map;
	double left_h;
	double right_h;
	double arg;
	chinese_word()
	{
		feq=0;
		left_h=0.0;
		right_h=0.0;
		arg=0.0;
	}
	bool   operator <  (const   chinese_word&   rhs   )  const   //升序排序时必须写的函数
	{   
		return   feq   <   rhs.feq; 
	}

	bool   operator >  (const   chinese_word&   rhs   )  const   //降序排序时必须写的函数
	{   
		return   feq   >   rhs.feq; 
	}
};
typedef vector<wstring> words_t;
typedef vector<chinese_word> chinese_word_t;

void print_words(words_t &m,int max=-1)
{
	words_t::iterator iter;
	int i=0;
	for(iter=m.begin();iter!=m.end();iter++)
	{
		i++;
		if(max!=-1&&max<i) break;
		wcout<<*iter<<endl;
	}
}

void print_words2(chinese_word_t &m,int max=-1)
{
	chinese_word_t::iterator iter;
	int i=0;
	for(iter=m.begin();iter!=m.end();iter++)
	{

		if(max!=-1&&max<i) break;
		if(iter->str.length()>=1)
		{
			i++;
			wcout<<iter->str<<L" "<<iter->feq<<endl;
		}
	}
}
int _tmain2(int argc, _TCHAR* argv[])
{
	const char *filename="a.txt";
	const char *outname="out.txt";
	remove_copy_if(istreambuf_iterator<char>(ifstream(filename,ios_base::binary)),istreambuf_iterator<char>(),ostreambuf_iterator<char>(ofstream(outname,ios_base::binary)),isspace);
	remove(filename);
	rename(outname,filename);
	return 0;
}
std::wstring StringToWString(const std::string& s)
{
	std::wstring temp(s.length(),L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp;
}
std::string WStringToString(const std::wstring& s)
{
	std::string temp(s.length(), ' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp;
}

void replaceChars(string& modifyMe, const string& findMe, const string& newChars) {
	size_t i = modifyMe.find(findMe, 0);
	if(i != string::npos)
		modifyMe.replace(i, findMe.size(), newChars);
}
string& replaceAll(string& context, const string& from, const string& to) {
	size_t lookHere = 0;
	size_t foundHere;

	while((foundHere = context.find(from, lookHere) != string::npos)) {
		context.replace(foundHere, from.size(), to);
		lookHere = foundHere + from.size();
	}
	return context;
}

string& trim(string &str, string::size_type pos = 0)
{
	static const string delim = "\t"; //删除空格或者tab字符
	pos = str.find_first_of(delim, pos);
	if (pos == string::npos)
		return str;
	return trim(str.erase(pos, 1));
}
string& trim2(string &str, string::size_type pos = 0)
{
	static const string delim = " "; //删除空格或者tab字符
	pos = str.find_first_of(delim, pos);
	if (pos == string::npos)
		return str;
	return trim(str.erase(pos, 1));
}
wstring & trim_w(wstring &aa)
{
	wstring bb=L"";
	for(int i=0;i<aa.length();i++)
	{
		if(aa[i]!=' '&&aa[i]!='\t')
		{
			//wcout<<L"test "<<aa[i]<<L" end"<<endl;
			bb+=aa[i];

		}
	}
	return bb;
}
wstring filter(wstring &mm)
{
	for(int i=0;i<mm.length();i++)
	{
		if((int)mm[i]>=0x3000&&(int)mm[i]<=0x303F)
		{
			//wcout<<L"find: "<<mm[i]<<endl;
			mm[i]=' ';
		}
		if((int)mm[i]>=0x2000&&(int)mm[i]<=0x206F)
		{
			//wcout<<L"find: "<<mm[i]<<endl;
			mm[i]=' ';
		}
		if((int)mm[i]>=0xff01&&(int)mm[i]<=0xff1f)
		{
			//wcout<<L"find: "<<mm[i]<<endl;
			mm[i]=' ';
		}

	}

	return mm;
}
int compare(void *a,void *b)
{

	chinese_word *m1=(chinese_word*)a;
	chinese_word *m2=(chinese_word*)b;

	return (m1->feq-m2->feq);
}


void  process_words(wstring &m,map<wstring,chinese_word>& f_words)
{

	//反顺序扫描
	wstring::reverse_iterator iter=m.rbegin();
	wstring::reverse_iterator iter2;
	for(;iter!=m.rend();iter++)
	{
		wstring tmp=L"";
		for(iter2=iter;iter2!=m.rend();iter2++)
		{
			tmp+=*iter2;
			wstring item=tmp.substr(0,1); //获取右邻字
			wstring item_tmp=tmp.substr(1,d);
			wstring item2(item_tmp.rbegin(),item_tmp.rend());
			map<wstring,chinese_word>::iterator mit;
			if((mit=f_words.find(item2))==f_words.end())
			{
				chinese_word tmpword;
				tmpword.str=item2;
				//map<wstring,int>::hit;
				//if((hit=tmpword.right_map.find(
				if(item!=L"")
				{
					tmpword.right_map.insert(pair<wstring,int>(item,1));
				}
				f_words.insert(pair<wstring,chinese_word>(item2,tmpword));
			}
			else
			{
				
				mit->second.feq++;
				map<wstring,int>::iterator hit;
				if(item!=L"")
				{
			
					if((hit=mit->second.right_map.find(item))!=mit->second.right_map.end())
					{
						hit->second++;

					}
					else
					{

						mit->second.right_map.insert(pair<wstring,int>(item,1));
					}
				}

			}

		}
	}

}

int process_line(wstring &str,map<wstring,chinese_word>& f_words,words_t& content)
{

	char buf[MAX_LINE];
	string tmpstr;
	wcstombs(buf,str.c_str(),MAX_LINE);
	istringstream mystr;
	mystr.str(buf);
	while(!mystr.eof())
	{
		string onestr;
		mystr>>onestr;
		wchar_t wcs[MAX_LINE];
		if(onestr.length()>MAX_LINE) cout<<"error"<<endl;
		mbstowcs(wcs,onestr.c_str(),MAX_LINE);
		wstring tmpstr(wcs);
		content.push_back(tmpstr);
		process_words(tmpstr,f_words);
		//wcout<<tmpstr<<endl;
	}
	return 0;

}
int _tmain(int argc, _TCHAR* argv[])
{
	std::locale loc = std::locale::global( std::locale("chs",std::locale::ctype) );
	setlocale(LC_CTYPE,"");
	const char *filename="a.txt";
	fstream fs(filename);
	string ms;
	wstring m=L"";
	map<wstring,chinese_word> f_words;
	words_t content;

	while(!fs.eof()){
		fs>>ms;
		wchar_t wcs[MAX_LINE];
		if(ms.length()>MAX_LINE) cout<<"error"<<endl;
		mbstowcs(wcs,ms.c_str(),MAX_LINE);
		wstring tmpstr(wcs);
		filter(tmpstr);
		process_line(tmpstr,f_words,content);
		//cin.get();
	}
	fs.close();

	cout<<"read words compelte"<<endl;

	chinese_word_t c_vc;
	map<wstring,chinese_word>::iterator m_it;
	for(m_it=f_words.begin();m_it!=f_words.end();m_it++)
	{
		chinese_word tmp;
		tmp.str=m_it->first;
		tmp=m_it->second;
		//	wcout<<m_it->first<<L" "<<m_it->second<<endl;
		c_vc.push_back(tmp);
	}

	sort(c_vc.begin(),c_vc.end(),greater<chinese_word>());
	print_words2(c_vc,5000);


	return 0;
}

