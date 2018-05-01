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

template<typename T>
class Stack
{
	class Elements
	{
	public:
		T data;
		Elements* next;
		Elements(T, Elements* = NULL);
	};
private:
	Elements* top;
	size_t space;

	void initialization();
public:
	Stack();
	~Stack();

	void destroy();
	void push(const T& element);
	T onTop();
	bool pop(T& element);
	size_t getSize() const;
	bool isEmpty() const;
};

template<typename T>
Stack<T>::Elements::Elements(T data, Elements* next)
{
	this->data = data;
	this->next = next;
}

template<typename T>
Stack<T>::Stack()
{
	initialization();
}

template<typename T>
Stack<T>::~Stack()
{
	destroy();
}

template<typename T>
void Stack<T>::initialization()
{
	top = 0;
	space = 0;
}

//deletes previous information in the stack
template<typename T>
void Stack<T>::destroy()
{
	Elements* temporary;

	while (top)
	{
		temporary = top;
		top = top->next;
		delete temporary;
	}

	initialization();
}

//includes new element on the top of the stack
template<typename T>
void Stack<T>::push(const T & element)
{
	top = new Elements(element, top);
	space++;
}

//returns the element on the top without deleting it
template<typename T>
T Stack<T>::onTop()
{
	return top->data;
}

//returns the element on the top and deletes it from the stack
template<typename T>
bool Stack<T>::pop(T & element)
{
	if (space == 0)
		return false;

	element = top->data;

	Elements* old = top;
	top = top->next;
	delete old;

	space--;

	return true;
}

//returns the size of stack
template<typename T>
size_t Stack<T>::getSize() const
{
	return space;
}

//checks if the stack is empty
template<typename T>
bool Stack<T>::isEmpty() const
{
	return (space == 0);
}
