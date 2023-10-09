// This program allows two users to play tic-tac-toe

// Virginia Bowling
// 00096744
// vbowlin2@my.athens.edu

// Assighment 10 Problem 2
// Due: April 19, 2021

#include<iostream>
using namespace std;


// Global variables
const int ROWS = 3, COLS = 3;

typedef char TicTacToe[ROWS][COLS];

// function prototypes
void displayBoard(TicTacToe);
void playerTurn(TicTacToe, char);
bool gameOver(int, TicTacToe, char);
bool testPlayerWin(TicTacToe, char);
bool canPlayerWin(bool);
void displayWinner(char);


int main()
{
	bool endGame = false;
	int moveCount = 0;
	char player1 = 'X';
	char player2 = 'O';
	bool canPlayer1Win = false;
	bool canPlayer2Win = false;

	TicTacToe gameBoard = {{'*', '*', '*'},		//Initialize game board
							{'*', '*', '*'},
							{'*', '*', '*'}};

	while (endGame != true)		// Keep playing til game over
	{

		displayBoard(gameBoard);
		cout << "Player X: ";		// Prompt player 1 turn
		playerTurn(gameBoard, player1);
		canPlayer1Win = canPlayerWin(endGame);		// can player win?
		moveCount++;		// increment move counter
		endGame = gameOver(moveCount, gameBoard, player1);	// test for winner and see if game over

		if(endGame == true)
			return 0;

		displayBoard(gameBoard);
		cout << "Player O: ";		// Prompt player 2 turn
		playerTurn(gameBoard, player2);
		canPlayer2Win = canPlayerWin(endGame);		// Can player win? 
		moveCount++;		// Increment move counter
		endGame = gameOver(moveCount, gameBoard, player2);		// test for winner and see if game over

		if(endGame == true)
			return 0;

	}

}


//****************************************************************************
void displayBoard(TicTacToe array)
{
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			cout << array[row][col];
		}
		cout << endl;
	}
}

//****************************************************************************
void playerTurn(TicTacToe array, char symbol)
{
	int row;
	int col;
	bool validMove = false;

	do
	{
		cout << "Please enter the row and column of the space you would like to put an " << symbol << endl;
		cin >> row >> col;		// User choose playing space

		if(array[row - 1][col - 1] == '*')		// if open space
		{
			array[row - 1][col -1] = symbol;	// Mark array with player symbol
			validMove = true;
		}
		else		// if space is invlaid or already taken
		{
			cout << "That space is already taken or invalid. Please try again.\n";
			validMove = false;
		}

	}while (validMove == false);		// input validation until player makes a valid move
}



//****************************************************************************
bool gameOver(int moves, TicTacToe array, char symbol)
{
	bool gameOver = false;
	bool winner = false;
	int maxMoves = 9;
	// Check to see if any open spaces
	if (moves == maxMoves)		// If there are no more moves available and game is not over...
	{
		gameOver = true;
		cout << "The game is a tie.\n";	
	}
	winner = testPlayerWin(array, symbol);		// test to see if the player won
	if (winner == true)
		gameOver = true;
	if (gameOver == true)
		displayBoard(array);
	return gameOver;		// is game over?
}



//****************************************************************************
bool testPlayerWin(TicTacToe array, char symbol)

{
	int symbolCount = 0;
	bool win = false;

	// Test across
	for (int row = 0; row < ROWS; row++)
		{
			for (int col = 0; col < COLS; col++)
			{
				if (array[row][col]==symbol)
				{
					symbolCount++;
				}
			}
				
				if(symbolCount == 3)
					win = true;
				else 
				{
					win = false;
					symbolCount = 0;
				}	
		}

	// Test Down
	symbolCount = 0;
	if (win != true)
	{
	for (int col = 0; col < COLS; col++)
		{

			for (int row = 0; row < ROWS; row++)
			{
				if (array[row][col]==symbol)
				{
					symbolCount++;
				}
			}

				if(symbolCount == 3)
					win = true;
				else
				{ 
					win = false;
					symbolCount = 0;
				}
		}
	}	

	// Test diagonal from L to R
	symbolCount = 0;
	if (win != true)
	{
	for (int row_and_Col = 0; row_and_Col < ROWS; row_and_Col++)
		{
			if (array[row_and_Col][row_and_Col] == symbol)
			{
				symbolCount++;
			}
		}
			if(symbolCount == 3)
				win = true;
			else 
				win = false;
	}

	// Test diagonal from R to L
	symbolCount = 0;
	if (win != true)
	{
	for (int row = 2, col = 0; row < ROWS && col < COLS; row--, col++)
		{
			if (array[row][col] == symbol)
			{
				symbolCount++;
			}
		}
			if(symbolCount == 3)
				win = true;
			else 
				win = false;
	}


		if (win == true)
			displayWinner(symbol);

	return win;
}



//****************************************************************************
bool canPlayerWin(bool gameOver)
{
	bool playerCANwin;
	if(gameOver == true)
		playerCANwin = false;
	else if (gameOver != true)
		playerCANwin = true;
	return playerCANwin;
}



//****************************************************************************
void displayWinner(char winner)
{
 cout << "The winner is Player " << winner << endl;		// display the winner to screen 
}
