#pragma once

#include "../../rapidxml/rapidxml.hpp"
#include "../Network/Network.h"

using namespace rapidxml;

class ScriptData {
public:
  map<string, ScriptData*> _elems;
  DataPair _data;
  enum VarType {
    TNULL = 0, //Nothing
    TNUMERIC = 1,
    TSTRING = 2,
    TCHAR = 3,
    TBOOLEAN = 4,
    TARRAY = 5,
    TVECTOR = 6,
    TDATA = 7
  };
  size_t _instances;
  VarType type;
  ScriptData();
  //ScriptData* copy();
  void CopyContent(ScriptData* _from);
  ~ScriptData();
};

namespace ScriptApiFunctions {
  //In:
  //Root: NUMERIC number to convert
  //Out:
  //Root: STRING result
  ScriptData* num_to_str(ScriptData& _args);

  //In:
  //Root: STRING number to convert
  //Out:
  //Root: NUMERIC result
  ScriptData* str_to_num(ScriptData& _args);

  //In:
  //Root: Array of STRING, concat order lexographic (std::map)
  //Out:
  //Root: STRING result
  ScriptData* str_concat(ScriptData& _args);

  //In:
  //Root: STRING
  //Out:
  //Root: NUMERIC result, string length
  ScriptData* str_len(ScriptData& _args);

  //In:
  //str STRING
  //i NUMERIC
  //Out:
  //str STRING
  ScriptData* str_index(ScriptData& _args);

  //In:
  //a NUMERIC
  //b NUMERIC
  //Out:
  //r NUMERIC
  ScriptData* num_rand(ScriptData& _args);
}

ScriptData* CopyPtr(ScriptData* _in);
bool DeletePtr(ScriptData* _in);

typedef ScriptData*(*APICall)(ScriptData&);

extern map<string, APICall> apiMap;

class ScriptInstruction {
public:
  virtual ScriptData* run(ScriptData& _args);
  virtual void load(xml_node<> *data);
  virtual ~ScriptInstruction() {
    //throw 1;
  }
};

class ScriptIIfElse : public ScriptInstruction {
public:
  ScriptInstruction* _then;
  ScriptInstruction* _else;
  ScriptInstruction* _condition;
  ScriptData* run(ScriptData& _args);
  void load(xml_node<> *data);
  ~ScriptIIfElse();
};

class ScriptILoop : public ScriptInstruction {
public:
  ScriptInstruction* _code;
  ScriptInstruction* _condition;
  ScriptData* run(ScriptData& _args);
  void load(xml_node<> *data);
  ~ScriptILoop();
};

class ScriptIAssign : public ScriptInstruction {
public:
  ScriptInstruction* _to;
  ScriptInstruction* _val;
  ScriptData* run(ScriptData& _args);
  void load(xml_node<> *data);
  ~ScriptIAssign();
};

class ScriptICopy : public ScriptInstruction {
public:
  string _to;
  string _from;
  ScriptData* run(ScriptData& _args);
  void load(xml_node<> *data);
  ~ScriptICopy();
};

class ScriptIConstant : public ScriptInstruction {
public:
  ScriptData* _val;
  ScriptData* run(ScriptData& _args);
  void load(xml_node<> *data);
  ~ScriptIConstant();
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
  ScriptData* run(ScriptData& _args);
  void load(xml_node<> *data);
  ~ScriptIMath();
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
  ScriptData* run(ScriptData& _args);
  void load(xml_node<> *data);
  ~ScriptILogic();
};

class ScriptIVariable : public ScriptInstruction {
public:
  string _arg;
  ScriptData* run(ScriptData& _args);
  void load(xml_node<> *data);
  ~ScriptIVariable();
};

class ScriptIIndex : public ScriptInstruction {
public:
  ScriptInstruction* _arg;
  ScriptInstruction* _ind;
  ScriptData* run(ScriptData& _args);
  void load(xml_node<> *data);
  ~ScriptIIndex();
};

class ScriptIFunctionCall : public ScriptInstruction {
public:
  list<pair<string, ScriptInstruction*>> _arguments;
  ScriptInstruction* _function;
  ScriptData* run(ScriptData& _args);
  void load(xml_node<> *data);
  ~ScriptIFunctionCall();
};

class ScriptIAPICall : public ScriptInstruction {
public:
  APICall _func;
  list<pair<string, ScriptInstruction*>> _arguments;
  ScriptData* run(ScriptData& _args);
  void load(xml_node<> *data);
};

class ScriptIBlock : public ScriptInstruction {
public:
  list<ScriptInstruction*> _instructions;
  ScriptData* run(ScriptData& _args);
  void load(xml_node<> *data);
  ~ScriptIBlock();
};