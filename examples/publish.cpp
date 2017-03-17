#include "client.hpp"

int main()
{
  yadisk::Client client{"token"};
  auto responce = client.publish("http://hostname/path/to/file.dat");
  auto code = responce ["code"];
  auto link = responce ["href"];
}
