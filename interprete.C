# include <cassert>
# include <cstring>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

# include <iostream>
# include <tuple>
# include <stdexcept>
# include <string>
# include <sstream>

# include <readline/readline.h>
# include <readline/history.h>

using namespace std;

# include <list.H>

using namespace Designar;

# include <helpers.H>
# include <expnode.H>

string get_prompt(size_t i)
{
  stringstream s;
  s << i << " $ ";
  return s.str();
}

int main()
{
  cout << "Command line for PR3 programming language\n"
       << "If yo want to quit, type exit and enter\n\n";
  
  size_t counter = 0;

  Enviroment env;

  while (true)
    {
      string prompt = get_prompt(counter);

      char * line = readline(prompt.c_str());
      
      if (line == nullptr)
	break;

      string prog = line;
      
      if (prog == "exit")
	break;

      Exp * expr = nullptr;

      try
	{
	  expr = parse(remove_whites(line));

	  if (expr == nullptr)
	    continue;
	  
	  Exp * result = expr->eval(env);
	  cout << result->to_string() << endl;
	  delete result;
	  delete expr;
	  ++counter;
	}
      catch(const domain_error & e)
	{
	  delete expr;
	  cout << "Error: " << e.what() << endl;
	}
      catch(const logic_error & e)
	{
	  cout << "Error: " << e.what() << endl;
	}

      add_history(line);
      free(line);
    }
  
  return 0;
};
