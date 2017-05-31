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
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_set>
using namespace std;

#define ASSIGNOP (UINT8_C(1))
#define MULOP (UINT8_C(2))
#define ADDOP (UINT8_C(3))
#define INCOP (UINT8_C(4))
#define RELOP (UINT8_C(5))
#define NOT (UINT8_C(6))
#define OR (UINT8_C(7))
#define AND (UINT8_C(8))
#define STREAMIN (UINT8_C(9))
#define STREAMOUT (UINT8_C(10))
#define CIN (UINT8_C(17))
#define COUT (UINT8_C(18))
#define ELSE (UINT8_C(19))
#define ENDL (UINT8_C(20))
#define FLOAT (UINT8_C(21))
#define INT (UINT8_C(22))
#define IF (UINT8_C(23))
#define RETURN (UINT8_C(24))
#define WHILE (UINT8_C(25))
#define INT_LITERAL (UINT8_C(33))
#define FLT_LITERAL (UINT8_C(34))
#define STR_LITERAL (UINT8_C(35))
#define ID (UINT8_C(65))

enum SymbolKind : uint_least8_t {
  op_assign_kind = ASSIGNOP,         // the = operator
  op_mul_kind = MULOP,               // the *, /, and % operators
  op_add_kind = ADDOP,               // the + and - operators
  op_inc_kind = INCOP,               // the += and -= operators
  op_not_kind = NOT,                 // the ! operator
  op_and_kind = AND,                 // the && operator
  op_or_kind = OR,                   // the || operator
  op_rel_kind = RELOP,               // the ==, !=, <, <=, >, and >= operators
  op_streamin_kind = STREAMIN,       // the >> operator
  op_streamout_kind = STREAMOUT,     // the << operator
  kw_cin_kind = CIN,                 // the keyword cin
  kw_cout_kind = COUT,               // the keyword cout
  kw_else_kind = ELSE,               // the keyword else
  kw_endl_kind = ENDL,               // the keyword endl
  kw_float_kind = FLOAT,             // the keyword float
  kw_int_kind = INT,                 // the keyword int
  kw_if_kind = IF,                   // the keyword if
  kw_return_kind = RETURN,           // the keyword return
  kw_while_kind = WHILE,             // the keyword while
  identifier_kind = ID,              // identifiers
  int_literal_kind = INT_LITERAL,    // integer literals
  float_literal_kind = FLT_LITERAL,  // float literals
  string_literal_kind = STR_LITERAL, // string literals
};

bool is_valid_int_literal(const string &n) noexcept;

bool is_valid_float_literal(const string &n) noexcept;

constexpr bool is_valid_string_literal(const string &n) noexcept;

bool is_valid_identifier(const string &n) noexcept;

SymbolKind determine_kind(const string &n) noexcept;

struct Symbol final {
private:
  SymbolKind kind;
  string name;

public:
  constexpr bool operator==(const Symbol &other) const noexcept;
  constexpr bool operator!=(const Symbol &other) const noexcept;
  Symbol() noexcept = default;
  Symbol(const Symbol &other) = default;
  Symbol(Symbol &&other) noexcept = default;
  inline explicit Symbol(const string &n) noexcept
      : kind(determine_kind(n)), name(n) {}
  ~Symbol() noexcept = default;
  Symbol &operator=(const Symbol &other) = default;
  Symbol &operator=(Symbol &&other) noexcept = default;
  inline Symbol &operator=(const string &other) noexcept {
    return *this = Symbol(other);
  }
  inline Symbol &operator=(string &&other) noexcept {
    return *this = Symbol(move(other));
  }
  constexpr explicit operator const string &() const noexcept;
  constexpr SymbolKind get_kind() const noexcept;
  constexpr const string &get_name() const noexcept;
  friend struct hash<Symbol>;
};

template <> struct hash<Symbol> {
public:
  size_t operator()(const Symbol &s) const noexcept;
};

class SymbolTable final {
private:
  SymbolTable *_Nullable next = nullptr;
  unordered_set<Symbol> table;

public:
  bool operator==(const SymbolTable &other) = delete;
  bool operator!=(const SymbolTable &other) = delete;
  SymbolTable() noexcept = default;
  SymbolTable(const SymbolTable &other) = default;
  SymbolTable(SymbolTable &&other) noexcept = default;
  ~SymbolTable() noexcept = default;
  SymbolTable &operator=(const SymbolTable &other) = default;
  SymbolTable &operator=(SymbolTable &&other) noexcept = default;
  const Symbol *_Nonnull lookup(const Symbol &other) noexcept;
  inline const Symbol *_Nonnull lookup(const string &other) noexcept {
    return this->lookup(Symbol(other));
  }
  inline const Symbol *_Nonnull lookup(string &&other) noexcept {
    return this->lookup(Symbol(move(other)));
  }
  void nest() noexcept;
  void drop() noexcept;
};
