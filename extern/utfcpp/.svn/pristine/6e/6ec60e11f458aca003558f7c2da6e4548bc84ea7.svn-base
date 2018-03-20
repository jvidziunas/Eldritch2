#include "../../source/utf8.h"
using namespace utf8;

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

const char* TEST_FILE_PATH = "../../test_data/negative/utf8_invalid.txt";
const unsigned INVALID_LINES[] = { 75, 76, 82, 83, 84, 85, 93, 102, 103, 105, 106, 107, 108, 109, 110, 114, 115, 116, 117, 124, 125, 130, 135, 140, 145, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 169, 175, 176, 177, 207, 208, 209, 210, 211, 220, 221, 222, 223, 224, 232, 233, 234, 235, 236, 247, 248, 249, 250, 251, 252, 253, 257, 258, 259, 260, 261, 262, 263, 264, 268, 269}; 
const unsigned* INVALID_LINES_END = INVALID_LINES + sizeof(INVALID_LINES)/sizeof(unsigned);

int main()
{
    // Open the test file
    ifstream fs8(TEST_FILE_PATH);
    if (!fs8.is_open()) {
    cout << "Could not open " << TEST_FILE_PATH << endl;
    return 0;
    }

    // Read it line by line
    unsigned int line_count = 0;
    char byte;
    while (!fs8.eof()) {
        string line;
        while ((byte = static_cast<char>(fs8.get())) != '\n' && !fs8.eof()) 
	    line.push_back(byte);

        line_count++;
	// Print out lines that contain invalid UTF-8
	if (!is_valid(line.begin(), line.end())) {
	   const unsigned* u = find(INVALID_LINES, INVALID_LINES_END, line_count);
	   if (u == INVALID_LINES_END) 
	       cout << "Unexpected invalid utf-8 at line " << line_count << '\n';
        }
    }
}
