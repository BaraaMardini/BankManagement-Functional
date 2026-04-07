#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const string CLIENTS_FILE_NAME = "MyFile2.txt";

struct sUsers {
	string UserName;
	int Permissions;  // أو أي نوع يناسب طريقة تخزين الصلاحيات
};
 void SeeAllMenue();
enum enTransactionsMenuOptions {
	eDeposit = 1,
	eWithdraw = 2,
	eTotalBalance = 3,
	eBackToMain = 4
};
enum enMainMenuOptions {
	eShowClients = 1,
	eAddClient = 2,
	eDeleteClient = 3,
	eUpdateClient = 4,
	eFindClient = 5,
	eTransactions = 6,
	eManageUsers = 7,
	eLogout = 8
};

// Forward Declarations

void ShowTransactionsMenu();

// ================= STRUCTS =================
struct stClient {
	string accountNumber;
	string pinCode;
	string name;
	string phone;
	double accountBalance;
	bool markForDelete = true;
};

struct stUser {
	string userName;
	string password;
	int permissions;
	bool markForDelete = true;
};

stUser CurrentUser;

// ================= BASIC =================
void ResetScreen()
{
	system("cls");
	system("color 0F");
}

// ================= CONVERSION =================
string ConvertClientToLine(stClient client, string delimiter) {
	string result = "";

	result += client.accountNumber + delimiter;
	result += client.pinCode + delimiter;
	result += client.name + delimiter;
	result += client.phone + delimiter;
	result += to_string(client.accountBalance);

	return result;
}

stClient ConvertLineToClient(vector<string> clientData) {
	stClient client;

	client.accountNumber = clientData[0];
	client.pinCode = clientData[1];
	client.name = clientData[2];
	client.phone = clientData[3];
	client.accountBalance = stod(clientData[4]);

	return client;
}

// ================= STRING UTIL =================
vector<string> SplitString(string text, string delimiter)
{
	vector<string> result;
	short position = 0;
	string word;

	while ((position = text.find(delimiter)) != std::string::npos)
	{
		word = text.substr(0, position);

		if (word != "")
			result.push_back(word);

		text.erase(0, position + delimiter.length());
	}

	if (text != "")
		result.push_back(text);

	return result;
}

// ================= FILE =================
vector<stClient> LoadClientsFromFile()
{
	vector<stClient> clients;
	stClient client;
	fstream file;

	file.open(CLIENTS_FILE_NAME, ios::in);

	if (file.is_open())
	{
		string line;

		while (getline(file, line))
		{
			client = ConvertLineToClient(SplitString(line, "#//#"));
			clients.push_back(client);
		}

		file.close();
	}

	return clients;
}

// ================= DISPLAY =================
void PrintClientRecord(stClient client)
{
	cout << "| " << setw(15) << left << client.accountNumber;
	cout << "| " << setw(10) << left << client.pinCode;
	cout << "| " << setw(40) << left << client.name;
	cout << "| " << setw(12) << left << client.phone;
	cout << "| " << setw(12) << left << client.accountBalance;
}

void PrintAllClientsData(vector <stClient> clients)
{
	cout << "\n\t\t\t\t\tClient List (" << clients.size() << ") Client(s).";

	cout << "\n_\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";

	cout << "\n_\n";

	for (stClient client : clients)
	{
		PrintClientRecord(client);
		cout << endl;
	}

	cout << "\n_\n";

	char userChoice;
	cout << "Do you want to leave? [y/n]\n";
	cin >> userChoice;

	if (userChoice == 'n') {
		ResetScreen();
		SeeAllMenue();
	}
}

// ================= LOGIC =================
bool IsAccountExist(string accountNumber) {
	vector<stClient> clients = LoadClientsFromFile();

	for (stClient client : clients) {
		if (client.accountNumber == accountNumber) {
			return true;
		}
	}
	return false;
}

// ================= ADD CLIENT =================
string ReadNewClientData() {
	stClient client;

	cout << "Enter AccountNumber\n";
	cin >> client.accountNumber;

	if (IsAccountExist(client.accountNumber)) {
		cout << "Client already exists, enter another AccountNumber\n";
		cin >> client.accountNumber;
	}

	cout << "Enter PinCode\n";
	cin >> client.pinCode;

	cout << "Enter Name\n";
	cin >> client.name;

	cout << "Enter Phone\n";
	cin >> client.phone;

	cout << "Enter AccountBalance\n";
	cin >> client.accountBalance;

	return ConvertClientToLine(client, "#//#");
}

void AddNewClient() {
	fstream file;

	file.open(CLIENTS_FILE_NAME, ios::out | ios::app);

	if (file.is_open())
	{
		file << ReadNewClientData() << endl;
		file.close();
	}

	cout << "Do you want to add more clients? [y/n]\n";

	char userChoice;
	cin >> userChoice;

	if (userChoice == 'y') {
		ResetScreen();
		AddNewClient();
		SeeAllMenue();
	}

	ResetScreen();
	SeeAllMenue();
}

// ================= DELETE HELPERS =================
void MarkClientForDeletion(string accountNumber, vector<stClient>& clients) {

	for (stClient& client : clients) {
		if (client.accountNumber == accountNumber) {
			client.markForDelete = false;
		}
	}
}








void GetClientByAccountNumber(string accountNumber, stClient& clientResult, vector<stClient> clients) {

	for (stClient client : clients) {
		if (client.accountNumber == accountNumber) {
			clientResult = client;
		}
	}
}

void SaveClientsToFile(vector<stClient> clients) {
	fstream file;
	string line = "";

	file.open(CLIENTS_FILE_NAME, ios::out);

	if (file.is_open())
	{
		for (stClient client : clients) {
			if (client.markForDelete == true) {
				line = ConvertClientToLine(client, "#//#");
				file << line << endl;
			}
		}
		file.close();
	}
}

bool DeleteClient() {
	cout << "Enter AccountNumber\n";

	string accountNumber = "";
	stClient selectedClient;
	vector<stClient> clients = LoadClientsFromFile();
	char userChoice;

	cin >> accountNumber;

	if (!IsAccountExist(accountNumber)) {
		cout << "Account not found\n";
		system("pause>0");
		ResetScreen();
		SeeAllMenue();
		return true;
	}

	GetClientByAccountNumber(accountNumber, selectedClient, clients);
	PrintClientRecord(selectedClient);

	cout << "Are you sure you want to delete? [y/n]\n";
	cin >> userChoice;

	if (userChoice == 'n') {
		return true;
	}

	MarkClientForDeletion(accountNumber, clients);
	SaveClientsToFile(clients);

	system("pause>0");
	SeeAllMenue();
}

// ================= UPDATE =================
void UpdateClientData(string accountNumber, vector<stClient>& clients) {

	for (stClient& client : clients) {
		if (client.accountNumber == accountNumber) {

			cout << "Enter PinCode\n";
			cin >> client.pinCode;

			cout << "Enter Name\n";
			cin >> client.name;

			cout << "Enter Phone\n";
			cin >> client.phone;

			cout << "Enter AccountBalance\n";
			cin >> client.accountBalance;
		}
	}
}

bool UpdateClientInfo() {
	string accountNumber = "";
	vector<stClient> clients = LoadClientsFromFile();

	cout << "Enter Account Number to update\n";
	cin >> accountNumber;

	if (!IsAccountExist(accountNumber)) {
		cout << "Account not found\n";
		system("pause>0");
		ResetScreen();
		SeeAllMenue();
		return true;
	}

	UpdateClientData(accountNumber, clients);
	SaveClientsToFile(clients);

	system("pause>0");
	ResetScreen();
	SeeAllMenue();
}

// ================= FIND =================
bool FindClient() {
	string accountNumber = "";
	stClient client;

	cout << "Enter Account Number to find\n";
	cin >> accountNumber;

	if (!IsAccountExist(accountNumber)) {
		cout << "Account not found\n";
		system("pause>0");
		ResetScreen();
		SeeAllMenue();
		return true;
	}

	GetClientByAccountNumber(accountNumber, client, LoadClientsFromFile());
	PrintClientRecord(client);

	system("pause>0");
	ResetScreen();
	SeeAllMenue();
}

// ================= TRANSACTIONS =================
void DepositAmount(vector<stClient>& clients, string accountNumber) {

	cout << "Enter deposit amount\n";

	double amount = 0;
	cin >> amount;

	for (stClient& client : clients) {
		if (client.accountNumber == accountNumber) {
			client.accountBalance += amount;
		}
	}
}

void WithdrawAmount(vector<stClient>& clients, string accountNumber) {

	cout << "Enter withdraw amount\n";

	double amount = 0;

	for (stClient& client : clients) {
		if (client.accountNumber == accountNumber) {

			do {
				cin >> amount;
			} while (amount > client.accountBalance);

			client.accountBalance -= amount;
		}
	}
}

void Deposit() {
	cout << "Enter Account Number\n";

	string accountNumber = "";
	vector<stClient> clients = LoadClientsFromFile();
	stClient client;

	cin >> accountNumber;

	GetClientByAccountNumber(accountNumber, client, clients);
	PrintClientRecord(client);

	DepositAmount(clients, accountNumber);
	SaveClientsToFile(clients);

	ResetScreen();
}

void Withdraw() {
	cout << "Enter Account Number\n";

	string accountNumber = "";
	vector<stClient> clients = LoadClientsFromFile();
	stClient client;

	cin >> accountNumber;

	GetClientByAccountNumber(accountNumber, client, clients);
	PrintClientRecord(client);

	WithdrawAmount(clients, accountNumber);
	SaveClientsToFile(clients);

	ResetScreen();
}

// ================= TRANSACTIONS MENU (ENUM) =================
void ShowTransactionsMenu() {

	cout << "\n==================================\n";
	cout << "        Transactions Menu\n";
	cout << "==================================\n";

	cout << "[1] Deposit\n";
	cout << "[2] Withdraw\n";
	cout << "[3] Total Balance\n";
	cout << "[4] Main Menu\n";

	cout << "\nChoose [1-4]: ";

	int userChoice;
	cin >> userChoice;

	switch ((enTransactionsMenuOptions)userChoice) {

	case eDeposit:
		ResetScreen();
		Deposit();
		break;

	case eWithdraw:
		ResetScreen();
		Withdraw();
		break;

	case eTotalBalance:
		ResetScreen();
		break;

	case eBackToMain:
		ResetScreen();
		SeeAllMenue();
		break;
	}
}

































// ================= USER CONVERSION =================
string ConvertUserToLine(stUser user, string delimiter) {
	string result = "";

	result += user.userName + delimiter;
	result += (user.password) + delimiter;
	result += to_string(user.permissions);

	return result;
}
stUser ConvertLineToUser(vector<string> userData) {
	stUser user;
	user.userName = userData[0];
	user.password = userData[1];  // string وليس double
	user.permissions = stoi(userData[2]);
	return user;
}

// ================= LOAD USERS =================
vector<stUser> LoadUsersFromFile()
{
	vector<stUser> users;
	stUser user;
	fstream file;

	file.open("B.txt", ios::in);

	if (file.is_open())
	{
		string line;

		while (getline(file, line))
		{
			user = ConvertLineToUser(SplitString(line, "#//#"));
			users.push_back(user);
		}

		file.close();
	}

	return users;
}

// ================= DISPLAY USERS =================
void PrintUserRecord(stUser user)
{
	cout << "| " << setw(15) << left << user.userName;
	cout << "| " << setw(10) << left << user.password;
}

void PrintAllUsers()
{
	vector<stUser> users = LoadUsersFromFile();

	cout << "\n\t\t\t\t\tUsers List (" << users.size() << ") User(s).";

	cout << "\n_\n";
	cout << "| " << left << setw(15) << "User Name";
	cout << "| " << left << setw(10) << "Password";
	cout << "\n_\n";

	for (stUser user : users)
	{
		PrintUserRecord(user);
		cout << endl;
	}

	cout << "\n_\n";

	char userChoice;
	cout << "Do you want to leave? [y/n]\n";
	cin >> userChoice;

	if (userChoice == 'n') {
		ResetScreen();
		SeeAllMenue();
	}
}

// ================= ADD USER =================
string ReadNewUserData() {
	stUser user;
	char answer;
	int permissions = 0;

	cout << "Enter UserName\n";
	cin >> user.userName;

	cout << "Enter Password\n";
	cin >> user.password;

	cout << "Access to List Clients? [y/n]\n";
	cin >> answer;
	if (answer == 'y') permissions += 1;

	cout << "Access to Add Client? [y/n]\n";
	cin >> answer;
	if (answer == 'y') permissions += 2;

	cout << "Access to Delete Client? [y/n]\n";
	cin >> answer;
	if (answer == 'y') permissions += 4;

	cout << "Access to Update Client? [y/n]\n";
	cin >> answer;
	if (answer == 'y') permissions += 8;

	cout << "Access to Find Client? [y/n]\n";
	cin >> answer;
	if (answer == 'y') permissions += 16;

	cout << "Access to Transactions? [y/n]\n";
	cin >> answer;
	if (answer == 'y') permissions += 32;

	cout << "Access to Manage Users? [y/n]\n";
	cin >> answer;
	if (answer == 'y') permissions += 64;

	user.permissions = permissions;

	return ConvertUserToLine(user, "#//#");
}

void AddNewUser() {
	fstream file;

	file.open("B.txt", ios::out | ios::app);

	if (file.is_open())
	{
		file << ReadNewUserData() << endl;
		file.close();
	}
}

// ================= DELETE USER HELPERS =================
void GetUserByUsername(string userName, stUser& userResult, vector<stUser> users) {

	for (stUser user : users) {
		if (user.userName == userName) {
			userResult = user;
		}
	}
}

void MarkUserForDeletion(string userName, vector<stUser>& users) {

	for (stUser& user : users) {
		if (user.userName == userName) {
			user.markForDelete = false;
		}
	}
}

void SaveUsersToFile(vector<stUser> users) {
	fstream file;
	string line = "";

	file.open("B.txt", ios::out);

	if (file.is_open())
	{
		for (stUser user : users) {
			if (user.markForDelete == true) {
				line = ConvertUserToLine(user, "#//#");
				file << line << endl;
			}
		}
		file.close();
	}
}

// ================= DELETE USER =================
bool DeleteUser() {
	cout << "Enter UserName\n";

	string userName = "";
	stUser selectedUser;
	vector<stUser> users = LoadUsersFromFile();
	char userChoice;

	cin >> userName;

	GetUserByUsername(userName, selectedUser, users);
	PrintUserRecord(selectedUser);

	cout << "Are you sure you want to delete? [y/n]\n";
	cin >> userChoice;

	if (userChoice == 'n') {
		return true;
	}

	MarkUserForDeletion(userName, users);
	SaveUsersToFile(users);

	system("pause>0");
	SeeAllMenue();
}









// ====== ENUM PERMISSIONS ======
enum enPermissions {
	pListClients = 1,
	pAddClient = 2,
	pDeleteClient = 4,
	pUpdateClient = 8,
	pFindClient = 16,
	pTransactions = 32,
	pManageUsers = 64,
	pAll = -1
};
void UpdateUserData(string userName, vector<stUser>& users) {

	for (stUser& user : users) {
		if (user.userName == userName) {
			cout << "Enter New Password:\n";
			cin >> user.password;
		}
	}
}

bool UpdateUser() {

	string userName = "";
	vector<stUser> users = LoadUsersFromFile();

	cout << "Enter UserName to update:\n";
	cin >> userName;

	UpdateUserData(userName, users);
	SaveUsersToFile(users);

	system("pause>0");
	ResetScreen();
	SeeAllMenue();
	return true;
}
// ====== FIND USER ======
bool FindUser() {

	string userName = "";
	stUser user;

	cout << "Enter UserName to find:\n";
	cin >> userName;

	GetUserByUsername(userName, user, LoadUsersFromFile());
	PrintUserRecord(user);

	system("pause>0");
	ResetScreen();
	SeeAllMenue();
	return true;
}
// ====== MANAGE USERS MENU ======
void ManageUsersMenu() {

	cout << "==================================\n";
	cout << "        Manage Users Menu\n";
	cout << "==================================\n";

	cout << "[1] List Users\n";
	cout << "[2] Add New User\n";
	cout << "[3] Delete User\n";
	cout << "[4] Update User\n";
	cout << "[5] Find User\n";
	cout << "[6] Main Menu\n";

	int choice = 0;
	cin >> choice;

	switch (choice) {
	case 1: PrintAllUsers(); break;
	case 2: ResetScreen(); AddNewUser(); break;
	case 3: ResetScreen(); DeleteUser(); break;
	case 4: ResetScreen(); UpdateUser(); break;
	case 5: ResetScreen(); FindUser(); break;
	case 6: ResetScreen(); SeeAllMenue(); break;
	}
}
// ====== CHECK LOGIN ======
bool CheckLogin(string userName, string password) {

	vector<stUser> users = LoadUsersFromFile();

	for (stUser user : users) {
		if (user.userName == userName && user.password == password) {
			return true;
		}
	}
	return false;
}

// ====== LOGIN ======
string Login() {
	cout << "=========== Login ===========" << endl;
	string UserName;
	string Password;
	bool valid = false;

	do {
		cout << "Enter UserName: ";
		cin >> UserName;
		cout << "Enter Password: ";
		cin >> Password;

		vector<stUser> users = LoadUsersFromFile();
		for (stUser user : users) {
			if (user.userName == UserName && user.password == Password) {
				valid = true;
				break;
			}
		}

		if (!valid) {
			cout << "Invalid username or password. Try again.\n";
		}

	} while (!valid);

	return UserName;
}
stUser GetCurrentUser(string UserName) {
	vector<stUser> Users = LoadUsersFromFile();
	for (stUser user : Users) {
		if (user.userName == UserName)  // لاحظ userName بدل UserName
			return user;
	}

	// لو المستخدم مش موجود نرجع مستخدم فارغ
	return stUser();
}
// ====== MAIN MENU ======
void SeeAllMenue() {
	cout << "========== Main Menu ==========" << endl;
	cout << "[1] Show Clients" << endl;
	cout << "[2] Add Client" << endl;
	cout << "[3] Delete Client" << endl;
	cout << "[4] Update Client" << endl;
	cout << "[5] Find Client" << endl;
	cout << "[6] Transactions" << endl;
	cout << "[7] Manage Users" << endl;
	cout << "[8] Logout" << endl;

	int Choice;
	cin >> Choice;

	switch ((enMainMenuOptions)Choice) {
	case eShowClients:
		if (CurrentUser.permissions == pAll || (CurrentUser.permissions & pListClients)) {
			ResetScreen();
			PrintAllClientsData(LoadClientsFromFile());
		}
		else {
			cout << "Access Denied\n";
			system("pause>0");
			ResetScreen();
		}
		break;

	case eAddClient:
		if (CurrentUser.permissions == pAll || (CurrentUser.permissions & pAddClient)) {
			ResetScreen();
			AddNewClient();
		}
		else {
			cout << "Access Denied\n";
			system("pause>0");
			ResetScreen();
		}
		break;

	case eDeleteClient:
		if (CurrentUser.permissions == pAll || (CurrentUser.permissions & pDeleteClient)) {
			ResetScreen();
			DeleteClient();
		}
		else {
			cout << "Access Denied\n";
			system("pause>0");
			ResetScreen();
		}
		break;

	case eUpdateClient:
		if (CurrentUser.permissions == pAll || (CurrentUser.permissions & pUpdateClient)) {
			ResetScreen();
			UpdateClientInfo();
		}
		else {
			cout << "Access Denied\n";
			system("pause>0");
			ResetScreen();
		}
		break;

	case eFindClient:
		if (CurrentUser.permissions == pAll || (CurrentUser.permissions & pFindClient)) {
			ResetScreen();
			FindClient();
		}
		else {
			cout << "Access Denied\n";
			system("pause>0");
			ResetScreen();
		}
		break;

	case eTransactions:
		if (CurrentUser.permissions == pAll || (CurrentUser.permissions & pTransactions)) {
			ResetScreen();
			ShowTransactionsMenu();
		}
		else {
			cout << "Access Denied\n";
			system("pause>0");
			ResetScreen();
		}
		break;

	case eManageUsers:
		if (CurrentUser.permissions == pAll || (CurrentUser.permissions & pManageUsers)) {
			ResetScreen();
			ManageUsersMenu();
		}
		else {
			cout << "Access Denied\n";
			system("pause>0");
			ResetScreen();
		}
		break;

	case eLogout:
		cout << "Logging out...\n";
		exit(0);  // نخرج من البرنامج
		break;

	default:
		cout << "Invalid Choice\n";
		system("pause>0");
		ResetScreen();
		break;
	}

	// العودة للقائمة بعد أي عملية
	SeeAllMenue();
}
// ====== MAIN ======
int main() {
	string CurrentUserName = Login();
	CurrentUser = GetCurrentUser(CurrentUserName);
	ResetScreen();
	SeeAllMenue();  // القائمة تستخدم CurrentUser مباشرة
}