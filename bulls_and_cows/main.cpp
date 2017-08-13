#include "bullsandcows.h"

#include<iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#include<stdexcept>
using std::out_of_range;
using std::runtime_error;

int main()
try
{
	// display main menu
	display_glbl_menu();

	string choice {""};

	while (true) {
		getline(cin, choice);

		if (choice.size() > 1) {
			cout << "Please enter a valid choice." << endl;
			continue;
		}

		switch (choice[0])
		{
		case '1':
			bullsandcowsc_offline();
			break;

		case '2':
			cout << endl;
			bullsandcowsc_network();
			break;

		case 'x':
			break;

		default:
			cout << "\nPlease enter a valid choice." << endl;
			break;
		}

		if (choice[0] == 'x')
			break;

		cout << endl;
		display_glbl_menu();
	}

	cout << "\nGoodbye." << endl;
	return 0;
}
catch (out_of_range& e) {
	cerr << "Error: " << e.what() << endl;
	return 1;
}
catch (runtime_error&e) {
	cerr << "Error: " << e.what() << endl;
	return 2;
}
catch (...) {
	cerr << "Error: unknown exception caught !" << endl;
	return 3;
}