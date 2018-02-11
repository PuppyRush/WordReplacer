
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <direct.h>		//mkdir
#include <Windows.h>
#include <fstream>
#include <istream>
#include <string>
#include <map>

#define BUF_SIZE	512
using namespace std;

string getFilename(const string str) {
	size_t pos=string::npos;
	if ( (pos =str.find_last_of('\\'))>0) {
		return str.substr(pos+1, str.size() - pos);
	}
	return str;
}

wchar_t* toWChar(const char *str) {

	size_t len = strlen(str);
	wchar_t *wc = new wchar_t[len + 1];

	mbstowcs(wc, str, len);
	wc[len] = NULL;
	return wc;
}

void trimLeft(string &str) {

	size_t count = 0;
	auto c = str.begin();
	while (!str.empty() && c != str.end()) {

		switch (str[count]) {
		case '?':
			count++;
			break;
		default:
			c = str.end()-1;
		}
		
		c++;
	}

	str = str.substr(count, str.size() - count);
}

int main(int argc, const char **argv) {

	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.precision(10);

	if (argc <= 4 || (argc - 2) % 2 == 0) {
		cout << "illegal arguments. check this command" << endl << "[origin path],[new path], ([old word],[new word]) , (,) ,..." << endl;
		exit(0);
	}

	string path(argv[1]);
	map<string,string> words;
	for (int i = 3; i < argc; i+=2) {
		words.insert(make_pair(string(argv[i]),string(argv[i+1])) );
	}

	string newpath = argv[2];
	newpath = newpath.append("\\").append(getFilename(path));

	if (_mkdir(argv[2]) == ENOENT)
	{
		cout << "fail in making directory";
		exit(0);
	}

	trimLeft(newpath);
	trimLeft(path);

	auto w_path = toWChar(path.c_str());
	auto w_newpath = toWChar(newpath.c_str());

	if (!CopyFile(w_path, w_newpath, false)) {
		cout << "Error: " << GetLastError() << endl;
		cout << "cant find the file. close this program. bye." << endl;
		exit(0);
	}
	
	bool isFinded = false;
	char buf[BUF_SIZE];
	memset(buf, 0, BUF_SIZE);

	ifstream fs(newpath, std::ifstream::in);
	ofstream os(path, std::ofstream::out);
	fs.seekg(0, std::ios_base::beg);
	bool isEof = false;
	while (fs.getline(buf, BUF_SIZE)) {
		
		size_t prev_pos = fs.tellg();
		string s(buf);

		auto it = words.begin();
		while (it != words.end()) {
			size_t pos = string::npos;
			if ((pos=s.find(it->first)) != string::npos) {
				s = s.replace(pos, it->first.size(), it->second, 0,it->second.size());
				memset(buf, 0, BUF_SIZE);
				strcat_s(buf, BUF_SIZE, s.c_str());
				it = words.erase(it);
				isFinded = true;
			}
			it++;
		}
		const size_t endpos = strlen(buf);
		buf[endpos] = '\n';
		buf[endpos + 1] = 0;
		os << buf;
	}
	if (isFinded)
		cout << "success changing words. bye.\n";
	else
		cout << "fail chainging words. cant find them or there was something errors."<<endl;
	cout << "note : a origin file was moved for safe ( " << path.c_str() << " -> " << newpath.c_str() << endl;
}

