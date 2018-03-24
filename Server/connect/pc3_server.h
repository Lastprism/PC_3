#include <string.h>
#include <vector>
#include <mysql/mysql.h>
#include <stdio.h>
#include <string>

#define _PC3_SERVER_H_ 0

using namespace std;
struct mysql_connector{
	char server[32];
	char user[32];
	char passwd[32];
	char database[32];
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

	mysql_connector(const char server[],const char user[],const char passwd[],const char database[]){
		strcpy(this->server,server);
		strcpy(this->user,user);
		strcpy(this->passwd,passwd);
		strcpy(this->database,database);

		conn = mysql_init(NULL);

		if (!mysql_real_connect(conn,server,user,passwd,database,0,NULL,CLIENT_MULTI_RESULTS)){
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}
	}
	mysql_connector(){}
	void close(){
		mysql_close(conn);
	}

	void connect_to(const char server[],const char user[],const char passwd[],const char database[]){
		strcpy(this->server,server);
		strcpy(this->user,user);
		strcpy(this->passwd,passwd);
		strcpy(this->database,database);

		conn = mysql_init(NULL);

		if (!mysql_real_connect(conn,server,user,passwd,database,0,NULL,CLIENT_MULTI_RESULTS)){
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}
	}

	vector< vector<string> > query(const char q[]){
		vector< vector<string> > result;
		if (conn == NULL){
			printf("Cannot conncet to database!\n");
			exit(1);
		}

		if (mysql_query(conn,q)){
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}
		res = mysql_use_result(conn);
		if (res == NULL) return result;

		while ((row = mysql_fetch_row(res)) != NULL){
			vector<string> tmprow;
			int fields_num = mysql_num_fields(res);
			for (int i = 0;i<fields_num;i++){
				tmprow.push_back(row[i]);
			}
			result.push_back(tmprow);
		}
		mysql_free_result(res);
		return result;
	}
};

struct request_handle{
	mysql_connector connector;
	char corePath[100];
	char judgePath[100];
	void connect_to(const char server[],const char user[],const char passwd[],const char database[]){
		connector.connect_to(server,user,passwd,database);
		strcpy(corePath,"/var/www/html/ProjectE/");
		strcpy(judgePath,"/var/www/html/contest/judge/");
	}
	void close(){
		connector.close();
	}
	void create_tables(){
		connector.query("CREATE TABLE `submission` (`runid` char(10) NOT NULL,`userid` char(32) DEFAULT NULL,`problemid` char(10) DEFAULT NULL,`status` char(20) DEFAULT NULL,`time` char(32) DEFAULT NULL,PRIMARY KEY (`runid`)) ENGINE=InnoDB DEFAULT CHARSET=latin1");
		connector.query("CREATE TABLE `user` (`username` char(32) NOT NULL,`passwd` char(32) DEFAULT NULL,PRIMARY KEY (`username`)) ENGINE=InnoDB DEFAULT CHARSET=latin1");
		connector.query("CREATE TABLE `problemset` (`problemid` char(10) NOT NULL,PRIMARY KEY (`problemid`)) ENGINE=InnoDB DEFAULT CHARSET=latin1");
	}
	void drop_tables(){
		connector.query("DROP TABLE submission");
		connector.query("DROP TABLE user");
		connector.query("DROP TABLE problemset");
	}
	//use in judge.cpp
	void update_runid_status(const char runid[],const char status[]){
		char q[500];
		sprintf(q,"UPDATE submission SET status='%s' WHERE runid='%s'",status,runid);
		connector.query(q);
	}
	//use in submit procedure
	void add_record(const char runid[],const char userid[],const char problemid[],const char status[],const char time[]){
		char q[500];
		sprintf(q,"INSERT INTO submission VALUES('%s','%s','%s','%s','%s')",runid,userid,problemid,status,time);
		connector.query(q);
	}
	long long RSHash(string str){
		int b = 378551;
		int a = 63689;
		long long hash = 0;
		for (int i = 0;i<str.length();i++)
			hash=hash*a+str[i],a=a*b;
		return hash;
	}
	//use in login procedure
	vector<string> get_problemset(){
		vector<vector<string> >result = connector.query("SELECT * FROM problemset");
		vector<string> ret;
		int size = result.size();
		for (int i = 0;i<size;i++)
			ret.push_back(result[i][0]);
		return ret;
	}
	void add_user(const char username[],const char passwd[]){
		char q[200];
		sprintf(q,"INSERT INTO user VALUES('%s','%s')",username,passwd);
		connector.query(q);
	}
	void delete_user(const char username[]){
		char q[200];
		sprintf(q,"DELETE FROM user WHERE username='%s'",username);
		connector.query(q);
	}
	//use in login procedure
	bool check_login(const char username[],long long h_passwd){
		char q[500];
		sprintf(q,"SELECT passwd FROM user WHERE username='%s'",username);
		vector<vector<string> > result = connector.query(q);
		if (result.size()!=1) return false;
		long long hpasswd = RSHash(result[0][0]);
		return hpasswd == h_passwd;
	}
	/*
	filename,
	language,
	userid,
	h_passwd,
	problemid,
	runid
	*/
	int submit_code(const char filename[],const char language[],const char userid[],long long h_passwd,const char problemid[],const char runid[]){
		if (!check_login(userid,h_passwd)) return -1;

		char new_filename[100];
		sprintf(new_filename,"%s%s.%s",corePath,runid,language);
		rename(filename,new_filename);
		char now_time[100];
		time_t now = time(NULL);
		strftime(now_time, 100, "%Y-%m-%d-%H:%M", localtime(&now));
		char time_id[100];
		sprintf(time_id,"%s-%s",now_time,runid);

		add_record(runid,userid,problemid,"waiting",now_time);

		char q_order[200];
		if (language[0] == 'c'){
			sprintf(q_order,"%squeue1234 push \"./compile %s.cpp %sdata/%s.conf %sdata/ \" %s %s %s",judgePath,runid,corePath,problemid,corePath,problemid,userid,time_id);
			system(q_order);
			sprintf(q_order,"%squeue1235 push \"./compile %s.cpp %sdata/%s.conf %sdata/ \" %s %s %s",judgePath,runid,corePath,problemid,corePath,problemid,userid,time_id);
			system(q_order);
		}
		else if (language[0] == 'j'){
			sprintf(q_order,"%squeue1234 push \"./compileJAVA %s.java %sdata/%s.conf %sdata/ \" %s %s %s",judgePath,runid,corePath,problemid,corePath,problemid,userid,time_id);
			system(q_order);
			sprintf(q_order,"%squeue1235 push \"./compileJAVA %s.java %sdata/%s.conf %sdata/ \" %s %s %s",judgePath,runid,corePath,problemid,corePath,problemid,userid,time_id);
			system(q_order);
		}

		return 0;
	}
	string query_status(const char userid[],long long h_passwd,const char runid[]){
		if (!check_login(userid,h_passwd)) return "";
		char q[200];
		sprintf(q,"SELECT status FROM submission WHERE runid='%s'",runid);
		vector<vector<string> > result = connector.query(q);
		if (result.size() != 1) return "";
		else return result[0][0];
	}
};

