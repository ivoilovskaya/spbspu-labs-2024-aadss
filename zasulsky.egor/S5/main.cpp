#include <fstream>
#include "taskSumm.hpp"
#include "traverseCmds.hpp"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Error: Wrong parameters amount!\n";
    return 2;
  }

  std::ifstream input(argv[2]);
  if (!input)
  {
    std::cerr << "Error: Can't open a file!\n";
    return 2;
  }

  using base_t = zasulsky::avlTree< long long, std::string >;
  base_t data;

  zasulsky::fillTree(input, data);
  if (data.isEmpty())
  {
    zasulsky::outEmpty(std::cout);
    return 0;
  }

  zasulsky::avlTree< std::string, std::function< void(std::ostream&, base_t&) > > travCmds;
  travCmds.insert({ "ascending", zasulsky::ascending });
  travCmds.insert({ "descending", zasulsky::descending });
  travCmds.insert({ "breadth", zasulsky::breadth });

  std::string direct = argv[1];

  if (!data.isEmpty())
  {
    try
    {
      auto cmdIter = travCmds.find(direct);
      if (cmdIter != travCmds.end() && !direct.empty())
      {
        (*cmdIter).second(std::cout, data);
      }
      else if (!direct.empty())
      {
        zasulsky::outInvCommand(std::cout);
        return 1;
      }
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << "\n";
      return 1;
    }
  }
  else
  {
    zasulsky::outEmpty(std::cout);
  }

  return 0;
}
