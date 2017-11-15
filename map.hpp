/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"
#include <iostream>
using namespace std;
int mmax(int x, int y){
    return x>y ? x:y;
}

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<Key, T> value_type;
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
    struct fnode{
        fnode * next;
        fnode * pre;
        fnode * left;
        fnode * right;
        value_type* data;
        int height;
        fnode():next(NULL), pre(NULL), left(NULL), right(NULL), height(0), data(NULL){}
        fnode(const value_type& x):next(NULL), pre(NULL), left(NULL), right(NULL), height(0){data = new value_type(x);}
        ~fnode(){
            if (data != NULL){
                delete data;
            }
            data = NULL;
        }
    };
	class const_iterator;
	class iterator {
	private:
        fnode *p;
        const map* owner;
	public:
		iterator() {
		    p = new fnode();
		}
		iterator(const value_type& x, const map* o){
            p = new fnode(x);
            owner = o;
		}
		iterator(fnode * f, const map*o){
            p = f;
            owner = o;
		}
		iterator(const map* o){
            owner = o;
            p = new fnode();
        }
		iterator(const iterator &other) {
            p = &other.return_node();
            owner = &other.return_owner();
		}
		~iterator(){}
		void clear(){
            if (p != NULL) {
                delete p;
            }
            p = NULL;
            owner = NULL;
		}
		void recover(const map* o){
            p = new fnode();
            owner = o;
		}
        iterator operator+(const int &n) const {
            if (n<0) {
                int x = -n;
                iterator tmp = *this;
                while (x--){
                    tmp--;
                }
                return tmp;
            }
            int x = n;
            iterator tmp = *this;
            while (x--){
                tmp++;
            }
            return tmp;
        }
        iterator operator-(const int &n) const {
            if (n<0) {
                int x = -n;
                iterator tmp = *this;
                while (x--){
                    tmp++;
                }
                return tmp;
            }
            int x = n;
            iterator tmp = *this;
            while (x--)
            {
                tmp--;
            }
            return tmp;
        }
		iterator operator++(int) {
            iterator tmp = *this;
            if (p->next == NULL) throw index_out_of_bound();
            p = p->next;
            return tmp;
		}
		iterator & operator++() {
            if (p->next == NULL) throw index_out_of_bound();
            p = p->next;
            return *this;
		}
		iterator operator--(int) {
            iterator tmp = *this;
            if (p->pre == NULL) throw index_out_of_bound();
            p = p->pre;
            return tmp;
		}
		iterator & operator--() {
            if (p->pre == NULL) throw index_out_of_bound();
            p = p->pre;
            return *this;
		}
		void changeroot(fnode * t){
            p = t;
		}
		value_type & operator*() const {
            if (p->next == NULL || p->pre == NULL) throw invalid_iterator();
            return *(p->data);
		}
		bool operator==(const iterator &rhs) const {
            return &(*(p->data)) == &(*(rhs.nodeptr()->data));
		}
		bool operator==(const const_iterator &rhs) const {
            return &(*(p->data)) == &(*(rhs.nodeptr()->data));
		}
		bool operator!=(const iterator &rhs) const {
            return &(*(p->data)) != &(*(rhs.nodeptr()->data));
		}
		bool operator!=(const const_iterator &rhs) const {
            return &(*(p->data)) != &(*(rhs.nodeptr()->data));
		}
		value_type* operator->() const noexcept {
            return p->data;
		}
		fnode& return_node()const{
            if (p == NULL) throw invalid_iterator();
            return *p;
		}
		const map& return_owner()const{
            return *owner;
		}
		fnode * nodeptr()const{
            if (p == NULL) throw invalid_iterator();
            return p;
		}
	};
	class const_iterator {
		private:
			fnode * p;
			const map * owner;
		public:
			const_iterator() {
				p = new fnode();
			}
			const_iterator(fnode *f, const map* o){
                p = f;
                owner = o;
			}
			const_iterator(const const_iterator &other) {
				p = &other.return_node();
				owner = &other.return_owner();
			}
			const_iterator(const iterator &other) {
				p = &other.return_node();
				owner = &other.return_owner();
			}
			~const_iterator(){}
			void clear(){
                if (p != NULL) {
                    delete p;
                }
                p = NULL;
                owner = NULL;
            }
            void recover(const map* o){
                p = new fnode();
                owner = o;
            }
            const_iterator operator+(const int &n) const {
                if (n<0) {
                    int x = -n;
                    const_iterator tmp = *this;
                    while (x--){
                        tmp--;
                    }
                    return tmp;
                }
                int x = n;
                const_iterator tmp = *this;
                while (x--){
                    tmp++;
                }
                return tmp;
            }
            const_iterator operator-(const int &n) const {
                if (n<0) {
                    int x = -n;
                    const_iterator tmp = *this;
                    while (x--){
                        tmp++;
                    }
                    return tmp;
                }
                int x = n;
                const_iterator tmp = *this;
                while (x--)
                {
                    tmp--;
                }
                return tmp;
            }
            const_iterator operator++(int) {
                const_iterator tmp = *this;
                if (p->next == NULL) throw index_out_of_bound();
                p = p->next;
                return tmp;
            }
            const_iterator & operator++() {
                if (p->next == NULL) throw index_out_of_bound();
                p = p->next;
                return *this;
            }
            const_iterator operator--(int) {
                const_iterator tmp = *this;
                if (p->pre == NULL) throw index_out_of_bound();
                p = p->pre;
                return tmp;
            }
            const_iterator & operator--() {
                if (p->pre == NULL) throw index_out_of_bound();
                p = p->pre;
                return *this;
            }
            bool operator==(const iterator &rhs) const {
                return operator->() == rhs.operator->();
            }
            bool operator==(const const_iterator &rhs) const {
                return operator->() == rhs.operator->();
            }
            bool operator!=(const iterator &rhs) const {
                return operator->() != rhs.operator->();
            }
            bool operator!=(const const_iterator &rhs) const {
                return operator->() != rhs.operator->();
            }
            value_type & operator*() const {
                if (p->next == NULL || p->pre == NULL) throw invalid_iterator();
                return *(p->data);
            }
            value_type* operator->() const noexcept {
                return p->data;
            }
            fnode& return_node()const{
                if (p == NULL) throw invalid_iterator();
                return *p;
            }
            const map& return_owner()const{
                return *owner;
            }
            fnode * nodeptr()const{
                if (p == NULL) throw invalid_iterator();
                return p;
            }
	};
private:
    iterator head;
	iterator tail;
	fnode* root;
	size_t len;
	int h(fnode * t){
        return t==NULL ? 0 : t->height;
	}
	fnode* LL(fnode *t){
        fnode * t1 = t->left;
        t->left = t1->right;
        t1->right = t;
        t->height = mmax(h(t->left), h(t->right))+1;
        t1->height = mmax(h(t1->left), h(t))+1;
        return t1;
	}
	fnode* RR(fnode *t){
        fnode * t1 = t->right;
        t->right = t1->left;
        t1->left = t;
        t->height = mmax(h(t->left), h(t->right))+1;
        t1->height = mmax(h(t1->right), h(t))+1;
        return t1;
	}
	fnode* LR(fnode *t){
        t->left = RR(t->left);
        return LL(t);
	}
	fnode* RL(fnode *t){
        t->right = LL(t->right);
        return RR(t);
	}
	pair<iterator, bool> insert1(const value_type &value, fnode *& f, fnode *& t, int dir){
	    Compare com;
        if (t==NULL){
            t = new fnode(value);
            if (dir == 1){//right
                f->next->pre = t;
                t->next = f->next;
                f->next = t;
                t->pre = f;
            }
            else if (dir == -1){//left
                f->pre->next = t;
                t->pre = f->pre;
                f->pre = t;
                t->next = f;
            }
            iterator tmp(t, this);
            pair<iterator, bool> re(tmp, true);
            return re;
        }
        else if(com(value.first, t->data->first)){
            pair<iterator, bool> re(insert1(value, t, t->left, -1));
            if (h(t->left)-h(t->right) == 2){
                if (com(value.first, t->left->data->first)){
                    t = LL(t);
                }
                else t = LR(t);
            }
            t->height = mmax(h(t->left), h(t->right)) + 1;
            return re;
        }
        else if (com(t->data->first, value.first)){
            pair<iterator, bool> re(insert1(value, t, t->right, 1));
            if (h(t->right)-h(t->left) == 2){
                if (com(t->right->data->first, value.first)){
                    t = RR(t);
                }
                else t = RL(t);
            }
            t->height = mmax(h(t->left), h(t->right)) + 1;
            return re;
        }
        else{
            len--;
            iterator tmp(t, this);
            pair<iterator, bool> re(tmp, false);
            return re;
        }
	}
	fnode* erase1(value_type& value, fnode *&t){
	    Compare com;
        if (t == NULL) return NULL;
        if (com(value.first, t->data->first)){
            t->left = erase1(value, t->left);
            if (h(t->right) - h(t->left) == 2)
            {
                fnode *r =  t->right;
                if (h(r->left) > h(r->right))
                    t = RL(t);
                else
                    t = RR(t);
            }
        }
        else if (com(t->data->first, value.first)){
            t->right = erase1(value, t->right);
            if (h(t->left) - h(t->right) == 2)
            {
                fnode *l =  t->left;
                if (h(l->right) > h(l->left))
                    t = LR(t);
                else
                    t = LL(t);
            }
        }
        else if (t->left != NULL && t->right != NULL){
            if (h(t->left) > h(t->right)){
                fnode * tmp = t->left;
                while (tmp->right != NULL) tmp = tmp->right;
                delete t->data;
                t->data = new value_type(*(tmp->data));
                t->left = erase1(*(tmp->data), t->left);
            }
            else{
                fnode * tmp = t->right;
                while (tmp->left != NULL) tmp = tmp->left;
                delete t->data;
                t->data = new value_type(*(tmp->data));
                t->right = erase1(*(tmp->data), t->right);
            }
        }
        else{
            fnode * oldnode = t;
            t = (t->left != NULL) ? t->left : t->right;
            oldnode->pre->next = oldnode->next;
            oldnode->next->pre = oldnode->pre;
            delete oldnode;
        }
        return t;
	}
	iterator find1(const Key &key, fnode*t)const{
	    //cout << "here1" << endl;
	    Compare com;
        if (t == NULL) return tail;
        if (com(key, t->data->first)){
            return find1(key, t->left);
        }
        else if (com(t->data->first, key)){
            return find1(key, t->right);
        }
        iterator tmp(t, this);
        return tmp;
	}
	const_iterator find2(const Key &key, fnode*t)const{
	    //cout << "here2" << endl;
	    Compare com;
        if (t == NULL){
            const_iterator tmp(tail);
            return tmp;
        }
        if (com(key, t->data->first)){
            return find2(key, t->left);
        }
        else if (com(t->data->first, key)){
            return find2(key, t->right);
        }
        const_iterator tmp(t, this);
        return tmp;
	}
public:
	map():head(this), tail(this), root(NULL) {
        head.return_node().next = &tail.return_node();
        head.return_node().pre = NULL;
        tail.return_node().pre = &head.return_node();
        tail.return_node().next = NULL;
        len = 0;
	}
	map(const map &other):head(this), tail(this), root(NULL){
        head.return_node().next = &tail.return_node();
        head.return_node().pre = NULL;
        tail.return_node().pre = &head.return_node();
        tail.return_node().next = NULL;
        len = 0;
        if (!other.empty()){
            iterator tmp = other.begin();
            while(tmp != other.end()){
                //cout << (*tmp).second << endl;
                insert(*tmp);
                tmp++;
            }
        }
	}
	map & operator=(const map &other) {
        if (this == &other) return *this;
        if (!empty()) clear();
        if (!other.empty()){
            iterator tmp = other.begin();
            while(tmp != other.end()){
                insert(*tmp);
                tmp++;
            }
        }
        return *this;
    }
	~map() {
        clear();
        head.clear();
        tail.clear();
	}
	T & at(const Key &key) {
        iterator tmp = find(key);
        if (tmp == tail) throw index_out_of_bound();
        return (*tmp).second;
	}
	const T & at(const Key &key) const {
        iterator tmp = find(key);
        if (tmp == tail) throw index_out_of_bound();
        return (*tmp).second;
	}
	T & operator[](const Key &key) {//insert when none
        iterator tmp = find(key);
        if (tmp == tail){
            T t;
            pair<Key, T> tm(key, t);
            insert(tm);
            tmp = find(key);
        }
        return (*tmp).second;
	}
	const T & operator[](const Key &key) const {
        iterator tmp = find(key);
        if (tmp == tail) throw index_out_of_bound();
        return (*tmp).second;
	}
	iterator begin() const{
        return head+1;
	}
	const_iterator cbegin() const {
        const_iterator tmp(head+1);
        return tmp;
	}
	iterator end()const {
        return tail;
	}
	const_iterator cend() const {
        const_iterator tmp(tail);
        return tmp;
	}
	bool empty() const {
        return len == 0;
    }
	size_t size() const {
        return len;
	}
	void clear() {
        iterator tmp = head+1;
        while (tmp != tail){
            tmp++;
            (tmp-1).clear();
        }
        head.return_node().next = &tail.return_node();
        head.return_node().pre = NULL;
        tail.return_node().pre = &head.return_node();
        tail.return_node().next = NULL;
        len = 0;
        root = NULL;
	}
	pair<iterator, bool> insert(const value_type &value) {
	    len++;
	    if (len == 1){
            root = new fnode(value);
            root->next = &tail.return_node();
            tail.return_node().pre = root;
            root->pre = &head.return_node();
            head.return_node().next = root;
            iterator it(root, this);
            pair<iterator, bool> re(it, true);
            return re;
	    }
        else{
            return insert1(value, root, root, 0);
        }
	}
	void erase(iterator pos) {
        if (&pos.return_owner() != this) throw invalid_iterator();
        if (pos == end()) throw invalid_iterator();
        //(pos - 1).return_node().next = &(pos + 1).return_node();
        //(pos + 1).return_node().pre = &(pos - 1).return_node();
        root = erase1(*pos, root);
        len--;
	}
	size_t count(const Key &key) const {
        if (find(key) == end()) return 0;
        return 1;
	}
	iterator find(const Key &key) {
        if (len == 0) return tail;
        return find1(key, root);
	}
	const_iterator find(const Key &key) const {
        if (len == 0){
            const_iterator tmp(tail);
            return tmp;
        }
        return find2(key, root);
	}
};

}

#endif
