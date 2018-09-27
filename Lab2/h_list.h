#pragma once
#include <iostream>
#include <string>
namespace h_list{
    typedef std::string base;
    struct s_expr;
    struct two_ptr{
        s_expr* hd;
        s_expr* tl;
    };
    struct s_expr{
        bool tag;
        struct{
            base atom;
            two_ptr pair;
        } node;
    };

    typedef s_expr* lisp;
    void print_s_expr( lisp s );
    lisp head (const lisp& s);
    lisp tail (const lisp& s);
    lisp cons (const lisp& h, const lisp& t);
    lisp make_atom (const base x);
    bool isAtom (const lisp& s);
    bool isNull (const lisp& s);
    void destroy (const lisp& s);
    lisp concat(const lisp& y, const lisp& z);
    lisp reverse(const lisp s);
    lisp rev(const lisp s, const lisp z);

    void read_lisp (lisp& y, std::istream& in, std::ostream& err);
    void read_s_expr(char prev, lisp& y, std::istream& in, std::ostream& err);
    void read_seq (lisp& y, std::istream& in, std::ostream& err);

    void write_lisp (const lisp x, std::ostream& out);
    void write_seq (const lisp x, std::ostream& out);

    lisp copy_lisp (const lisp x);
}
