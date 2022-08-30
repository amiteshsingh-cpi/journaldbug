#include <iostream>
#include <cstring>
#include <asio.hpp>
#include <thread>
#include <chrono>

#include <spdlog/spdlog.h>
#include "spdlog/sinks/syslog_sink.h"
#include "spdlog/fmt/bin_to_hex.h"
#include <systemd/sd-journal.h>

using namespace std::chrono_literals;
namespace spd = spdlog;


using work_guard_type = asio::executor_work_guard<asio::io_context::executor_type>;

int main(int argc, char **argv)
{
   std::string ident = "[test]";
   bool debug = false;

   if (debug)
     {
        spdlog::default_logger()->set_level(spd::level::trace);
     }
   else
     {
        spd::set_default_logger(spd::syslog_logger_mt("syslog", ident, LOG_PID));
        spdlog::default_logger()->set_level(spd::level::debug);
     }
   spd::warn("TestServiceStarted");
   sd_journal_send("PRIORITY=%d", 1,
                   "MESSAGE=%s", "sd_journal_send:TestServiceStarted",
                   NULL);

   asio::io_context io;
   work_guard_type work_guard(io.get_executor());

   io.run();
   spd::warn("TestServiceStopped");
   sd_journal_send("PRIORITY=%d", 1,
                   "MESSAGE=%s", "sd_journal_send:TestServiceStopped",
                   NULL);

   return 0;
}
