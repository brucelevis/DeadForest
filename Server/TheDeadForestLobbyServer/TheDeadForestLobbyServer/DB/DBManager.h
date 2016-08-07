#pragma once

#include <iostream>
#include <string>
#include "mysql.h"
#include "protocol.h"

using namespace std;

namespace realtrick
{
	class DBManager {

	private:

		MYSQL conn;

		MYSQL *connection = NULL;

		MYSQL_RES *sql_result;

		MYSQL_ROW sql_row;

		int query_stat;

		int fields;

	public:

		DBManager() {};

		~DBManager() {
			mysql_close(connection);
		}


		void init();

		int setLastUserId(int &pid);

		int getLastUserId(int &pid);

		int createGeustID(const int &pid, const std::string &nickName);

		int checkIDExist(const int &pid, std::string &nickName);

		int getUserInfo(const int &pid);

		int checkNicknameExist(const string &nickname);

		int setUserNickname(int pid, string nickname);

	};
}
