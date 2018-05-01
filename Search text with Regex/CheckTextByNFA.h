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
#include<iostream>

//class, responsible for checking if the line is recognised by the NFA
class CheckTextByNFA
{
private:
	bool match;

	void addNewState(DynamicArray<State>&, State*, size_t);
	size_t step(DynamicArray<State>, char, DynamicArray<State>&, size_t);
	bool isTheSame(DynamicArray<State>);
public:
	CheckTextByNFA();
	bool checkLine(State*, std::string);
};

//adds a new state to the list of states
void CheckTextByNFA::addNewState(DynamicArray<State>& currentStates, State* state, size_t countUniqueStates)
{
	//if there is no such state or it already exist, the function is stopped
	if (state == NULL || state->getLastlist() == countUniqueStates)
	{
		return;
	}
	//otherwise the value of the state's lastlist is set to the counter of unique states
	state->setLastlist(countUniqueStates);
	//checks if the state can have two next states and adds them to the list of current states recursively
	if (state->hasTwoStates())
	{
		addNewState(currentStates, state->getFirstNextState(), countUniqueStates);
		addNewState(currentStates, state->getSecondNextState(), countUniqueStates);
		return;
	}
	//if it has only one next state, it is appended in the array
	currentStates.append(*state);

}

//helps move on to the next symbol in the NFA
size_t CheckTextByNFA::step(DynamicArray<State> currentStates, char symbol, DynamicArray<State>& nextStates, size_t countUniqueStates)
{
	//increments the counter with one, for the next unique state
	countUniqueStates++;
	//checkes every state in the current states
	for (size_t i = 0; i < currentStates.getSize(); i++)
	{
		State state = currentStates.getElement(i);
		//if the given symbol is the same, then it is added to the array of states - next states
		if (symbol == state.getTransitSymbol())
		{
			//a special ocassion - if the symbol is the empty word, then a bool variable is set to true
			if (symbol == '\0') match = true;
			addNewState(nextStates, state.getFirstNextState(), countUniqueStates);
		}
	}
	return countUniqueStates;
}

//checks in the end if the text responds to the NFA
bool CheckTextByNFA::isTheSame(DynamicArray<State> finalStates)
{
	//checks every state in the final states
	for (size_t i = 0; i < finalStates.getSize(); i++)
	{
		State state = finalStates.getElement(i);
		//if the state is the last one, then the line is recognised by the NFA
		if (state.isFinalState()) return true;
	}
	//checks if the array is empty and there is a match with the empty word
	if (finalStates.getSize() == 0 && match)
	{
		//if the first state is the final, so it is recongnised
		State state = finalStates.getElement(0);
		if (state.isFinalState()) return true;
	}
	//otherewise it isn't
	return false;
}

CheckTextByNFA::CheckTextByNFA()
{
	match = false;
}

//checks the line with the NFA by beginning from the initial state
bool CheckTextByNFA::checkLine(State* initialState, std::string line)
{
	//creates two arrays of states, the first one will contain the current states and the second one will create the next states, one step ahead
	DynamicArray<State> currentStates = DynamicArray<State>();
	DynamicArray<State>	nextStates = DynamicArray<State>();
	//creates a counter for the unique states
	size_t countUniqueStates = 0;
	//the initial state of the NFA is added to the list of current states
	addNewState(currentStates, initialState, countUniqueStates);
	//a loop for every symbol in the line
	for (size_t i = 0; i < line.size(); i++)
	{
		//updates the value of the counter and adds the following states to the list of next states if the symbol is the same
		countUniqueStates = step(currentStates, line[i], nextStates, countUniqueStates);
		//NFA moves forward by assigning the list of current states with the list of next states
		currentStates = nextStates;
		nextStates = DynamicArray<State>();
	}
	//if the line is the empty word
	if (line.size() == 0)
	{
		//updates the value of the counter and adds the following states to the list of next states if the symbol is '\0'
		countUniqueStates = step(currentStates, line[0], nextStates, countUniqueStates);
		//NFA moves forward by assigning the list of current states with the list of next states
		currentStates = nextStates;
		nextStates = DynamicArray<State>();
	}
	//checks if the NFA has reached its final state i.e. it has recognised the line
	return isTheSame(currentStates);
}