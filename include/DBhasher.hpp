//Copyright [2021] <nickgeo.winner@gmail.com>

#ifndef INCLUDE_DBHASHER_HPP_
#define INCLUDE_DBHASHER_HPP_

#include <rocksdb/db.h>

#include <iostream>

#include "PicoSHA2/picosha2.h"
#include "ThreadPool/ThreadPool.h"
#include "atomic"
#include "data_piece.hpp"
#include "logger.hpp"
#include "mutex"
#include "rocksdb/slice.h"
#include "safe_queue.hpp"
#include "shared_mutex"
#include "vector"
#include "string"

enum database{
  new_database [[maybe_unused]] = 0,
  src_database [[maybe_unused]] = 1
};

class DBhasher {
 public:
  explicit DBhasher(std::string _kDBpath, std::string _new_path,
                    const size_t& _threads_count, std::string _log_level);
  ~DBhasher();
  void perform();

 private:
  void print_db(database db);
  void get_descriptors();
  void create_new_db();
  void start_reading();
  void start_hashing();
  void start_writing();
  void close_both_db();

 private:
  size_t threads_count;
  std::string src_path;
  std::string new_path;
  rocksdb::DB* src_db;
  rocksdb::DB* new_db;
  std::vector<rocksdb::ColumnFamilyHandle*> src_handles;
  std::vector<rocksdb::ColumnFamilyHandle*> new_handles;
  std::vector<rocksdb::ColumnFamilyDescriptor> descriptors;
  safe_queue<data_piece>* data_to_hash;
  safe_queue<data_piece>* data_to_write;
  std::atomic_bool stop_hash;
  std::atomic_bool stop_read;
  std::atomic_bool stop_write;
  std::atomic_int pieces_to_hash;
  std::atomic_int pieces_to_write;
  std::atomic_int pieces_to_read;
  std::shared_mutex global_work;
  std::string log_level;
};

#endif  // INCLUDE_DBHASHER_HPP_
