/**
 * Author: Tran Quang Loc (darkkcyan)
 * Notes: I changed the grammar a bit. The expression is have a pair of brackets surround it now. Doing that made the parsing easier.
 *        
 */
#include <bits/stdc++.h>
using namespace std;

#define llong long long 
#define xx first
#define yy second
#define len(x) ((int)x.size())
#define rep(i,n) for (int i = -1; ++ i < n; )
#define rep1(i,n) for (int i = 0; i ++ < n; )
#define all(x) x.begin(), x.end()

#define DPC dynamic_pointer_cast 

struct Token {
  virtual llong execute() = 0;
  virtual void print(const string& indent = "") {}
};

struct PositiveIntegerToken : Token {
  llong val;
  PositiveIntegerToken() : val(0) {}
  void addDigit(char dig) {
    assert(isdigit(dig));
    val = val * 10 + (dig - '0');
  }

  virtual llong execute() override {
    return val;
  }

  virtual void print(const string& indent = "") override {
    clog << indent << val << endl;
  }
};

struct ComponentToken : Token {
  shared_ptr<Token> left, right;
  ComponentToken(shared_ptr<Token> a, shared_ptr<Token> b): left(a), right(b) {}
  virtual llong execute() override {
    return left->execute() * right->execute();
  }

  virtual void print(const string& indent = "") override {
    clog << indent << "Component(" << endl;
    left->print(indent + '\t');
    right->print(indent + '\t');
    clog << indent << ')' << endl;
  }
};

struct ExpressionToken : Token {
  shared_ptr<Token> left, right;
  ExpressionToken(shared_ptr<Token> a, shared_ptr<Token> b): left(a), right(b) {}
  virtual llong execute() override {
    return left->execute() + right->execute();
  }

  virtual void print(const string& indent = "") override {
    clog << indent << "Expression(" << endl;
    left->print(indent + '\t');
    right->print(indent + '\t');
    clog << indent << ')' << endl;
  }
};

template<typename CharItor> 
tuple<shared_ptr<Token>, CharItor> parse(CharItor it, CharItor end_it) {
  if (it == end_it) throw make_tuple("Expected more characters", it);
  auto cur_char = *(it++);

  if (isdigit(cur_char)) {
    auto res = make_shared<PositiveIntegerToken>();
    res->val = cur_char - '0';
    for (; it != end_it and isdigit(*it); ++it) res->addDigit(*it);
    return make_tuple(res, it);
  }

  if (cur_char == '(') {
    shared_ptr<Token> res, temp, *rhs;
    tie(res, it) = parse(it, end_it);
    rhs = &res;

    for (; it != end_it; ) {
      switch (*(it++)) {
        case ')': 
          return make_tuple(res, it);
        case '+': {
          tie(temp, it) = parse(it, end_it);
          auto expr = make_shared<ExpressionToken>(res, temp);
          rhs = &(expr->right);
          res = expr;
          break;
        }
        case '*': {
          tie(temp, it) = parse(it, end_it);
          *rhs = make_shared<ComponentToken>(*rhs, temp);
          break;
        }
        default:
          throw make_tuple("Expected an opeartor ('+' or '*') or ')'.", it);
      }
    }
    throw make_tuple("Expected an ')'", it);
  }
  throw make_tuple("Expected a number or an '('.", it);
}

shared_ptr<Token> parse(string str) {
  str = '(' + str + ')';
  shared_ptr<Token> ans;
  auto it = str.begin();
  tie(ans, it) = parse(it, str.end());
  if (it != str.end()) throw make_tuple("Expected an ending.", it);
  return ans;
}

int main(void) {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int t; cin >> t;
    string line;
    getline(cin, line);
    while (t--) {
      getline(cin, line);
      try {
        cout << parse(line)->execute() << '\n';
      } catch (tuple<const char*, string::iterator> e) {
        // clog << get<0>(e) << " '" << (int)*get<1>(e) << "'"<< endl;
        cout << "ERROR\n";
      }
    }
    return 0;
}
