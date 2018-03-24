#include "pc3_server.h"

int main()
{
    request_handle test;
    test.connect_to("localhost","root","123456","contest");
    test.create_tables();
    test.close();
    return 0;
}
