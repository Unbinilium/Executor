#pragma once

#include <array>
#include <cstdio>
#include <cstdint>
#include <future>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace ubn {
    namespace __executor::impl {
        enum __policy { async_until_finish };

        enum __errors { no_implementation, popen_failed };

        std::unordered_map<__errors, const char *> __error_msg {
            {__errors::no_implementation, "policy no implementation!"},
            {__errors::popen_failed, "popen() failed!"}
        };

        template <std::size_t buffer_size>
        static inline decltype(auto) __async_until_finish(const std::string_view& cmd) {
            auto task{[](const std::string_view& __cmd) -> std::unique_ptr<std::stringstream> {
                std::array<char, buffer_size> buffer{};
                std::unique_ptr<std::stringstream> result{std::make_unique<std::stringstream>()};
                std::unique_ptr<FILE, decltype(&pclose)> pipe{popen(__cmd.data(), "r"), pclose};
                if (!pipe) { throw std::runtime_error(__error_msg.at(__errors::popen_failed)); }
                while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) { *result << buffer.data(); }
                return result;
            }};
            auto ftr{std::make_unique<std::future<std::unique_ptr<std::stringstream>>>(std::async(std::launch::async, std::move(task), cmd))};
            return ftr;
        }
    }

    namespace executor {
        using policy = __executor::impl::__policy;
        using errors = __executor::impl::__errors;

        static inline constexpr std::size_t buffer_size{64ul};

        template <std::size_t BufferSize = buffer_size>
        static inline constexpr decltype(auto) executor(const policy exec_policy, const std::string_view& cmd) {
            if (exec_policy == policy::async_until_finish) {
                return __executor::impl::__async_until_finish<BufferSize>(cmd);
            } else {
                throw std::runtime_error(__executor::impl::__error_msg.at(errors::no_implementation));
            }
        }

        template <std::size_t BufferSize = buffer_size>
        static inline constexpr decltype(auto) executor(const std::string_view& cmd) {
            return executor<BufferSize>(policy::async_until_finish, cmd);
        }
    }
}