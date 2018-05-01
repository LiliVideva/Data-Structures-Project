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
#include"DynamicArray.h"
#include"State.h"

//class, representing a list of states and assign one as an initial in NFA
class Fragment
{
private:
	State* initialState;
	DynamicArray<State*>* listOfStates;

	void initialization();
public:
	Fragment();
	Fragment(State* initialState, DynamicArray<State*>* listOfStates);
	~Fragment();

	State* getInitialState() const;
	DynamicArray<State*>* getListOfStates() const;
};

void Fragment::initialization()
{
	initialState = NULL;
	listOfStates = NULL;
}

Fragment::Fragment()
{
	initialization();
}

Fragment::Fragment(State* initialState, DynamicArray<State*>* listOfStates)
{
	this->initialState = initialState;
	this->listOfStates = listOfStates;
}

Fragment::~Fragment()
{
	delete initialState;
	listOfStates->~DynamicArray();
	initialization();
}

//returns the initial state
State* Fragment::getInitialState() const
{
	return initialState;
}

//returns the list of states that a fragment contains
DynamicArray<State*>* Fragment::getListOfStates() const
{
	return listOfStates;
}