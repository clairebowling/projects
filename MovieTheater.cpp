
#include<iostream>
#include<string>
#include<cstdlib>
#include<iomanip>
using namespace std;


const int ROWS = 8;
const int COLS = 9;
// declare a string array for the format of the movie theater seating
typedef string MovieType[ROWS][COLS];


// Function Prototypes
int displayMovieInfo();
void initializeSeating(MovieType);
void displaySeats(MovieType);
int userTickets(int);
void checkEmptySeats(MovieType);
void selectSeat(MovieType, int);
void administratorMenu();
void administratorFunction(int);


// declare global variables
const float ADULT_MATINEE = 6.00, ADULT_EVENING = 8.00;
const float CHILD_MATINEE = 4.00, CHILD_EVENING = 6.00;

const string SEAT_TAKEN = "# ";
const string SEAT_AVAILABLE = "* ";
const string SEAT_BLOCKED = "X ";

int adultMatineeTickets = 0;
int adultEveningTickets = 0;
int childMatineeTickets = 0;
int childEveningTickets = 0;

float adultMatineeProft = 0.0;
float adultEveningProfit = 0.0;
float childMatineeProfit = 0.0;
float childEveningProfit = 0.0;


int main()
{
	int movieSelected;
	int numSeats = 0;

	MovieType FreeBirdsMatinee;		// array that displays the seating for Free Birds at 3:30
	MovieType FreeBirdEvening;		// array that displays the seating for Free Birds at 6:00 
	MovieType BeethovenMatinee; 	// array that displays the seating for Beethoven at 5:30
	MovieType BeethovenEvening;		// array that displays the seating for Beethoven at 7:30

	// initialize all seats to appear available
	initializeSeating(FreeBirdsMatinee);
	initializeSeating(FreeBirdEvening);
	initializeSeating(BeethovenMatinee);			
	initializeSeating(BeethovenEvening);


	while(movieSelected != -99)
	{
		movieSelected = displayMovieInfo();

		numSeats = userTickets(movieSelected);

		if(movieSelected == 1)	// use each array as an arguement specific to each movie showing
		{
			selectSeat(FreeBirdsMatinee, numSeats);
		}
		else if (movieSelected == 2)
		{
			selectSeat(FreeBirdEvening, numSeats);
		}
		else if (movieSelected == 3)
		{
			selectSeat(BeethovenMatinee, numSeats);
		}
		else if (movieSelected ==4)
		{
			selectSeat(BeethovenEvening, numSeats);
		}
	}
	return 0;
}


//****************************************************************************
int displayMovieInfo()
{
	int movieTimeSelection = 0;
	cout << setw(37) << "MOVIE SCHEDULE\n"; 
	cout << "Free Birds" << setw(23) << "3:30" << setw(21) << "6:00\n";
	cout << "Beethoven" << setw(24) << "5:30" << setw(22) << "7:30\n\n";

	cout << "Adult tickets are: " << setw(25) << "$6.00 (matinee before 6:00pm)" << setw(42) 
	<< "$8.00 (evening)\nChild tickets are: " << setw(25) << "$4.00 (matinee before 6:00pm)"
	<< setw(25) << "$6.00 (evening)\n\n\n";

	while(movieTimeSelection < 1 || movieTimeSelection > 4)
	{
		cout << "Please select a choice from the menu 1 through 4. \n"
		<< "1. Free Birds at 3:30\n" << "2. Free Birds at 6:00\n"
		<< "3. Beethoven at 5:30\n" << "4. Beethoven at 7:30\n";

		cin >> movieTimeSelection;	// user enter their choice of movie

		if(movieTimeSelection == -99)		// Administator code...
			{
				cout << "You have entered the administator portal" << endl;
				administratorMenu();
			}
	}

return movieTimeSelection;
}


//****************************************************************************
void initializeSeating(MovieType array)
{
	// label seat numbers
	array[0][0] = "seats ";
	array[0][1] = "1 ";
	array[0][2] = "2 ";
	array[0][3] = "3 ";
	array[0][4] = "4 ";
	array[0][5] = "5 ";
	array[0][6] = "6 ";
	array[0][7] = "7 ";
	array[0][8] = "8 ";

	// Label Rows
	array[1][0] = "Row 1 "; 
	array[2][0] = "Row 2 ";
	array[3][0] = "Row 3 ";
	array[4][0] = "Row 4 ";
	array[5][0] = "Row 5 ";
	array[6][0] = "Row 6 ";
	array[7][0] = "Row 7 ";

	// Make every seat display available
	for (int row = 1; row < ROWS; row ++)
	{
		for(int col = 1; col < COLS; col++)
			array[row][col] = "* ";
	}
}


//****************************************************************************
void displaySeats(MovieType array)
{
	for (int row = 0; row < ROWS; row ++)
	{
		for(int col = 0; col < COLS; col++)
			cout << array[row][col];		// Display seating
		cout << endl;
	}
}


//****************************************************************************
int userTickets(int movie)
{
	float ticketSale;
	int adultTickets;
	int childTickets;
	float adultTicketSale;
	float childTicketSale;
	int numTickets;

	cout << "How many adult tickets would you like to purchase?\n";
	cin >> adultTickets;	// user input adult tickets

	cout << "\nHow many child tickets would you like to purchase?\n";
	cin >> childTickets;	// user input child tickets

	numTickets = adultTickets + childTickets;

	if(numTickets > 56)	// input validation for theater size
	{
		cout << "There are not enough seats in the theater for your purchase. Please enter a number of tickets less than or" 
		" equal to 56\n";

		cout << "How many adult tickets would you like to purchase?\n";
		cin >> adultTickets;

		cout << "\nHow many child tickets would you like to purchase?\n";
		cin >> childTickets;		
	}

	if(movie == 1 || movie == 3)	// matinee pricing
	{
		adultTicketSale = adultTickets * ADULT_MATINEE;
		childTicketSale = childTickets * CHILD_MATINEE;

		ticketSale = adultTicketSale + childTicketSale;

		// Add purchase to global variable to be used by administrator
		adultMatineeTickets += adultTickets;
		childMatineeTickets += childTickets;

		adultMatineeProft += adultTicketSale;
		childMatineeProfit += childTicketSale;
	}
	else if (movie == 2 || movie == 4)		// evening pricing
	{
		adultTicketSale = adultTickets * ADULT_EVENING;
		childTicketSale = childTickets * CHILD_EVENING;

		ticketSale = adultTicketSale + childTicketSale;

		// Add purchase to global variable to be used by administrator
		adultEveningTickets += adultTickets;	
		childEveningTickets += childTickets;

		adultEveningProfit += adultTicketSale;
		childEveningProfit += childTicketSale;
	}

	cout << "The total for " << numTickets << " tickets is $" << setprecision(2) << fixed << showpoint
	<< ticketSale << endl << endl;		// Display the ticket purchase total

	return numTickets;
}


//****************************************************************************
void checkEmptySeats(MovieType array)
{
	int seatsAvailable = 0;
	for(int row = 1; row < ROWS; row++)
	{
		for(int col = 1; col < COLS; col++)
		{
			if (array[row][col] == SEAT_AVAILABLE)
				seatsAvailable++;
		}
	}
	if (seatsAvailable == 0)
	{
		cout << "Sorry. This showing is sold out. Please select a different option from the menu.\n";
		displayMovieInfo();
	}
}


//****************************************************************************
void selectSeat(MovieType array, int numTickets)
{
	int row;
	int seatNum;
	string holdSeat = "! ";
	bool validSeat = false;

	displaySeats(array);

		for (int count = 0; count < numTickets; count++)
		{
			do
			{
				cout << "Please enter the row and seat number of the seat you would like to reserve.\n";
				cin >> row >> seatNum;	// User enter row and seat number that they would like to reserve
				
				while(row > 7 || seatNum > 8 || row < 1 || seatNum < 1)
				{
					cout << "Error: Please enter a valid row and seat number.\n";
					cin >> row >> seatNum;
				}

				if(array[row][seatNum] == SEAT_TAKEN  || array[row][seatNum] == SEAT_BLOCKED)
				{
					cout << "Error: That space is already taken or invalid. Please try again.\n";
					validSeat = false;
				}
				else		// if space is invlaid or already taken
				{
					array[row][seatNum] = SEAT_TAKEN;	// Mark array with player symbol
					validSeat = true;

					// Temporarily block seat so that it can still be selected during this purchase
					if(array[row -1 ][seatNum] != SEAT_TAKEN && array[row -1 ][seatNum] == SEAT_AVAILABLE)
						array[row - 1][seatNum] = holdSeat;
					if(array[row + 1][seatNum] != SEAT_TAKEN && array[row + 1][seatNum] == SEAT_AVAILABLE)
						array[row + 1][seatNum] = holdSeat;				
					if(array[row][seatNum - 1] != SEAT_TAKEN && array[row][seatNum - 1] == SEAT_AVAILABLE)
						array[row][seatNum - 1] = holdSeat;				
					if(array[row][seatNum + 1] != SEAT_TAKEN && array[row][seatNum + 1] == SEAT_AVAILABLE)
						array[row][seatNum + 1] = holdSeat;
				}
			}while (validSeat == false);
		}

	// Permanently block any seat that was temporarily blocked off so that no other patrons may purchase it
	for (int horizontalPos = 1; horizontalPos < ROWS; horizontalPos++)
	{
		for(int verticalPos = 1; verticalPos < COLS; verticalPos++)
		{
			if (array[horizontalPos][verticalPos] == holdSeat && array[horizontalPos][verticalPos]!= SEAT_TAKEN)
				array[horizontalPos][verticalPos] = SEAT_BLOCKED;
		}

	}
	displaySeats(array);	// Show updated seating
}


//****************************************************************************
void administratorMenu()
{
	int choice;
	do
	{
	cout << "Option 1: Print number of tickets sold\nOption 2: Print profits\n"
	"Option 3: Back to select movie menu\nOption 4: Shut down theater for the day\n";		 // Display administrator options
	cout << "Hello administrator, please select an option from the menu by entering a number 1 through 4.\n";
	cin >> choice;		// Allow administator to choose from the menu
	} while(choice < 1 || choice > 4);		// Input validation

	administratorFunction(choice);
}


//****************************************************************************
void administratorFunction(int menuChoice)
{
	int option;
	float totalProfit;
	int totalTickets;

	switch(menuChoice)
	{
		case 1:
		do
		{
		cout << "Please select an option by entering 1 or 2\n1: Print number of tickets sold by type\n"
		"2: Print the total number of tickets sold\n";
		cin >> option;	// administrator input
		}while (option < 1 || option > 2);

		if(option == 1)
		{
			// TYPE
			cout << "The total number of adult matinee tickets sold is " << adultMatineeTickets << endl;
			cout << "The total number of child matinee tickets sold is " << childMatineeTickets << endl;
			cout << "The total number of adult evening tickets sold is " << adultEveningTickets << endl;
			cout << "The total number of child evening tickets sold is " << childEveningTickets << endl << endl << endl;
		}
		else if(option == 2)
		{
			// TOTAl
			totalTickets = adultMatineeTickets + childMatineeTickets + adultEveningTickets + childEveningTickets;
			cout << "The total number of tickets sold is " << totalTickets << endl << endl << endl;
		}
		break;


		case 2:
		do
		{
		cout << "Please select an option by entering 1 or 2\n1: Print the profit of tickets sold by type\n"
		"2: Print the total profit\n";
		cin >> option;
		}while (option < 1 || option > 2);

		if(option == 1)
		{
			// TYPE
			cout << "The total profit from adult matinee tickets is $" << adultMatineeProft << endl;
			cout << "The total profit from child matinee tickets is $" << childMatineeProfit << endl;
			cout << "The total profit from adult evening tickets is $" << adultEveningProfit << endl;
			cout << "The total profit from child evening tickets is $" << childEveningProfit << endl << endl << endl;
		}
		if(option == 2)
		{
			// TOTAL
			totalProfit = adultMatineeProft + childMatineeProfit + adultEveningProfit + childEveningProfit;
			cout << "The total profit is $" << totalProfit << endl << endl << endl;	
		}
		break;


		case 3:
		displayMovieInfo();

		case 4: 
		cout << "Shutting down theater. There are no more movies available to watch.\n";
		exit(0);		// Shut down theater for the day

	}
}

