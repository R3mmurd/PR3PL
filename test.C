# include <cassert>
# include <cstring>

# include <iostream>
# include <tuple>
# include <stdexcept>
# include <string>
# include <sstream>

using namespace std;

# include <list.H>

using namespace Designar;

# include <helpers.H>

# include <expnode.H>

int main()
{
  Enviroment env;
  
  Int * n = new Int(10);
  assert(n->value == 10);
  delete n;

  Add * add = new Add(new Int(4), new Int(5));
  Int * add_result = static_cast<Int *>(add->eval(env));

  delete add;

  assert(add_result->value == 9);
  delete add_result;

  add = new Add(new Int(4), new Pair(new Int(5), new Int(6)));

  try
    {
      add->eval(env);
    }
  catch(const domain_error & e)
    {
      assert(strcmp(e.what(), "add applied to non-int") == 0);
    }

  delete add;

  add = new Add(new Int(4), new Snd(new Pair(new Int(5), new Int(6))));

  add_result = static_cast<Int *>(add->eval(env));

  delete add;

  assert(add_result->value == 10);

  delete add_result;

  add = new Add(new Int(10),
		new IfGreater(new Mul(new Int(3),
				      new Neg(new Int(4))),
			      new Neg(new Int(50)),
			      new Let("x", new Int(10),
				      new Add(new Var("x"), new Var("x"))),
			      new Fst(new Pair(new Int(4), new Int(8)))));

  add_result = static_cast<Int *>(add->eval(env));

  delete add;

  assert(add_result->value == 30);


  delete add_result;

  assert(remove_whites("hello    world!\t bye \n   \n world!") ==
	 "helloworld!byeworld!");

  Exp * e = parse("<add, <snd, <intpair, <int, 1>, <int, 10>>>, <ifgreater, <mul, <int, 3>, <neg, <int, 4>>>, <neg, <int, 5>>, <let, x, <int, 10>, <add, <var, x>, <var, x>>>, <fst, <intpair,<int, 4>,<int, 8>>>>>");

  Exp * result = e->eval(env);

  delete e;

  assert(static_cast<Int *>(result)->value == 14);

  delete result;

  e = parse("<add, <int, 10>, <snd, <divmod, <let, x, <int, 20>, <ifgreater, <int, 5>, <int,4>, <let, y, <int, 4>, <mul, <var, x>, <var, y>>>, <var, x>>>, <int, 2>>>>");

  result = e->eval(env);

  delete e;

  assert(static_cast<Int *>(result)->value == 10);

  delete result;
  
  cout << "Everything ok!\n";
  
  return 0;
}

