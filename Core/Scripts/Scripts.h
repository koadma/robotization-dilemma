#pragma once

#include "../Network/Network.h"

class ScriptData {
public:
  map<string, ScriptData*> _elems;
  DataPair _data;
  enum VarType {
    TNULL = 0, //Nothing
    TNUMERIC = 1,
    TSTRING = 2,
    TBOOLEAN = 3,
    TARRAY = 4,
    TVECTOR = 5,
    TDATA = 6
  };
  size_t _instances;
  VarType type;
  ScriptData();
  //ScriptData* copy();
  void CopyContent(ScriptData* _from);
  ~ScriptData();
};

ScriptData* CopyPtr(ScriptData* _in);
bool DeletePtr(ScriptData* _in);

class ScriptInstruction {
public:
  virtual ScriptData* run(ScriptData& _args) {
    throw 1;
    return NULL;
  }
};

class ScriptIBlock : public ScriptInstruction {
public:
  list<ScriptInstruction*> _instructions;
  ScriptData* run(ScriptData& _args) {
    ScriptData* res = NULL;
    for (auto&& it : _instructions) {
      res = it->run(_args);
      if (res != NULL) {
        break;
      }
    }
    return res;
  }
  ~ScriptIBlock() {
    while (_instructions.size()) {
      delete *(_instructions.begin());
      _instructions.erase(_instructions.begin());
    }
  }
};

class ScriptIIfElse : public ScriptInstruction {
public:
  ScriptInstruction* _if;
  ScriptInstruction* _else;
  ScriptInstruction* _condition;
  ScriptData* run(ScriptData& _args) {
    ScriptData* condition = _condition->run(_args);
    if (condition->type == ScriptData::TBOOLEAN) {
      if (condition->_data.toType<bool>()) {
        DeletePtr(condition);
        return _if->run(_args);
      }
      else {
        DeletePtr(condition);
        return _else->run(_args);
      }
    }
  }
  ~ScriptIIfElse() {
    delete _if;
    delete _else;
    delete _condition;
  }
};

class ScriptILoop : public ScriptInstruction {
public:
  ScriptInstruction* _code;
  ScriptInstruction* _condition;
  ScriptData* run(ScriptData& _args) {
    ScriptData* condition = _condition->run(_args);
    if (condition->type == ScriptData::TBOOLEAN) {
      while (condition->type == ScriptData::TBOOLEAN && condition->_data.toType<bool>()) {
        ScriptData* insres = _code->run(_args);
        if (insres != NULL) {
          DeletePtr(condition);
          return insres;
        }
        DeletePtr(condition);
        condition = _condition->run(_args);
      }
    }
    DeletePtr(condition);
    return NULL;
  }
  ~ScriptILoop() {
    delete _code;
    delete _condition;
  }
};

class ScriptIAssign : public ScriptInstruction {
public:
  ScriptInstruction* _to;
  ScriptInstruction* _val;
  ScriptData* run(ScriptData& _args) {
    ScriptData* val = _val->run(_args);
    ScriptData* to = _to->run(_args);
    to->CopyContent(val);
    DeletePtr(to);
    DeletePtr(val);

    return NULL;
  }
  ~ScriptIAssign() {
    delete _val;
    delete _to;
  }
};

class ScriptICopy : public ScriptInstruction {
public:
  string _to;
  string _from;
  ScriptData* run(ScriptData& _args) {
    if (_args._elems.count(_to)) {
      DeletePtr(_args._elems[_to]);
    }
    _args._elems[_to]->CopyContent(_args._elems[_from]);

    return NULL;
  }
  ~ScriptICopy() {
  }
};

class ScriptIConstant : public ScriptInstruction {
public:
  ScriptData* _val;
  ScriptData* run(ScriptData& _args) {
    ScriptData* res = new ScriptData();
    res->CopyContent(_val);
    return res;
  }
  ~ScriptIConstant() {
    delete _val;
  }
};

class ScriptIMath : public ScriptInstruction {
public:
  ScriptInstruction* _arg1;
  ScriptInstruction* _arg2;
  enum Operation {
    PLUS = 1,
    MINUS = 2,
    MULTI = 3,
    DIV = 4,
    POW = 5,
    LOG = 6,
    MOD = 7,
    ATAN2 = 8,
    L = 9,
    LEQ = 10,
    EQ = 11,
    NEQ = 12,
    GEQ = 13,
    G = 14,
    SQRT = 16,
    SIN = 17,
    COS = 18,
    TAN = 19,
    ASIN = 20,
    ACOS = 21,
    ATAN = 22
  };
  Operation _oper;
  ScriptData* run(ScriptData& _args) {
    ScriptData* res1 = _arg1->run(_args);
    if (res1->type != ScriptData::TNUMERIC) {
      throw 1;
      return NULL;
    }
    ScriptData* res2 = NULL;
    if (_oper < 16) {
      res2 = _arg2->run(_args);
      if (res2->type != ScriptData::TNUMERIC) {
        throw 1;
        return NULL;
      }
    }
    ScriptData* s = new ScriptData();
    switch (_oper) {
      case PLUS:
        s->type = ScriptData::TNUMERIC;
        s->_data.fromType<double>(res1->_data.toType<double>() + res2->_data.toType<double>());
        break;
      case MINUS:
        s->type = ScriptData::TNUMERIC;
        s->_data.fromType<double>(res1->_data.toType<double>() - res2->_data.toType<double>());
        break;
      case MULTI:
        s->type = ScriptData::TNUMERIC;
        s->_data.fromType<double>(res1->_data.toType<double>() * res2->_data.toType<double>());
        break;
      case DIV:
        s->type = ScriptData::TNUMERIC;
        s->_data.fromType<double>(res1->_data.toType<double>() / res2->_data.toType<double>());
        break;
      case POW:
        s->type = ScriptData::TNUMERIC;
        s->_data.fromType<double>(pow(res1->_data.toType<double>(), res2->_data.toType<double>()));
        break;
      case LOG:
        s->type = ScriptData::TNUMERIC;
        s->_data.fromType<double>(log(res1->_data.toType<double>()) / log(res2->_data.toType<double>()));
        break;
      case MOD:
        s->type = ScriptData::TNUMERIC;
        s->_data.fromType<double>(fmodf(res1->_data.toType<double>(), res2->_data.toType<double>()));
        break;
      case ATAN2:
        s->type = ScriptData::TNUMERIC;
        s->_data.fromType<double>(atan2(res1->_data.toType<double>(), res2->_data.toType<double>()));
        break;
      case L:
        s->type = ScriptData::TBOOLEAN;
        s->_data.fromType<bool>(res1->_data.toType<double>() < res2->_data.toType<double>());
        break;
      case LEQ:
        s->type = ScriptData::TBOOLEAN;
        s->_data.fromType<bool>(res1->_data.toType<double>() <= res2->_data.toType<double>());
        break;
      case EQ:
        s->type = ScriptData::TBOOLEAN;
        s->_data.fromType<bool>(res1->_data.toType<double>() == res2->_data.toType<double>());
        break;
      case NEQ:
        s->type = ScriptData::TBOOLEAN;
        s->_data.fromType<bool>(res1->_data.toType<double>() != res2->_data.toType<double>());
        break;
      case GEQ:
        s->type = ScriptData::TBOOLEAN;
        s->_data.fromType<bool>(res1->_data.toType<double>() >= res2->_data.toType<double>());
        break;
      case G:
        s->type = ScriptData::TBOOLEAN;
        s->_data.fromType<bool>(res1->_data.toType<double>() > res2->_data.toType<double>());
        break;
      case SQRT:
        s->type = ScriptData::TNUMERIC;
        s->_data.fromType<double>(sqrt(res1->_data.toType<double>()));
        break;
      case SIN:
        s->type = ScriptData::TNUMERIC;
        s->_data.fromType<double>(sin(res1->_data.toType<double>()));
        break;
      case COS:
        s->type = ScriptData::TNUMERIC;
        s->_data.fromType<double>(cos(res1->_data.toType<double>()));
        break;
      case TAN:
        s->type = ScriptData::TNUMERIC;
        s->_data.fromType<double>(tan(res1->_data.toType<double>()));
        break;
      case ASIN:
        s->type = ScriptData::TNUMERIC;
        s->_data.fromType<double>(asin(res1->_data.toType<double>()));
        break;
      case ACOS:
        s->type = ScriptData::TNUMERIC;
        s->_data.fromType<double>(acos(res1->_data.toType<double>()));
        break;
      case ATAN:
        s->type = ScriptData::TNUMERIC;
        s->_data.fromType<double>(atan(res1->_data.toType<double>()));
        break;
    }
    DeletePtr(res1);
    DeletePtr(res2);
    return s;
  }
  ~ScriptIMath() {
    delete _arg1;
    delete _arg2;
  }
};

class ScriptILogic : public ScriptInstruction {
public:
  ScriptInstruction* _arg1;
  ScriptInstruction* _arg2;
  enum Operation {
    EQ = 1,
    NEQ = 2,
    AND = 3,
    NAND = 4,
    OR = 5,
    NOR = 6,
    XOR = 7,
    NXOR = 8,
    NOT = 16
  };
  Operation _oper;
  ScriptData* run(ScriptData& _args) {
    ScriptData* res1 = _arg1->run(_args);
    if (res1->type != ScriptData::TBOOLEAN) {
      throw 1;
      return NULL;
    }
    ScriptData* res2;
    if (_oper < 16) {
      res2 = _arg1->run(_args);
      if (res2->type != ScriptData::TBOOLEAN) {
        throw 1;
        return NULL;
      }
    }
    ScriptData* s;
    switch (_oper) {
    case EQ:
      s->type = ScriptData::TBOOLEAN;
      s->_data.fromType<bool>((res1->_data.toType<bool>() == res2->_data.toType<bool>()));
      break;
    case NEQ:
      s->type = ScriptData::TBOOLEAN;
      s->_data.fromType<bool>(!(res1->_data.toType<bool>() == res2->_data.toType<bool>()));
      break;
    case AND:
      s->type = ScriptData::TBOOLEAN;
      s->_data.fromType<bool>((res1->_data.toType<bool>() & res2->_data.toType<bool>()));
      break;
    case NAND:
      s->type = ScriptData::TBOOLEAN;
      s->_data.fromType<bool>(!(res1->_data.toType<bool>() & res2->_data.toType<bool>()));
      break;
    case OR:
      s->type = ScriptData::TBOOLEAN;
      s->_data.fromType<bool>((res1->_data.toType<bool>() | res2->_data.toType<bool>()));
      break;
    case NOR:
      s->type = ScriptData::TBOOLEAN;
      s->_data.fromType<bool>(!(res1->_data.toType<bool>() | res2->_data.toType<bool>()));
      break;
    case XOR:
      s->type = ScriptData::TBOOLEAN;
      s->_data.fromType<bool>((res1->_data.toType<bool>() ^ res2->_data.toType<bool>()));
      break;
    case NXOR:
      s->type = ScriptData::TBOOLEAN;
      s->_data.fromType<bool>(!(res1->_data.toType<bool>() ^ res2->_data.toType<bool>()));
      break;
    case NOT:
      s->type = ScriptData::TBOOLEAN;
      s->_data.fromType<bool>(!(res1->_data.toType<bool>()));
      break;
    }
    DeletePtr(res1);
    DeletePtr(res2);
    return s;
  }
  ~ScriptILogic() {
    delete _arg1;
    delete _arg2;
  }
};

class ScriptIVariable : public ScriptInstruction {
public:
  string _arg;
  ScriptData* run(ScriptData& _args) {
    ScriptData* res;
    if(_args._elems.count(_arg) == 0) {
      _args._elems[_arg] = new ScriptData();
    }
    res = CopyPtr(_args._elems[_arg]);
    return res;
  }
  ~ScriptIVariable() {
  }
};

class ScriptIIndex : public ScriptInstruction {
public:
  ScriptInstruction* _arg;
  ScriptInstruction* _ind;
  ScriptData* run(ScriptData& _args) {
    ScriptData* ind = _ind->run(_args);
    ScriptData* arg = _arg->run(_args);
    string index = ind->_data.toType<string>();
    ScriptData* res;
    if (arg->_elems.count(index) == 0) {
      arg->_elems[index] = new ScriptData();
    }
    res = CopyPtr(arg->_elems[index]);
    DeletePtr(ind);
    DeletePtr(arg);
    return res;
  }
  ~ScriptIIndex() {
    delete _arg;
    delete _ind;
  }
};

class ScriptIFunctionCall : public ScriptInstruction {
public:
  list<pair<string, ScriptInstruction*>> _arguments;
  ScriptInstruction* _function;
  ScriptData* run(ScriptData& _args) {
    ScriptData _nargs;
    for (auto&& it : _arguments) {
      ScriptData* _nval;
      _nval = it.second->run(_args);
      _nargs._elems.insert({it.first, _nval});
    }
    ScriptData* res = _function->run(_nargs);
    return res;
  }
  ~ScriptIFunctionCall() {
    delete _function;
    while (_arguments.size()) {
      delete _arguments.begin()->second;
      _arguments.erase(_arguments.begin());
    }
  }
};

class ScriptIAPICall : public ScriptInstruction {
public:
  ScriptData*(_func)(ScriptData&);
  ScriptData* run(ScriptData& _args) {
    return _func(_args);
  }
};