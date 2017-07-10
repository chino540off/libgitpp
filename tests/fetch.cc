#include <cassert>
#include <iostream>

#include <git/repository.hh>

int main(int /* argc */, char const * argv[])
{
  if (git::repository::is_repo(argv[1]))
  {
    auto r = git::repository::open(argv[1]);

    assert(r != nullptr);

    try
    {
      r->fetch(argv[2]);
    }
    catch (git::exception & e)
    {
      std::cerr << e.what() << std::endl;
      return 1;
    }
  }
  else
  {
    return 1;
  }
}
