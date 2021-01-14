#include <iostream>
#include <algorithm>

template <typename T>
class BinTree{
private:
    class Node{
    public:
        Node(T const& val):value(val){}
        ~Node(){
            delete left;
            delete right;
        }
        T value;
        Node* left = nullptr;
        Node* right = nullptr;
    };

public:
    BinTree(){}
    ~BinTree(){
        delete root;
    }

    void insert(T const& val){
        insert_value(val, root);
    }

    void print(std::ostream& out){
        print_tree(root, out);
    }

    int get_max_height(){
        return calc_max_height(root);
    }

private:
    void insert_value(T const& val, Node*& node){
        if(!node){
            node = new Node(val);
            return;
        }
        if(val >= node->value){
            insert_value(val, node->left);

        } else {
            insert_value(val, node->right);
        }
    }

    void print_tree(Node*& node, std::ostream& out, int level=0, bool left=false){
        if(node){
            print_tree(node->left, out, level+1, true);
            for(int i = 0;i < level;i++) out << "   ";
            if(level && left) out << " /";
            else if(level) out << " \\";
            out <<'('<<node->value<<')'<< std::endl;
            print_tree(node->right, out, level+1);
        }
        if(!level) out <<"------"<< std::endl;
    }

    int calc_max_height(Node*& node){
        if(!node) return 0;
        else{
            int l = calc_max_height(node->left);
            int r = calc_max_height(node->right);
            return std::max(l, r) + 1;
        }
    }

    Node* root = nullptr;
};
