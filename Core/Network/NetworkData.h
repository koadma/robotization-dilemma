#pragma once

#include "../Maths/helperFunctions.h"

#define PACKET_HEADER_LEN 4 //Used to store packet ID and data length;
#define PACKET_HEADER_TYPE int

typedef union {
  struct {
    char chars[PACKET_HEADER_LEN];
  } chararr;
  PACKET_HEADER_TYPE i;
} Packet_Header_Convertor;
//Breaks PACKET_HEADER_TYPE into PACKET_HEADER_LEN chars


class DataPair {
public:
  unsigned char* _data;
  int _len;
  DataPair(int len = 0) {
    _len = len;
    _data = new unsigned char[_len];
  }
  template<typename T> void fromType(T val) {
    delete[] _data;
    serialize<T>(val, &_data, _len);
  }
  template<typename T> T toType() {
    return deserializeT<T>(_data, _len);
  }

  ~DataPair() {
    delete[] _data;
  }
};

class DataElement {
public:
  DataPair* _core;
  vector<DataElement*> _children;

  DataElement() {
    _core = new DataPair(0);
  }
  int getLen() {
    int len = 4;
    for (DataElement* d : _children) {
      len += 1 + d->getLen();
    }
    len += 5;
    len += _core->_len;
    return len;
  }
  void fill(unsigned char* data, int &start) {
    Packet_Header_Convertor conv;
    conv.i = getLen() - 4;
    for (int i = 0; i < PACKET_HEADER_LEN; i++) {
      data[start + i] = conv.chararr.chars[i];
    }
    start += 4;

    for (DataElement* d : _children) {
      data[start] = 0;
      start++;
      d->fill(data, start);
    }
    data[start] = 1;
    start++;
    conv.i = _core->_len;
    for (int i = 0; i < PACKET_HEADER_LEN; i++) {
      data[start + i] = conv.chararr.chars[i];
    }
    start += 4;
    for (int i = 0; i < _core->_len; i++) {
      data[start + i] = _core->_data[i];
    }
    start += _core->_len;
  }
  void empty(unsigned char* data, int &start) {
    Packet_Header_Convertor conv;

    for (int i = 0; i < PACKET_HEADER_LEN; i++) {
      conv.chararr.chars[i] = data[start + i];
    }
    start += 4;

    int end = conv.i + start;

    while (start < end) {
      int type = data[start];
      start++;
      if (type == 0) {
        DataElement* newElem = new DataElement();
        newElem->empty(data, start);
        _children.push_back(newElem);
      }
      if (type == 1) {
        for (int i = 0; i < PACKET_HEADER_LEN; i++) {
          conv.chararr.chars[i] = data[start + i];
        }
        start += 4;

        int nlen = conv.i;

        delete _core;
        _core = new DataPair(nlen);

        for (int i = 0; i < _core->_len; i++) {
          _core->_data[i] = data[start + i];
        }
        start += _core->_len;
      }
    }
  }
  void addChild(DataElement* add) {
    _children.push_back(add);
  }
  ~DataElement() {
    for (DataElement* d : _children) {
      delete d;
    }
    _children.clear();
    delete _core;
  }
};


template<typename T>
void vSFunc(T& val, DataElement* elem) {
  elem->_core->fromType<T>(val);
}

template<typename T>
void vGFunc(T& val, DataElement* elem) {
  val = elem->_core->toType<T>();
}

template<typename T>
void oSFunc(T& val, DataElement* elem) {
  val.set(elem);
}

template<typename T>
void oGFunc(T& val, DataElement* elem) {
  val.get(elem);
}

/*template<typename T>
void setVecO(vector<T>& vec, DataElement* data, void(sFunc)(void(sFunc)(T&, DataElement*))) {
  for (auto&& it : vec) {
    DataElement* ne;
    sFunc<T>(it, ne);
    data->addChild(ne);
  }
}

template<typename T>
void getVecO(vector<T>& vec, DataElement* data, void(gFunc)(T&, DataElement*)) {
  for (auto&& it : data->_childer) {
    T nval;
    nval.get(it->_core);
    vec.push_back(nval);
  }
}
*/