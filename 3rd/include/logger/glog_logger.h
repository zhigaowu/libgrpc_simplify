/*
 * ==========================================================================
 *
 *       Filename:  glog_logger.h
 *
 *    Description:  glog helping class
 * 
 *   glog compile: [Windows]cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=/mingw64
 *                 [Linux]cmake -S . -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local
 *   glog install: cmake --build build --target install
 *   
 *     glog usage: FIND_PACKAGE(glog REQUIRED)
 *                 TARGET_LINK_LIBRARIES(target PRIVATE glog::glog)
 *
 *        Version:  1.0
 *        Created:  2024-04-09 18:30:00
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Zhigao Wu, zhigao.wu@baltamatica.com
 * ==========================================================================
 */

#ifndef UTIL_GLOG_LOGGER_H
#define UTIL_GLOG_LOGGER_H

/*
 * we can define GOOGLE_STRIP_LOG macro in CMakeLists.txt to 
 * instruct glog to remove all strings which fall below a certain severity level by using the GOOGLE_STRIP_LOG
 * INFO: 0
 * WARNING: 1
 * ERROR: 2
 * FATAL: 3
*/
#include <glog/logging.h>

#include <string>
#include <functional>

#include <filesystem>
#include <fstream>

#include <unistd.h>

namespace google {
    enum LogSeverityExtention {
    GLOG_DEBUG = -1,
    #  ifdef DEBUG
    #  error DEBUG macro is defined.
    #  endif
    DEBUG = GLOG_DEBUG
    };
}

#define GDEBUG() VLOG(1)
#define GINFO() LOG(INFO)
#define GWARN() LOG(WARNING)
#define GERROR() LOG(ERROR)
#define GFATAL() LOG(FATAL)

#define GDEBUG_CLASS(className) VLOG(1) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GINFO_CLASS(className) LOG(INFO) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GWARN_CLASS(className) LOG(WARNING) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GERROR_CLASS(className) LOG(ERROR) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GFATAL_CLASS(className) LOG(FATAL) << "[" << #className << "::" << __FUNCTION__ << "] "

#define GDEBUG_IF(condition) VLOG_IF(1, condition)
#define GINFO_IF(condition) LOG_IF(INFO, condition)
#define GWARN_IF(condition) LOG_IF(WARNING, condition)
#define GERROR_IF(condition) LOG_IF(ERROR, condition)
#define GFATAL_IF(condition) LOG_IF(FATAL, condition)

#define GDEBUG_CLASS_IF(className, condition) VLOG_IF(1, condition) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GINFO_CLASS_IF(className, condition) LOG_IF(INFO, condition) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GWARN_CLASS_IF(className, condition) LOG_IF(WARNING, condition) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GERROR_CLASS_IF(className, condition) LOG_IF(ERROR, condition) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GFATAL_CLASS_IF(className, condition) LOG_IF(FATAL, condition) << "[" << #className << "::" << __FUNCTION__ << "] "

#define GDEBUG_EVERY_N(n) VLOG_EVERY_N(1, n)
#define GINFO_EVERY_N(n) LOG_EVERY_N(INFO, n)
#define GWARN_EVERY_N(n) LOG_EVERY_N(WARNING, n)
#define GERROR_EVERY_N(n) LOG_EVERY_N(ERROR, n)
#define GFATAL_EVERY_N(n) LOG_EVERY_N(FATAL, n)

#define GDEBUG_CLASS_EVERY_N(className, n) VLOG_EVERY_N(1, n) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GINFO_CLASS_EVERY_N(className, n) LOG_EVERY_N(INFO, n) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GWARN_CLASS_EVERY_N(className, n) LOG_EVERY_N(WARNING, n) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GERROR_CLASS_EVERY_N(className, n) LOG_EVERY_N(ERROR, n) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GFATAL_CLASS_EVERY_N(className, n) LOG_EVERY_N(FATAL, n) << "[" << #className << "::" << __FUNCTION__ << "] "

#define GDEBUG_EVERY_T(t) LOG_EVERY_T(1, t)
#define GINFO_EVERY_T(t) LOG_EVERY_T(INFO, t)
#define GWARN_EVERY_T(t) LOG_EVERY_T(WARNING, t)
#define GERROR_EVERY_T(t) LOG_EVERY_T(ERROR, t)
#define GFATAL_EVERY_T(t) LOG_EVERY_T(FATAL, t)

#define GDEBUG_CLASS_EVERY_T(className, t) LOG_EVERY_T(1, t) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GINFO_CLASS_EVERY_T(className, t) LOG_EVERY_T(INFO, t) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GWARN_CLASS_EVERY_T(className, t) LOG_EVERY_T(WARNING, t) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GERROR_CLASS_EVERY_T(className, t) LOG_EVERY_T(ERROR, t) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GFATAL_CLASS_EVERY_T(className, t) LOG_EVERY_T(FATAL, t) << "[" << #className << "::" << __FUNCTION__ << "] "

#define GDEBUG_IF_EVERY_N(condition, n) VLOG_IF_EVERY_N(1, condition, n)
#define GINFO_IF_EVERY_N(condition, n) LOG_IF_EVERY_N(INFO, condition, n)
#define GWARN_IF_EVERY_N(condition, n) LOG_IF_EVERY_N(WARNING, condition, n)
#define GERROR_IF_EVERY_N(condition, n) LOG_IF_EVERY_N(ERROR, condition, n)
#define GFATAL_IF_EVERY_N(condition, n) LOG_IF_EVERY_N(FATAL, condition, n)

#define GDEBUG_CLASS_IF_EVERY_N(className, condition, n) VLOG_IF_EVERY_N(1, condition, n) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GINFO_CLASS_IF_EVERY_N(className, condition, n) LOG_IF_EVERY_N(INFO, condition, n) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GWARN_CLASS_IF_EVERY_N(className, condition, n) LOG_IF_EVERY_N(WARNING, condition, n) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GERROR_CLASS_IF_EVERY_N(className, condition, n) LOG_IF_EVERY_N(ERROR, condition, n) << "[" << #className << "::" << __FUNCTION__ << "] "
#define GFATAL_CLASS_IF_EVERY_N(className, condition, n) LOG_IF_EVERY_N(FATAL, condition, n) << "[" << #className << "::" << __FUNCTION__ << "] "

namespace glog {

    using user_initialize_function = std::function<void(const std::string& argv0)>;

    class Logger {
        std::string _dump_path;
        std::ofstream _dump_stream;

        Logger()
            : _dump_stream()
        {}

        ~Logger()
        {
            google::ShutdownGoogleLogging();
        }
        
        static void defaultPrefixFormatter(std::ostream& s, const google::LogMessage& m, void*) {
            s << google::GetLogSeverityName(m.severity())[0]
            << std::setw(2) << 1 + m.time().month()
            << std::setw(2) << m.time().day()
            << ' '
            << std::setw(2) << m.time().hour() << ':'
            << std::setw(2) << m.time().min()  << ':'
            << std::setw(2) << m.time().sec() << "."
            << std::setw(6) << m.time().usec()
            << ' '
            << std::setfill(' ') << std::setw(5)
            << m.thread_id() << std::setfill('0')
            << ' '
            << m.basename() << ':' << m.line() << "]";
        }

        static void defaultDumpSaver(const char* data, size_t size)
        {
            Instance().DumpData(data, size);
        }

    public:
        static Logger& Instance()
        {
            static Logger singleton;
            return singleton;
        }

        // crash -> google::InstallFailureSignalHandler()[https://github.com/google/glog#id26] google::InstallFailureWriter()
        // check -> google::InstallFailureFunction()

        void Initialize(
            const char* argv0, 
            const std::string& log_dir, 
            const std::string& log_prefix = "",  
            google::LogSeverity min_log_level = google::GLOG_INFO, 
            const user_initialize_function& user_initialize = [] (const std::string& argv0) {
                #ifndef NDEBUG
                FLAGS_alsologtostderr = true;
                FLAGS_colorlogtostderr = true;
                FLAGS_stderrthreshold = google::GLOG_INFO;
                FLAGS_v = 1;

                #else
                FLAGS_v = 0;
                FLAGS_logbufsecs = 0;
                FLAGS_logbuflevel = google::GLOG_DEBUG;
                #endif

                FLAGS_max_log_size = 64;
                FLAGS_stop_logging_if_full_disk = true;

                using namespace std::chrono_literals;
                google::EnableLogCleaner(24h * 7); // remove logs before 7 days

                {
                    // set dump path and dump file name

                    std::ostringstream dump_path_stream;
                    dump_path_stream << FLAGS_log_dir;

                    {
                        // make crash directory in the log path
                        dump_path_stream << "crash";
                        std::filesystem::create_directories(dump_path_stream.str());
                    }

                    {
                        // make sure argv0 does not contain path
                        size_t seperator_index = argv0.find_last_of('\\');
                        if (seperator_index == std::string::npos)
                        {
                            seperator_index = argv0.find_last_of('/');
                        }

                        if (seperator_index != std::string::npos)
                        {
                            dump_path_stream << "/" << argv0.substr(seperator_index + 1);
                        }
                        else
                        {
                            dump_path_stream << "/" <<  argv0;
                        }
                    }

                    dump_path_stream << "." << getpid() << ".dump";

                    Instance().SetDumpPath(dump_path_stream.str());
                }
            }, 
            void (*dumpSaver)(const char* data, size_t size) = defaultDumpSaver)
        {
            FLAGS_minloglevel = min_log_level;

            // set log directory
            if (!log_dir.empty())
            {
                // make sure dump path end with path seperator
                char ch = log_dir[log_dir.size() - 1];
                if ('/' != ch && '\\' != ch)
                {
                    FLAGS_log_dir = log_dir + "/";
                }
                else
                {
                    FLAGS_log_dir = log_dir;
                }
            }

            user_initialize(argv0);
            
            if (!log_prefix.empty())
            {
                std::string log_prefix_with_path = FLAGS_log_dir + log_prefix;

                google::SetLogDestination(google::GLOG_INFO, std::string(log_prefix_with_path + ".INFO.").c_str());
                google::SetLogDestination(google::GLOG_WARNING, std::string(log_prefix_with_path + ".WARNING.").c_str());
                google::SetLogDestination(google::GLOG_ERROR, std::string(log_prefix_with_path + ".ERROR.").c_str());
                google::SetLogDestination(google::GLOG_FATAL, std::string(log_prefix_with_path + ".FATAL.").c_str());
            }

            FLAGS_timestamp_in_logfile_name = true;

            google::InstallPrefixFormatter(defaultPrefixFormatter);

            if (dumpSaver)
            {
                google::InstallFailureSignalHandler();
                google::InstallFailureWriter(dumpSaver);
            }

            google::InitGoogleLogging(argv0);
        }

        void SetDumpPath(const std::string& dump_path)
        {
            _dump_path = dump_path;
        }

        void DumpData(const char* data, size_t size)
        {
            if (_dump_stream.is_open())
            {
                _dump_stream << std::string(data, size);
                _dump_stream.flush();
            }
            else
            {
                _dump_stream.open(_dump_path, std::ios::app);

                if (_dump_stream.is_open())
                {
                    _dump_stream << std::string(data, size);
                    _dump_stream.flush();
                }
                else
                {
                    GERROR() << "create dump file failed: " << _dump_path;
                }
            }
        }

        void ChangeMinLogLevel(int min_log_level = google::GLOG_INFO)
        {
            switch (min_log_level)
            {
            case google::GLOG_DEBUG:
            {
                FLAGS_v = 1;
                FLAGS_minloglevel = google::GLOG_INFO;
                break;
            }
            case google::GLOG_INFO:
            case google::GLOG_WARNING:
            case google::GLOG_ERROR:
            case google::GLOG_FATAL:
            {
                FLAGS_v = 0;
                FLAGS_minloglevel = min_log_level;
                break;
            }            
            default:
                break;
            }
        }
    };
}

#endif // UTIL_GLOG_LOGGER_H
