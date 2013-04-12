// wordseg.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <sstream> 
#include<math.h>
#define MAX_LINE 5000
int d=5;
int MAX=99999999;
struct chinese_word{
	wstring str;
	int feq;  //频次
	hash_map<wstring,int> left_map;
	hash_map<wstring,int> right_map;
	double left_h;
	double right_h;
	double arg;
	double f_h; //最后的信息熵
	chinese_word()
	{
		feq=0;
		left_h=0.0;
		right_h=0.0;
		arg=0.0;
	}
	bool   operator <  (const   chinese_word&   rhs   )  const   //升序排序时必须写的函数
	{   
		if(f_h==rhs.f_h)
		{
			return arg<rhs.arg;
		}
		return   f_h   <   rhs.f_h; 
	}

	bool   operator >  (const   chinese_word&   rhs   )  const   //降序排序时必须写的函数
	{   
		if(f_h==rhs.f_h)
		{
			return arg>rhs.arg;
		}
		return   f_h   >   rhs.f_h; 
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
void output_words2(fstream &fs,chinese_word_t &m,int max=-1)
{
	chinese_word_t::iterator iter;
	int i=0;
	for(iter=m.begin();iter!=m.end();iter++)
	{

		if(max!=-1&&max<i) break;
		if(iter->str.length()>=1)
		{
			i++;
				fs<<iter->str.c_str()<<" "<<iter->arg<<" "<<iter->f_h<<endl;
			//fs<<iter->str.c_str()<<" "<<iter->feq<<" "<<iter->left_h<<" "<<iter->right_h<<endl;
		//	fs.write((char *) iter->str.c_str(), iter->str.length() * sizeof(wchar_t));
			//fs.write(" ",1);
			//wcout<<iter->str.c_str()<<L" "<<iter->feq<<L" "<<iter->left_h<<L" "<<iter->right_h<<endl;

			wcout<<iter->str.c_str()<<L" "<<iter->arg<<L" "<<iter->f_h<<endl;
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


void  process_words(wstring &m,hash_map<wstring,chinese_word>& f_words)
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
			wstring item,item_tmp,item2;
			if(tmp.length()>d)
			{
				item=tmp.substr(d,1); //获取右邻字
			    item_tmp=tmp.substr(0,d);
				item2=wstring(item_tmp.rbegin(),item_tmp.rend());
			}
			else
			{
				item=L"";
				item2=wstring(tmp.rbegin(),tmp.rend());;

			}
			//wcout<<item2<<L" "<<item<<endl;
			hash_map<wstring,chinese_word>::iterator mit;
			if((mit=f_words.find(item2))==f_words.end())
			{
				chinese_word tmpword;
				tmpword.str=item2;
				tmpword.feq=1;
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
				hash_map<wstring,int>::iterator hit;
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
void  process_words_right(wstring &instr,hash_map<wstring,chinese_word>& f_words)
{
	wstring m(instr.rbegin(),instr.rend()); //字符串逆序

	//反顺序扫描
	wstring::reverse_iterator iter=m.rbegin();
	wstring::reverse_iterator iter2;
	for(;iter!=m.rend();iter++)
	{
		wstring tmp=L"";
		for(iter2=iter;iter2!=m.rend();iter2++)
		{
			tmp+=*iter2;
			wstring item,item_tmp,item2;
			if(tmp.length()>d)
			{
				item=tmp.substr(d,1); //获取右邻字
			    item_tmp=tmp.substr(0,d);
				item2=wstring(item_tmp.begin(),item_tmp.end());
			}
			else
			{
				item=L"";
				item2=wstring(tmp.begin(),tmp.end());;

			}
			hash_map<wstring,chinese_word>::iterator mit;
			//wcout<<item2<<L" "<<item<<endl;
			//f_words.find(
			if((mit=f_words.find(item2))==f_words.end())
			{
				/*chinese_word tmpword;
				tmpword.str=item2;
				//map<wstring,int>::hit;
				//if((hit=tmpword.right_map.find(
				if(item!=L"")
				{
					tmpword.left_map.insert(pair<wstring,int>(item,1));
				}
				f_words.insert(pair<wstring,chinese_word>(item2,tmpword));*/
				cout<<"error"<<endl;
			}
			else
			{
				
				hash_map<wstring,int>::iterator hit;
				if(item!=L"")
				{
			
					if((hit=mit->second.left_map.find(item))!=mit->second.left_map.end())
					{
						hit->second++;

					}
					else
					{

						mit->second.left_map.insert(pair<wstring,int>(item,1));
					}
				}

			}

		}
	}

}

int process_line(wstring &str,hash_map<wstring,chinese_word>& f_words,words_t& content)
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
		//cout<<endl;
		process_words_right(tmpstr,f_words);
		//wcout<<tmpstr<<endl;
	}
	return 0;

}
double compute_h(hash_map<wstring,int> &h_map)
{
	double mm=0.0;
	hash_map<wstring,int>::iterator m_it;
	int count=0;
	for(m_it=h_map.begin();m_it!=h_map.end();m_it++)
	{
		count+=m_it->second;


	}
	//cout<<count<<endl;
	if(count==0)
		return MAX;
	for(m_it=h_map.begin();m_it!=h_map.end();m_it++)
	{
		if(m_it->second==0) continue;
		mm+=fabs(m_it->second*log((double)m_it->second/double(count)));

	}
	
	return mm;
}
int compute_words(hash_map<wstring,chinese_word> &f_words,vector<chinese_word> &c_vc)
{
	hash_map<wstring,chinese_word>::iterator m_it;
	for(m_it=f_words.begin();m_it!=f_words.end();m_it++)
	{
		chinese_word tmp;
		m_it->second.left_h=compute_h(m_it->second.left_map);
		m_it->second.right_h=compute_h(m_it->second.right_map);



		m_it->second.f_h=m_it->second.left_h>m_it->second.right_h?m_it->second.right_h:m_it->second.left_h;
		m_it->second.left_map.clear();
		m_it->second.right_map.clear();
		//cout<<m_it->second.left_h<<" "<<m_it->second.right_h<<endl;

		wstring tmpstr=m_it->first;
		//枚举该文本片段有哪两个组合而来
		bool has_l,has_r;
		int feq_l,feq_r;
		double min_feq=MAX;
		for(int i=1;i<=tmpstr.length();i++)
		{
			has_l=false;
			has_r=false;
			wstring str_l,str_r;
			str_l=tmpstr.substr(0,i);
			str_r=tmpstr.substr(i);
			hash_map<wstring,chinese_word>::iterator l_it,r_it;

			if((l_it=f_words.find(str_l))!=f_words.end())
			{
				has_l=true;
				feq_l=l_it->second.feq;
			}
			if(str_r==L"") continue;
			if((r_it=f_words.find(str_r))!=f_words.end())
			{
				has_r=true;
				feq_r=r_it->second.feq;
			}
			if(has_l&&has_r)
			{
				double tmp_feq=m_it->second.feq/double(feq_r*feq_l);
				if(tmp_feq<min_feq)
				{
					min_feq=tmp_feq;
				}
			}

		}
		m_it->second.arg=min_feq;
		tmp=m_it->second;

		//	wcout<<m_it->first<<L" "<<m_it->second<<endl;
		c_vc.push_back(tmp);
	//	cin.get();
	}
	

	sort(c_vc.begin(),c_vc.end(),greater<chinese_word>());

	return 0;

}
int _tmain(int argc, _TCHAR* argv[])
{
	std::locale loc = std::locale::global( std::locale("chs",std::locale::ctype) );
	setlocale(LC_CTYPE,"");
	const char *filename="a.txt";
	const char *filename2="b.txt";
	fstream fs(filename);
	fstream ofs(filename2,wios::out|wios::binary);
	//ofs.imbue(locale(std::locale("chs"),"",LC_CTYPE));
	string ms;
	wstring m=L"";
	hash_map<wstring,chinese_word> f_words;
	chinese_word_t c_vc;
	words_t content;
	while(!fs.eof()){
		fs>>ms;
		wchar_t wcs[MAX_LINE];
		if(ms.length()>MAX_LINE) cout<<"error"<<endl;
		mbstowcs(wcs,ms.c_str(),MAX_LINE);
		wstring tmpstr(wcs);
		filter(tmpstr);
		process_line(tmpstr,f_words,content);
	}
	fs.close();
	cout<<"read words compelte"<<endl;
	compute_words(f_words,c_vc);
	cout<<"process complete"<<endl;
	output_words2(ofs,c_vc);
	ofs.close();
	return 0;
}

