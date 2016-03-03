#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<sstream>
#include<stack>
#include<limits.h>
#include<algorithm>
#include<math.h>

using namespace std;

struct Node
{
	int nodeNumber;
	int cost;
	int depth;
	int turnForPlayer;
	int moveAt;
	char board[9];
	vector<int> childNodes;
	int parentNode;
	Node()
	{
		cost = INT_MIN;
		parentNode = -1;
		nodeNumber = -1;
		depth = -1;
	}
};

//Method declarations
//void ClearBoard(int currentNode);
//Global variable declarations
//int player;
void PrintBoard(Node node);
bool IsBoardFull(Node node);
vector<Node> nodes;
int MINIMAX_DECISION(int currentNode);
int MAX_VALUE(int currentNode);
int MIN_VALUE(int currentNode);
void ComputeMoves(int currentNode);
int WhoWon(Node node);

int main(int argc, char *argv[])
{
	Node start;
	int spot, whowon;
	//first not required, just for optimization
	bool firstMove = true;

	start.depth = 0;
	start.nodeNumber = 0;
	start.turnForPlayer = 2;

	//set the initial board
	for (int i = 0; i < 9; i++)
		start.board[i] = ' ';

	//012
	//345
	//678
	/*start.board[0] = 'O';
	start.board[2] = 'X';
	start.board[4] = 'X';
	start.board[7] = 'O';
	*/

	PrintBoard(start);

	//loop through moves
	//human is always 0
	while (!IsBoardFull(start))
	{
		cout << "\n Move (1-9): ";
		cin >> spot;

		//check invalid moves
		if (spot < 1 || spot > 9)
		{
			cout << "\n Move can be from 1-9";
			continue;
		}

		if (start.board[spot - 1] != ' ')
		{
			cout << "\n Already filled, Please choose another move.";
			continue;
		}

		start.board[spot - 1] = 'O';

		//check if anyone won, if yes then show and exit
		whowon = WhoWon(start);
		if (whowon != 0)
			break;


		if (firstMove)
		{
			if (start.board[4] == ' ')
				start.board[4] = 'X';
			else
				start.board[0] = 'X';
			firstMove = false;
		}
		else
		{
			//run minimax here and mark the board with x before printing board
			//clears everything before running again
			nodes.clear();
			nodes.push_back(start);
			spot = MINIMAX_DECISION(0);
			start.board[spot - 1] = 'X';
		
			//check if anyone won, if yes then show and exit
			whowon = WhoWon(start);
			if (whowon != 0)
				break;
		}
		
		PrintBoard(start);
	}
	PrintBoard(start);
	if (whowon == 1)
		cout << "\nYou Win!!";
	else if (whowon == 2)
		cout << "\nComputer Wins!!";
	else
		cout << "\n DRAW";

	cout << "\n\nEnter 0 to exit!";

	cin >> whowon;
}

void PrintBoard(Node node)
{
	cout << " " << node.board[0] << " | " << node.board[1] << " | " << node.board[2];
	cout << "\n--------------\n";
	cout << " " << node.board[3] << " | " << node.board[4] << " | " << node.board[5];
	cout << "\n--------------\n";
	cout << " " << node.board[6] << " | " << node.board[7] << " | " << node.board[8];
	cout << "\n--------------\n";
}

//check if board is full
bool IsBoardFull(Node node)
{
	for (int i = 0; i < 9; i++)
	{
		if (node.board[i] == ' ')
			return false;
	}
	return true;
}

//Main wrapper function for deciding where to mark next
int MINIMAX_DECISION(int currentNode)
{
	int returncost;
	//get the max cost
	returncost = MAX_VALUE(currentNode);
	//loop thorugh values in all child nodes of parent and make a move with the first node having max cost
	for (int i = 0; i < nodes[currentNode].childNodes.size(); i++)
	{
		if (nodes[nodes[currentNode].childNodes[i]].cost == returncost)
		{
			return nodes[nodes[currentNode].childNodes[i]].moveAt;
		}
	}
}


//max function
int MAX_VALUE(int currentNode)
{
	int value, calculatedCost;
	if (IsBoardFull(nodes[currentNode]) || WhoWon(nodes[currentNode]) != 0)
	{
		return nodes[currentNode].cost;
	}

	value = INT_MIN;
	ComputeMoves(currentNode);

	for (int j = 0; j < nodes[currentNode].childNodes.size(); j++)
	{
		value = max(value, MIN_VALUE(nodes[currentNode].childNodes[j]));
	}
	nodes[currentNode].cost = value;
	return value;
}

int MIN_VALUE(int currentNode)
{
	int value, calculatedCost;
	if (IsBoardFull(nodes[currentNode]) || WhoWon(nodes[currentNode]) != 0)
	{
		return nodes[currentNode].cost;
	}

	value = INT_MAX;
	ComputeMoves(currentNode);

	for (int j = 0; j < nodes[currentNode].childNodes.size(); j++)
	{
		value = min(value, MAX_VALUE(nodes[currentNode].childNodes[j]));
	}

	nodes[currentNode].cost = value;
	return value;
}

//Get all valid moves for current board state
void ComputeMoves(int currentNode)
{
	int player = nodes[currentNode].turnForPlayer;
	int whowon;

	for (int i = 0; i < 9; i++)
	{
		if (nodes[currentNode].board[i] == ' ')
		{
			Node newNode;
			newNode.turnForPlayer = player == 1 ? 2 : 1;
			newNode.depth = nodes[currentNode].depth + 1;
			newNode.nodeNumber = nodes.size();
			newNode.parentNode = currentNode;
			newNode.moveAt = i + 1;

			for (int j = 0; j < 9; j++)
			{
				newNode.board[j] = nodes[currentNode].board[j];
			}
			if (player == 1)
				newNode.board[i] = 'O';
			else
				newNode.board[i] = 'X';

			whowon = WhoWon(newNode);
			if (whowon == 1)
				newNode.cost = -10;
			else if (whowon == 2)
				newNode.cost = 10 - newNode.depth;
			else
				newNode.cost = 0;
			nodes[currentNode].childNodes.push_back(nodes.size());
			nodes.push_back(newNode);
		}
	}
}

int WhoWon(Node node)
{
	//0 - 012: 1- 345: 2- 678
	//check rows
	for (int i = 0; i < 3; i++)
	{
		if (node.board[i * 3] == node.board[i * 3 + 1] && node.board[i * 3 + 1] == node.board[i * 3 + 2])
		{
			if (node.board[i * 3] == 'X')
				return 2;
			else if (node.board[i * 3] == 'O')
				return 1;
		}
	}

	//check columns
	for (int i = 0; i < 3; i++)
	{
		if (node.board[i] == node.board[i + 3] && node.board[i + 3] == node.board[i + 6])
		{
			if (node.board[i] == 'X')
				return 2;
			else if (node.board[i] == 'O')
				return 1;
		}
	}

	//check diagonals
	if (node.board[0] == node.board[4] && node.board[4] == node.board[8])
	{
		if (node.board[0] == 'X')
			return 2;
		else if (node.board[0] == 'O')
			return 1;
	}
	if (node.board[2] == node.board[4] && node.board[4] == node.board[6])
	{
		if (node.board[2] == 'X')
			return 2;
		else if (node.board[2] == 'O')
			return 1;
	}

	return 0;
}
