
#include <iostream>
#include <cmath>

using namespace std;


enum BlockStatus {
	Free,
	Occuped,
	Splitted
};


class Block {
public:
	Block(size_t position, size_t size, size_t minSize) {
		this->position = position;
		this->size = size;
		this->minSize = minSize;
	}

	~Block() {
		free(ptr);
		free(left);
		free(right);
	}

	int* AddData(size_t dataSize) {
		switch (status) {
		case Free:
			if (size == minSize) {
				addData(dataSize);
				return ptr;
			}

			if (dataSize <= size / 2) {
				split();
				return left->AddData(dataSize);
			}

			addData(dataSize);
			return ptr;

		case Occuped:
			return nullptr;

		case Splitted:
			if (dataSize <= size / 2) {
				int* ptrFromLeft = left->AddData(dataSize);
				return ptrFromLeft != nullptr ? ptrFromLeft
					: right->AddData(dataSize);
			}
			return nullptr;
		}
	}

	void ReleaseData(int* ptr) {
		switch (status) {
		case Occuped:
			if (this->ptr == ptr) {
				cout << "Data released from ";
				PrintBlockData();
				cout << endl;

				free(this->ptr);
				status = Free;
			}
			break;

		case Splitted:
			left->ReleaseData(ptr);
			right->ReleaseData(ptr);
			if (left->status == Free 
				&& right->status == Free)
				mergeChildBlocks();
			break;
		}		
	}

	void PrintBlockData() {
		cout << "[" << position << ":" << position + size - 1 << "]";
	}

private:
	BlockStatus status = Free;
	size_t position;
	size_t size;
	size_t minSize;

	int* ptr;

	Block* left;
	Block* right;

	void addData(size_t dataSize) {
		cout << "Block ";
		PrintBlockData();
		cout << " filled with " << dataSize
			<< " bytes" << endl;

		ptr = (int*)malloc(dataSize);

		status = Occuped;
	}

	void split() {
		left = new Block(position, size / 2, minSize);
		right = new Block(position + size / 2, size / 2, minSize);

		status = Splitted;
	}

	void mergeChildBlocks() {
		cout << "Blocks ";
		left->PrintBlockData();
		cout << ", ";
		right->PrintBlockData();
		cout << " merged in ";
		PrintBlockData();
		cout << endl;

		free(left);
		free(right);

		status = Free;		
	}
};


class BuddyAllocator {
public:
	//Конструктор принимает размер наименьшего блока, 
	//который может быть получен при разбиении блоков,
	//и размер наибольшего блока
	//в виде степени 2
	BuddyAllocator(size_t minPow, 
		size_t maxPow) {
		minBlockSize = pow(2, minPow);
		maxBlockSize = pow(2, maxPow);

		root = new Block(0, maxBlockSize, minBlockSize);
	}

	~BuddyAllocator() {
		free(root);
	}

	int* allocate(size_t dataSize) {
		if (dataSize > maxBlockSize)
			throw invalid_argument("Size of data is bigger than maximum block size");
		return root->AddData(dataSize);
	}

	void deallocate(int* ptr) {
		root->ReleaseData(ptr);
	}

private:
	size_t maxBlockSize;
	size_t minBlockSize;
	Block* root;
};


int main()
{
	BuddyAllocator* buddy = new BuddyAllocator(3, 10);
	cout << "Add big block" << endl;
	int* a = buddy->allocate(512);
	cout << endl << "Add first medium block" << endl;
	int* b = buddy->allocate(60);
	cout << endl << "Release big block" << endl;
	buddy->deallocate(a);
	cout << endl << "Add very small block" << endl;
	int* c = buddy->allocate(5);
	cout << endl << "Add second medium block" << endl;
	int* d = buddy->allocate(256);
	cout << endl << "Release second medium block" << endl;
	buddy->deallocate(d);
	cout << endl << "Release very small block" << endl;
	buddy->deallocate(c);
	cout << endl << "Release first medium block" << endl;
	buddy->deallocate(b);
}
