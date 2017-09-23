#include "Scripts.h"

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
  type = _from->type;
  _data._len = _from->_data._len;
  delete _data._data;
  _data._data = new unsigned char[_data._len];
  for (size_t i = 0; i < _data._len; i++) {
    _data._data[i] = _from->_data._data[i];
  }
  while (_elems.size()) {
    DeletePtr(_elems.begin()->second);
  }
  for (auto&& it : _from->_elems) {
    ScriptData* nval = new ScriptData();
    nval->CopyContent(it.second);
    _elems.insert({ it.first, nval });
  }
}
ScriptData::~ScriptData() {
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