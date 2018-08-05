#include <iostream>
#include <list>
#include "sqlite3.h"
#include <cstring>
#include <string>
#include <conio.h>


using namespace std;
class User {
public:
	int  id;
	char *username;
	char *password;
	char *birthday;
	char *fullName;
	char *Address;
	//int flag; //danh dau dang nhap

public:
	User() {};
	void SignUp();
	void SignIn();
	bool isSignIn(); // kiem tra da dang nhap chua
	void SignOut();
	bool Check(User u);
	void ShowMess(int);
	void ShowMessDetail(int);
	void SendMess(int);
	void AddFriend(int);
	void ShowFriend(int);
	void BlockFriend(int);

};

class ConnectSQL {
public:
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char *szSQL;
	const char *pzTest;
	sqlite3_stmt *stmt ;
	list<string> answer;
public:
	static int callback(void *, int, char **, char **);
	void Open();
	void InsertSQL(sqlite3 *db, char *, char *, char *, char *, char *);
	int SelectSQL(sqlite3 *db, char *fn, char *ln);
	int TransNameToId(char *frien);
	void SelectMessenger(sqlite3 *db, int id);
	void ShowMessDetail(sqlite3 *db, int id1, int id2);
	bool CheckBlock(sqlite3 *db, int id1, int id2);
	void WriteToMess(sqlite3 *db, int id1, int id2, char *mes, char *time);
	bool CheckFriend(sqlite3 *db, int id1, int id2);
	void WriteToFriend(sqlite3 *db, int id1, int id2);
	void ListFriend(sqlite3 *db, int id1);
	void BlockFriend(sqlite3 *db, int id1, int id2);

};