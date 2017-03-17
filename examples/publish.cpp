#include "client.hpp"

int main()
{
  yadisk::client client{"toke"};
  client.publish("http://hostname/path/to/file.dat");
}
