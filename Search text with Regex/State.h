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

//class, representing the states and the transition between them in NFA
class State
{
private:
	char transitSymbol;
	State* firstNextState;
	State* secondNextState;
	bool twoStates;
	bool finalState;
	int lastlist;

	void initialization();
	void destroy(State* state);
public:
	State();
	State(State*, State*, bool, bool);
	State(char, State*, State*, bool, bool);
	~State();

	char getTransitSymbol() const;
	State* getFirstNextState() const;
	State* getSecondNextState() const;
	bool hasTwoStates() const;
	bool isFinalState() const;
	void setLastlist(int index);
	int getLastlist() const;
	void connectToState(State* state);
};

void State::initialization()
{
	firstNextState = NULL;
	secondNextState = NULL;
	twoStates = false;
	finalState = true;
	lastlist = -1;
}

void State::destroy(State* state)
{
	if (!state) return;
	destroy(state->firstNextState);
	destroy(state->secondNextState);
	delete state;
	initialization();
}

State::State()
{
	initialization();
}

State::State(State* firstPossibleState, State* secondPossibleState, bool twoPossibleStates, bool finalState)
{
	this->firstNextState = firstPossibleState;
	this->secondNextState = secondPossibleState;
	this->twoStates = twoPossibleStates;
	this->finalState = finalState;
	this->lastlist = -1;
}

State::State(char transitSymbol, State* firstPossibleState, State* secondPossibleState, bool finalState, bool twoPossibleStates)
{
	this->transitSymbol = transitSymbol;
	this->firstNextState = firstPossibleState;
	this->secondNextState = secondPossibleState;
	this->twoStates = twoPossibleStates;
	this->finalState = finalState;
	this->lastlist = -1;
}

State::~State()
{
	destroy(firstNextState);
	destroy(secondNextState);
	initialization();
}

//returns the transition symbol between two states
char State::getTransitSymbol() const
{
	return transitSymbol;
}

//returns the first states that follows
State* State::getFirstNextState() const
{
	return firstNextState;
}

//returns the second state that follows
State* State::getSecondNextState() const
{
	return secondNextState;
}

//checks if there are two possibilities for next states
bool State::hasTwoStates() const
{
	if (twoStates) return true;
	return false;
}

//checks if this state is the end of the NFA
bool State::isFinalState() const
{
	if (finalState) return true;
	return false;
}

//sets the value of the state's lastlist to index
void State::setLastlist(int index)
{
	this->lastlist = index;
}

//returns the value of the state's lastlist
int State::getLastlist() const
{
	return lastlist;
}

//sets a state to the first next state,or if it's occupied - to the second next state
void State::connectToState(State* state)
{
	if (firstNextState == NULL)
	{
		firstNextState = state;
	}
	if (twoStates && secondNextState == NULL)
	{
		secondNextState = state;
	}
}