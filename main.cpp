#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <set>

using namespace std;

int main(int argc, const char **argv) {

	if (argc <= 1)
		return -1;

	

	string path(argv[0]);
	set<string> words;
	for (int i = 1; i < argc; i++) {
		words.insert(string(argv[i]));
	}

	wchar_t _path[257];
	wchar_t newpath[20];
	
	mbstowcs_s(_path, path.c_str(), strlen(path.c_str()) + 1);
	mbstowcs(_path, "C:/temp_rsc", strlen("C:/temp.rsc") + 1);
	CopyFile(_path, newpath, true);

	fstream fs(path, std::ifstream::in || std::ostream::out);
	fs.seekg(0, std::ios_base::beg);
	while (fs.is_open() && !fs.eof()) {
		char buf[256];
		memset(buf, 0, 256);

		size_t prev_pos = fs.tellp();
		fs.getline(buf, 256);
		string s(buf);

		auto it = s.begin();
		while (it != s.end()) {
			size_t pos = 0;
			if ((pos=s.find(*it)) != string::npos) {
				fs.seekg(prev_pos + pos);
			}
			it++;
		}

		cout << s;
	}
}