/**

   @author: Alejandro J. Mujica.
   @id: 
*/

struct Enviroment;

struct Exp
{
  enum class ExpType
  {
    INT, PAIR, FST, SND, VAR, NEG, ADD, MUL, DIVMOD, LET, IFGREATER,
    DEF, FUN, CALL, VOID, CLOSURE, ISVOID
  };
  
  ExpType type;

  Exp(ExpType t)
    : type(t)
  {
    // empty
  }

  ExpType get_type() const
  {
    return type;
  }
  
  virtual ~Exp()
  {
    // empty
  }
  
  virtual void destroy() = 0;

  virtual Exp * clone() = 0;

  virtual Exp * eval(Enviroment &) = 0;

  virtual string to_string() const = 0;
};

using VarList = SLList<tuple<string, Exp *>>;

struct Enviroment : public VarList
{
  Enviroment()
    : VarList()
  {
    // Empty
  }
  
  Enviroment(const Enviroment & env)
  {
    for (auto t : env)
      this->append(make_tuple(std::get<0>(t), std::get<1>(t)->clone()));
  }
  
  ~Enviroment()
  {
    while (not this->is_empty())
      {
	auto t = this->remove_first();
	delete std::get<1>(t);
      }
  }
};

Exp * envlookup(Enviroment & env, const string & var_name)
{
  auto a = env.search_ptr([&var_name] (auto t)
			  {
			    return get<0>(t) == var_name;
			  });
  
  if (a == nullptr)
    return nullptr;

  return get<1>(*a)->clone();
};

struct Void : public Exp
{
  Void()
    : Exp(ExpType::VOID)
  {
    // empty
  }

  ~Void()
  {
    destroy();
  }
  
  void destroy() override
  {
    // empty
  }
  
  Exp * clone() override
  {
    return new Void();
  }
  
  Exp * eval(Enviroment &) override
  {
    return clone();
  }
  
  string to_string() const override
  {
    return "<void>";
  }
};

struct Int : public Exp
{
  int value;

  Int()
    : Exp(ExpType::INT), value(0)
  {
    // empty
  }
  
  Int(int v)
    : Exp(ExpType::INT), value(v)
  {
    // empty
  }

  ~Int()
  {
    destroy();
  }
  
  void destroy() override
  {
    // empty
  }

  Exp * clone() override
  {
    return new Int(value);
  }
  
  Exp * eval(Enviroment &) override
  {
    return clone();
  }

  string to_string() const override
  {
    stringstream s;
    s << "<int, " << value << ">";
    return s.str();
  }
};

struct IsVoid : public Exp
{
  Exp * e;
  
  IsVoid()
    : Exp(ExpType::ISVOID), e(nullptr)
  {
    // empty
  }

  IsVoid(Exp * _e)
    : Exp(ExpType::ISVOID), e(_e)
  {
    // empty
  }

  ~IsVoid()
  {
    destroy();
  }
  
  void destroy() override
  {
    delete e;
  }
  
  Exp * clone() override
  {
    return new IsVoid(e->clone());
  }
  
  Exp * eval(Enviroment & env) override
  {
    Exp * ee = e->eval(env);

    Exp * ret_val = ee->get_type() == ExpType::VOID ? new Int(1) : new Int(0);

    delete ee;
    
    return ret_val;
  }
  
  string to_string() const override
  {
    stringstream s;
    s << "<isvoid, " << e->to_string() << ">";
    return s.str();
  }
};

struct Pair : public Exp
{
  Exp * e1;
  Exp * e2;

  Pair()
    : Exp(ExpType::PAIR), e1(nullptr), e2(nullptr)
  {
    // empty
  }
  
  Pair(Exp * _e1, Exp * _e2)
    : Exp(ExpType::PAIR), e1(_e1), e2(_e2)
  {
    // empty
  }

  ~Pair()
  {
    destroy();
  }

  void destroy() override
  {
    delete e1;
    delete e2;
  }

  Exp * clone() override
  {
    return new Pair(e1->clone(), e2->clone());
  }
  
  Exp * eval(Enviroment & env) override
  {
    Exp * ee1 = e1->eval(env);
    Exp * ee2 = e2->eval(env);

    return new Pair(ee1, ee2);
  }

  string to_string() const override
  {
    stringstream s;
    s << "<pair, " << e1->to_string() << ", " << e2->to_string() << ">";
    return s.str();
  }
};

struct Fst : public Exp
{
  Exp * e;

  Fst()
    : Exp(ExpType::FST), e(nullptr)
  {
    // empty
  }
  
  Fst(Exp * _e)
    : Exp(ExpType::FST), e(_e)
  {
    // empty
  }

  ~Fst()
  {
    destroy();
  }
  
  void destroy() override
  {
    delete e;
  }
  
  Exp * clone() override
  {
    return new Fst(e->clone());
  }
  
  Exp * eval(Enviroment & env) override
  {
    Exp * p = e->eval(env);

    if (p->get_type() != ExpType::PAIR)
      {
	delete p;
	throw domain_error("fst applied to non-pair");
      }
    
    Exp * ret_val = static_cast<Pair *>(p)->e1->clone();

    delete p;
    
    return ret_val;
  }

  string to_string() const override
  {
    stringstream s;
    s << "<fst, " << e->to_string() << ">";
    return s.str();
  }
};

struct Snd : public Exp
{
  Exp * e;

  Snd()
    : Exp(ExpType::SND), e(nullptr)
  {
    // empty
  }
  
  Snd(Exp * _e)
    : Exp(ExpType::SND), e(_e)
  {
    // empty
  }

  ~Snd()
  {
    destroy();
  }

  void destroy() override
  {
    delete e;
  }
  
  Exp * clone() override
  {
    return new Snd(e->clone());
  }
  
  Exp * eval(Enviroment & env) override
  {
    Exp * p = e->eval(env);

    if (p->get_type() != ExpType::PAIR)
      {
	delete p;
	throw domain_error("snd applied to non-pair");
      }
    
    Exp * ret_val = static_cast<Pair *>(p)->e2->clone();

    delete p;
    
    return ret_val;
  }

  string to_string() const override
  {
    stringstream s;
    s << "<snd, " << e->to_string() << ">";
    return s.str();
  }
};

struct Neg : public Exp
{
  Exp * e;

  Neg()
    : Exp(ExpType::NEG), e(nullptr)
  {
    // empty
  }
  
  Neg(Exp * _e)
    : Exp(ExpType::NEG), e(_e)
  {
    // empty
  }

  ~Neg()
  {
    destroy();
  }
  
  void destroy() override
  {
    delete e;
  }

  Exp * clone() override
  {
    return new Neg(e->clone());
  }
  
  Exp * eval(Enviroment & env) override
  {
    Exp * ee = e->eval(env);

    if (ee->get_type() != ExpType::INT)
      {
	delete ee;
	throw domain_error("neg applied to non-int");
      }
    
    int value = static_cast<Int *>(ee)->value;
    delete ee;
    return new Int(-value);
  }

  string to_string() const override
  {
    stringstream s;
    s << "<neg, " << e->to_string() << ">";
    return s.str();
  }
};

struct Def : public Exp
{
  string var_name;
  Exp * e;

  Def()
    : Exp(ExpType::DEF), var_name(""), e(nullptr)
  {
    // empty
  }

  Def(const string & v, Exp * _e)
    : Exp(ExpType::DEF), var_name(v), e(_e)
  {
    // empty
  }

  ~Def()
  {
    destroy();
  }

  Exp * clone() override
  {
    return new Def(var_name, e->clone());
  }
  
  Exp * eval(Enviroment & env) override
  {
    Exp * ee = e->eval(env);
    
    env.insert(make_tuple(var_name, ee));
    
    return new Void();
  }
  
  void destroy() override
  {
    // empty
  }

  string to_string() const override
  {
    stringstream s;
    s << "<def, " << var_name << ", " << e->to_string() << ">";
    return s.str();
  }
};

struct Var : public Exp
{
  string var_name;

  Var()
    : Exp(ExpType::VAR), var_name("")
  {
    // empty
  }
  
  Var(const string & n)
    : Exp(ExpType::VAR), var_name(n)
  {
    // empty
  }

  ~Var()
  {
    destroy();
  }

  Exp * clone() override
  {
    return new Var(var_name);
  }
  
  Exp * eval(Enviroment & env) override
  {
    Exp * e = envlookup(env, var_name);

    if (e == nullptr)
      throw domain_error("var does not exist");
    
    return e;
  }
  
  void destroy() override
  {
    // empty
  }

  string to_string() const override
  {
    stringstream s;
    s << "<var, " << var_name << ">";
    return s.str();
  }
};

struct Add : public Exp
{
  Exp * e1;
  Exp * e2;

  Add()
    : Exp(ExpType::ADD), e1(nullptr), e2(nullptr)
  {
    // empty
  }

  Add(Exp * _e1, Exp * _e2)
    : Exp(ExpType::ADD), e1(_e1), e2(_e2)
  {
    // empty
  }

  ~Add()
  {
    destroy();
  }
  
  void destroy() override
  {
    delete e1;
    delete e2;
  }

  Exp * clone() override
  {
    return new Add(e1->clone(), e2->clone());
  }
  
  Exp * eval(Enviroment & env) override
  {
    Exp * ee1 = e1->eval(env);
    Exp * ee2 = e2->eval(env);

    if (ee1->get_type() != ExpType::INT or ee2->get_type() != ExpType::INT)
      {
	delete ee1;
	delete ee2;
	throw domain_error("add applied to non-int");
      }

    int val = static_cast<Int *>(ee1)->value + static_cast<Int *>(ee2)->value;

    delete ee1;
    delete ee2;

    return new Int(val);
  }

  string to_string() const override
  {
    stringstream s;
    s << "<add, " << e1->to_string() << ", " << e2->to_string() << ">";
    return s.str();
  }
};

struct Mul : public Exp
{
  Exp * e1;
  Exp * e2;

  Mul()
    : Exp(ExpType::MUL), e1(nullptr), e2(nullptr)
  {
    // empty
  }

  Mul(Exp * _e1, Exp * _e2)
    : Exp(ExpType::MUL), e1(_e1), e2(_e2)
  {
    // empty
  }

  ~Mul()
  {
    destroy();
  }
  
  void destroy() override
  {
    delete e1;
    delete e2;
  }

  Exp * clone() override
  {
    return new Mul(e1->clone(), e2->clone());
  }

  Exp * eval(Enviroment & env) override
  {
    Exp * ee1 = e1->eval(env);
    Exp * ee2 = e2->eval(env);
    
    if (ee1->get_type() != ExpType::INT or ee2->get_type() != ExpType::INT)
      {
	delete ee1;
	delete ee2;
	throw domain_error("mul applied to non-int");
      }
    
    int val = static_cast<Int *>(ee1)->value * static_cast<Int *>(ee2)->value;
    
    delete ee1;
    delete ee2;
    
    return new Int(val);
  }

  string to_string() const override
  {
    stringstream s;
    s << "<mul, " << e1->to_string() << ", " << e2->to_string() << ">";
    return s.str();
  }
};

struct DivMod : public Exp
{
  Exp * e1;
  Exp * e2;

  DivMod()
    : Exp(ExpType::DIVMOD), e1(nullptr), e2(nullptr)
  {
    // empty
  }
  
  DivMod(Exp * _e1, Exp * _e2)
    : Exp(ExpType::DIVMOD), e1(_e1), e2(_e2)
  {
    // empty
  }

  ~DivMod()
  {
    destroy();
  }

  void destroy() override
  {
    delete e1;
    delete e2;
  }

  Exp * clone() override
  {
    return new DivMod(e1->clone(), e2->clone());
  }

  Exp * eval(Enviroment & env) override
  {
    Int * ee1 = static_cast<Int *>(e1->eval(env));
    Int * ee2 = static_cast<Int *>(e2->eval(env));

    if (ee1->get_type() != ExpType::INT or ee2->get_type() != ExpType::INT)
      {
	delete ee1;
	delete ee2;
	throw domain_error("divmod applied to non-int");
      }

    int div = ee1->value / ee2->value;
    int mod = ee1->value % ee2->value;
    
    delete ee1;
    delete ee2;
    
    return new Pair(new Int(div), new Int(mod));
  }

  string to_string() const override
  {
    stringstream s;
    s << "<divmod, " << e1->to_string() << ", " << e2->to_string() << ">";
    return s.str();
  }
};

struct Let : public Exp
{
  string var;
  Exp * e;
  Exp * body;

  Let()
    : Exp(ExpType::LET), e(nullptr), body(nullptr)
  {
    // empty
  }

  Let(const string & v, Exp * _e, Exp * b)
    : Exp(ExpType::LET), var(v), e(_e), body(b)
  {
    // empty
  }

  ~Let()
  {
    destroy();
  }
  
  void destroy() override
  {
    delete e;
    delete body;
  }

  Exp * clone() override
  {
    return new Let(var, e->clone(), body->clone());
  }

  Exp * eval(Enviroment & env) override
  {
    Exp * v = e->eval(env);

    Enviroment new_env = env;
    new_env.insert(make_tuple(var, v));
    return body->eval(new_env);
  }

  string to_string() const override
  {
    stringstream s;
    s << "<let, " << var << ", " << e->to_string() << ", "
      << body->to_string() << ">";
    return s.str();
  }
};

struct IfGreater : public Exp
{
  Exp * e1;
  Exp * e2;
  Exp * e3;
  Exp * e4;
  
  IfGreater()
    : Exp(ExpType::IFGREATER), e1(nullptr), e2(nullptr), e3(nullptr), e4(nullptr)
  {
    // empty
  }
  
  IfGreater(Exp * _e1, Exp * _e2, Exp * _e3, Exp * _e4)
    : Exp(ExpType::IFGREATER), e1(_e1), e2(_e2), e3(_e3), e4(_e4)
  {
    // empty
  }

  ~IfGreater()
  {
    destroy();
  }
  
  void destroy() override
  {
    delete e1;
    delete e2;
    delete e3;
    delete e4;
  }

  Exp * clone() override
  {
    return new IfGreater(e1->clone(), e2->clone(), e3->clone(), e4->clone());
  }

  Exp * eval(Enviroment & env) override
  {
    Exp * ee1 = e1->eval(env);
    Exp * ee2 = e2->eval(env);
    Exp * res = nullptr;

    if (ee1->get_type() != ExpType::INT or ee2->get_type() != ExpType::INT)
      {
	delete ee1;
	delete ee2;
	throw domain_error("ifgreater applied to non-int");
      }

    if (static_cast<Int *>(ee1)->value > static_cast<Int *>(ee2)->value)
      res = e3->eval(env);
    else
      res = e4->eval(env);

    delete ee1;
    delete ee2;
    return res;
  }

  string to_string() const override
  {
    stringstream s;
    s << "<ifgreater, " << e1->to_string() << ", " << e2->to_string() << ", "
      << e3->to_string() << ", " << e4->to_string() << ">";
    return s.str();
  }
};

struct Closure : public Exp
{
  Enviroment env;
  Exp * fun;
  
  Closure()
    : Exp(ExpType::CLOSURE), env(), fun(nullptr)
  {
    // empty
  }

  Closure(const Enviroment & e, Exp * f)
    : Exp(ExpType::CLOSURE), env(e), fun(f)
  {
    // empty
  }
  
  ~Closure()
  {
    destroy();
  }
  
  void destroy() override
  {
    delete fun;
  }
  
  Exp * clone() override
  {
    return new Closure(env, fun->clone());
  }
  
  Exp * eval(Enviroment &) override
  {
    return clone();
  }
  
  string to_string() const override
  {
    return "<closure>";
  }
};

struct Fun : public Exp
{
  string name;
  string formal;
  Exp * body;

  Fun()
    : Exp(ExpType::FUN), name(""), formal(""), body(nullptr)
  {
    // empty
  }

  Fun(const string & n, const string & f, Exp * b)
    : Exp(ExpType::FUN), name(n), formal(f), body(b)
  {
    // empty
  }

  ~Fun()
  {
    destroy();
  }

  void destroy() override
  {
    delete body;
  }
  
  Exp * clone() override
  {
    return new Fun(name, formal, body->clone());
  }
  
  Exp * eval(Enviroment & env) override
  {
    Closure * c = new Closure(env, clone());
    env.insert(make_tuple(name, c));
    return new Void();
  }
  
  string to_string() const override
  {
    stringstream s;
    s << "<fun, " << name << ", " << formal << ", " << body->to_string() << ">";
    return s.str();
  }
};

struct Call : public Exp
{
  string fname;
  Exp * actual;

  Call()
    : Exp(ExpType::CALL), fname(""), actual(nullptr)
  {
    // empty
  }

  Call(const string & n, Exp * e)
    : Exp(ExpType::CALL), fname(n), actual(e)
  {
    // empty
  }

  ~Call()
  {
    destroy();
  }
  
  void destroy() override
  {
    delete actual;
  }
  
  Exp * clone() override
  {
    return new Call(fname, actual->clone());
  }
  
  Exp * eval(Enviroment & env) override
  {
    Exp * c = envlookup(env, fname);

    if (c == nullptr)
      throw domain_error("function name does not exist");

    if (c->get_type() != ExpType::CLOSURE)
      {
	delete c;
	throw domain_error("call applied to non-closure");
      }

    Closure * closure = static_cast<Closure *>(c);

    Enviroment new_env = closure->env;
    Fun * fun = static_cast<Fun *>(closure->fun);

    new_env.insert(make_tuple(fun->name, c->clone()));
    new_env.insert(make_tuple(fun->formal, actual->eval(env)));

    Exp * ret = fun->body->eval(new_env);

    delete c;

    return ret;
  }
  
  string to_string() const override
  {
    stringstream s;
    s << "";
    return s.str();
  }
};  

Exp * parse(const string & p, int & pos)
{
  if (pos >= p.size())
    return nullptr;

  if (p[pos] != '<')
    return nullptr;

  if (p.substr(pos + 1, 4) == "void") 
    {
      pos += 6; 
      return new Void();
    }
  else
    {
      size_t end_name = p.find_first_of(',', pos);
      
      // Extraigo nombre de la exprsión
      string name = p.substr(pos + 1, end_name - pos - 1);
      
      pos = end_name + 1;
      
      if (name == "int") // Es una expresión entera
	{
	  Int * ret = new Int();
	  
	  // extraigo el valor entero
	  size_t end_val = p.find_first_of('>', end_name);

	  if (p[end_val] != '>')
	    {
	      delete ret;
	      return nullptr;
	    }

	  string val = p.substr(end_name + 1, end_val - end_name - 1);

	  if (not is_str_num(val))
	    {
	      delete ret;
	      return nullptr;
	    }

	  ret->value = stoi(val);

	  pos = end_val + 1;

	  return ret;
	}
      else if (name == "pair")
	{
	  Pair * ret = new Pair();

	  ret->e1 = parse(p, pos);

	  if (ret->e1 == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }

	  if (p[pos] != ',')
	    {
	      delete ret;
	      return nullptr;
	    }

	  ++pos;
      
	  ret->e2 = parse(p, pos);

	  if (ret->e2 == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }

	  if (p[pos] != '>')
	    {
	      delete ret;
	      return nullptr;
	    }

	  ++pos;
      
	  return ret;
	}
      else if (name == "fst")
	{
	  Fst * ret = new Fst();
	  ret->e = parse(p, pos);

	  if (ret->e == nullptr)
	    return nullptr;

	  if (p[pos] != '>')
	    {
	      delete ret;
	      return nullptr;
	    }
      
	  ++pos;
	  return ret;
	}
      else if (name == "snd")
	{
	  Snd * ret = new Snd();
	  ret->e = parse(p, pos);

	  if (ret->e == nullptr)
	    return nullptr;

	  if (p[pos] != '>')
	    {
	      delete ret;
	      return nullptr;
	    }
      
	  ++pos;
	  return ret;
	}
      else if (name == "def")
	{
	  Def * ret = new Def();

	  size_t end_val = p.find_first_of(',', end_name + 1);
	  
	  if (p[end_val] != ',')
	    {
	      delete ret;
	      return nullptr;
	    }

	  ret->var_name = p.substr(end_name + 1, end_val - end_name - 1);
	  pos = end_val + 1;

	  ret->e = parse(p, pos);

	  if (ret->e == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }
	  
	  if (p[pos] != '>')
	    {
	      
	      delete ret;
	      return nullptr;
	    }

	  ++pos;
	  
	  return ret;
	}
      else if (name == "var")
	{
	  Var * ret = new Var();
      
	  // extraigo el nombre de la variable
	  size_t end_val = p.find_first_of('>', end_name);

	  if (p[end_val] != '>')
	    {
	      delete ret;
	      return nullptr;
	    }
      
	  ret->var_name = p.substr(end_name + 1, end_val - end_name - 1);
	  pos = end_val + 1;
      
	  return ret;
	}
      else if (name == "isvoid")
	{
	  IsVoid * ret = new IsVoid();
	  ret->e = parse(p, pos);

	  if (ret->e == nullptr)
	    return nullptr;

	  if (p[pos] != '>')
	    {
	      delete ret;
	      return nullptr;
	    }
      
	  ++pos;
	  return ret;
	}
      else if (name == "neg")
	{
	  Neg * ret = new Neg();
	  ret->e = parse(p, pos);

	  if (ret->e == nullptr)
	    return nullptr;

	  if (p[pos] != '>')
	    {
	      delete ret;
	      return nullptr;
	    }
      
	  ++pos;
	  return ret;
	}
      else if (name == "add")
	{
	  Add * ret = new Add();

	  ret->e1 = parse(p, pos);

	  if (ret->e1 == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }

	  if (p[pos] != ',')
	    {
	      delete ret;
	      return nullptr;
	    }

	  ++pos;
      
	  ret->e2 = parse(p, pos);

	  if (ret->e2 == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }

	  if (p[pos] != '>')
	    {
	      delete ret;
	      return nullptr;
	    }
      
	  ++pos;

	  return ret;
	}
      else if (name == "mul")
	{
	  Mul * ret = new Mul();

	  ret->e1 = parse(p, pos);

	  if (ret->e1 == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }

	  if (p[pos] != ',')
	    {
	      delete ret;
	      return nullptr;
	    }

	  ++pos;
      
	  ret->e2 = parse(p, pos);

	  if (ret->e2 == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }

	  if (p[pos] != '>')
	    {
	      delete ret;
	      return nullptr;
	    }
      
	  ++pos;

	  return ret;
	}
      else if (name == "divmod")
	{
	  DivMod * ret = new DivMod();

	  ret->e1 = parse(p, pos);

	  if (ret->e1 == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }
	
	  if (p[pos] != ',')
	    {
	      delete ret;
	      return nullptr;
	    }

	  ++pos;
      
	  ret->e2 = parse(p, pos);

	  if (ret->e2 == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }

	  if (p[pos] != '>')
	    {
	      delete ret;
	      return nullptr;
	    }
      
	  ++pos;

	  return ret;
	}
      else if (name == "let")
	{
	  Let * ret = new Let();
      
	  // extraigo el nombre de la variable
	  size_t end_var = p.find_first_of(',', end_name + 1);

	  if (p[end_var] != ',')
	    {
	      delete ret;
	      return nullptr;
	    }

	  ret->var = p.substr(end_name + 1, end_var - end_name - 1);

	  pos = end_var + 1;

	  ret->e = parse(p, pos);

	  if (ret->e == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }

	  if (p[pos] != ',')
	    {
	      delete ret;
	      return nullptr;
	    }

	  ++pos;
      
	  ret->body = parse(p, pos);

	  if (ret->body == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }

	  if (p[pos] != '>')
	    {
	      delete ret;
	      return nullptr;
	    }
      
	  ++pos;
      
	  return ret;
	}
      else if (name == "ifgreater")
	{
	  IfGreater * ret = new IfGreater();

	  ret->e1 = parse(p, pos);

	  if (ret->e1 == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }

	  if (p[pos] != ',')
	    {
	      delete ret;
	      return nullptr;
	    }

	  ++pos;
      
	  ret->e2 = parse(p, pos);

	  if (ret->e2 == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }

	  if (p[pos] != ',')
	    {
	      delete ret;
	      return nullptr;
	    }

	  ++pos;
      
	  ret->e3 = parse(p, pos);

	  if (ret->e3 == nullptr)
	    {
	      cout << "null\n";
	      delete ret;
	      return nullptr;
	    }

	  if (p[pos] != ',')
	    {
	      cout << "coma\n";
	      delete ret;
	      return nullptr;
	    }

	  ++pos;
      
	  ret->e4 = parse(p, pos);

	  if (ret->e4 == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }

	  if (p[pos] != '>')
	    {
	      delete ret;
	      return nullptr;
	    }
      
	  ++pos;

	  return ret;
	}
      else if (name == "fun")
	{
	  Fun * ret = new Fun();

	  size_t end_fname = p.find_first_of(',', end_name + 1);
	  
	  if (p[end_fname] != ',')
	    {
	      delete ret;
	      return nullptr;
	    }

	  ret->name = p.substr(end_name + 1, end_fname - end_name - 1);

	  size_t end_val = p.find_first_of(',', end_fname + 1);

	  if (p[end_val] != ',')
	    {
	      delete ret;
	      return nullptr;
	    }
	  
	  ret->formal = p.substr(end_fname + 1, end_val - end_fname - 1);
	  
	  pos = end_val + 1;

	  ret->body = parse(p, pos);

	  if (ret->body == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }
	  
	  if (p[pos] != '>')
	    {
	      delete ret;
	      return nullptr;
	    }

	  ++pos;
	  
	  return ret;
	}
      else if (name == "call")
	{
	  Call * ret = new Call();

	  size_t end_val = p.find_first_of(',', end_name + 1);
	  
	  if (p[end_val] != ',')
	    {
	      delete ret;
	      return nullptr;
	    }

	  ret->fname = p.substr(end_name + 1, end_val - end_name - 1);
	  pos = end_val + 1;

	  ret->actual = parse(p, pos);

	  if (ret->actual == nullptr)
	    {
	      delete ret;
	      return nullptr;
	    }
	  
	  if (p[pos] != '>')
	    {
	      delete ret;
	      return nullptr;
	    }

	  ++pos;
	  
	  return ret;
	}
    }
  return nullptr;
}

Exp * parse(const string & prog)
{
  if (prog.empty())
    return nullptr;
  
  int p = 0;
  Exp * ret = parse(remove_whites(prog), p);

  if (ret == nullptr)
    throw logic_error("bad formed expression");

  return ret;
}
