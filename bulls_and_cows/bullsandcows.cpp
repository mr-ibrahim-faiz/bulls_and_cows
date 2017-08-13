#include "bullsandcows.h"

#include<iostream>
using std::cin;
using std::cerr;
using std::cout;
using std::endl;
using std::streamsize;

#include<stdexcept>
using std::runtime_error;
using std::out_of_range;

#include<limits>
using std::numeric_limits;

#include<ctime>
using std::time;

#include<algorithm>
using std::sort;
using std::adjacent_find;
using std::random_shuffle;

#include<string>
using std::string;

// Bulls_and_cows functions

// converts char to size_t
size_t char_to_size_t(const char& c)
// converts a char digit value into an integral digit value
// c must be the representation of a digit ranging from 0 to 9
// returs the converted digit as a size_t digit value
// throws an out_of_range exception if c is not a digit
{
	switch (c)
	{
	case '0':
		return 0;
		break;
	case '1':
		return 1;
		break;
	case '2':
		return 2;
		break;
	case '3':
		return 3;
		break;
	case '4':
		return 4;
		break;
	case '5':
		return 5;
		break;
	case '6':
		return 6;
		break;
	case '7':
		return 7;
		break;
	case '8':
		return 8;
		break;
	case '9':
		return 9;
		break;
	default:
		throw out_of_range("char_to_size_t character out of range.");
		break;
	}
}

// checks if a vetcor has unique elements
template<typename T>
bool has_unique_elements(T t)
// sort and then search for the first occurence of two matching consecutives elements
// if it finds one the template returns false
// use algorithm's sort and adjacent_find
{
	sort(t.begin(), t.end());
	T::iterator it = adjacent_find(t.begin(), t.end());
	if (it != t.end()) {
		return false;
	}
	return true;
}

// gets a random vector of size_t with n unique elements
vector<size_t> get_random_vector(const size_t& n)
// randomly initializes a vector of 10 unique digits and resizes it
// n must be less than or equal to 10
// returns the resized vector of size_t with n unique elements
// throws out_of_range exception if n is out of bounds
{
	if (n > 10)
		throw out_of_range("get_random_vector parameter out of range.");

	vector<size_t> number = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	srand((unsigned int)time(NULL));
	random_shuffle(number.begin(), number.end());

	number.resize(n);

	return number;
}

// counts bulls and cows
void count_bulls_and_cows(const vector<size_t>& secret_number,
	const vector<size_t>& guess_number, size_t& bulls_count, size_t& cows_count)
	// compares two vectors and counts bulls and cows
	// secret_number and guess_number shouldn't be empty and their sizes must be equal
	// write its results in variables bulls_count and cows_count
	// throws a runtime_error exception if one of the input vectors is empty
	// throws a runtime_error exception if the input vectors' sizes aren'te equal
{
	if (secret_number.size() == 0 || guess_number.size() == 0)
		throw runtime_error("count_bulls_and cows invalid input. Empty vector.");
	else if (secret_number.size() != guess_number.size())
		throw runtime_error("count_bulls_and cows invalid inputs. Size mismatch.");

	bulls_count = cows_count = 0;

	for (size_t i = 0; i < secret_number.size(); ++i)
	{
		for (size_t j = 0; j < guess_number.size(); ++j)
		{
			if (secret_number[i] == guess_number[j]) {
				if (i == j)
					++bulls_count;
				else
					++cows_count;
			}
			else {}
		}
	}
}

// display main menu
void display_glbl_menu()
{
	cout << "[1] Offline" << endl;
	cout << "[2] Network" << endl;
	cout << "[x] Exit" << endl;
}

// display network menu
void display_ntwk_menu()
{
	cout << "[1] Start Game" << endl;
	cout << "[2] Join Game" << endl;
	cout << "[x] Menu" << endl;
}

// ask player to enter a number of digits to guess
void asks_number_of_digits()
{
	cout << "\nPlease enter a number of digits to guess: " << endl;
}

// gets number of digits to guess
size_t gets_number_of_digits()
// prompts the player to provide a number of digits to guess
// returns a number of digits
{
	size_t number_of_digits {0};
	cin >> number_of_digits;

	while (!cin) {
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		cout << "Please enter a valid number." << endl;
		cin >> number_of_digits;
	}

	cin.ignore((numeric_limits<streamsize>::max)(), '\n');

	return number_of_digits;
}


// Data transmission functions

// exception class used to report data transmission errors
class data_transmission_error : public runtime_error
{
	using runtime_error::runtime_error;
};

// sends size_t number
void send_size_t_number(const SOCKET& socket, const size_t& number)
// the socket must be valid
// throws an data_transmission_error exception if socket is invalid
// throws an data_transmission_error exception if send failed
{
	if (socket == INVALID_SOCKET)
		throw data_transmission_error("send_size_t_number. Invalid socket.");

	int error_result = send(socket, (char*)&number, sizeof(size_t), 0);
	if (error_result == SOCKET_ERROR)
		throw data_transmission_error("send_size_t_number. Unable to send data.");
}

// recieves size_t number
size_t recieve_size_t_number(const SOCKET& socket)
// the socket must be valid
// returns the number recieved
// throws a data_transmission_error exception if socket is invalid
// throws a data_transmission_error exception if recv failed
{
	if (socket == INVALID_SOCKET)
		throw data_transmission_error("recieve_size_t_number. Invalid socket.");

	size_t number;

	int error_result = recv(socket, (char*)&number, sizeof(size_t), MSG_WAITALL);
	if (error_result == SOCKET_ERROR)
		throw data_transmission_error("recieve_size_t_number. Unable to recieve data.");

	return number;
}

// sends vector of size_t
void send_size_t_vector(const SOCKET& socket, const vector<size_t>& vec) 
// the socket must be valid
// the vector musn't be empty
// throws a data_transmission_error exception if socket is invalid
// throws a data_transmission_error exception if the vector is empty
// throws a data_transmission_error exception if send failed
{
	if (socket == INVALID_SOCKET)
		throw data_transmission_error("send_size_t_number. Invalid socket.");
	else if (vec.size() == 0)
		throw data_transmission_error("send_size_t_number. Invalid vector.");

	unsigned int sendbuf[10]{ 0 };
	for (size_t i = 0; i < vec.size(); ++i)
		sendbuf[i] = vec[i];
	int error_result = send(socket, (char*)sendbuf, sizeof(size_t) * 10, 0);
	if (error_result == SOCKET_ERROR)
		throw data_transmission_error("send_size_t_vector. Unable to send data.");
}

// recieves vector of size_t
vector<size_t> recieve_size_t_vector(const SOCKET& socket) 
// the socket must be valid
// returns the vector recieved
// throws a data_transmission_error exception if socket is invalid
// throws a data_transmission_error exception if recv failed
{
	if (socket == INVALID_SOCKET)
		throw data_transmission_error("send_size_t_number. Invalid socket.");

	vector<size_t> vec(10, 0);
	size_t recvbuf[10]{ 0 };
	int error_result = recv(socket, (char*)recvbuf, sizeof(size_t) * 10, MSG_WAITALL);
	if (error_result == SOCKET_ERROR)
		throw data_transmission_error("recieve_size_t_vector. Unable to recieve data.");

	for (size_t i = 0; i < vec.size(); ++i)
		vec[i] = recvbuf[i];

	return vec;
}


// Main functions

// core of game
int game_core(const vector<size_t>& secret_number, size_t& bulls_count, size_t& cows_count)
// core of bulls and cow game
// requires the secret number vector
// outputs results in bulls_count and cows_count
// if no errors occur, returns 0
{
	// gets number of digits to guess
	const size_t number_of_digits = secret_number.size();

	vector<size_t> guess_number(number_of_digits, 0);

	// gets users' guess
	cout << "Guess: ";

	bool is_guess_valid { false };

	while (!is_guess_valid) {
		try {
			string sinput { "" };
			getline(cin, sinput);
			while (sinput.size() != number_of_digits) {
				cout << "Please enter " << number_of_digits << " digits." << endl;
				getline(cin, sinput);
			}

			for (size_t i = 0; i < guess_number.size(); ++i) {
				guess_number[i] = char_to_size_t(sinput[i]);
			}
		}
		catch (out_of_range) {
			cout << "Invalid character(s) found. Please try again." << endl;
			continue;
		}

		// checks if all numbers are unique and then display vector
		if (has_unique_elements(guess_number)) {
			is_guess_valid = true;
		}
		else
		{
			cout << "The digits must be all different. Please try again." << endl;
		}
	}

	// compare guess number to secret number and count 
	count_bulls_and_cows(secret_number, guess_number, bulls_count, cows_count);

	return 0;
}

// command line version of bulls and cows game offline
int bullsandcowsc_offline()
// main function called for single player
// number_of_digits range allowed [3, 10)
// if no errors occur, returns 0   
{
	// gets number of digits to guess
	asks_number_of_digits();
	size_t number_of_digits = gets_number_of_digits();
	while (number_of_digits < 3 || number_of_digits > 10) {
		cout << "Please enter a digit between 3 and 10." << endl;
		number_of_digits = gets_number_of_digits();
	}

	cout << endl;

	// initialize secret number by randomly choosin it
	vector<size_t> secret_number = get_random_vector(number_of_digits);

	bool is_game_finished { false };

	while (!is_game_finished) {

		size_t bulls_count { 0 };
		size_t cows_count { 0 };

		// calls core of bulls and cow game
		game_core(secret_number, bulls_count, cows_count);

		// display results
		if (bulls_count == number_of_digits) {
			cout << "\nYou win !" << endl;
			is_game_finished = true;
		}

		else
			cout << bulls_count << ((bulls_count > 1) ? " bulls " : " bull ") << "and "
			<< cows_count << ((cows_count > 1) ? " cows." : " cow.") << endl;
	}

	return 0;
}

// core of bulls and cow game network
int network_game_core(const SOCKET& socket, const vector<size_t>& secret_number)
// if no errors occur, returns 0  
{
	// gets number of digits to guess
	const size_t number_of_digits = secret_number.size();

	bool is_game_finished { false };

	while (!is_game_finished) {

		size_t bulls_count { 0 };
		size_t cows_count { 0 };

		// calls core of bulls and cow game
		game_core(secret_number, bulls_count, cows_count);

		// send bulls count
		send_size_t_number(socket, bulls_count);

		// recieve other player bulls count
		cout << "Waiting for friend..." << endl;
		size_t bulls_count_opponent = recieve_size_t_number(socket);

		// display results
		if (bulls_count == number_of_digits) {}
		else
			cout << bulls_count << ((bulls_count > 1) ? " bulls " : " bull ") << "and "
			<< cows_count << ((cows_count > 1) ? " cows." : " cow.") << endl;

		if (bulls_count == number_of_digits || bulls_count_opponent == number_of_digits) {
			is_game_finished = true;

			if (bulls_count > bulls_count_opponent)
				cout << "\nYou win !" << endl;
			else if (bulls_count < bulls_count_opponent)
				cout << "\nYour lose !" << endl;
			else
				cout << "\nDraw !" << endl;
		}
	}

	shutdown_connection(socket);

	return 0;
}

// command line version of bulls and cows game network
int bullsandcowsc_network()
// main function called for 2 players on network
// number_of_digits range allowed [3, 10)
// if no errors occur, returns 0  
{
	// display network menu
	display_ntwk_menu();

	string choice { "" };

	while (true) {
		getline(cin, choice);

		if (choice.size() > 1)
		{
			cout << "Please enter a valid choice." << endl;
			continue;
		}

		switch (choice[0]) {
		case '1':
		{
			SOCKET client_socket = server();

			// if the socket is invalid, returns to network menu
			if (client_socket == INVALID_SOCKET) {
				cout << "Unable to start a game." << endl;
				cout << "Returning to menu..." << endl;
				break;
			}

			try
			{	
				// gets number of digits to guess
				asks_number_of_digits();
				size_t number_of_digits = gets_number_of_digits();
				while (number_of_digits < 3 || number_of_digits > 10) {
					cout << "Please enter a digit between 3 and 10." << endl;
					number_of_digits = gets_number_of_digits();
				}

				// sends number of digits to guess
				send_size_t_number(client_socket, number_of_digits);

				cout << endl;

				// initializes secret number by randomly choosin it
				vector<size_t> secret_number = get_random_vector(number_of_digits);

				// sends secret number
				send_size_t_vector(client_socket, secret_number);

				// calls network game core
				network_game_core(client_socket, secret_number);
			}
			catch (data_transmission_error) {
				cerr << "\nError: connection lost." << endl;
				cout << "Returning to menu..." << endl;
				shutdown_connection(client_socket);
			}
		}
			break;

		case '2':
		{
			// prompts the user to provide the opponent's ip
			string ip_address { "" };
			cout << "\nPlease enter an IP address: " << endl;
			getline(cin, ip_address);

			SOCKET connect_socket = client(ip_address);

			// lets the user retry 3 times to connect to the server
			size_t retry_times { 1 };
			while (connect_socket == INVALID_SOCKET)
			{
				cout << "Press enter to retry.";
				getchar();
				cout << "Retrying...(" << retry_times++ << ")" << endl;

				connect_socket = client(ip_address);

				if (retry_times == 4 && connect_socket == INVALID_SOCKET) {
					cout << "Unable to join a game." << endl;
					cout << "Returning to menu..." << endl;
					break;
				}
			}

			// if the socket is invalid, returns to network menu
			if (connect_socket == INVALID_SOCKET)
				break;

			cout << "\nConnected to server." << endl;
			cout << "Waiting for friend..." << endl;

			try
			{
				// gets number of digits to guess by recieving it and displays it
				size_t number_of_digits = recieve_size_t_number(connect_socket);
				cout << "\n" << number_of_digits << " digits to guess !" << endl;

				cout << endl;

				// initialize secret number by recieving it
				vector<size_t> secret_number = recieve_size_t_vector(connect_socket);
				secret_number.resize(number_of_digits);

				// calls network game core
				network_game_core(connect_socket, secret_number);
			}
			catch (data_transmission_error) {
				cerr << "\nError: connection lost." << endl;
				cout << "Returning to menu." << endl;
				shutdown_connection(connect_socket);
			}
		}
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
		display_ntwk_menu();
	}

	return 0;
}