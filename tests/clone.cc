#include <iostream>

#include <git/blob.hh>
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

  auto c = git::commit::lookup(r, "9f03d0c131f6343fee00f5dc5485c047e5cd2033");

  c->get_tree()->walk([&](auto root, auto entry)
  {
    if (entry.type() == GIT_OBJ_BLOB)
    {
      auto b = git::blob::lookup(r, entry.id());

      std::cout << path << "/" << root << entry.name() << std::endl;
    }
    else if (entry.type() == GIT_OBJ_TREE)
    {
      std::cout << path << "/" << root << entry.name() << std::endl;
    }
    return 0;

    //  fwrite(git_blob_rawcontent(blob), (size_t)git_blob_rawsize(blob), 1, stdout);

    //}
  });
}
