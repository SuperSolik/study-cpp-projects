#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <QDebug>

class Coder{
private:
    struct Node {
        Node(std::string _chars, int frequence) {
            chars = _chars;
            freq = frequence;
        }

        Node(Node* _left, Node* _right){
            chars[0] = '\0';
            freq = _left->freq + _right->freq;
            left = _left;
            right = _right;
        }

        std::string chars;
        int freq = 0;
        Node* left = nullptr;
        Node* right = nullptr;
        std::string code = "";

        ~Node(){
            delete left;
            delete right;
        }
    };

    struct HuffCompare{
        bool operator()(const Node* first, const Node* second) const{
            return first->freq > second->freq;
        }
    };

    struct ShanCompare{
        bool operator()(const Node* first, const Node* second) const{
            return first->freq < second->freq;
        }
    };

public:
    Coder(){}

    ~Coder(){
        delete _root;
    }

    void shannon_encode(std::string const& file_name){
        clear();
        std::ifstream file;
        file.open(file_name, std::ios::in | std::ios::binary);
        std::string buf;
        while(file.good()){
            std::getline(file, buf);
            _text += buf;
        }
        std::map<char, int> freq;
        for(auto& ch : _text){
            freq[ch]++;
        }
        std::priority_queue<Node*, std::vector<Node*>, ShanCompare> sortedChars;
        for(auto& node : freq){
            sortedChars.push(new Node(std::string(1, node.first), node.second));
        }

        std::map<char, std::string> res;
        if(sortedChars.size() == 1){
            auto el = sortedChars.top();
            res[el->chars[0]] = "0";
            delete sortedChars.top();
            _map = res;
            return;
        }

        std::string start;
        while(sortedChars.size() > 0){
            auto el = sortedChars.top();
            start += el->chars;
            sortedChars.pop();
        }
        _root = new Node(start, _text.size());
        makeShannonCodes(_root, freq, res);
        _map = res;
    }

    void huffman_encode(std::string const& file_name){
        clear();
        std::ifstream file;
        file.open(file_name, std::ios::in | std::ios::binary);
        std::string buf;
        while(file.good()){
            std::getline(file, buf);
            _text += buf;
        }
        std::map<char, int> freq;
        for(auto& ch : _text){
            freq[ch]++;
        }
        std::priority_queue<Node*, std::vector<Node*>, HuffCompare> sortedChars;
        for(auto& node : freq){
            sortedChars.push(new Node(std::string(1, node.first), node.second));
        }

        std::map<char, std::string> res;
        if(sortedChars.size() == 1){
            auto el = sortedChars.top();
            res[el->chars[0]] = "0";
            delete sortedChars.top();
            _map = res;
            return;
        }

        while(sortedChars.size() > 1){
            auto left = sortedChars.top();
            sortedChars.pop();
            auto right = sortedChars.top();
            sortedChars.pop();
            sortedChars.push(new Node(left, right));
        }
        _root = sortedChars.top();
        sortedChars.pop();
        makeHuffmanCodes(_root, res);
        _map = res;
    }

    std::string encode_file(std::string const& file_name){
        std::ofstream file;
        file.open(file_name, std::ios::out | std::ios::binary);
        write_tree(_root, file);
        file.write(" ", 1);
        std::string res;
        int count  = 0;
        unsigned char ch = 0;
        for(auto& c : _text){
            std::string code = _map[c];
            res += _map[c];
            for(int i = 0; i < code.size(); i++){
                if(code[i] == '1')
                    ch = ch | (1 << (7 - count));
                count++;
                if(count == 8){
                    file.write(reinterpret_cast<char*>(&ch), 1);
                    ch = 0;
                    count = 0;
                }
            }
        }
        file.close();
        return res;
    }

    std::string decode_file(std::string const& file_name){
        clear();
        std::ifstream file;
        file.open(file_name, std::ios::in | std::ios::binary);
        _root = read_tree(file);
        file.seekg(1, std::ios::cur);
        unsigned char ch;
        int count = 0;
        bool b;
        Node* r = _root;
        std::string res;
        file.read(reinterpret_cast<char*>(&ch), 1);
        while(file.good()){
            b = ch & (1 << (7 - count));
            if(b) r = r->right;
            else r = r->left;
            if(!r->left && !r->right){
                res += r->chars;
                r = _root;
            }
            count++;
            if(count == 8){
                count = 0;
                file.read(reinterpret_cast<char*>(&ch), 1);
            }
        }
        file.close();
        return res;
    }

    std::map<char, std::string> get_map() const{
        return _map;
    }

private:


    void write_tree(Node* node, std::ofstream& file){
        if(!node->left && !node->right){
            file.write("1", sizeof(char));
            file.write(reinterpret_cast<char*>(&node->chars[0]), sizeof(char));
        } else {
            file.write("0", sizeof(char));
            write_tree(node->left, file);
            write_tree(node->right, file);
        }
    }

    Node* read_tree(std::ifstream& file){
        unsigned char ch;
        file.read(reinterpret_cast<char*>(&ch), 1);
        if(ch == '1'){
            file.read(reinterpret_cast<char*>(&ch), 1);
            return new Node(std::string(1, ch), 0);
        } else {
            Node* left = read_tree(file);
            Node* right = read_tree(file);
            return new Node(left, right);
        }
    }

    void clear(){
        _text.clear();
        _map.clear();
        if(_root){
            delete _root;
        }
    }

    void makeShannonCodes(Node* node, std::map<char, int>& freq, std::map<char, std::string>& map, std::string code=""){
        if (node->chars.size() == 1) map[node->chars[0]] = code;
        if(node->chars.size() > 1) {
            int startPos = 0;
            int first_part = 0;
            int total_freq = node->freq;
            for (startPos = 0; startPos < node->chars.size(); startPos++) {
                if (first_part >= (total_freq - first_part) || (startPos + 1 == node->chars.size())) break;
                first_part += freq[node->chars[startPos]];
            }
            Node* left = new Node({node->chars.begin(), node->chars.begin() + startPos}, first_part);
            Node* right = new Node({node->chars.begin() + startPos, node->chars.end()}, total_freq - first_part);
            node->left = left;
            node->right = right;
            makeShannonCodes(node->left, freq, map, code+"0");
            makeShannonCodes(node->right, freq, map, code+"1");
        }
    }

    void makeHuffmanCodes(Node* node, std::map<char, std::string>& map, std::string code=""){
        if (!node) return;
        if (node->chars[0] != '\0') map[node->chars[0]] = code;
        makeHuffmanCodes(node->left, map, code+"0");
        makeHuffmanCodes(node->right, map, code+"1");
    }

    Node* _root = nullptr;
    std::map<char, std::string> _map;
    std::string _text = "";
};

#endif
