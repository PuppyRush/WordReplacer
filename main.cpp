
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
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

int main(int argc, const char **argv) {

	if (argc <= 2 || (argc - 2) % 2 == 1) {
		cerr << "illegal arguments. check this command" << endl << "[origin path],([old word],[new word]) , (,) ,..." << endl;
		exit(0);
	}

	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.precision(10);

	string path(argv[1]);
	map<string,string> words;
	for (int i = 2; i < argc; i+=2) {
		words.insert(make_pair(string(argv[i]),string(argv[i+1])) );
	}

	string temppath = "c:/temp_src/";
	temppath = temppath.append(getFilename(path));

	wchar_t _path[BUF_SIZE];
	wchar_t newpath[BUF_SIZE];
	
	mbstowcs(_path, path.c_str(), strlen(path.c_str()) + 1);
	mbstowcs(newpath, temppath.c_str(), temppath.size() + 1);
	if (!CopyFile(_path, newpath, false)) {
		cerr << "Error: " << GetLastError() << endl;
		cerr << "need a directory necessarily-> c:\temp_src. check this dir. close this program. bye." << endl;
		exit(0);
	}
	
	char buf[BUF_SIZE];
	memset(buf, 0, BUF_SIZE);

	ifstream fs(temppath, std::ifstream::in);
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
			}
			it++;
		}
		const size_t endpos = strlen(buf);
		buf[endpos] = '\n';
		buf[endpos + 1] = 0;
		os << buf;
	}
	cout << "success changing words. bye.\n";
	cout << "note : a origin file was moved to c:\temp_src";
}