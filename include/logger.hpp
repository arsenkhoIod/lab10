//Copyright [2021] <nickgeo.winner@gmail.com>

#ifndef INCLUDE_LOGGER_HPP_
#define INCLUDE_LOGGER_HPP_

#include <iomanip>
#include <iostream>
#include <vector>
#include "boost/log/trivial.hpp"
#include "boost/log/sinks.hpp"
#include "boost/log/core.hpp"
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/support/date_time.hpp>
#include "string"


namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp",
boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity",
boost::log::trivial::severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(thread_id, "ThreadID",
boost::log::attributes::current_thread_id::value_type)

class logger{
 public:
  static void logging_init(const std::string& severity_level){
    typedef sinks::synchronous_sink< sinks::text_file_backend > sink_f;
    boost::log::formatter formatter = expr::stream
        << std::setw(7) << std::setfill('0')
        << line_id << std::setfill(' ') << " | "
        << "T." << thread_id << " | "
        << expr::format_date_time(timestamp, "%Y-%m-%d, %H:%M:%S.%f") << " "
        << "[" << severity << "]"
        << " - " << expr::smessage;
    boost::shared_ptr< sinks::text_file_backend > backend =
        boost::make_shared< sinks::text_file_backend >(
            keywords::file_name = "log%2N.log",
            keywords::rotation_size = 100 * 1024 * 1024,
            keywords::time_based_rotation =
                sinks::file::rotation_at_time_point(12, 0, 0));
    boost::shared_ptr< sink_f > sink_file(new sink_f(backend));
    sink_file->set_formatter(formatter);
    logging::core::get()->add_sink(sink_file);

    logging::trivial::severity_level min_severity_level;
    if ((severity_level == "trace") || (severity_level == "default")){
      min_severity_level = logging::trivial::trace;
    } else if (severity_level == "info") {
      min_severity_level = logging::trivial::info;
    } else {
      min_severity_level = logging::trivial::fatal;
    }

    logging::core::get()->set_filter(
        logging::trivial::severity >= min_severity_level);

    logging::add_common_attributes();
  }
};


#endif  // INCLUDE_LOGGER_HPP_
