#include <set>
#include "vset.h"
#include "vset_ordered.h"
#include <vector>
#include <utility>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;
using namespace sortedvector;

template<typename S, typename G>
vector<pair<int,double>> timeinsert(int x0, int dx, int x1,
			int n, G generator) {
	vector<pair<int,double>> ret;
	for(int x=x0;x<=x1;x+=dx)
		ret.emplace_back(x,0.0);
	auto lasttime = high_resolution_clock::now();
	for(int i=0;i<n;i++) {
		S s;
		for(auto &e : ret) {
			while(s.size()<e.first-1)
				s.insert(generator());
			auto v = generator();
			auto t0 = high_resolution_clock::now();
			s.insert(v);
			auto t1 = high_resolution_clock::now();
			e.second += duration_cast<microseconds>(t1-t0).count();
		}
		auto ctime = high_resolution_clock::now();
		if (duration_cast<seconds>(ctime-lasttime).count()>1) {
			cout << i << '/' << n << " = " << floor((double)i/n*100) << '%' << endl;;
			lasttime = ctime;
		}
	}
	for(auto &e : ret)
		e.second /= n;
	return ret;
}
template<typename S, typename G>
vector<pair<int,double>> timelookup(int x0, int dx, int x1,
			int n, G generator) {
	vector<pair<int,double>> ret;
	for(int x=x0;x<=x1;x+=dx)
		ret.emplace_back(x,0.0);
	auto lasttime = high_resolution_clock::now();
	for(int i=0;i<n;i++) {
		S s;
		for(auto &e : ret) {
			while(s.size()<e.first)
				s.insert(generator());
			auto v = generator();
			auto t0 = high_resolution_clock::now();
			s.find(v);
			auto t1 = high_resolution_clock::now();
			e.second += duration_cast<microseconds>(t1-t0).count();
		}
		auto ctime = high_resolution_clock::now();
		if (duration_cast<seconds>(ctime-lasttime).count()>1) {
			cout << i << '/' << n << " = " << floor((double)i/n*100) << '%' << endl;;
			lasttime = ctime;
		}
	}
	for(auto &e : ret)
		e.second /= n;
	return ret;
}

int main(int argc, char **argv) {
	int x0 = argc>1 ? atoi(argv[1]) : 10;
	int dx = argc>2 ? atoi(argv[2]) : 10;
	int x1 = argc>3 ? atoi(argv[3]) : 1000;
	int n = argc>4 ? atoi(argv[4]) : 1000;

	std::random_device rd;
	std::default_random_engine rand(rd());
	std::uniform_int_distribution<int> uniform(0,x1*2);
	auto randint = [&rand,&uniform](){
		return uniform(rand);
	};

	//auto sres = timeinsert<set<int>>(x0,dx,x1,n,randint);
	//auto vres = timeinsert<vset<int>>(x0,dx,x1,n,randint);
	auto sres = timelookup<set<int>>(x0,dx,x1,n,randint);
	//auto vres = timelookup<vset<int>>(x0,dx,x1,n,randint);
	auto vores = timelookup<vset_ordered<int>>(x0,dx,x1,n,randint);

	for(int i=0;i<sres.size();i++)
		//cout << sres[i].first << ' ' << sres[i].second << ' ' << vres[i].second << ' ' << vores[i].second << endl;
		cout << sres[i].first << ' ' << sres[i].second << ' ' << vores[i].second << endl;
}
