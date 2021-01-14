#include <iostream>
#include <vector>
#include <string>

std::vector<int> compute_prefix_function(const std::string& s) {
	int len = s.length();
	std::vector<int> p(len);
	p[0] = 0;
    int k = 0;
	for (int i = 1; i < len; ++i) {
		while ((k > 0) && (s[k] != s[i]))
			k = p[k - 1];
		if (s[k] == s[i])
			++k;
		p[i] = k;
	}
	return p;
}

std::string kmp(const std::string& p, const std::string& s){
    std::string ind;
    if(p == s){
        ind.append("0");
        return ind;
    }
    auto pf = compute_prefix_function(p);
    for (int k = 0, i = 0; i < s.size(); i++){
		while ((k > 0) && (p[k] != s[i]))
			k = pf[k-1];
		if (p[k] == s[i])
			k++;
		if (k == p.size())
			ind.append(std::to_string(i - p.size() + 1) + ",");
	}
	if(ind.empty()) ind.append("-1");
	else ind.erase(ind.end()-1);
    return ind;
}

int main(){
    std::string pattern, str;
    std::getline(std::cin, pattern);
    std::getline(std::cin, str);
    auto res = kmp(pattern, str);
    std::cout << res << std::endl;
    return 0;
}
