#pragma once

#include <spdlog/spdlog.h>

#include <cstdint>
#include <stdexcept>

namespace VKernel
{

    // Log Level
    enum class LogLevel : uint8_t
    {
        debug,
        info,
        warn,
        error,
        fatal
    };
    struct LogMessage
    {
        LogLevel level;
        std::string log;
    };

    class LogSystem final
    {
    public:
    public:
        // Construct
        LogSystem();
        ~LogSystem();

        // print log
        template <typename... TARGS>
        void log(LogLevel level, TARGS &&...args)
        {
            // log level
            switch (level)
            {
            case LogLevel::debug:
                m_logger->debug(std::forward<TARGS>(args)...);
                break;
            case LogLevel::info:
                m_logger->info(std::forward<TARGS>(args)...);
                break;
            case LogLevel::warn:
                m_logger->warn(std::forward<TARGS>(args)...);
                break;
            case LogLevel::error:
                m_logger->error(std::forward<TARGS>(args)...);
                break;
            case LogLevel::fatal:
                m_logger->critical(std::forward<TARGS>(args)...);
                fatalCallback(std::forward<TARGS>(args)...);
                break;
            default:
                break;
            }

            if (m_messages.size() >= 10)
            {
                m_messages.erase(m_messages.begin());
            }

            std::string formattedMessage = fmt::format(std::forward<TARGS>(args)...);
            m_messages.push_back({level, std::move(formattedMessage)});
        }

        // fatal Callback func
        template <typename... TARGS>
        void fatalCallback(TARGS &&...args)
        {
            const std::string format_str = fmt::format(std::forward<TARGS>(args)...);
            throw std::runtime_error(format_str); // throw Exception
        }

        std::vector<LogMessage> GetMessage() { return m_messages; }

    private:
        // log handle
        std::shared_ptr<spdlog::logger> m_logger;
        std::vector<LogMessage> m_messages;
    };

} // namespace VKernel