#include <cassert>
#include <iostream>

#include <git/blob.hh>
#include <git/commit.hh>
#include <git/reference.hh>
#include <git/repository.hh>

int main(int /* argc */, char const * argv[])
{
  auto r = git::repository::open(argv[1]);
  auto ref = git::reference::lookup(r, argv[2]);

  std::cout << "name(ref): " << ref->name() << std::endl;
  std::cout << "name(resolve(ref)): " << ref->resolve()->name() << std::endl;

  git::tree::lookup(git::commit::lookup(r, ref->resolve()->target()))->walk([&](auto root, auto entry)
  {
    if (entry.type() == GIT_OBJ_BLOB)
    {
      auto b = git::blob::lookup(r, entry.id());

      std::cout << root << entry.name() << " size: " << b->size() << std::endl;
    }
    else if (entry.type() == GIT_OBJ_TREE)
    {
      std::cout << root << entry.name() << std::endl;
    }
    return 0;
  });
}
