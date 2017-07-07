#include <iostream>

#include <git/commit.hh>
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

  auto c = git::commit(r, "9f03d0c131f6343fee00f5dc5485c047e5cd2033");

  c.tree_walk([path](auto root, auto entry)
  {
    if (git_tree_entry_type(entry) == GIT_OBJ_BLOB)
      std::cout << path << "/" << root << git_tree_entry_name(entry) << std::endl;
  });
}
