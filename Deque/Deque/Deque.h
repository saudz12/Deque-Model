#pragma once
#include <vector>
#include <iostream>
#include <array>

template <class T = int> class Deque
{
	int poz_head, poz_tail; // poz in the block
	int head, tail; //poz in the array

	int nr_blocks, nr_elems;
	int empty_left, empty_right; //unused blocks

protected:
	using block = std::array<T, 8>; //fixed block of 8 elements of T type

	std::vector<block*> construct; //STL vector of pointers to block; easier find - constant; harder push_front and resize;

	void alloc(int state);
	void dealloc(int state);
	bool snake();
	
public:
	Deque();
	~Deque();

	void print();

	T& operator[](int);
	T& at(int);

	bool empty();
	int size();

	T front();
	T back();

	void push_back(const T& key);
	void push_front(const T& key);
	void delete_elem(int poz);

	void pop_front();
	void pop_back();
	void insert(int poz, const T& key); //inserts in poz, pushes all elements
	 
};

template<class T>
void Deque<T>::alloc(int state)
{
	if (state == 0 && empty_left) {
		empty_left--;
		head--;
		return;
	}
	if (state == 1 && empty_right) {
		empty_right--;
		tail++;
		return;
	}
	block* newB = new block;
	if (newB == nullptr) {
		std::cerr << "Alloc err\n";
		exit(0);
	}
	nr_blocks++;
	switch (state)
	{
	case 0: { //with linked lists only have to add new one to the left/right;
		std::vector<block*> aux = construct;
		construct.resize(nr_blocks);
		construct[0] = newB;
		for (int i = 1; i < nr_blocks; i++)
			construct[i] = aux[i - 1];
		head--;
		break;
	}
	case 1: {
		construct.push_back(nullptr);
		construct[nr_blocks - 1] = newB;
		tail++;
		break;
	}
	default:
		std::cerr << "Alloc err\n";
		exit(0);
	}
}

template<class T>
void Deque<T>::dealloc(int state)
{
	if (state == 0 && empty_left >= 2) {
		nr_blocks--;
		std::vector<block*> aux = construct;
		delete construct[0];
		construct.resize(nr_blocks);
		for (int i = 0; i < nr_blocks; i++)
			construct[i] = aux[i + 1];	
		tail--;
		return;
	}
	if (state == 1 && empty_right >= 2) {
		nr_blocks--;
		delete construct[nr_blocks];
		construct.pop_back();
		tail--;
		return;
	}
	switch (state)
	{
	case 0: {
		head++;
		return;
	}
	case 1: {
		tail--;
		return;
	}
	default: {
		std::cerr << "Dealloc err\n";
	}
	}
	return;
}

template<class T>
bool Deque<T>::snake()
{
	return tail == head && poz_head == poz_tail;
}

template<class T>
Deque<T>::Deque()
{
	poz_head = 3;
	poz_tail = 3;
	head = 1;
	tail = 1;

	nr_blocks = 3;
	nr_elems = 0;

	empty_left = 1;
	empty_right = 1;

	construct.resize(3);
	for (int i = 0; i < 3; i++)
		construct[i] = new block;
	
}

template<class T>
Deque<T>::~Deque()
{
	for (int i = empty_left; i < nr_blocks - empty_right; i++)
		delete construct[i];
}

template<class T>
void Deque<T>::print()
{
	int trough = 0;

	/*
	//funky
	block* aux;
	for (int i = poz_head; i < 8 && trough < nr_elems; i++, trough++) {
		aux = construct[head];
		std::cout << aux->operator[](i) << " ";
	}
	for (int i = 1 + empty_left; i < nr_blocks - 1 - empty_right && trough < nr_elems; i++)
		for (int j = 0; j < 8 && trough < nr_elems; j++, trough++) {
			aux = construct[i];
			std::cout << aux->operator[](j) << " ";
		}
	for (int i = 0; i <= poz_tail && trough < nr_elems; i++, trough++) {
		aux = construct[tail];
		std::cout << aux->operator[](i) << " ";
	}*/

	for (int i = 0; i < nr_elems; i++)
		std::cout << (*this)[i] << " ";
	
	std::cout << '\n';

	return;
}

template<class T>
T& Deque<T>::operator[](int i)
{
	if (i >= nr_elems) {
		std::cerr << "Out of bounds\n";
		exit(0);
	}
	if (poz_head + i < 8) 
		return (*construct[head])[poz_head + i];
	if (nr_elems - poz_tail - 1 <= i)
		return (*construct[tail])[poz_tail - (nr_elems - i - 1)];
	int aux = i;
	aux -= (8 - poz_head);
	return (*construct[1 + aux / 8])[aux % 8];
}

template<class T>
T& Deque<T>::at(int poz)
{
	return (*this)[poz];
}

template<class T>
bool Deque<T>::empty()
{
	return nr_elems == 0 && snake();
}

template<class T>
int Deque<T>::size()
{
	return nr_elems;
}

template<class T>
T Deque<T>::front()
{
	if (empty()) {
		std::cerr << "Invalid op.\n";
		exit(1);
	}
	return (*this)[0];
}

template<class T>
T Deque<T>::back()
{
	if (empty()) {
		std::cerr << "Invalid op.\n";
		exit(1);
	}
	return (*this)[nr_elems-1];
}

template<class T>
void Deque<T>::push_back(const T& key)
{
	if (empty()) {
		(*construct[tail])[poz_tail] = key;
		nr_elems++;
		return;
	}
	nr_elems++;
	poz_tail++;
	if (poz_tail > 7) {
		alloc(1);
		poz_tail = 0;
	}
	(*construct[tail])[poz_tail] = key;
}

template<class T>
void Deque<T>::push_front(const T& key)
{
	if (empty()) {
		(*construct[head])[poz_head] = key;
		nr_elems++;
		return;
	}
	nr_elems++;
	poz_head--;
	if (poz_head < 0) {
		alloc(0);
		poz_head = 7;
	}
	(*construct[head])[poz_head] = key;
}

template<class T>
void Deque<T>::delete_elem(int poz)
{
	if (poz >= nr_elems) {
		std::cerr << "Out of bounds.\n";
		exit(0);
	}
	if (poz == 0) {
		pop_front();
		return;
	}
	if (poz == nr_elems - 1) {
		pop_back();
		return;
	}
	switch (poz > nr_elems/2)
	{
	case 0: {
		for (int i = poz; i > 0; i--)
			(*this)[i] = (*this)[i - 1];
		nr_elems--;
		(*this)[0] = T();
		if (nr_elems == 0)
			return;
		poz_head++;
		if (poz_head >= 8) {
			empty_left++;
			dealloc(0);
			poz_head = 0;
		}
		return;
	}
	case 1: {
		for (int i = poz; i < nr_elems - 1; i++)
			(*this)[i] = (*this)[i + 1];
		nr_elems--;
		(*this)[nr_elems] = T();
		if (nr_elems == 0)
			return;
		poz_tail--;
		if (poz_tail < 0) {
			empty_right++;
			dealloc(1);
			poz_tail = 7;
		}
		return;
	}
	default:
		break;
	}
	return;
}

template<class T>
void Deque<T>::pop_front()
{
	if (empty()) {
		std::cout << "The deque is empty.\n";
		return;
	}
	nr_elems--;
	(*construct[head])[poz_head] = T();
	if (nr_elems == 0)
		return;
	poz_head++;
	if (poz_head >= 8) {
		empty_left++;
		dealloc(0);
		poz_head = 0;
	}
}

template<class T>
void Deque<T>::pop_back()
{
	if (empty()) {
		std::cout << "The deque is empty.\n";
		return;
	}
	nr_elems--;
	(*construct[tail])[poz_tail] = T();
	if (nr_elems == 0)
		return;
	poz_tail--;
	if (poz_tail < 0) {
		empty_right++;
		dealloc(1);
		poz_tail = 7;
	}
}

template<class T>
void Deque<T>::insert(int poz, const T& key)
{
	if (poz >= nr_elems) {
		std::cerr << "Out of bounds.\n";
		exit(0);
	}
	if (poz == 0) {
		push_front(key);
		return;
	}
	switch (poz > nr_elems / 2) {
	case 0: {
		T aux = (*this)[0];
		for (int i = 0; i < poz; i++) {
			(*this)[i] = (*this)[i + 1];
		}
		(*this)[poz] = key;
		push_front(aux);
		return;
	}
	case 1: {
		push_back((*this)[nr_elems - 1]);
		for (int i = nr_elems - 1; i > poz; i--) {
			(*this)[i] = (*this)[i - 1];
		}
		(*this)[poz] = key;
		return;
	}
	default: {
		std::cerr << "Insert err.\n";
		exit(0);
	}
	}
}
