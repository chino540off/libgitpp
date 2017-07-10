#include <iostream>

#include <git/blob.hh>
#include <git/commit.hh>
#include <git/reference.hh>
#include <git/repository.hh>

int main(int argc, char const * argv[])
{
  auto path = "/tmp/lol";
  auto repo = "git://github.com/openjdk-mirror/jdk7u-langtools";

  std::shared_ptr<git::repository> r;

  if (git::repository::is_repo(path))
  {
    r = git::repository::open(path);
    r->fetch();
  }
  else
  {
    r = git::repository::clone(repo, path);
  }

  git::reference::foreach(r, [](git::reference & ref)
  {
    std::cout << ref.type() << " " << ref.name() << " " << git_oid_tostr_s(ref.target()) << std::endl;
    return 0;
  });

  auto ref = git::reference::lookup(r, "refs/heads/master");

  git::tree::lookup(git::commit::lookup(r, ref->target()))->walk([&](auto root, auto entry)
  {
    if (entry.type() == GIT_OBJ_BLOB)
    {
      auto b = git::blob::lookup(r, entry.id());

      //std::cout << path << "/" << root << entry.name() << std::endl;
    }
    else if (entry.type() == GIT_OBJ_TREE)
    {
      //std::cout << path << "/" << root << entry.name() << std::endl;
    }
    return 0;
  });
}
