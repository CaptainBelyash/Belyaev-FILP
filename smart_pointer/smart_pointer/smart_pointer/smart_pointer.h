
#include <iostream>

class SmartPointer
{
public:
	SmartPointer(T* ptr) {
		ref = ptr;
		ref_count = (unsigned*)malloc(sizeof(unsigned));
		*ref_count = 1;
	}

	SmartPointer(SmartPointer& sptr) {
		ref = sptr.ref;
		ref_count = sptr.ref_count;
		++(*ref_count);
	}

	/* �������������� �������� ��������� (eqal), ������� ����� �� ����������
	 * ���� ���������������� ��������� � ������, ���������� ������ ������� ���������
	 * ����� ���������, � ������ - ���������.
	 */
	SmartPointer& operator=(SmartPointer& sptr) {
		/* ���� ��� ��������� �������, ������� ���� ������. */
		if (*ref_count > 0) {
			remove();
		}
		if (this != &sptr) {
			ref = sptr.ref;
			ref_count = sptr.ref_count;
			++(*ref_count);
		}
		return *this;
	}

	~SmartPointer() {
		remove(); // ������� ���� ������ �� ������.
	}

	T operator*() {
		return *ref;
	}

protected:
	void remove() {
		--(*ref_count);
		if (ref_count == 0) {
			delete ref;
			free(ref_count);
			ref = NULL;
			ref_count = NULL;
		}
	}

	T* ref;
	unsigned* ref_count;
};

