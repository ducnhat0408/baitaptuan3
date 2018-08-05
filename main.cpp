#include <iostream>
#include<conio.h>
#include <cstring>
#include<stdio.h>
#include <ctime>
#include "sqlite3.h"
#include <string>
#include <sstream>
#include "appchat1.h"

using namespace std;

int main() {
	while (1) {
		cout << "-------Chon chuc nang: ------\n"
			"1.Dang ky\n"
			"2.Dang nhap\n"
			"3.Hien thi tin nhan\n"
			"4.Hien thi chi tiet tin nhan\n"
			"5.Gui tin nhan\n"
			"6.Them ban be\n"
			"7.Hien thi ban be\n"
			"8.Block\n"
			"9.Dang xuat\n"
			"-----------------------\n";

		char choose;
		cin >> choose;
		User us = User();
		switch (choose) {
		case '1':
			us.SignUp();
			break;
		case '2':
			us.SignIn();
			break;

		case '3':
			us.ShowMess(us.id);
			break;
		case '4':
			us.ShowMessDetail(us.id);
			break;
		case '5':
			us.SendMess(us.id);
			break;
		case '6':
			us.AddFriend(us.id);
			break;
		case '7':
			us.ShowFriend(us.id);
			break;
		case '8':
			us.BlockFriend(us.id);
			break;
		case '9':
			us.SignOut();
			break;
		default:
			cout << "Xin moi nhap lai\n";
			break;
		}

	}

	return 0;
}