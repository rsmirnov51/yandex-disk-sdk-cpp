#include "client.hpp"

int main()
{
  yadisk::client client{"token"};
  client.publish("http://hostname/path/to/file.dat");
}
