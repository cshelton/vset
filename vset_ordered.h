#ifndef VSET_ORDERED_H
#define VSET_ORDERED_H

#include <functional>
#include <memory>
#include <vector>
#include <utility>
#include <algorithm>

namespace sortedvector {

template<typename Key, typename Compare = std::less<Key>,
		typename Allocator = std::allocator<Key>>
class vset_ordered {
public:
	using base_type = std::vector<Key,Allocator>;
	using key_type = Key;
	using value_type = Key;
	using size_type = typename base_type::size_type;
	using difference_type = typename base_type::difference_type;
	using key_compare = Compare;
	using value_compare = Compare;
	using allocator_type = Allocator;
	using reference = value_type &;
	using const_reference = const value_type &;
	using pointer = typename std::allocator_traits<Allocator>::pointer;
	using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
	using iterator = typename base_type::iterator;
	using const_iterator = typename base_type::const_iterator;
	using reverse_iterator = typename base_type::reverse_iterator;
	using const_reverse_iterator = typename base_type::const_reverse_iterator;

	using mytype = vset_ordered<Key,Compare,Allocator>;

	// constructors:
	explicit vset_ordered(const Compare & comp = Compare(),
			const Allocator &alloc = Allocator())
					: _comp(comp), _v(alloc) { }

	// for C++14, I believe the explicit below should be removed
	explicit vset_ordered(const Allocator &alloc)
				: _comp(Compare()), _v(alloc) {}

	// for C++14, need following
	template<class InputIt>
	vset_ordered(InputIt first, InputIt last, const Compare &comp = Compare(),
						const Allocator &alloc = Allocator() )
			: _comp(comp), _v(first,last,alloc) {
		resort();
	}

	template<class InputIt>
	vset_ordered(InputIt first, InputIt last, const Allocator &alloc = Allocator() )
			: _comp(Compare()), _v(first,last,alloc) {
		resort();
	}

	vset_ordered(const vset_ordered &s) = default;
	vset_ordered(const vset_ordered &s, const Allocator &alloc)
			: _comp(s._comp), _v(s._v,alloc) {}

	vset_ordered(vset_ordered &&s) = default;
	vset_ordered(vset_ordered &&s, const Allocator &alloc)
		: _comp(std::move(s._comp)), _v(std::move(s._v),alloc) {}

	vset_ordered(std::initializer_list<value_type> init,
			const Compare &comp = Compare(),
			const Allocator &alloc = Allocator())
				: _comp(comp), _v(init,alloc) {}
	// for C++14, need following
	vset_ordered(std::initializer_list<value_type> init,
			const Allocator &alloc = Allocator())
				: _comp(Compare()), _v(init,alloc) {}

	// destructor:
	~vset_ordered() = default;

	// assignment:
	mytype &operator=(const mytype &) = default;
	mytype &operator=(mytype &&) = default;
	mytype &operator=(std::initializer_list<value_type> ilist) {
		_v = ilist;
		resort;
	}

	// other functions:
	allocator_type get_allocator() const { return _v.get_allocator(); }

	void swap(vset_ordered &s) {
		std::swap(_comp,s._comp);
		_v.swap(s._v);
	}

	size_type count(const Key &key) const {
		std::pair<const_iterator,const_iterator> r
			= std::equal_range(_v.begin(),_v.end(),key,_comp);
		return r.second-r.first;
	}

	key_compare key_comp() const { return _comp; }
	value_compare value_comp() const { return _comp; }

	// comparisons:
	
	bool operator==(const mytype &rhs) const {
		return _v==rhs._v;
	}
	bool operator!=(const mytype &rhs) const {
		return _v!=rhs._v;
	}
	bool operator<(const mytype &rhs) const {
		return _v<rhs._v;
	}
	bool operator<=(const mytype &rhs) const {
		return _v<=rhs._v;
	}
	bool operator>(const mytype &rhs) const {
		return _v>rhs._v;
	}
	bool operator>=(const mytype &rhs) const {
		return _v>=rhs._v;
	}

	// for C++14 need to add templated count (only for compares that
	//  for which Compare::is_transparent is valid) -- not quite sure how

	// removal:
	void clear() { _v.clear(); }

	iterator erase(const_iterator pos) {
		return _v.erase(pos);
	}

	//iterator erase(const_iterator first, const_iterator last) {
	iterator erase(iterator first, iterator last) {
		return _v.erase(first,last);
	}

	size_type erase(const key_type &key) {
		std::pair<iterator,iterator> r
			= std::equal_range(_v.begin(),_v.end(),key,_comp);
		erase(r.first,r.second);
		return r.second-r.first;
	}

	// iterators:
	iterator begin() { return _v.begin(); }
	const_iterator begin() const { return _v.begin(); }
	const_iterator vbegin() const { return _v.cbegin(); }
	iterator rbegin() { return _v.rbegin(); }
	const_iterator rbegin() const { return _v.rbegin(); }

	iterator end() { return _v.end(); }
	const_iterator end() const { return _v.end(); }
	const_iterator vend() const { return _v.cend(); }
	iterator rend() { return _v.rend(); }
	const_iterator rend() const { return _v.rend(); }

	// size functions:

	bool empty() const { return _v.empty(); }
	size_type size() const { return _v.size(); }
	size_type max_size() const { return _v.max_size(); }
	
	void shrink_to_fit() { _v.shrink_to_fit(); }

	// find:
	
	iterator find(const Key &key) {
		iterator loc = std::lower_bound(_v.begin(),_v.end(),key,_comp);
		if (loc==_v.end() || _comp(key,*loc)) return _v.end();
		return loc;
	}

	const_iterator find(const Key &key) const {
		const_iterator loc = std::lower_bound(_v.begin(),_v.end(),key,_comp);
		if (loc==_v.end() || _comp(key,*loc)) return _v.end();
		return loc;
	}

	std::pair<iterator,iterator> equal_range(const Key &key) {
		return std::equal_range(_v.begin(),_v.end(),key,_comp);
	}

	std::pair<const_iterator,const_iterator> equal_range(const Key &key) const {
		return std::equal_range(_v.begin(),_v.end(),key,_comp);
	}

	iterator lower_bound(const Key &key) {
		return std::lower_bound(_v.begin(),_v.end(),key,_comp);
	}
	const_iterator lower_bound(const Key &key) const {
		return std::lower_bound(_v.begin(),_v.end(),key,_comp);
	}
	iterator upper_bound(const Key &key) {
		return std::upper_bound(_v.begin(),_v.end(),key,_comp);
	}
	const_iterator upper_bound(const Key &key) const {
		return std::upper_bound(_v.begin(),_v.end(),key,_comp);
	}
	
	// for C++14 need to add templated find, equal_range, lower_bound,
	// and upper_bound
	// (only for compares that for which Compare::is_transparent is valid)
	// -- not quite sure how

	// insert & emplace:
	//
	// (note bsearch, use of hints and the like might be pointless,
	// it might be better just to place at the end and then swap into
	// position.  However, if the item already exists, this method does
	// not work very well)
	

	std::pair<iterator,bool> insert(const value_type &value) {
		iterator loc = std::lower_bound(_v.begin(),_v.end(),value,_comp);
		if (loc!=_v.end() && _comp(*loc,value)) return {loc,false};
		return {_v.insert(loc,value),true};
	}

	std::pair<iterator,bool> insert(value_type &&value) {
		iterator loc = std::lower_bound(_v.begin(),_v.end(),value,_comp);
		if (loc!=_v.end() && _comp(*loc,value)) return {loc,false};
		return {_v.insert(loc,std::move(value)),true};
	}

	iterator insert(const_iterator hint, const value_type &value) {
		if (_v.empty()) return _v.insert(_v.end(),value);
		if (hint==_v.begin()) {
			if (_comp(value,*hint)) return _v.insert(hint,value);
			if (_comp(*hint,value)) {
				++hint;
				if (hint==_v.end() || _comp(value,*hint))
					return _v.insert(hint,value);
				else return insert(value).first;
			} else return hint; // equal...
		} else if (hint==_v.end()) {
			--hint;
			if (_comp(*hint,value))
				return _v.insert(_v.end(),value);
			if (_comp(value,*hint))
				return insert(value).first;
			else return _v.end(); // equal...
		} else {
			if (_comp(value,*hint)) {
				--hint;
				if (_comp(*hint,value))
					return _v.insert(++hint,value);
				else return insert(value).first;
			} else if (_comp(*hint,value)) {
				++hint;
				if (hint==_v.end() || _comp(value,*hint))
					return _v.insert(hint,value);
				else return insert(value).first;
			} else return hint; // equal...
		}
	}
	
	iterator insert(const_iterator hint, value_type &&value) {
		if (_v.empty()) return _v.insert(_v.end(),std::move(value));
		if (hint==_v.begin()) {
			if (_comp(value,*hint)) return _v.insert(hint,std::move(value));
			if (_comp(*hint,value)) {
				++hint;
				if (hint==_v.end() || _comp(value,*hint))
					return _v.insert(hint,std::move(value));
				else return insert(std::move(value)).first;
			} else return hint; // equal...
		} else if (hint==_v.end()) {
			--hint;
			if (_comp(*hint,value))
				return _v.insert(_v.end(),std::move(value));
			if (_comp(value,*hint))
				return insert(std::move(value)).first;
			else return _v.end(); // equal...
		} else {
			if (_comp(value,*hint)) {
				--hint;
				if (_comp(*hint,value))
					return _v.insert(++hint,std::move(value));
				else return insert(std::move(value)).first;
			} else if (_comp(*hint,value)) {
				++hint;
				if (hint==_v.end() || _comp(value,*hint))
					return _v.insert(hint,std::move(value));
				else return insert(std::move(value)).first;
			} else return hint; // equal...
		}
	}

	template<typename inputit>
	void insert(inputit first, inputit last) {
		for(;first!=last;++first) insert(*first);
	}

	void insert(std::initializer_list<value_type> ilist) {
		for(auto &&x : ilist) insert(x);
	}

	template<typename... Args>
	std::pair<iterator,bool> emplace(Args &&... args) {
		// not sure how to do this (especially if item might already
		// exist -- so placing at the end and swapping into place is
		// not good) except by...
		value_type val(std::forward<Args>(args)...);
		return insert(std::move(val));
	}

	template<typename... Args>
	std::pair<iterator,bool> emplace(const_iterator hint, Args &&... args) {
		// not sure how to do this (especially if item might already
		// exist -- so placing at the end and swapping into place is
		// not good) except by...
		value_type val(std::forward<Args>(args)...);
		return insert(hint,std::move(val));
	}

	

	
		
protected:

	void resort() {
		std::sort(_v.begin(),_v.end(),_comp);
	}

	Compare _comp;
	base_type _v;

};



}

#endif

