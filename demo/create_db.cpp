
// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"

using namespace rocksdb;

#if defined(OS_WIN)
std::string kDBPath = "C:\\Windows\\TEMP\\rocksdb_column_families_example";
#else
std::string kDBPath = "hmmm";
#endif

int main() {
  // open DB
  DB* db;

  Options options;
  options.create_if_missing = true;

  Status s = DB::Open(options, kDBPath, &db);
  assert(s.ok());

  // create column family
  ColumnFamilyHandle* cf;
  s = db->CreateColumnFamily(ColumnFamilyOptions(), "new_cf", &cf);
  assert(s.ok());

  // close DB
  s = db->DestroyColumnFamilyHandle(cf);
  assert(s.ok());
  delete db;

  // open DB with two column families
  std::vector<ColumnFamilyDescriptor> column_families;
  // have to open default column family
  column_families.push_back(ColumnFamilyDescriptor(
      kDefaultColumnFamilyName, ColumnFamilyOptions()));
  // open the new one, too
  column_families.push_back(ColumnFamilyDescriptor(
      "new_cf", ColumnFamilyOptions()));
  std::vector<ColumnFamilyHandle*> handles;
  s = DB::Open(DBOptions(), kDBPath, column_families, &handles, &db);
  assert(s.ok());

  std::cout << "Handles size" << handles.size() << std::endl;

  // put and get from non-default column family
  s = db->Put(WriteOptions(), handles[1], Slice("key"), Slice("value"));
  assert(s.ok());
  std::string value;
  s = db->Get(ReadOptions(), handles[1], Slice("key"), &value);
  assert(s.ok());

  std::cout << value << std::endl;

  // atomic write
  WriteBatch batch;

  for (auto handle : handles){
    for (size_t i = 0; i < 100; ++i){
      batch.Put(handle, Slice("key" + std::to_string(i)), Slice("value" + std::to_string(i)));
    }
  }
  s = db->Write(WriteOptions(), &batch);
  assert(s.ok());

  s = db->Get(ReadOptions(), handles[1], Slice("key3"), &value);
  assert(s.ok());
  std::cout << value << std::endl;

  // drop column family
  //s = src_db->DropColumnFamily(src_handles[1]);
  assert(s.ok());

  // close src_db
  for (auto handle : handles) {
    s = db->DestroyColumnFamilyHandle(handle);
    assert(s.ok());
  }
  delete db;

  return 0;
}
