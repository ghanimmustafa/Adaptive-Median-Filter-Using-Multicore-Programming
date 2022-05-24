// Compile:
// g++ calculate_averages.cpp -o calculate_averages
#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <fstream>
using namespace std;

int main(int argc, char* argv[]) {

	if(argc <= 2)
		  std:: cout << "<file_name.txt> <average_per_lines>" <<std::endl;
		  return -1; 	
	std::ifstream file(argv[2]);
	if (file.is_open()) {
		 std::string line;
		 while (std::getline(file, line)) {
		     // using printf() in all tests for consistency
		     cout << line <<  std::endl;
		 }
		 file.close();
	}
		return 0;
}
