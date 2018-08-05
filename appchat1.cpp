#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "appchat1.h"
#include <string>
#include <list>
#include <ctime>
#include <sstream>
#include <stdio.h>

using namespace std;

int flag = 0;
void User::SignUp() {
	cout << "nhap thong tin dang ky!" << endl;
	username = new (char);
	birthday = new (char);
	password = new char;
	fullName = new (char);
	Address = new (char);

	cout << "Nhap vao username: ";
	cin >> username;
	cout << "Nhap password: ";
	cin >> password;
	cout << "Nhap birthday: ";
	cin >> birthday;
	cout << "Nhap fullname: ";
	cin.ignore(1);
	cin.get(fullName, 20);
	cout << "Nhap Address: ";
	cin.ignore(1);
	cin.get(Address, 20);

	ConnectSQL cn;
	cn.Open();

	cn.InsertSQL(cn.db, username, password, birthday, fullName, Address);

}
void  User::SignIn() {
	username = new (char);
	password = new (char);

	cout << "Nhap vao user: ";
	cin >> username;
	cout << "Nhap pass: ";
	cin >> password;
	ConnectSQL cn;
	cn.Open();

	if (cn.SelectSQL(cn.db, username, password) > 0) {
		cout << "Dang Nhap Thanh Cong\n";
		id = cn.SelectSQL(cn.db, username, password);
		flag = 1;
	}
	else {
		cout << "Sign in fail!\n";
		flag = 0;
	}
	//cn.SelectUser1();
}
void User::SignOut() {
	flag = 0;
	cout << "Ban da dang xuat.\n";
}

bool User::isSignIn() {
	if (flag == 1) return true;
	return false;
}

void User::ShowMess(int id) {
	//cout << id;
	// cout << this->id << " " << this->flag;
	if (isSignIn()) {
		ConnectSQL cn;
		cn.Open();
		cn.SelectMessenger(cn.db, id);
	}
	else {
		cout << "Ban chua dang nhap.\n\n";
	}



}
void User::ShowMessDetail(int id) {
	if (isSignIn()) {
		int id2;
		cout << "Chon nguoi dung: ";
		char *frien;
		frien = new char;
		cin >> frien;
		ConnectSQL cn;
		cn.Open();
		id2 = cn.TransNameToId(frien);

		if (id2 > 0)
			cn.ShowMessDetail(cn.db, id, id2);
		else cout << "Tai khoan khong ton tai\n";
	}
	else {
		cout << "Ban chua dang nhap.\n\n";
	}

}

void User::SendMess(int id) {
	if (isSignIn()) {
		int id2;
		cout << "Chon nguoi dung: ";
		char *frien;
		frien = new char;
		cin >> frien;
		ConnectSQL cn;
		cn.Open();
		id2 = cn.TransNameToId(frien);
		cout << id;
		//cout << id2;
		if (id2 > 0) {
			char *mess, *dt;
			mess = new char;
			dt = new char;
			if (!cn.CheckBlock(cn.db, id, id2)) {
				cout << "tai khoan cua ban bi block\n";
			}
			else {
				cout << "Nhap Tin nhan: \n";
				cin.ignore(1);
				cin.get(mess, 200);
				cout << mess;
				time_t hientai = time(0);
				dt = ctime(&hientai);
				cn.WriteToMess(cn.db, id, id2, mess, dt);
				cn.ShowMessDetail(cn.db, id, id2);
			}
		}
		else cout << "Tai khoan khong ton tai\n";

	}
	else {
		cout << "Ban chua dang nhap.\n\n";
	}

}

void User::AddFriend(int id) {
	if (isSignIn()) {
		cout << "Nhap ten ban muon them: ";
		char *newfr;
		newfr = new char;
		cin >> newfr;
		ConnectSQL cn;
		cn.Open();
		int id2 = cn.TransNameToId(newfr);
		if (id2 > 0) {
			if (cn.CheckBlock(cn.db, id, id2) && !cn.CheckFriend(cn.db, id, id2)) {
				cn.WriteToFriend(cn.db, id, id2);
				cout << "them ban thanh cong\n";
			}
			else {
				cout << "ban da bi chan hoac nguoi ay da la ban\n\n\n";
				cn.CheckFriend(cn.db, id, id2);
			}
		}
		else cout << "Tai khoan khong ton tai\n";
	}
	else {
		cout << "Ban chua dang nhap.\n\n";
	}

}

void User::ShowFriend(int id) {
	if (isSignIn()) {
		ConnectSQL cn;
		cn.Open();
		cn.ListFriend(cn.db, id);
	}
	else {
		cout << "Ban chua dang nhap.\n\n";
	}
}

void User::BlockFriend(int id1) {
	if (isSignIn()) {
		cout << "Chon nguoi block :( \n";
		char *name;
		name = new char;
		cin >> name;
		ConnectSQL cn;
		cn.Open();
		int id2 = cn.TransNameToId(name);
		if (id2 > 0) {
			cn.BlockFriend(cn.db, id1, id2);
			cout << "ban da block thanh cong nguoi nay\n";
			cn.ListFriend(cn.db, id1);
		}
		else cout << "tai khoan khong ton tai\n";
	}
	else {
		cout << "Ban chua dang nhap.\n\n";
	}
}

int ConnectSQL::callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
void ConnectSQL::Open() {
	rc = sqlite3_open("appchat.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else {
		//  fprintf(stderr, "Opened database successfully\n");

	}

}
void ConnectSQL::InsertSQL(sqlite3 *db, char *fn, char *ln, char *bir, char *name, char *add) {
	if (!db)return;

	szSQL = "insert into user1 (username, password, birthday, fullName, Address) values (?,?,?,?,?)";

	int rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);

	if (rc == SQLITE_OK) {
		// bind the value
		sqlite3_bind_text(stmt, 1, fn, strlen(fn), 0);
		sqlite3_bind_text(stmt, 2, ln, strlen(ln), 0);
		sqlite3_bind_text(stmt, 3, bir, strlen(bir), 0);
		sqlite3_bind_text(stmt, 4, name, strlen(name), 0);
		sqlite3_bind_text(stmt, 5, add, strlen(add), 0);

		
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else {

		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		// break the loop if error occur
	}
}
int ConnectSQL::SelectSQL(sqlite3 *db, char *us, char *pa) {
	int id = 0;
	if (!db)return 0;

	szSQL = "SELECT * FROM  user1 WHERE username = ? and password = ? ";

	rc = sqlite3_prepare(db, szSQL, strlen(szSQL), &stmt, &pzTest);
	if (rc == SQLITE_OK) {

		sqlite3_bind_text(stmt, 1, us, strlen(us), 0);
		sqlite3_bind_text(stmt, 2, pa, strlen(pa), 0);

		while (sqlite3_step(stmt) == SQLITE_ROW) {
			stringstream str;
			str << sqlite3_column_text(stmt, 0);
			str >> id;
		}
		sqlite3_finalize(stmt);
	}
	else {

		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		// break the loop if error occur
	}
	return id;

}


int ConnectSQL::TransNameToId(char *frien) {
	int id2 = -1;
	szSQL = "SELECT id FROM user1 WHERE username = ?";
	rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);

	if (rc == SQLITE_OK) {
		sqlite3_bind_text(stmt, 1, frien, strlen(frien), 0);
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			stringstream str;
			str << sqlite3_column_text(stmt, 0);
			str >> id2;

		}
		sqlite3_finalize(stmt);
	}
	else {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	return id2;

}



void ConnectSQL::SelectMessenger(sqlite3 *db, int id) {
	if (!db)return;
	szSQL = "SELECT DISTINCT user1.username FROM"
		"(SELECT * FROM messenger where idsen = ? ) as A LEFT JOIN user1 ON A.idrec = user1.id";
	rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
	if (rc == SQLITE_OK) {
		// bind the value
		sqlite3_bind_int(stmt, 1, id);
		// commit
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			answer.push_back(string((const char *)sqlite3_column_text(stmt, 0)));
		}
		sqlite3_finalize(stmt);
		cout << "-----Danh sach tin nhan-------\n";
		for (list<std::string>::const_iterator it = answer.begin(); it != answer.end(); it++)
			cout << *it << endl;

	}
	else {

		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		// break the loop if error occur
	}
}


void ConnectSQL::ShowMessDetail(sqlite3 *db, int id1, int id2) {
	if (!db)return;
	cout << "Chi tiet\n";
	szSQL = "SELECT * FROM (SELECT * FROM messenger WHERE (idsen = ? OR idsen = ?) AND (idrec = ? OR idrec =?) ) AS B LEFT JOIN  user1 ON user1.id = B.idsen";
	rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
	// cout << id1 << id2;
	if (rc == SQLITE_OK) {
		// bind the value
		sqlite3_bind_int(stmt, 1, id1);
		sqlite3_bind_int(stmt, 2, id2);
		sqlite3_bind_int(stmt, 3, id1);
		sqlite3_bind_int(stmt, 4, id2);
		// commit
		while (sqlite3_step(stmt) == SQLITE_ROW) {

			cout << "   " << string((const char *)sqlite3_column_text(stmt, 5)) << ": " << endl;
			cout << "       " << string((const char *)sqlite3_column_text(stmt, 2)) << "-";
			cout << string((const char *)sqlite3_column_text(stmt, 3)) << endl;

		}
		sqlite3_finalize(stmt);

	}
	else {

		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		// break the loop if error occur
	}
}

bool ConnectSQL::CheckBlock(sqlite3 *db, int id1, int id2) {
	int isblock = 0;
	if (!db)return 0;
	szSQL = "SELECT isblock FROM friend WHERE (id1 = ? AND id2 = ?) OR (id1 = ? AND id2 = ?)";
	rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
	if (rc == SQLITE_OK) {
		// bind the value
		sqlite3_bind_int(stmt, 1, id1);
		sqlite3_bind_int(stmt, 2, id2);
		sqlite3_bind_int(stmt, 3, id2);
		sqlite3_bind_int(stmt, 4, id1);

		// commit
		//sqlite3_step(stmt);
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			stringstream str;
			str << sqlite3_column_text(stmt, 0);
			str >> isblock;
			sqlite3_finalize(stmt);
		}
	}
	else {

		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		// break the loop if error occur
	}
	return !isblock;
}
void ConnectSQL::WriteToMess(sqlite3 *db, int id1, int id2, char *mes, char *time) {
	szSQL = "INSERT INTO messenger VALUES (?,?,?,?)";
	rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
	if (rc == SQLITE_OK) {
		// bind the value
		sqlite3_bind_int(stmt, 1, id1);
		sqlite3_bind_int(stmt, 2, id2);
		sqlite3_bind_text(stmt, 3, mes, strlen(mes), 0);
		sqlite3_bind_text(stmt, 4, time, strlen(time), 0);
		// commit
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);

	}
	else {

		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		// break the loop if error occur
	}

}

bool ConnectSQL::CheckFriend(sqlite3 *db, int id1, int id2) {
	int isFriend = 0;
	szSQL = "SELECT * FROM friend WHERE ( id1 = ? AND id2 = ?) OR ( id1 = ? AND id2 = ?)";
	rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
	if (rc == SQLITE_OK) {
		// bind the value
		sqlite3_bind_int(stmt, 1, id1);
		sqlite3_bind_int(stmt, 2, id2);
		sqlite3_bind_int(stmt, 3, id2);
		sqlite3_bind_int(stmt, 4, id1);
		// commit
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			stringstream str;
			str << sqlite3_column_text(stmt, 0);
			str >> isFriend;

			if (isFriend != 0) isFriend = 1;
			else isFriend = 0;

			sqlite3_finalize(stmt);
		}
	}
	else {

		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		// break the loop if error occur
	}
	return isFriend;
}

void ConnectSQL::WriteToFriend(sqlite3 *db, int id1, int id2) {
	if (!db)return;
	szSQL = "INSERT INTO friend VALUES (?,?,?)";
	rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
	if (rc == SQLITE_OK) {
		// bind the value
		int isblock = 0;
		sqlite3_bind_int(stmt, 1, id1);
		sqlite3_bind_int(stmt, 2, id2);
		sqlite3_bind_int(stmt, 3, isblock);

		// commit
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);

	}
	else {

		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		// break the loop if error occur
	}
}

void ConnectSQL::ListFriend(sqlite3 *db, int id1) {
	cout << "Danh sach ban be\n";
	if (!db)return;
	szSQL = "SELECT DISTINCT user1.username FROM"
		"(SELECT * FROM friend where (id1 = ? OR id2 = ?) AND isblock = 0) as A LEFT JOIN user1 ON (A.id2 = user1.id OR A.id1 = user1.id)";
	rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
	if (rc == SQLITE_OK) {
		// bind the value
		sqlite3_bind_int(stmt, 1, id1);
		sqlite3_bind_int(stmt, 1, id1);
		// commit
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			cout << string((const char *)sqlite3_column_text(stmt, 0)) << endl;
		}
		sqlite3_finalize(stmt);

		cout << "-------------------------------------\n";
	}
	else {

		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		// break the loop if error occur
	}
}

void ConnectSQL::BlockFriend(sqlite3 *db, int id1, int id2) {
	if (!db)return;
	szSQL = "UPDATE friend SET isblock = 1 WHERE id1 = ? AND id2 = ?";
	rc = sqlite3_prepare_v2(db, szSQL, strlen(szSQL), &stmt, &pzTest);
	if (rc == SQLITE_OK) {
		// bind the value
		sqlite3_bind_int(stmt, 1, id1);
		sqlite3_bind_int(stmt, 2, id2);
		// commit
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);

	}
	else {

		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		// break the loop if error occur
	}
}