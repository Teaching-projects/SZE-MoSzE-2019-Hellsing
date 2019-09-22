#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
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

string getInputFolderName(string userinput) {
	string path_ending = "";
	vector<string> splitted_input = split(userinput, ' ');
	for (int i = 1; i < splitted_input.size(); i++) {
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
	if (input.find("help") != -1) {
		return 0;
	}
	else if (input.find("ls") != -1) {
		return 1;
	}
	else if (input.find("cd..") != -1) {
		return 2;
	}
	else if (input.find("mkdir") != -1) {
		return 3;
	}
	else if (input.find("cd") != -1) {
		return 4;
	}
}

bool all_dots(string userinput) {
	bool alldots = true;
	for (int i = 0; i < userinput.length(); i++) {
		if (userinput[i] != '.' && userinput[i]!=' ') {
			alldots = false;
		}
	}
	return alldots;
}

//actual console commands
void ls_function() {
	for (const auto& entry : std::experimental::filesystem::directory_iterator(current_path)) {
		cout << entry.path() << "\n";
	}
}

void cd_function(string userinput) {
	string path_ending = getInputFolderName(userinput);
	if (path_ending != "") {
		fs::path p(current_path + path_ending);
		if (fs::exists(p) && fs::is_directory(p) && !all_dots(path_ending)) {
			current_path = current_path + path_ending + "\\";
			hidden_path = hidden_path + path_ending + "/";
		}
		else {
			cout << "No such directory.\n";
		}
	}
	else {
		current_path = "C:\\";
		hidden_path = "C:/";
	}
}

void cd_back_function(string userinput) {
	vector<string> splitted_path = split(hidden_path, '/');
	current_path = "";
	hidden_path = "";
	for (int i = 0; i < splitted_path.size() - 1; i++) {
		current_path += splitted_path[i] + "\\";
		hidden_path += splitted_path[i] + "/";
	}
}

void mkdir_function(string userinput) {
	try {
		fs::create_directory(current_path + getInputFolderName(userinput));
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
			case 0: cout << "\nFunkciok:" <<
				"\n-ls\t\tAz aktualis konyvtar tartalmanak kilistazasa." <<
				"\n-cd\t\tRoot mappaba lepes." <<
				"\n-cd 'mappa neve'\tAlmappaba lepes." <<
				"\n-cd..\t\tSzulomappaba lepes." <<
				"\n-mkdir\t\tMappa letrehozasa.\n";
				break;
			case 1: ls_function();
				break;
			case 2: cd_back_function(userinput);
				break;
			case 3: mkdir_function(userinput);
				break;
			case 4: cd_function(userinput);
				break;
		}
	}
}
}
