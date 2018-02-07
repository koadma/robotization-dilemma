#include "Scripts.h"

/*//In:
//Root: NUMERIC number to convert
//Out:
//Root: STRING result
ScriptData* ScriptApiFunctions::num_to_str(ScriptData& _args) {
  ScriptData* res = new ScriptData();
  res->CopyContent(&_args);
  res->type = ScriptData::TSTRING;
  return res;
}

//In:
//Root: STRING number to convert
//Out:
//Root: NUMERIC result
ScriptData* ScriptApiFunctions::str_to_num(ScriptData& _args) {
  ScriptData* res = new ScriptData();
  res->CopyContent(&_args);
  res->type = ScriptData::TNUMERIC;
  return res;
}

//In:
//Root: Array of STRING, concat order lexographic (std::map)
//Out:
//Root: STRING result
ScriptData* ScriptApiFunctions::str_concat(ScriptData& _args) {
  ScriptData* res = new ScriptData();
  res->type = ScriptData::TSTRING;
  for (auto&& it : _args._elems) {
    res->_data.fromType<string>(res->_data.toType<string>() + it.second->_data.toType<string>());
  }
  return res;
}

//In:
//Root: STRING
//Out:
//Root: NUMERIC result, string length
ScriptData* ScriptApiFunctions::str_len(ScriptData& _args) {
  ScriptData* res = new ScriptData();
  res->type = ScriptData::TNUMERIC;
  res->_data.fromType<int>(_args._data.toType<string>().length());
  return res;
}

//In:
//str STRING
//i NUMERIC
//Out:
//str STRING
ScriptData* ScriptApiFunctions::str_index(ScriptData& _args) {
  ScriptData* res = new ScriptData();
  res->type = ScriptData::TNUMERIC;
  res->_data.fromType<char>(_args._elems["str"]->_data.toType<string>()[_args._elems["i"]->_data.toType<int>()]);
  return res;
}

//In:
//a NUMERIC
//b NUMERIC
//Out:
//r NUMERIC
ScriptData* ScriptApiFunctions::num_rand(ScriptData& _args) {
  ScriptData* res = new ScriptData();
  res->type = ScriptData::TNUMERIC;
  double a = _args._elems["a"]->_data.toType<double>();
  double b = _args._elems["b"]->_data.toType<double>();
  res->_data.fromType<double>(a + (b - a)*ran1());
  return res;
}*/

ScriptData::ScriptData() {
  _instances = 1;
}
/*ScriptData* ScriptData::copy() {
  ScriptData* nval = new ScriptData();
  nval->_instances = 1;
  nval->type = type;
  nval->_data._len = _data._len;
  nval->_data._data = new unsigned char[_data._len];
  for (int i = 0; i < _data._len; i++) {
    nval->_data._data[i] = _data._data[i];
  }
  for (auto&& it : _elems) {
    nval->_elems.insert({ it.first, it.second->copy()});
  }
  return nval;
}*/
void ScriptData::CopyContent(ScriptData* _from) {
  if (_data != NULL) {
    delete _data;
    _data = NULL;
  }
  switch (_from->_data->type())
  {
  case ScriptDataBase::TNULL:
    _data = NULL;
    break;
  case ScriptDataBase::TNUMERIC:
    _data = new ScriptDataNumber(((ScriptDataNumber*)_from->_data)->_num);
    break;
  case ScriptDataBase::TSTRING:
    _data = new ScriptDataString(((ScriptDataString*)_from->_data)->_str);
    break;
  case ScriptDataBase::TCHAR:
    _data = new ScriptDataChar(((ScriptDataChar*)_from->_data)->_chr);
    break;
  case ScriptDataBase::TBOOLEAN:
    _data = new ScriptDataBool(((ScriptDataBool*)_from->_data)->_bl);
    break;
  case ScriptDataBase::TARRAY:
    _data = NULL;
    break;
  case ScriptDataBase::TVECTOR:
    _data = new ScriptDataVector(((ScriptDataVector*)_from->_data)->_vec);
    break;
  default:
    throw 1;
    _data = NULL;
    break;
  }
  while (_elems.size()) {
    DeletePtr(_elems.begin()->second);
    _elems.erase(_elems.begin());
  }
  for (auto&& it : _from->_elems) {
    ScriptData* nval = new ScriptData();
    nval->CopyContent(it.second);
    _elems.insert({ it.first, nval });
  }
}
ScriptData::~ScriptData() {
  delete _data;
  if (_instances != 0) {
    throw 1;
  }
  while (_elems.size()) {
    DeletePtr(_elems.begin()->second);
    _elems.erase(_elems.begin());
  }
}


ScriptData* CopyPtr(ScriptData* _in) {
  if (_in != NULL) {
    _in->_instances += 1;
  }
  return _in;
}
bool DeletePtr(ScriptData* _in) {
  if (_in != NULL) {
    _in->_instances -= 1;
    if (_in->_instances == 0) {
      delete _in;
      return true;
    }
  }
  return false;
}
map<string, APICall> apiMap;


ScriptData* ScriptInstruction::run(ScriptData& _args) {
  throw 1;
  return NULL;
}
void ScriptInstruction::load(xml_node<> *data) {
  throw 1;
}

ScriptData* ScriptIIfElse::run(ScriptData& _args) {
  ScriptData* condition = _condition->run(_args);
  if (condition->_data->type() == ScriptDataBase::TBOOLEAN) {
    if (((ScriptDataBool*)condition->_data)->_bl) {
      DeletePtr(condition);
      return _then->run(_args);
    }
    else {
      DeletePtr(condition);
      return _else->run(_args);
    }
  }
}
void ScriptIIfElse::load(xml_node<> *data) {
  xml_node<> *pElem = data->first_node("cond");
  ScriptIBlock* condition = new ScriptIBlock;
  if (pElem) {
    condition->load(pElem);
  }
  else {
    ScriptIConstant* _false = new ScriptIConstant();
    _false->_val->_data = new ScriptDataBool(false);
    condition->_instructions.push_back(_false);
  }

  if (_condition != NULL) {
    delete _condition;
  }
  _condition = condition;

  pElem = data->first_node("then");
  ScriptIBlock* then = new ScriptIBlock();
  if (pElem) {
    then->load(pElem);
  }
  
  if (_then != NULL) {
    delete _then;
  }
  _then = then;

  pElem = data->first_node("else");
  ScriptIBlock* elsen = new ScriptIBlock();
  if (pElem) {
    elsen->load(pElem);
  }

  if (_else != NULL) {
    delete _else;
  }
  _else = elsen;
}
ScriptIIfElse::~ScriptIIfElse() {
  delete _then;
  delete _else;
  delete _condition;
}

ScriptData* ScriptILoop::run(ScriptData& _args) {
  ScriptData* condition = _condition->run(_args);
  while (condition->_data->type() == ScriptDataBase::TBOOLEAN && ((ScriptDataBool*)condition->_data)->_bl) {
    ScriptData* insres = _code->run(_args);
    if (insres != NULL) {
      DeletePtr(condition);
      return insres;
    }
    DeletePtr(condition);
    condition = _condition->run(_args);
  }
  DeletePtr(condition);
  return NULL;
}
void ScriptILoop::load(xml_node<> *data) {
  xml_node<> *pElem = data->first_node("cond");
  ScriptIBlock* condition = new ScriptIBlock;
  if (pElem) {
    condition->load(pElem);
  }
  else {
    ScriptIConstant* _false = new ScriptIConstant();
    _false->_val->_data = new ScriptDataBool(false);
    condition->_instructions.push_back(_false);
  }

  if (_condition != NULL) {
    delete _condition;
  }
  _condition = condition;

  pElem = data->first_node("then");
  ScriptIBlock* then = new ScriptIBlock();
  if (pElem) {
    then->load(pElem);
  }

  if (_code != NULL) {
    delete _code;
  }
  _code = then;
}
ScriptILoop::~ScriptILoop() {
  delete _code;
  delete _condition;
}

ScriptData* ScriptIAssign::run(ScriptData& _args) {
  ScriptData* val = _val->run(_args);
  ScriptData* to = _to->run(_args);
  to->CopyContent(val);
  DeletePtr(to);
  DeletePtr(val);

  return NULL;
}
void ScriptIAssign::load(xml_node<> *data) {
  xml_node<>* pElem = data->first_node("to");
  ScriptIBlock* to = new ScriptIBlock();
  if (pElem) {
    to->load(pElem);
  }

  if (_to != NULL) {
    delete _to;
  }
  _to = to;

  pElem = data->first_node("val");
  ScriptIBlock* val = new ScriptIBlock();
  if (pElem) {
    val->load(pElem);
  }

  if (_val != NULL) {
    delete _val;
  }
  _val = val;
}
ScriptIAssign::~ScriptIAssign() {
  delete _val;
  delete _to;
}

ScriptData* ScriptICopy::run(ScriptData& _args) {
  if (_args._elems.count(_to)) {
    DeletePtr(_args._elems[_to]);
  }
  _args._elems[_to]->CopyContent(_args._elems[_from]);

  return NULL;
}
void ScriptICopy::load(xml_node<> *data) {
  _to = data->first_attribute("to")->value();
  _from = data->first_attribute("from")->value();
}
ScriptICopy::~ScriptICopy() {
}

ScriptData* ScriptIConstant::run(ScriptData& _args) {
  ScriptData* res = new ScriptData();
  res->CopyContent(_val);
  return res;
}
void ScriptIConstant::load(xml_node<> *data) {
  if (_val != NULL) {
    delete _val;
  }
  _val = new ScriptData();
  string type = data->first_attribute("type")->value();
  if (type == "num") {
    _val->_data = new ScriptDataNumber(strTo<double>(data->value()));
  }
  if (type == "bool") {
    _val->_data = new ScriptDataBool(strTo<bool>(data->value()));
  }
  if (type == "str") {
    _val->_data = new ScriptDataString(strTo<string>(data->value()));
  }
  if (type == "char") {
    _val->_data = new ScriptDataChar(strTo<char>(data->value()));
  }
}
ScriptIConstant::~ScriptIConstant() {
  DeletePtr(_val);
}

ScriptData* ScriptIMath::run(ScriptData& _args) {
  ScriptData* res1 = _arg1->run(_args);
  if (res1->_data->type() != ScriptDataBase::TNUMERIC) {
    throw 1;
    return NULL;
  }
  ScriptData* res2 = NULL;
  if (_oper < 16) {
    res2 = _arg2->run(_args);
    if (res2->_data->type() != ScriptDataBase::TNUMERIC) {
      throw 1;
      return NULL;
    }
  }
  ScriptData* s = new ScriptData();
  switch (_oper) {
  case PLUS:
    s->_data = new ScriptDataNumber(((ScriptDataNumber*)(res1->_data))->_num + ((ScriptDataNumber*)(res2->_data))->_num);
    break;
  case MINUS:
    s->_data = new ScriptDataNumber(((ScriptDataNumber*)(res1->_data))->_num - ((ScriptDataNumber*)(res2->_data))->_num);
    break;
  case MULTI:
    s->_data = new ScriptDataNumber(((ScriptDataNumber*)(res1->_data))->_num * ((ScriptDataNumber*)(res2->_data))->_num);
    break;
  case DIV:
    s->_data = new ScriptDataNumber(((ScriptDataNumber*)(res1->_data))->_num / ((ScriptDataNumber*)(res2->_data))->_num);
    break;
  case POW:
    s->_data = new ScriptDataNumber(pow(((ScriptDataNumber*)(res1->_data))->_num , ((ScriptDataNumber*)(res2->_data))->_num));
    break;
  case LOG:
    s->_data = new ScriptDataNumber(log(((ScriptDataNumber*)(res1->_data))->_num) + log(((ScriptDataNumber*)(res2->_data))->_num));
    break;
  case MOD:
    s->_data = new ScriptDataNumber(modulo(((ScriptDataNumber*)(res1->_data))->_num, ((ScriptDataNumber*)(res2->_data))->_num));
    break;
  case ATAN2:
    s->_data = new ScriptDataNumber(atan2(((ScriptDataNumber*)(res1->_data))->_num, ((ScriptDataNumber*)(res2->_data))->_num));
    break;
  case L:
    s->_data = new ScriptDataBool(((ScriptDataNumber*)(res1->_data))->_num < ((ScriptDataNumber*)(res2->_data))->_num);
    break;
  case LEQ:
    s->_data = new ScriptDataBool(((ScriptDataNumber*)(res1->_data))->_num <= ((ScriptDataNumber*)(res2->_data))->_num);
    break;
  case EQ:
    s->_data = new ScriptDataBool(((ScriptDataNumber*)(res1->_data))->_num == ((ScriptDataNumber*)(res2->_data))->_num);
    break;
  case NEQ:
    s->_data = new ScriptDataBool(((ScriptDataNumber*)(res1->_data))->_num != ((ScriptDataNumber*)(res2->_data))->_num);
    break;
  case GEQ:
    s->_data = new ScriptDataBool(((ScriptDataNumber*)(res1->_data))->_num >= ((ScriptDataNumber*)(res2->_data))->_num);
    break;
  case G:
    s->_data = new ScriptDataBool(((ScriptDataNumber*)(res1->_data))->_num > ((ScriptDataNumber*)(res2->_data))->_num);
    break;
  case SQRT:
    s->_data = new ScriptDataNumber(sqrt(((ScriptDataNumber*)(res1->_data))->_num));
    break;
  case SIN:
    s->_data = new ScriptDataNumber(sin(((ScriptDataNumber*)(res1->_data))->_num));
    break;
  case COS:
    s->_data = new ScriptDataNumber(cos(((ScriptDataNumber*)(res1->_data))->_num));
    break;
  case TAN:
    s->_data = new ScriptDataNumber(tan(((ScriptDataNumber*)(res1->_data))->_num));
    break;
  case ASIN:
    s->_data = new ScriptDataNumber(asin(((ScriptDataNumber*)(res1->_data))->_num));
    break;
  case ACOS:
    s->_data = new ScriptDataNumber(acos(((ScriptDataNumber*)(res1->_data))->_num));
    break;
  case ATAN:
    s->_data = new ScriptDataNumber(atan(((ScriptDataNumber*)(res1->_data))->_num));
    break;
  }
  DeletePtr(res1);
  DeletePtr(res2);
  return s;
}
void ScriptIMath::load(xml_node<> *data) {
  xml_attribute<> *opern = data->first_attribute("oper");
  if(opern) {
    string oper = opern->value();
    if(oper == "+") {
      _oper = PLUS;
    }
    if (oper == "-") {
      _oper = MINUS;
    }
    if (oper == "*") {
      _oper = MULTI;
    }
    if (oper == "/") {
      _oper = DIV;
    }
    if (oper == "^") {
      _oper = POW;
    }
    if (oper == "log") {
      _oper = LOG;
    }
    if (oper == "%") {
      _oper = MOD;
    }
    if (oper == "atan2") {
      _oper = ATAN2;
    }
    if (oper == "l") {
      _oper = L;
    }
    if (oper == "lew") {
      _oper = LEQ;
    }
    if (oper == "eq") {
      _oper = EQ;
    }
    if (oper == "neq") {
      _oper = NEQ;
    }
    if (oper == "geq") {
      _oper = GEQ;
    }
    if (oper == "g") {
      _oper = G;
    }
    if (oper == "sqrt") {
      _oper = SQRT;
    }
    if (oper == "sin") {
      _oper = SIN;
    }
    if (oper == "cos") {
      _oper = COS;
    }
    if (oper == "tan") {
      _oper = TAN;
    }
    if (oper == "asin") {
      _oper = ASIN;
    }
    if (oper == "acos") {
      _oper = ACOS;
    }
    if (oper == "atan") {
      _oper = ATAN;
    }
  }

  xml_node<>* pElem = data->first_node("lhs");
  ScriptIBlock* lhs = new ScriptIBlock();
  if (pElem) {
    lhs->load(pElem);
  }

  if (_arg1 != NULL) {
    delete _arg1;
  }
  _arg1 = lhs;

  pElem = data->first_node("rhs");
  ScriptIBlock* rhs = new ScriptIBlock();
  if (pElem) {
    rhs->load(pElem);
  }

  if (_arg2 != NULL) {
    delete _arg2;
  }
  _arg2 = rhs;
}
ScriptIMath::~ScriptIMath() {
  delete _arg1;
  delete _arg2;
}

ScriptData* ScriptILogic::run(ScriptData& _args) {
  ScriptData* res1 = _arg1->run(_args);
  if (res1->_data->type() != ScriptDataBase::TBOOLEAN) {
    throw 1;
    return NULL;
  }
  ScriptData* res2 = NULL;
  if (_oper < 16) {
    res2 = _arg2->run(_args);
    if (res2->_data->type() != ScriptDataBase::TBOOLEAN) {
      throw 1;
      return NULL;
    }
  }
  ScriptData* s = new ScriptData();
  switch (_oper) {
  case EQ:
    s->_data = new ScriptDataBool(((ScriptDataBool*)(res1->_data))->_bl == ((ScriptDataBool*)(res2->_data))->_bl);
    break;
  case NEQ:
    s->_data = new ScriptDataBool(((ScriptDataBool*)(res1->_data))->_bl != ((ScriptDataBool*)(res2->_data))->_bl);
    break;
  case AND:
    s->_data = new ScriptDataBool(((ScriptDataBool*)(res1->_data))->_bl & ((ScriptDataBool*)(res2->_data))->_bl);
    break;
  case NAND:
    s->_data = new ScriptDataBool(!(((ScriptDataBool*)(res1->_data))->_bl & ((ScriptDataBool*)(res2->_data))->_bl));
    break;
  case OR:
    s->_data = new ScriptDataBool(((ScriptDataBool*)(res1->_data))->_bl | ((ScriptDataBool*)(res2->_data))->_bl);
    break;
  case NOR:
    s->_data = new ScriptDataBool(!(((ScriptDataBool*)(res1->_data))->_bl | ((ScriptDataBool*)(res2->_data))->_bl));
    break;
  case XOR:
    s->_data = new ScriptDataBool(((ScriptDataBool*)(res1->_data))->_bl ^ ((ScriptDataBool*)(res2->_data))->_bl);
    break;
  case NXOR:
    s->_data = new ScriptDataBool(!(((ScriptDataBool*)(res1->_data))->_bl ^ ((ScriptDataBool*)(res2->_data))->_bl));
    break;
  case NOT:
    s->_data = new ScriptDataBool(!((ScriptDataBool*)(res1->_data))->_bl);
    break;
  }
  DeletePtr(res1);
  DeletePtr(res2);
  return s;
}
void ScriptILogic::load(xml_node<> *data) {
  xml_attribute<> *opern = data->first_attribute("oper");
  if (opern) {
    string oper = opern->value();
    if (oper == "eq") {
      _oper = EQ;
    }
    if (oper == "neq") {
      _oper = NEQ;
    }
    if (oper == "and") {
      _oper = AND;
    }
    if (oper == "nand") {
      _oper = NAND;
    }
    if (oper == "or") {
      _oper = OR;
    }
    if (oper == "nor") {
      _oper = NOR;
    }
    if (oper == "xor") {
      _oper = XOR;
    }
    if (oper == "nxor") {
      _oper = NXOR;
    }
    if (oper == "not") {
      _oper = NOT;
    }
  }

  xml_node<>* pElem = data->first_node("lhs");
  ScriptIBlock* lhs = new ScriptIBlock();
  if (pElem) {
    lhs->load(pElem);
  }

  if (_arg1 != NULL) {
    delete _arg1;
  }
  _arg1 = lhs;

  pElem = data->first_node("rhs");
  ScriptIBlock* rhs = new ScriptIBlock();
  if (pElem) {
    rhs->load(pElem);
  }

  if (_arg2 != NULL) {
    delete _arg2;
  }
  _arg2 = rhs;
}
ScriptILogic::~ScriptILogic() {
  delete _arg1;
  delete _arg2;
}

ScriptData* ScriptIVariable::run(ScriptData& _args) {
  ScriptData* res;
  if (_args._elems.count(_arg) == 0) {
    _args._elems[_arg] = new ScriptData();
  }
  res = CopyPtr(_args._elems[_arg]);
  return res;
}
void ScriptIVariable::load(xml_node<> *data) {
  _arg = data->first_attribute("var")->value();
}
ScriptIVariable::~ScriptIVariable() {
}

ScriptData* ScriptIIndex::run(ScriptData& _args) {
  ScriptData* ind = _ind->run(_args);
  ScriptData* arg = _arg->run(_args);
  string index = ind->_data->getString();
  ScriptData* res;
  if (arg->_elems.count(index) == 0) {
    arg->_elems[index] = new ScriptData();
  }
  res = CopyPtr(arg->_elems[index]);
  DeletePtr(ind);
  DeletePtr(arg);
  return res;
}
void ScriptIIndex::load(xml_node<> *data) {
  xml_node<>* pElem = data->first_node("arg");
  ScriptIBlock* arg = new ScriptIBlock();
  if (pElem) {
    arg->load(pElem);
  }

  if (_arg != NULL) {
    delete _arg;
  }
  _arg = arg;

  pElem = data->first_node("ind");
  ScriptIBlock* ind = new ScriptIBlock();
  if (pElem) {
    ind->load(pElem);
  }

  if (_ind != NULL) {
    delete _ind;
  }
  _ind = ind;
}
ScriptIIndex::~ScriptIIndex() {
  delete _arg;
  delete _ind;
}

ScriptData* ScriptIFunctionCall::run(ScriptData& _args) {
  ScriptData _nargs;
  for (auto&& it : _arguments) {
    ScriptData* _nval;
    _nval = it.second->run(_args);
    _nargs._elems.insert({ it.first, _nval });
  }
  ScriptData* res = _function->run(_nargs);
  return res;
}
void ScriptIFunctionCall::load(xml_node<> *data) {
  xml_node<> *pElem = data->first_node("func");
  ScriptIBlock* function = new ScriptIBlock();
  if (pElem) {
    function->load(pElem);
  }

  if (_function != NULL) {
    delete _function;
  }
  _function = function;

  for (xml_node<> *pElem = data->first_node("arg"); pElem; pElem = pElem->next_sibling("arg")) {
    ScriptIBlock* b = new ScriptIBlock();
    b->load(pElem);
    _arguments.push_back({pElem->first_attribute("name")->value(), b});
  }
}
ScriptIFunctionCall::~ScriptIFunctionCall() {
  delete _function;
  while (_arguments.size()) {
    delete _arguments.begin()->second;
    _arguments.erase(_arguments.begin());
  }
}

void ScriptIAPICall::load(xml_node<> *data) {
  _func = apiMap[data->first_attribute("name")->value()];

  for (xml_node<> *pElem = data->first_node("arg"); pElem; pElem = pElem->next_sibling("arg")) {
    ScriptIBlock* b = new ScriptIBlock();
    b->load(pElem);
    _arguments.push_back({ pElem->first_attribute("name")->value(), b });
  }
}
ScriptData* ScriptIAPICall::run(ScriptData& _args) {
  ScriptData _nargs;
  for (auto&& it : _arguments) {
    ScriptData* _nval;
    _nval = it.second->run(_args);
    _nargs._elems.insert({ it.first, _nval });
  }
  return _func(_nargs);
}

ScriptData* ScriptIBlock::run(ScriptData& _args) {
  ScriptData* res = NULL;
  for (auto&& it : _instructions) {
    res = it->run(_args);
    if (res != NULL) {
      break;
    }
  }
  return res;
}
void ScriptIBlock::load(xml_node<> *data) {
  while (_instructions.size()) {
    delete *(_instructions.begin());
    _instructions.erase(_instructions.begin());
  }
  for (xml_node<> *pElem = data->first_node(); pElem; pElem = pElem->next_sibling()) {
    string name = pElem->name();
    if (name == "ifelse") {
      ScriptInstruction* nins = new ScriptIIfElse();
      nins->load(pElem);
      _instructions.push_back(nins);
    }
    if (name == "loop") {
      ScriptInstruction* nins = new ScriptILoop();
      nins->load(pElem);
      _instructions.push_back(nins);
    }
    if (name == "assign") {
      ScriptInstruction* nins = new ScriptIAssign();
      nins->load(pElem);
      _instructions.push_back(nins);
    }
    if (name == "copy") {
      ScriptInstruction* nins = new ScriptICopy();
      nins->load(pElem);
      _instructions.push_back(nins);
    }
    if (name == "constant") {
      ScriptInstruction* nins = new ScriptIConstant();
      nins->load(pElem);
      _instructions.push_back(nins);
    }
    if (name == "math") {
      ScriptInstruction* nins = new ScriptIMath();
      nins->load(pElem);
      _instructions.push_back(nins);
    }
    if (name == "logic") {
      ScriptInstruction* nins = new ScriptILogic();
      nins->load(pElem);
      _instructions.push_back(nins);
    }
    if (name == "var") {
      ScriptInstruction* nins = new ScriptIVariable();
      nins->load(pElem);
      _instructions.push_back(nins);
    }
    if (name == "index") {
      ScriptInstruction* nins = new ScriptIIndex();
      nins->load(pElem);
      _instructions.push_back(nins);
    }
    if (name == "func") {
      ScriptInstruction* nins = new ScriptIFunctionCall();
      nins->load(pElem);
      _instructions.push_back(nins);
    }
    if (name == "api") {
      ScriptInstruction* nins = new ScriptIAPICall();
      nins->load(pElem);
      _instructions.push_back(nins);
    }
    if (name == "block") {
      ScriptInstruction* nins = new ScriptIBlock();
      nins->load(pElem);
      _instructions.push_back(nins);
    }
  }
}
ScriptIBlock::~ScriptIBlock() {
  while (_instructions.size()) {
    delete *(_instructions.begin());
    _instructions.erase(_instructions.begin());
  }
}