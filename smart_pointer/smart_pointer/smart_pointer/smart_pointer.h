#include <iostream>

using namespace std;

template<typename T>
class SmartPointer
{
public:
	T* ref;
	unsigned* ref_count;

	SmartPointer(T* ptr) {
		ref = ptr;
		ref_count = (unsigned*)malloc(sizeof(unsigned));
		*ref_count = 1;
	}

	T Get() {
		return *ref;
	}
	
	void Set(T* newRef) {
		ref = newRef;
		(*ref_count)++;
	}

	void Release() {
		(*ref_count)--;
		if (*ref_count == 0) {
			free(ref_count);
			delete ref;

			ref_count = nullptr;
			ref = nullptr;
		}
			
	}

	SmartPointer& operator=(SmartPointer& newPointer) {
		if (*ref_count > 0) {
			Release();
		}

		if (this != &newPointer) {
			ref = newPointer.ref;
			ref_count = newPointer.ref_count;
			(*ref_count)++;
		}
		return *this;
	}

	T* operator->() {
		return ref;
	}
	
	T& operator*() {
		return *ref;
	}
};