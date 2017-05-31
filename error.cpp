#include "error.h"
#include <sstream>

static uint_least8_t maximum_severity = UINT8_C(0);

constexpr uintmax_t Position::current_line() const noexcept {
  return this->line;
}

constexpr uintmax_t Position::current_column() const noexcept {
  return this->column;
}

void Position::next_line() noexcept {
  this->line++;
  this->column = 0;
}

void Position::advance_columns(const uintmax_t amount) noexcept {
  this->column += amount;
}

ostream &operator<<(ostream &stream, const Position &position) noexcept {
  return stream << position.line << ":" << position.column;
}

Position::operator string() const noexcept {
  stringstream ss;
  ss << *this;
  return ss.str();
}

void handle_message(const severity sev, const Position &position,
                    const string &msg) noexcept {
  switch (sev) {
  case severity_note:
    clog << "Note[" << position << "]: " << msg << endl;
    if (maximum_severity < 1)
      maximum_severity = 1;
    break;
  case severity_warning:
    cerr << "Warning[" << position << "]: " << msg << endl;
    if (maximum_severity < 2)
      maximum_severity = 2;
    break;
  case severity_error:
    cerr << "Error[" << position << "]: " << msg << endl;
    if (maximum_severity < 3)
      maximum_severity = 3;
    break;
  case severity_fatal:
    if (maximum_severity < 4)
      maximum_severity = 4;
    cerr << "Fatal error[" << position << "]: " << msg << endl;
    exit(1);
  }
}
void handle_message(const severity sev, const string &msg) noexcept {
  switch (sev) {
  case severity_note:
    clog << "Note: " << msg << endl;
    if (maximum_severity < 1)
      maximum_severity = 1;
    break;
  case severity_warning:
    cerr << "Warning: " << msg << endl;
    if (maximum_severity < 2)
      maximum_severity = 2;
    break;
  case severity_error:
    cerr << "Error: " << msg << endl;
    if (maximum_severity < 3)
      maximum_severity = 3;
    break;
  case severity_fatal:
    if (maximum_severity < 4)
      maximum_severity = 4;
    cerr << "Fatal error: " << msg << endl;
    exit(1);
  }
}

bool has_error_occurred() noexcept {
  return maximum_severity >= severity_error;
}
