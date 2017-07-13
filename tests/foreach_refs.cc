#include <iostream>

#include <git/reference.hh>
#include <git/repository.hh>
#include <git/oid.hh>

int main(int /* argc */, char const * argv[])
{
  auto r = git::repository::open(argv[1]);

  git::reference::foreach(r, [](git::reference & ref)
  {
    auto oid = git::oid(ref.target());

    std::cout << ref.type() << " " << ref.name() << " " << oid.str() << std::endl;
    return 0;
  });
}
