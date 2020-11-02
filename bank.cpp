#include <iostream>
#include <fstream>
#include <iomanip>
#include <conio.h>
using namespace std;
int choice;
class account
{
	int acno;
	char name[50];
	int deposit;
	char type;

public:
	void create_account();
	void show_account() const;
	void modify();
	void dep(int);
	void draw(int);
	void report() const;
	int retacno() const;
	int retdeposit() const;
	char rettype() const;
};

void account::create_account()
{
	cout << "\nEnter The account No. : ";
	cin >> acno;
	cout << "\n\nEnter The Name of The account Holder : ";
	cin.ignore();
	cin.getline(name, 50);
	do
	{
		cout << "\nEnter Type of The account (C/S) : ";
		cin >> type;
		type = toupper(type);
	} while ((type != 'C') && (type != 'S'));
	do
	{
		cout << "\nEnter The Initial amount(>=500 for Saving and >=1000 for current ) : ";
		cin >> deposit;
		break;
	} while ((deposit < 500 && type == 'S') || (deposit < 1000 && type == 'C')); // checking for valid amount;
	cout << "\n\n\nAccount Created Successfully";
}

void account::show_account() const
{
	cout << "\nAccount No. : " << acno;
	cout << "\nAccount Holder Name : ";
	cout << name;
	cout << "\nType of Account : " << type;
	cout << "\nBalance amount : " << deposit;
}

void account::modify()
{
	cout << "\nAccount No. : " << acno;
	cout << "\nModify Account Holder Name : ";
	cin.ignore();
	cin.getline(name, 50);
	do
	{
		cout << "\nModify Type of Account : ";
		cin >> type;
		type = toupper(type);
	} while ((type != 'C') && (type != 'S'));
	do
	{
		cout << "\nModify Balance amount(>=500 for Saving and >=1000 for current ): ";
		cin >> deposit;
		break;
	} while ((deposit < 500 && type == 'S') || (deposit < 1000 && type == 'C'));
	cout << "\n\n\nAccount Modified Successfully";
}

void account::dep(int x)
{

	deposit += x;
}

void account::draw(int x)
{
	deposit -= x;
}

void account::report() const
{
	cout << acno << setw(10) << " " << name << setw(10) << " " << type << setw(6) << deposit << endl;
}

int account::retacno() const
{
	return acno;
}

int account::retdeposit() const
{
	return deposit;
}

char account::rettype() const
{
	return type;
}

void write_account();
void display_sp(int);
void modify_account(int);
void delete_account(int);
void display_all();
void deposit_withdraw(int, int);
void intro();

int main()
{
	char ch;
	int num;
	intro();
	if (choice == 1)
	{
		system("cls");
		do
		{

			cout << "\n\n\n\t--------WELCOME TO BANKING SYSTEM-------";
			cout << "\n\n\t01. CREATE NEW ACCOUNT";
			cout << "\n\n\t02. DEPOSIT AMOUNT";
			cout << "\n\n\t03. WITHDRAW AMOUNT";
			cout << "\n\n\t04. BALANCE ENQUIRY";
			cout << "\n\n\t05. ALL ACCOUNT HOLDER LIST";
			cout << "\n\n\t06. CLOSE AN ACCOUNT";
			cout << "\n\n\t07. MODIFY AN ACCOUNT";
			cout << "\n\n\t08. EXIT";
			cout << "\n\n\tSelect Your Option (1-8) ";
			cin >> ch;
			switch (ch)
			{
			case '1':
				write_account();
				break;
			case '2':
				cout << "\n\n\tEnter The account No. : ";
				cin >> num;
				deposit_withdraw(num, 1);
				break;
			case '3':
				cout << "\n\n\tEnter The account No. : ";
				cin >> num;
				deposit_withdraw(num, 2);
				break;
			case '4':
				cout << "\n\n\tEnter The account No. : ";
				cin >> num;
				display_sp(num);
				break;
			case '5':
				display_all();
				break;
			case '6':
				cout << "\n\n\tEnter The account No. : ";
				cin >> num;
				delete_account(num);
				break;
			case '7':
				cout << "\n\n\tEnter The account No. : ";
				cin >> num;
				modify_account(num);
				break;
			case '8':
				cout << "\n\n\tThanks for using Bank Management System";
				goto xy;
			default:
				cout << "\n\n\tNot A Valid Option, Please choose a valid option\n";
			}
			cin.ignore();
			cout << "\n\n\tPress any key to continue\n ";
			cin.get();
		} while (ch);
	}
	if (choice == 2)
	{
		system("cls");
		cout << "\nWelcome to the Stock Checking Program: ";
		system("cd C:\\Users\\Dhruv\\Desktop\\Shortcuts\\pro");
		system("python stocks.py");
		system("cls");
		cout<<"\nProgram Ended Press Any Key to Continue.......";
		getch();
		system("cls");
	}
	xy:
		cout<<"\n\tTHANKS FOR VISITING !!!\n\n\tHAVE A WONDERFUL DAY AHEAD!!!\n\n\n";
	return 0;
}

void write_account()
{
	account ac;
	ofstream outFile;
	outFile.open("account.dat", ios::binary | ios::app);
	ac.create_account();
	outFile.write((char *)&ac, sizeof(account));
	outFile.close();
}

void display_sp(int n)
{
	account ac;
	bool flag = false;
	ifstream inFile;
	inFile.open("account.dat", ios::binary);
	if (!inFile)
	{
		cout << "File could not be opened!! Press any Key...";
		return;
	}

	while (inFile.read((char *)&ac, sizeof(account)))
	{
		if (ac.retacno() == n)
		{
			cout << "\nBALANCE DETAILS\n";
			ac.show_account();
			flag = true;
		}
	}
	inFile.close();
	if (flag == false)
		cout << "\n\nAccount number does not exist";
}

void modify_account(int n)
{
	bool found = false;
	account ac;
	fstream File;
	File.open("account.dat", ios::binary | ios::in | ios::out);
	if (!File)
	{
		cout << "File could not be open !! Press any Key...";
		return;
	}
	while (!File.eof() && found == false)
	{
		File.read((char *)&ac, sizeof(account));
		if (ac.retacno() == n)
		{
			ac.show_account();
			cout << "\n\nEnter The New Details of account" << endl;
			ac.modify();
			int pos = (-1) * static_cast<int>(sizeof(account));
			File.seekp(pos, ios::cur);
			File.write((char *)&ac, sizeof(account));
			cout << "\n\n\t Record Updated";
			found = true;
		}
	}
	File.close();
	if (found == false)
		cout << "\n\n Record Not Found ";
}

void delete_account(int n)
{
	account ac;
	ifstream inFile;
	ofstream outFile;
	inFile.open("account.dat", ios::binary);
	if (!inFile)
	{
		cout << "File could not be open !! Press any Key...";
		return;
	}
	outFile.open("Temp.dat", ios::binary);
	inFile.seekg(0, ios::beg);
	while (inFile.read((char *)&ac, sizeof(account)))
	{
		if (ac.retacno() != n)
		{
			outFile.write((char *)&ac, sizeof(account));
		}
	}
	inFile.close();
	outFile.close();
	remove("account.dat");
	rename("Temp.dat", "account.dat");
	cout << "\n\n\t Account Record Deleted Successfully..";
}

void display_all()
{
	account ac;
	ifstream inFile;
	inFile.open("account.dat", ios::binary);
	if (!inFile)
	{
		cout << "File could not be open !! Press any Key...";
		return;
	}
	cout << "\n\n\t\tACCOUNT HOLDER LIST\n\n";
	cout << "====================================================\n";
	cout << "A/c no.      NAME           Type  Balance\n";
	cout << "====================================================\n";
	while (inFile.read((char *)&ac, sizeof(account)))
	{
		ac.report();
	}
	inFile.close();
}

void deposit_withdraw(int n, int option)
{
	int amt;
	bool found = false;
	account ac;
	fstream File;
	File.open("account.dat", ios::binary | ios::in | ios::out);
	if (!File)
	{
		cout << "File could not be open !! Press any Key...";
		return;
	}
	while (!File.eof() && found == false)
	{
		File.read((char *)&ac, sizeof(account));
		if (ac.retacno() == n)
		{
			ac.show_account();
			if (option == 1)
			{
				cout << "\n\n\tTO DEPOSIT AMOUNT ";
				do
				{
					cout << "\n\nEnter The amount to be deposited:";
					cin >> amt;
					if (amt < 1)
					{
						cout << "\n Deposit amount can't be less than 1";
					}
				} while (amt < 1);
				ac.dep(amt);
			}
			if (option == 2)
			{
				cout << "\n\n\tTO WITHDRAW AMOUNT ";

				do
				{
					cout << "\n\nEnter The amount to be withdraw:";
					cin >> amt;
					if (amt < 1)
					{
						cout << "\n Withdraw amount can't be less than 1";
					}
				} while (amt < 1);
				int bal = ac.retdeposit() - amt;
				if ((bal < 500 && ac.rettype() == 'S') || (bal < 1000 && ac.rettype() == 'C'))
					cout << "Insufficient balance / minimum balance limit violation";
				else
					ac.draw(amt);
			}
			int pos = (-1) * static_cast<int>(sizeof(ac));
			File.seekp(pos, ios::cur);
			File.write((char *)&ac, sizeof(account));
			cout << "\n\n\t Record Updated";
			found = true;
		}
	}
	File.close();
	if (found == false)
		cout << "\n\n Record Not Found ";
}

void intro()
{
	system("cls");
	cout << "\n\n\n\tMODELLING AND SIMULATION PROJECT";
	cout << "\n\n\n\nMADE BY :Dhruv Kashyap 2K19/CO/121 & Dhruv Kumar 2K19/CO/122\n";
	cout << "\n\tDELHI TECHNOLOGICAL UNIVERSITY\n";
	cout<<"\nPress Any Key to Enter the code....";
	getch();
	system("cls");
	cout << "\n*********************Menu************************ \n\nPRESS 1 TO ENTER THE BANKING SYSTEM \n\n******************************************** \n\nPRESS 2 TO VIEW STOCK MARKET FORECASTING PROGRAM \n\n********************************************";
	cout << "\n";
	cin >> choice;
}
