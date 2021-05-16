//Copyright [2021] <nickgeo.winner@gmail.com>

#ifndef INCLUDE_DATA_PIECE_HPP_
#define INCLUDE_DATA_PIECE_HPP_

#include <utility>
#include "DBhasher.hpp"
#include "rocksdb/db.h"
#include "string"

class data_piece{
 public:
  data_piece(){}
  data_piece(rocksdb::ColumnFamilyHandle* _handle, std::string _key,
             std::string _value)
      :   handle(_handle)
      , key(std::move(_key))
      , value(std::move(_value))
  {}

 private:
  rocksdb::ColumnFamilyHandle* handle;
  std::string key;
  std::string value;

  friend class DBhasher;
};

#endif  // INCLUDE_DATA_PIECE_HPP_
