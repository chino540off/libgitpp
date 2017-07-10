#include <cassert>
#include <iostream>

#include <git/repository.hh>

int main(int /* argc */, char const * argv[])
{
  std::shared_ptr<git::repository> r = nullptr;
  try
  {
    r = git::repository::clone(argv[1], argv[2]);
  }
  catch (git::exception & e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  assert(git::repository::is_repo(argv[2]) == true);
  return 0;
}
