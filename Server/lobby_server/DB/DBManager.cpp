
#include "DBManager.h"

using namespace realtrick;

void DBManager::init()
{
	mysql_init(&conn);

	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char *)NULL, 0);
	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
	}

	mysql_query(connection, "set session character_set_connection=euckr;");
	mysql_query(connection, "set session character_set_results=euckr;");
	mysql_query(connection, "set session character_set_client=euckr;");
}

int DBManager::setLastUserId(int &pid)
{
	string query = "UPDATE t_etc set e_content = '" + to_string(pid) + "' WHERE e_key = 0;";
	if (pid == 1) {
		query = "INSERT into t_etc (e_key, e_content) values (0, '1');";
	}
	query_stat = mysql_query(connection, query.c_str());
	cout << "setLastUserId pid : " << pid << endl;

	if (query_stat != 0)
	{
		cout << "Mysql query error : " << mysql_error(&conn) << endl;
		return 2;
	}

	return 0;
}

int DBManager::getLastUserId(int &pid)
{
	string query = "SELECT e_content FROM t_etc WHERE e_key = 0;";
	query_stat = mysql_query(connection, query.c_str());

	if (query_stat != 0)
	{
		cout << "Mysql query error : " << mysql_error(&conn) << endl;
		return 2;
	}

	sql_result = mysql_store_result(connection);
	fields = mysql_num_fields(sql_result);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		string val = sql_row[0];
		pid = atoi(val.c_str());
		mysql_free_result(sql_result);
		return 0;
	}

	mysql_free_result(sql_result);
	return 101;
}

int DBManager::createGeustID(const int &pid, const std::string &nickName) {

	string query = "INSERT into t_user (u_idx, u_pid, u_nickname, u_win_cnt, u_lose_cnt, u_current_chapter) values (0, " + to_string(pid) + ", '"+ nickName+"', 0, 0, 0); ";
	query_stat = mysql_query(connection, query.c_str());

	if (query_stat != 0)
	{
		cout << "Mysql query error : " << mysql_error(&conn) << endl;
		return 2;
	}

	return 0;
}

int DBManager::checkIDExist(const int &pid, std::string &nickName) {

	string query = "SELECT u_nickname FROM t_user WHERE u_pid = " + to_string(pid) + ";";
	query_stat = mysql_query(connection, query.c_str());

	if (query_stat != 0)
	{
		cout << "Mysql query error : " << mysql_error(&conn) << endl;
		return 2;
	}

	sql_result = mysql_store_result(connection);
	fields = mysql_num_fields(sql_result);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		nickName = sql_row[0];
		mysql_free_result(sql_result);
		return 0;
	}

	mysql_free_result(sql_result);

	return 101;
}

int DBManager::getUserInfo(const int &pid) {

	string query = "SELECT * FROM t_user WHERE u_pid = " + to_string(pid) + ";";
	query_stat = mysql_query(connection, query.c_str());

	if (query_stat != 0)
	{
		cout << "Mysql query error : " << mysql_error(&conn) << endl;
		return 2;
	}

	sql_result = mysql_store_result(connection);
	fields = mysql_num_fields(sql_result);

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		for (int cnt = 0; cnt < fields; ++cnt) {
			cout << sql_row[cnt] << ", ";
			printf("%12s ", sql_row[cnt]);
		}
		cout << endl;
	}
    mysql_free_result(sql_result);
    return 0;
}

int DBManager::checkNicknameExist(const string &nickname) {

	string query = "SELECT * FROM t_user WHERE u_nickname = '" + nickname + "';";
	query_stat = mysql_query(connection, query.c_str());

	if (query_stat != 0)
	{
		cout << "Mysql query error : " << mysql_error(&conn) << endl;
		return 2;
	}
	return 0;
}

int DBManager::setUserNickname(int pid, string nickname) {
	string query = "UPDATE t_user set u_nickname = '" + nickname + "' WHERE u_pid = " + to_string(pid) + ";";
	query_stat = mysql_query(connection, query.c_str());

	if (query_stat != 0)
	{
		cout << "Mysql query error : " << mysql_error(&conn) << endl;
		return 2;
	}

	return 0;
}
