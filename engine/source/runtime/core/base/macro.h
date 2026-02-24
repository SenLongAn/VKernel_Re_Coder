#pragma once

#include "runtime/core/log/log_system.h"

#include "runtime/function/global/global_context.h"

#include <chrono>
#include <thread>

#define LOG_HELPER(LOG_LEVEL, ...) \
    g_runtime_global_context.m_logger_system->log(LOG_LEVEL, "[" + std::string(__FUNCTION__) + "] " + __VA_ARGS__);

#define LOG_DEBUG(...) LOG_HELPER(VKernel::LogLevel::debug, __VA_ARGS__);

#define LOG_INFO(...) LOG_HELPER(VKernel::LogLevel::info, __VA_ARGS__);

#define LOG_WARN(...) LOG_HELPER(VKernel::LogLevel::warn, __VA_ARGS__);

#define LOG_ERROR(...) LOG_HELPER(VKernel::LogLevel::error, __VA_ARGS__);

#define LOG_FATAL(...) LOG_HELPER(VKernel::LogLevel::fatal, __VA_ARGS__);
