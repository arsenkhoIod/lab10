#include "iostream"
#include <string>
#include "DBhasher.hpp"
#include "boost/program_options.hpp"

namespace po = boost::program_options;

int main(int argc, char **argv) {
  po::options_description desc("program options");
  desc.add_options()("src_path", po::value<std::string>(), "path to source database")(
      "new_path", po::value<std::string>(), "path to new database")(
      "log_level", po::value<std::string>(), "level of logging")(
      "threads_count", po::value<size_t>(),
      "number of threads, that will be used while processing")(
      "help", "learn about program options");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 0;
  }
  if (!vm.count("src_path")){
    std::cout << "src_path is required" << std::endl;
    return 0;
  }
  std::string src_path = vm["src_path"].as<std::string>();
  std::string new_path = (vm.count("new_path")) ?
                         vm["new_path"].as<std::string>() : (src_path+"_new");
  std::string log_level = (vm.count("log_level")) ?
                          vm["log_level"].as<std::string>() : "default";
  size_t threads_count = (vm.count("threads_count")) ?
                         vm["threads_count"].as<size_t>() : std::thread::hardware_concurrency();
  if (threads_count == 0) threads_count = std::thread::hardware_concurrency();
  DBhasher hasher(src_path, new_path, threads_count, log_level);
  hasher.perform();
  return 0;
}
