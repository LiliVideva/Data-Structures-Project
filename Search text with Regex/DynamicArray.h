/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Lilyana Videva
* @idnumber 61887
* @task 0
* @compiler VC
*
*/

#pragma once
#include<assert.h>

template<typename T>
class DynamicArray
{
private:
	T* data;
	size_t size, space;

	void ensureSize(size_t size);
	void resize(size_t newSpace);
public:
	DynamicArray(size_t initialSpace = 16);
	~DynamicArray();
	void append(const T& element);
	void clear();
	T getElement(size_t index) const;

	size_t getSize() const;
	size_t getSpace() const;
	bool isEmpty() const;
};


template<typename T>
DynamicArray<T>::DynamicArray(size_t initialSpace)
{
	data = nullptr;
	size = 0;
	space = initialSpace;
	assert(initialSpace > 0);
	data = new T[space];
}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
	clear();
}

//includes new element on the next free position in the array
template<typename T>
void DynamicArray<T>::append(const T& element)
{
	ensureSize(size + 1);
	data[size] = element;
	++size;
}

//deletes all previous information in the array
template<typename T>
void DynamicArray<T>::clear()
{
	delete[] data;
	data = nullptr;
	size = 0;
	space = 16;
	data = new T[space];
}

//returns the element on the position index
template<typename T>
T DynamicArray<T>::getElement(size_t index) const
{
	assert(index <= size);
	return data[index];
}

//returns the used space 
template<typename T>
size_t DynamicArray<T>::getSize() const
{
	return size;
}

//returns the allocated space
template<typename T>
size_t DynamicArray<T>::getSpace() const
{
	return space;
}

//checks if the array is empty
template<typename T>
bool DynamicArray<T>::isEmpty() const
{
	return size == 0;
}

//checks if the allocated space is enough and resizes the array if necessary
template<typename T>
void DynamicArray<T>::ensureSize(size_t size)
{
	assert(!data || space != 0);

	size_t newSize = data ? space : 1;
	while (newSize < size)
		newSize *= 2;

	if (newSize != space)
		resize(newSize);
}

//resizes the array by creating new larger one and moves all of the information from the old one
template<typename T>
void DynamicArray<T>::resize(size_t newSpace)
{
	T* newData = new T[newSpace];

	for (size_t i = 0; i < size && i < newSpace; ++i)
		newData[i] = data[i];

	delete[] data;
	data = newData;
	space = newSpace;
	size = size < space ? size : space;
}