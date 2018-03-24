#include"pc3_server.h"

int main()
{
    request_handle test;
    test.connect_to("localhost","root","123456","contest");
    test.drop_tables();
    test.close();
    return 0;
}
