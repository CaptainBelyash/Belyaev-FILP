
#include <iostream>
#include "smart_pointer.h"




int main() {
	char* ptr = new char(97);

	auto* first = new SmartPointer<char>(ptr);

	std::cout << "First get: " << first->Get()
		<< "; count:" << *(first->ref_count) << std::endl;
	
	auto* second = first;
	std::cout << "First get: " << first->Get()
		<< "; count:" << *(first->ref_count) << std::endl;
	std::cout << "Second get: " << second->Get()
		<< "; count:" << *(second->ref_count) << std::endl;
	ptr = new char(98);
	second->Set(ptr);
	std::cout << "First get: " << first->Get()
		<< "; count:" << *(first->ref_count) << std::endl;
	std::cout << "Second get: " << second->Get()
		<< "; count:" << *(second->ref_count) << std::endl;
	first->Release();
	std::cout << "First get: " << first->Get()
		<< "; count:" << *(first->ref_count) << std::endl;
	std::cout << "Second get: " << second->Get()
		<< "; count:" << *(second->ref_count) << std::endl;
}
