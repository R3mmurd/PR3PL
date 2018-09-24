# ifndef HELPERS_H
# define HELPERS_H

string remove_whites(const string & str)
{
  string ret_val = "";

  for (auto c : str)
    {
      if (c == ' ' or c == '\t' or c == '\n')
	continue;

      ret_val.push_back(c);
    }

  return ret_val;
}

bool is_str_num(const string & str)
{
  for (auto c : str)
    if (c < '0' or c > '9')
      return false;

  return true;
      
}

# endif // HELPERS_H
