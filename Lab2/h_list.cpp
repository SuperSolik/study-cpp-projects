#include <iostream>
#include <cstdlib>
#include "h_list.h"

namespace h_list{
//....................................
lisp head (const lisp& s){
    if (s != NULL)
        if (!isAtom(s))
            return s->node.pair.hd;
        else{
            std::cerr << "Error: Head(atom) \n";
            exit(1);
        }
    else{
        std::cerr << "Error: Head(nil) \n";
        exit(1);
    }
}
//.......................................
bool isAtom (const lisp& s){
    if(s == NULL)
        return false;
    else
        return (s -> tag);
}
//.......................................
bool isNull (const lisp& s){
    return s == NULL;
}
//.......................................
lisp tail (const lisp& s){
    if (s != NULL)
        if (!isAtom(s))
            return s->node.pair.tl;
        else{
            std::cerr << "Error: Tail(atom) \n";
            exit(1);
        }
    else{
        std::cerr << "Error: Tail(nil) \n";
        exit(1);
    }
}
//.......................................
lisp cons (const lisp& h, const lisp& t){
    lisp p;
    if (isAtom(t)){
        std::cerr << "Error: Cons(*, atom)\n";
        exit(1);
    }else{
        p = new s_expr;
        if ( p == NULL){
            std::cerr << "Memory not enough\n";
            exit(1);
        }else{
            p->tag = false;
            p->node.pair.hd = h;
            p->node.pair.tl = t;
            return p;
        }
    }
}
//...........................
lisp make_atom(const base x){
    lisp s;
    s = new s_expr;
    s -> tag = true;
    s->node.atom = x;
    return s;
}

//...........................
void destroy (const lisp& s){
    if ( s != NULL){
        if (!isAtom(s)){
            destroy (head(s));
            destroy (tail(s));
        }
        delete s;
    }
}
//...........................
lisp reverse(const lisp s){
    return rev(s, NULL);
}
//...........................
lisp rev(const lisp s, const lisp z){
    if(isNull(s)) return(z);
    else if(isAtom(head(s))) return(rev(tail(s), cons(head(s),z)));
    else return(rev(tail(s), cons(rev(head(s), NULL),z)));
}
//...........................
void read_lisp(lisp& y, std::istream& in, std::ostream& err){
    char x;
    do{
        in >> x;
    }while(x==' ');
    read_s_expr(x, y, in, err);
}
//...........................
void read_s_expr (char prev, lisp& y, std::istream& in, std::ostream& err){
    if (prev == ')'){
        err << "Wrong bracket : \')\' instead of \'(\'"<<" ";
        return;
    }
    else if ( prev != '(' ){
        std::string str;
        str += prev;
        char c;
        while(isalpha(in.peek())){
            in.get(c);
            str += c;
            if (in.peek() == '(') break;
        }
        y = make_atom(str);
    }
    else
        read_seq(y, in, err);
}
//...........................
void read_seq ( lisp& y, std::istream& in, std::ostream& err){
    char x;
    lisp p1, p2;

    if (!(in >> x)){
        err <<"Problems with istream"<<" ";
        return;
    } else {
        while(x==' ')
            in >> x;
        if (x == ')')
            y = NULL;
        else
        {
            read_s_expr (x, p1, in, err);
            read_seq(p2, in, err);
            y = cons(p1, p2);
        }
    }
}
//...........................

void write_lisp (const lisp x, std::ostream& out=std::cout){
    if (isNull(x))
        out << " ()";
    else if (isAtom(x))
        out << ' ' << x->node.atom;
    else{
        out << " (" ;
        write_seq(x, out);
        out << " )";
    }
}
//...........................
void write_seq (const lisp x, std::ostream& out=std::cout){
    if (!isNull(x)){
        write_lisp(head(x), out);
        write_seq(tail(x), out);
    }
}
}
