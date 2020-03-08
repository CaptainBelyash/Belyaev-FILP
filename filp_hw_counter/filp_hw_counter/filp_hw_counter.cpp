
#include <iostream>
#include <fstream>

using namespace std;

int fileSize(char fileName[]) {
	ifstream file(fileName, ios::binary | ios::ate);
	int lenght = int(file.tellg());
	file.close();
	return lenght;
}

char getDataFromFile(char fileName[]) {

}

void myFunc(char bookName[]) {
	int lenght = fileSize(bookName);
	cout << lenght << endl;
	ifstream bookFile;
	bookFile.open(bookName);
	char readerBuffer[50];
	int n;
	cout << "Say a number:" << endl;
	cin >> n;
	for (int i = 0; i < n; i++) {
		bookFile >> readerBuffer;
		cout << readerBuffer << " ";
	}
	bookFile.close();
}

int main()
{
	char bookName[] = "book.txt";
	myFunc(bookName);
}