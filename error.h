#pragma once
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif
#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>
using namespace std;

enum[[nodiscard]] never{};

[[noreturn]] inline void unreachable(const never &v[[maybe_unused]]) noexcept {
  __builtin_unreachable();
}

enum severity : uint_least8_t {
  severity_note = UINT8_C(1),
  severity_warning = UINT8_C(2),
  severity_error = UINT8_C(3),
  severity_fatal = UINT8_C(4),
};

struct Position final {
private:
  uintmax_t line = 0;
  uintmax_t column = 0;

public:
  constexpr uintmax_t current_line() const noexcept;
  constexpr uintmax_t current_column() const noexcept;
  void next_line() noexcept;
  void advance_columns(const uintmax_t amount) noexcept;
  constexpr Position() noexcept = default;
  constexpr Position(const Position &other) noexcept = default;
  constexpr Position(Position &&other) noexcept = default;
  constexpr explicit Position(const uintmax_t l, const uintmax_t c) noexcept
      : line(l), column(c) {}
  ~Position() noexcept = default;
  Position &operator=(const Position &other) noexcept = default;
  Position &operator=(Position &&other) noexcept = default;
  friend ostream &operator<<(ostream &stream,
                             const Position &position) noexcept;
  operator string() const noexcept;
};

void handle_message(const severity sev, const Position &position,
                    const string &msg) noexcept;
void handle_message(const severity sev, const string &msg) noexcept;
template <const severity sev>
inline conditional_t<sev == severity_fatal, never, void>
handle_message(const Position &position, const string &msg) noexcept {
  handle_message(sev, position, msg);
  if
    constexpr(sev == severity_fatal) __builtin_unreachable();
}
template <const severity sev>
inline conditional_t<sev == severity_fatal, never, void>
handle_message(const string &msg) noexcept {
  handle_message(sev, msg);
  if
    constexpr(sev == severity_fatal) __builtin_unreachable();
}

bool has_error_occurred() noexcept;
