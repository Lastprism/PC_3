#ifndef _PC3_SERVER_H_
#include "pc3_server.h"
#endif
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
using namespace std;

long long RSHash(string str){
	int b = 378551;
	int a = 63689;
	long long hash = 0;
	for (int i = 0;i<str.length();i++)
		hash=hash*a+str[i],a=a*b;
	return hash;
}
int main(){
	request_handle test;
	test.connect_to("localhost","root","123456","contest");

	test.submit_code("124.cpp","cpp","root",RSHash("3.1415926"),"1001","124");
	test.close();
}
