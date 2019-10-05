#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <fstream>
using namespace std;
namespace fs = std::experimental::filesystem;

//global variables
string current_path = "C:\\";
string hidden_path = "C:/";

//utility functions
vector<string> split(string input, char separator) {
	vector<string> return_value;
	string current_word = "";
	for (int i = 0; i < input.length(); i++) {
		if (input[i] != separator) {
			current_word += input[i];
		}
		else if (input[i] == separator) {
			if (current_word != "") {
				return_value.push_back(current_word);
				current_word = "";
			}
		}
		if (i == input.length() - 1) {
			if (current_word != "") {
				return_value.push_back(current_word);
				current_word = "";
			}
		}
	}
	return return_value;
}

string getInputFolderName(string userinput, int start = 1) {
	string path_ending = "";
	vector<string> splitted_input = split(userinput, ' ');
	for (int i = start; i < splitted_input.size(); i++) {
		path_ending += splitted_input[i];
		if (i != splitted_input.size() - 1) {
			path_ending += " ";
		}
	}
	return path_ending;
}

void printPath() {
	cout << current_path + " > ";
}

int input_to_case(string input) {
	string first_word = split(input, ' ')[0];
	int word_count = split(input, ' ').size();
	if (first_word == "help" && word_count == 1) {
		return 0;
	}
	else if (input == "ls") {
		return 1;
	}
	else if (input == "cd ..") {
		return 2;
	}
	else if (first_word == "mkdir" && word_count > 1) {
		return 3;
	}
	else if (first_word == "cd") {
		return 4;
	}
	else if (first_word == "rm") {
		if (split(input, ' ')[1] == "-rf" && word_count > 2) {
			return 5;
		}
		else if (word_count > 1){
			return 6;
		}
	}
	else if (first_word == "touch" && word_count > 1) {
		return 7;
	}
	else {
		return -1;
	}
}

bool all_dots(string userinput) {
	bool alldots = true;
	for (int i = 0; i < userinput.length(); i++) {
		if (userinput[i] != '.' && userinput[i] != ' ') {
			alldots = false;
		}
	}
	return alldots;
}

//actual console commands
void ls_function() {
	for (const auto& entry : std::experimental::filesystem::directory_iterator(current_path)) {
		cout << entry.path() << endl;
	}
}

void cd_function(string userinput) {
	string path_ending = getInputFolderName(userinput);
	if (path_ending != "") {
		fs::path p(current_path + path_ending);
		if (fs::exists(p) && fs::is_directory(p) && !all_dots(path_ending)) {
			current_path += path_ending + "\\";
			hidden_path += path_ending + "/";
		}
		else {
			cout << "No such directory." << endl;
		}
	}
	else {
		current_path = "C:\\";
		hidden_path = "C:/";
	}
}

void cd_back_function(string userinput) {
	vector<string> splitted_path = split(hidden_path, '/');
	if (splitted_path.size() > 1) {
		current_path = "";
		hidden_path = "";
		for (int i = 0; i < splitted_path.size() - 1; i++) {
			current_path += splitted_path[i] + "\\";
			hidden_path += splitted_path[i] + "/";
		}
	}
}

void mkdir_function(string userinput) {
	try {
		fs::create_directory(current_path + getInputFolderName(userinput));
	}
	catch (exception ex) {}
}

void rm_function(string userinput) {
	string path_ending = getInputFolderName(userinput);
	fs::path p(current_path + path_ending);
	if (fs::exists(p) && !(fs::is_directory(p))) {
		try {
			fs::remove(p);
		}
		catch (exception ex) {}
	}
	else if (!(fs::exists(p))) {
		cout << "No such directory." << endl;
	}
	else {
		cout << "rm: cannot remove '" + path_ending + "': Is a directory" << endl;
	}
}

void rm_rf_function(string userinput) {
	string path_ending = getInputFolderName(userinput, 2);
	fs::path p(current_path + path_ending);
	if (fs::exists(p)) {
		try {
			fs::remove_all(p);
		}
		catch (exception ex) {}
	}
	else {
		cout << "No such directory." << endl;
	}
}

void touch_function(string userinput) {
	try {
		ofstream file;
		file.open(current_path + getInputFolderName(userinput));
		file.close();
	}
	catch (exception ex) {}
}
//main
int main() {
	string userinput = " ";
	while (userinput != "") {
		printPath();
		getline(cin, userinput);
		switch (input_to_case(userinput)) {
		case -1: cout << '"' + userinput + '"' + " is not a valid command." << endl;
			break;
		case 0: cout << endl <<
			"Funkciok:" << endl <<
			"-ls\t\t\tAktualis konyvtar tartalmanak kilistazasa." << endl <<
			"-cd 'utvonal'\t\tMappak kozott leptetes." << endl <<
			"-cd..\t\t\tVisszalepes." << endl <<
			"-mkdir 'mappanev'\tMappa letrehozasa." << endl <<
			"-rm 'utvonal'\t\tObjektum torlese (kiveve mappa);" << endl <<
			"-rm -rf 'utvonal'\tObjektum (rekurziv) torlese" << endl <<
			"-touch 'fajlnev'\tFajl letrehozasa." << endl;
			break;
		case 1: ls_function();
			break;
		case 2: cd_back_function(userinput);
			break;
		case 3: mkdir_function(userinput);
			break;
		case 4: cd_function(userinput);
			break;
		case 5: rm_rf_function(userinput);
			break;
		case 6: rm_function(userinput);
			break;
		case 7: touch_function(userinput);
			break;
		}
	}
}
