
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

int fileSize(char* fileName) {
	ifstream file(fileName, ios::binary | ios::ate);
	int size = file.tellg();
	file.close();
	return size;
}

char* getTextFromFile(char* fileName, int size) {
	ifstream file;
	file.open(fileName);
	char* textBuffer = new char[size + 1];
	file.read(textBuffer, size);
	textBuffer[size] = '\0';
	file.close();
	return textBuffer;
}

char* getSubString(char* text, int start, int lenght) {
	char* word = new char[lenght + 1];
	memcpy(word, text + start, lenght);
	word[lenght] = '\0';
	return word;
}

struct StringComparer
{
	bool operator()(char const* a, char const* b) const
	{
		return strcmp(a, b) < 0;
	}
};

void wordCount(char* bookName) {
	int size = fileSize(bookName);
	char* text = getTextFromFile(bookName, size);
	map <char*, int, StringComparer> counter;
	int wordLenght = 0;
	int i;
	for (i = 0; i < size && text[i] > 0; i++) {
		
		if (isalpha(text[i])) {
			wordLenght++;
		}
		else {
			if (wordLenght != 0) {
				counter[getSubString(text, i - wordLenght, wordLenght)] += 1;
				wordLenght = 0;
			}
		}
	}
	if (wordLenght != 0)
		counter[getSubString(text, i - wordLenght, wordLenght)] += 1;


	for (map<char*, int, StringComparer>::iterator iter = counter.begin(); iter != counter.end(); iter++) {
		cout << iter->first << " : " << iter->second << '\n';
	}
}

int main()
{
	char bookName[] = "book.txt";
	wordCount(bookName);
}