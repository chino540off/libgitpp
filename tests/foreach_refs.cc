#include <iostream>

#include <git/reference.hh>
#include <git/repository.hh>

int main(int /* argc */, char const * argv[])
{
  auto r = git::repository::open(argv[1]);

  git::reference::foreach(r, [](git::reference & ref)
  {
    std::cout << ref.type() << " " << ref.name() << " " << git_oid_tostr_s(ref.target()) << std::endl;
    return 0;
  });
}
