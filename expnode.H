/**

   @author: Nombre de un autor.
   @id:     Cédula del autor.

   @author: Nombre del otro autor.
   @id:     Cédula del autor.

*/

// Deja esta línea tal como está
struct Enviroment;

// No modifiques este tipo
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

// No modifiques el siguiente alias
using VarList = SLList<tuple<string, Exp *>>;

// No modifiques el siguiente tipo
struct Enviroment : public VarList
{
  Enviroment()
    : VarList()
  {
    // Empty
  }
  
  Enviroment(Enviroment & env)
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

/* Busca en el conjunto de variables, la expresión asociada al nombre pasado
   como parámetro. Si no existe la variable, retorna nullptr.
  
   Es útil al evaluar el tipo Var. No la modifiques.
*/
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

// Se te provee solucionada
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
    return new Void();
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
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
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
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
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
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
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
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
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
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
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
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
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

  void destroy() override
  {
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
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

  void destroy() override
  {
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
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
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
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
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
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
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
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
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
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
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
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

  Closure(Enviroment & e, Exp * f)
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
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
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
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
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
    // Programa aquí la liberación de memoria de todos los hijos de this
  }

  Exp * clone() override
  {
    /* Programa aquí la creación de un nuevo objeto idéntico a this y retorna
       un valor adecuado.
    */
    return nullptr;
  }
  
  Exp * eval(Enviroment & env) override
  {
    // Programa aquí cómo se evalúa esta expresión y retorna un valor adecuado.
    return nullptr;
  }
  
  string to_string() const override
  {
    stringstream s;
    s << "";
    return s.str();
  }
};  

Exp * parse(const string & prog)
{
  /* Programa aquí tu rutina para convertir la cadena de un programa en 
     un árbol de expresiones en la cual retornes la raíz.
  */
  return nullptr;
}
