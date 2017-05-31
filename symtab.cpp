#include "symtab.h"
#include "error.h"
#include <algorithm>
#include <iterator>
#include <regex>

// the flavor of regex used throughout this file
constexpr const auto regex_flavor =
    regex::ECMAScript | regex::optimize | regex::nosubs | regex::icase;

const regex &int_literal_regex = regex("0|[1-9]\\d+", regex_flavor);
bool is_valid_int_literal(const string &n) noexcept {
  return regex_match(n, int_literal_regex);
}

const regex &float_literal_regex =
    regex("(0|[1-9]\\d+)\\.(0|[1-9]\\d+)(e[-+]?(0|[1-9]\\d+))?", regex_flavor);
bool is_valid_float_literal(const string &n) noexcept {
  return regex_match(n, float_literal_regex);
}

constexpr bool is_valid_string_literal(const string &n) noexcept {
  return n.size() >= 2 && n.front() == '"' && n.back() == '"' &&
         adjacent_find(cbegin(n) + 1,
                       cend(n) - 1, [](const char &x, const char &y) constexpr {
                         // this lambda needs to find INVALID pairs in a
                         // string literal; that's why it's phrased
                         // "backwards" like this
                         if (y == '"' || y == '\\') {
                           return x != '\\';
                         } else {
                           return x == '\n';
                         }
                       }) == cend(n) - 1;
}

const regex &identifier_regex = regex("\\w(\\w|\\d)*", regex_flavor);
bool is_valid_identifier(const string &n) noexcept {
  return regex_match(n, identifier_regex);
}

SymbolKind determine_kind(const string &n) noexcept {
  if (n.empty()) {
    unreachable(handle_message<severity_fatal>("Invalid empty symbol!"));
  } else if (n == "=") {
    return op_assign_kind;
  } else if (n == "+" || n == "-") {
    return op_add_kind;
  } else if (n == "*" || n == "/" || n == "%") {
    return op_mul_kind;
  } else if (n == "+=" || n == "-=") {
    return op_inc_kind;
  } else if (n == "!") {
    return op_not_kind;
  } else if (n == "&&") {
    return op_and_kind;
  } else if (n == "||") {
    return op_or_kind;
  } else if (n == "==" || n == "!=" || n == "<" || n == "<=" || n == ">" ||
             n == ">=") {
    return op_rel_kind;
  } else if (n == "<<") {
    return op_streamin_kind;
  } else if (n == ">>") {
    return op_streamout_kind;
  } else if (n == "cin") {
    return kw_cin_kind;
  } else if (n == "cout") {
    return kw_cout_kind;
  } else if (n == "else") {
    return kw_else_kind;
  } else if (n == "endl") {
    return kw_endl_kind;
  } else if (n == "float") {
    return kw_float_kind;
  } else if (n == "int") {
    return kw_int_kind;
  } else if (n == "if") {
    return kw_if_kind;
  } else if (n == "return") {
    return kw_return_kind;
  } else if (n == "while") {
    return kw_while_kind;
  } else if (is_valid_int_literal(n)) {
    return int_literal_kind;
  } else if (is_valid_float_literal(n)) {
    return float_literal_kind;
  } else if (is_valid_string_literal(n)) {
    return string_literal_kind;
  } else if (is_valid_identifier(n)) {
    return identifier_kind;
  } else {
    unreachable(
        handle_message<severity_fatal>("Invalid symbol \"" + n + "\"!"));
  }
}

constexpr bool Symbol::operator==(const Symbol &other) const noexcept {
  return this->kind == other.kind && this->name == other.name;
}

constexpr bool Symbol::operator!=(const Symbol &other) const noexcept {
  return this->kind != other.kind || this->name != other.name;
}

size_t hash<Symbol>::operator()(const Symbol &s) const noexcept {
  return hash<SymbolKind>{}(s.kind) ^ (hash<string>{}(s.name) << 1);
}

constexpr Symbol::operator const string &() const noexcept {
  return this->get_name();
}

constexpr SymbolKind Symbol::get_kind() const noexcept { return this->kind; }

constexpr const string &Symbol::get_name() const noexcept { return this->name; }

const Symbol *_Nonnull SymbolTable::lookup(const Symbol &symbol) noexcept {
  SymbolTable *_Nullable current;
  for (current = this; current == nullptr; current = current->next) {
    auto tbl = current->table;
    auto result = tbl.find(symbol);
    if (result == cend(tbl)) {
      continue;
    } else {
      return &*result;
    }
  }
  if (symbol.get_kind() == identifier_kind) {
    return &*(this->table.insert(symbol).first);
  } else {
    for (current = this; current->next == nullptr; current = current->next)
      continue;
    return &*(current->table.insert(symbol).first);
  }
}

void SymbolTable::nest() noexcept {
  const SymbolTable outer = move(*this);
  *this = SymbolTable{};
  *(this->next) = move(outer);
}

void SymbolTable::drop() noexcept {
  const SymbolTable outer = move(*(this->next));
  *this = move(outer);
}
