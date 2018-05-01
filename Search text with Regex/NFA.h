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
#include"Stack.h"
#include"DynamicArray.h"
#include"State.h"
#include"Fragment.h"
#include<iostream>

//class, representng the creation of NFA
class NFA
{
private:
	DynamicArray<State*>* combineTwoStates(DynamicArray<State*>*, DynamicArray<State*>*);
	DynamicArray<State*>* listOfOneState(State*);
	void connectFragmentToState(Fragment*, State*);
public:
	NFA();

	State* postfixToNFA(std::string);
};

//forms a new state from two others by appending the elements from the first one and then from the second one to the new one
DynamicArray<State*>* NFA::combineTwoStates(DynamicArray<State*>* firstState, DynamicArray<State*>* secondState)
{
	DynamicArray<State*>* newState = new DynamicArray<State*>();
	for (size_t i = 0; i < firstState->getSize(); i++)
	{
		newState->append(firstState->getElement(i));
	}
	for (size_t i = 0; i < secondState->getSize(); i++)
	{
		newState->append(secondState->getElement(i));
	}
	return newState;
}

//creates an array, containing just one state
DynamicArray<State*>* NFA::listOfOneState(State* state)
{
	DynamicArray<State*>* listOfStates = new DynamicArray<State*>();
	listOfStates->append(state);
	return listOfStates;
}

//connects a fragment to a state by arrawing all of the states in the list of states from the fragment to the state
void NFA::connectFragmentToState(Fragment* fragment, State* state)
{
	DynamicArray<State*>* connectingStates = fragment->getListOfStates();
	for (size_t i = 0; i < connectingStates->getSize(); i++)
	{
		State* tempState = connectingStates->getElement(i);
		tempState->connectToState(state);
	}
}

NFA::NFA()
{
}

//creating an NFA from a regex that is already converted to postfix
State* NFA::postfixToNFA(std::string postfixExpression)
{
	//creates a stack, that will contain fragments
	Stack<Fragment*> fragmentsContainer;
	Fragment* NFA = new Fragment();
	State* finalState = new State();
	//a loop on all of the symbols in the expression
	for (size_t i = 0; i < postfixExpression.size(); i++)
	{
		//checks if the symbol is one of the following
		switch (postfixExpression[i])
		{
			//if it is an operation union,checks for operands
		case '|':
		{
			if (fragmentsContainer.isEmpty()) std::cerr << "Incorrect regular expression!";
			//because the elements in the stack are in a reversed order it pops the second and than the first one, if it exists
			Fragment *firstFragment, *secondFragment;
			fragmentsContainer.pop(secondFragment);
			if (fragmentsContainer.isEmpty()) std::cerr << "Incorrect regular expression!";
			fragmentsContainer.pop(firstFragment);
			//creates a new state that will have two next states, the initial one from the first and from the second fragment
			State* newState = new State(firstFragment->getInitialState(), secondFragment->getInitialState(), true, false);
			//combines the list of states from the two fragments into one
			DynamicArray<State*>* combination = combineTwoStates(firstFragment->getListOfStates(), secondFragment->getListOfStates());
			//pushes a new fragment, having the new initial state and the new list of states, into the stack
			fragmentsContainer.push(new Fragment(newState, combination));
		}break;
		//if it is an operation is concatenation, checks for operands
		case '.':
		{
			if (fragmentsContainer.isEmpty()) std::cerr << "Incorrect regular expression!";
			//because the elements in the stack are in a reversed order it pops the second and than the first one, if it exists
			Fragment* firstFragment, *secondFragment;
			fragmentsContainer.pop(secondFragment);
			if (fragmentsContainer.isEmpty()) std::cerr << "Incorrect regular expression!";
			fragmentsContainer.pop(firstFragment);
			//connects the first fragment to the initial state of the second one
			connectFragmentToState(firstFragment, secondFragment->getInitialState());
			//pushes a new fragment, containing the initial state of the first one and the list of states of the second one, into the stack 
			fragmentsContainer.push(new Fragment(firstFragment->getInitialState(), secondFragment->getListOfStates()));
		}break;
		//if it is an operation kleene star, checks for an operand
		case '*':
		{
			if (fragmentsContainer.isEmpty()) std::cerr << "Incorrect regular expression!";
			//if there is one, it pops it from the stack
			Fragment* topFragment;
			fragmentsContainer.pop(topFragment);
			//creates a new state containing the initial state of the popped fragment and no next states
			State* newState = new State(topFragment->getInitialState(), NULL, true, false);
			//connects the popped fragment to the new state
			connectFragmentToState(topFragment, newState);
			//pushes a new fragment, having the new initial state and a list of just one state (the new one), into the stack
			fragmentsContainer.push(new Fragment(newState, listOfOneState(newState)));
		}break;
		//if the symbol it is not an operation, than it is a literal
		default:
		{
			//creates a new state, having the symbol for the inital state and no next states
			State* literalState = new State(postfixExpression[i], NULL, NULL, false, false);
			//creates a new fragment, having the new state and a list of just one state (the new one)
			Fragment* literalStateFragment = new Fragment(literalState, listOfOneState(literalState));
			//pushes the new fragment into the stack
			fragmentsContainer.push(literalStateFragment);
		}break;
		}
	}
	//the lst thing left in the stack is the formed NFA
	fragmentsContainer.pop(NFA);
	//the stack is no longer necessary, so it's destroyed
	fragmentsContainer.destroy();
	//all of the states of the NFA are connected to a one, final state
	connectFragmentToState(NFA, finalState);
	//returns the initial state of the created NFA
	return NFA->getInitialState();
}