#include <type_traits>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <tuple>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

#include "include/StyioException/Exception.hpp"
#include "include/StyioToken/Token.hpp"
#include "include/StyioUtil/Util.hpp"
#include "include/StyioAST/AST.hpp"
#include "include/StyioParser/Parser.hpp"

std::string read_styio_file()
{
  if (std::filesystem::exists("___.styio"))
  {
    std::ifstream file("___.styio");
    std::string contents;

    std::string str;
    while (std::getline(file, str))
    {
      contents += str;
      contents.push_back('\n');
    }

    contents += EOF;

    // std::cout << contents << std::endl;

    return contents;
  }

  return std::string("...");
}

void show_cwd() 
{
  std::filesystem::path cwd = std::filesystem::current_path();
  std::cout << cwd.string() << std::endl;

  // std::ofstream file(cwd.string());
  // file.close();
}

int main() {
  parse_program(read_styio_file());
  
  return 0;
}