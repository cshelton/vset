#include "vset.h"
#include <set>
#include <iostream>

using namespace sortedvector;
using namespace std;

int main(int argc, char **argv) {
	vset<int> v;
	set<int> s;

	while(true) {
		char c;
		cin >> c;
		cout << "command = " << c << endl;
		switch(c) {
			case 'i': {
				int i;
				cin >> i;
				v.insert(i);
				s.insert(i);
				break;
			}
			case 'e': {
				int i;
				cin >> i;
				v.emplace(i);
				s.emplace(i);
				break;
			}
			case 'c': {
				v.clear();
				s.clear();
				break;
			}
			case 'x': {
				int i;
				cin >> i;
				v.erase(i);
				s.erase(i);
				break;
			}
			case 's': {
				cout << v.size() << ' ' << s.size() << endl;
				break;
			}
			case 'f': {
				int i;
				cin >> i;
				cout << (v.find(i)!=v.end()) << ' ' << (s.find(i)!=s.end()) << endl;
				break;
			}
			case 'l': {
				int i;
				cin >> i;
				auto lb1 = v.lower_bound(i);
				if (lb1==v.end()) cout << "end" << endl;
				else cout << *lb1 << endl;
				auto lb2 = s.lower_bound(i);
				if (lb2==s.end()) cout << "end" << endl;
				else cout << *lb2 << endl;
				break;
			}
			case 'u': {
				int i;
				cin >> i;
				auto ub1 = v.upper_bound(i);
				if (ub1==v.end()) cout << "end" << endl;
				else cout << *ub1 << endl;
				auto ub2 = s.upper_bound(i);
				if (ub2==s.end()) cout << "end" << endl;
				else cout << *ub2 << endl;
				break;
			}
		}
		cout << "vset:";
		for(auto &x : v) cout << ' ' << x;
		cout << endl;
		cout << " set:";
		for(auto &x : s) cout << ' ' << x;
		cout << endl;
	}
}
