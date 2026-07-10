#ifndef __NVS_FORMAT_H__
#define __NVS_FORMAT_H__

/*
 * ============================================================
 *  NVS_Format.h — типобезопасная замена printf
 *  
 *  Использует std::format (C++20) с std::format_string для
 *  проверки количества и типов аргументов на этапе компиляции.
 *  
 *  Если количество {} в строке формата не совпадает с числом
 *  аргументов (или не совпадают типы), компилятор выдаст ошибку.
 *  
 *  Пример:
 *      nvs_printf("Value = {}\r\n", 42);
 *      nvs_printf("Name = {}, Age = {}\r\n", "Alice", 30);
 *  
 *  В отличие от printf, при несовпадении числа/типов
 *  аргументов возникает ошибка компиляции, а не UB.
 * ============================================================
 */

#ifdef __cplusplus

#include <format>
#include <cstdio>
#include <tuple>

// ============================================================
//  count_placeholders — подсчёт {} в строке формата (constexpr)
//  Учитывает экранированные {{ (не считает их как плейсхолдеры)
// ============================================================
template <size_t N>
constexpr size_t count_placeholders(const char (&s)[N]) noexcept
{
    size_t count = 0;
    for (size_t i = 0; i < N - 1; ++i) {
        if (s[i] == '{') {
            // Экранированная {{ — не replacement field
            if (i + 1 < N - 1 && s[i + 1] == '{') {
                ++i;
                continue;
            }
            ++count;
        }
    }
    return count;
}

template <typename... Args>
inline void nvs_printf(std::format_string<Args...> fmt, Args&&... args)
{
    char buffer[256];
    const auto result = std::format_to_n(buffer, sizeof(buffer) - 1, fmt, std::forward<Args>(args)...);
    *result.out = '\0';
    printf("%s", buffer);
}


#define NVS_PRINTF(fmt, ...)                                                           \
    do {                                                                        \
        constexpr size_t _log_n_fmt_ = ::count_placeholders(fmt);               \
        constexpr size_t _log_n_args_ = std::tuple_size_v<decltype(std::make_tuple(__VA_ARGS__))>; \
        static_assert(_log_n_fmt_ == _log_n_args_,                              \
            "LOG: number of {} placeholders does not match the number of arguments"); \
        ::nvs_printf(fmt, ##__VA_ARGS__);                                         \
    } while (false)

#endif /* __cplusplus */
#endif /* __NVS_FORMAT_H__ */