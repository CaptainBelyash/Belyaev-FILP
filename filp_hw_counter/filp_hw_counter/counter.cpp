
#include <ctime>
#include <iostream>
#include <fstream>
#include <map>
#include <set>

using namespace std;


class Buffer {
public:
	char* buffer;
	size_t pointer;
	size_t size = 1000000;
	Buffer* next;

	Buffer() {
		pointer = 0;
		buffer = new char[size];
		next = NULL;
	}

	~Buffer() {
		if (buffer != nullptr)
			delete buffer;
	}
};

class BufferList {
public:
	BufferList() {
		currentBuffer = new Buffer();
		firstBuffer = currentBuffer;
	}

	void* allocate(size_t length, size_t size) {

		if (checkOutOfBuffer(length * size)) {
			currentBuffer->next = new Buffer();
			currentBuffer = currentBuffer->next;
		}

		void* newPointer = currentBuffer->buffer + currentBuffer->pointer;
		currentBuffer->pointer += length * size;
		return newPointer;
	}

	~BufferList() {
		while (firstBuffer != NULL) {
			Buffer* buffToDelete = firstBuffer;
			firstBuffer = firstBuffer->next;
			delete buffToDelete;
		}
	}

private:
	Buffer* currentBuffer;
	Buffer* firstBuffer;

	bool checkOutOfBuffer(size_t size) {
		return currentBuffer->pointer + size > currentBuffer->size;
	}

};

template <class T> class CustomAlloc {
public:
	typedef T value_type;
	typedef T* pointer;

	BufferList* bufferList;

	CustomAlloc() {
		bufferList = new BufferList();
	};

	template<class U>
	CustomAlloc(CustomAlloc<U>& other) {
		bufferList = other.bufferList;
	}

	pointer allocate(size_t size) {
		return reinterpret_cast<T*>(bufferList->allocate(size, 
			sizeof(T)));
	};

	void deallocate(pointer p, size_t n) {};
};


int fileSize(char* fileName) {
	ifstream file(fileName, ios::binary | ios::ate);
	int size = file.tellg();
	file.close();
	return size;
}

struct PairComparer
{
	bool operator()(pair<char*, int> const pair1,
		pair<char*, int> const pair2) const
	{
		return pair1.second == pair2.second
			? strcmp(pair1.first, pair2.first) < 0
			: pair1.second > pair2.second;
	}
};

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

void printAnswer(set<std::pair<char*, int>, PairComparer> res, int size, int time) {
	cout << "Time: " << time << endl; 
	cout << "Size: " << size << endl;
	
	for (const auto& p : res)
		cout << p.first << ": " << p.second << endl;
}

int wordCount(char* bookName) {
	int size = fileSize(bookName);
	char* text = getTextFromFile(bookName, size);

	unsigned int startTime = clock();
	map <char*, int> counter;
	int wordLenght = 0;
	int i;
	for (i = 0; i < size && text[i] != '\0'; i++) {
		if (text[i] < 0)
			continue;
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
		
	unsigned int endTime = clock();

	set<std::pair<char*, int>, PairComparer> res;
	for (const auto& p : counter)
		res.emplace(p.first, p.second);
	printAnswer(res, size, endTime - startTime);
	return endTime - startTime;
}

int wordCountCustomAlloc(char* bookName) {
	int size = fileSize(bookName);
	char* text = getTextFromFile(bookName, size);

	unsigned int startTime = clock();
	map <char*, int, StringComparer,
		CustomAlloc<pair<const char*, int>>> counter;
	int wordLenght = 0;
	int i;
	for (i = 0; i < size && text[i] != '\0'; i++) {
		if (text[i] < 0)
			continue;

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

	unsigned int endTime = clock();

	set<std::pair<char*, int>, PairComparer> res;
	for (const auto& p : counter)
		res.emplace(p.first, p.second);
	printAnswer(res, size, endTime - startTime); 
	return endTime - startTime;
}

int main()
{
	char bookName[] = "book.txt";
	int standartAllocTime = wordCount(bookName);
	int customAllocTime = wordCountCustomAlloc(bookName);
	cout << "Standart alloc time: " << standartAllocTime << endl;
	cout << "Custom alloc time: " << customAllocTime << endl;
}