#include "commands.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Error: Wrong parameters amount!\n";
    return 2;
  }

  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "Error: unable to open the file\n";
    return 2;
  }

  using data_t = nikiforov::AvlTree< int, std::string >;
  using dictionariesTree = nikiforov::AvlTree< std::string, data_t >;

  dictionariesTree dictionaries;
  data_t dict;
  std::string strVocab;
  std::string nameVocab;

  while (!input.eof())
  {
    std::getline(input, strVocab);
    nameVocab = nikiforov::cutElem(strVocab);
    nikiforov::createDictionary(strVocab, dict);
    if (!nameVocab.empty())
    {
      dictionaries.emplace(nameVocab, dict);
    }
    dict.clear();
  }
  
  using namespace std::placeholders;
  nikiforov::AvlTree< std::string, std::function < void(dictionariesTree&, std::istream&, std::ostream&) > > cmds;
  {
    cmds.emplace("print", nikiforov::print);
    cmds.emplace("complement", nikiforov::complement);
    cmds.emplace("intersect", nikiforov::intersect);
  }
  
  std::string cmd;
  while (!std::cin.eof() && std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(dictionaries, std::cin, std::cout);
    }
    catch (const std::out_of_range&)
    {
      nikiforov::errorMessage(std::cout);
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
