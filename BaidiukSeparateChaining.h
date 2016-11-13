#ifndef ContDynArray_H
#define ContDynArray_H

#include <iostream>
#include "Container.h"

//std::cerr << "\n";

class ContDynArrayEmptyException : public ContainerException {
public:
    virtual const char *what() const noexcept override { return "BaidiukSeparateChaining: empty"; }
};


template<typename E, size_t N = 7>
class BaidiukSeparateChaining : public Container<E> {

    class Node {
    public:
        E info;
        Node *next;

        Node() {
            this->next = nullptr;
        }
    };


    size_t n, nmax;
    double maxFactor;
    Node **nArray;

    //test
    E lRem;
    E lAdd;
    size_t nAdd;
    size_t nRem;

    void add_(const E &);

    void remove_(const E &);

    void sort(E *, size_t) const;


    size_t hash(const E &e) const { return hashValue(e) % nmax; }

public:
    BaidiukSeparateChaining() {
        n = 0;
        nmax = N;
        maxFactor = 0.7;
        nArray = new Node *[nmax]();
        nAdd = 0;
        nRem = 0;
    }

    BaidiukSeparateChaining(std::initializer_list<E> el) : BaidiukSeparateChaining() {
        for (auto e: el)
            add(e);
    }

//erledigt
    virtual ~BaidiukSeparateChaining() {
        Node *p;
        Node *pForDel;
        for (size_t i = 0; i < nmax; ++i) {
            p = nArray[i];
            while (p) {
                pForDel = p;
                p = p->next;
                delete pForDel;
            }
        }
        delete[] nArray;
    }

    using Container<E>::add;

    virtual void add(const E e[], size_t len) override;

    using Container<E>::remove;

    virtual void remove(const E e[], size_t len) override;

    virtual bool member(const E &e) const override;

    virtual size_t size() const override { return n; }

    virtual E min() const override;

    size_t x() const override;

    virtual E max() const override;

    virtual std::ostream &print(std::ostream &o) const override;

    virtual size_t apply(std::function<void(const E &)> f, Order order = dontcare) const override;

};


//erledigt
template<typename E, size_t N>
void BaidiukSeparateChaining<E, N>::add_(const E &e) {
    size_t key = hash(e);
    Node *p = new Node;
    p->info = e;
    if (nArray[key]) {
        p->next = nArray[key];
    }
    nArray[key] = p;

    nAdd++;
    lAdd = p->info;
}

//erledigt
template<typename E, size_t N>
void BaidiukSeparateChaining<E, N>::add(const E e[], size_t len) {

    if (n + len > nmax * maxFactor) {
        size_t oldMax = nmax;
        nmax = 2 * (nmax + len);
        Node **oldArray = nArray;
        nArray = new Node *[nmax]();
        Node *p;
        Node *pForDel;
        nAdd = 0;


        for (size_t i = 0; i < oldMax; ++i) {
            p = oldArray[i];
            while (p) {
                add_(p->info);
                pForDel = p;
                p = p->next;
                delete pForDel;
            }
        }
        delete[] oldArray;
    }

    for (size_t i = 0; i < len; ++i) {
        if (!member(e[i])) {
            add_(e[i]);
            ++n;
        }
    }
}

//erledigt
template<typename E, size_t N>
bool BaidiukSeparateChaining<E, N>::member(const E &e) const {
    size_t key = hash(e);
    Node *p = nArray[key];
    while (p) {
        if (p->info == e) return true;
        p = p->next;
    }
    return false;
}

//erledigt
template<typename E, size_t N>
void BaidiukSeparateChaining<E, N>::remove(const E e[], size_t len) {
    for (size_t i = 0; i < len; ++i) {
        remove_(e[i]);
    }
}

//erledigt
template<typename E, size_t N>
void BaidiukSeparateChaining<E, N>::remove_(const E &e) {

    size_t key = hash(e);
    Node *p = nArray[key];
    Node *vorP = p;
    lRem = e;
    nRem++;


    while (p) {
        if (p->info == e) {
            if (nArray[key]->info == e) {
                nArray[key] = p->next;
            }
            vorP->next = p->next;
            delete p;
            n--;
            return;
        }
        vorP = p;
        p = p->next;
    }

}

//erledigt
template<typename E, size_t N>
E BaidiukSeparateChaining<E, N>::min() const {

    Node *p;
    E *eMin = nullptr;
    for (int i = 0; i < nmax; i++) {
        p = nArray[i];
        for (; p; p = p->next) {
            if (eMin == nullptr) eMin = &p->info;
            if (*eMin > p->info) eMin = &p->info;
        }
    }
    if (this->empty()) throw ContDynArrayEmptyException();
    return *eMin;
}

//erledigt
template<typename E, size_t N>
E BaidiukSeparateChaining<E, N>::max() const {

    Node *p;
    E *eMax = nullptr;
    for (int i = 0; i < nmax; i++) {
        p = nArray[i];
        while (p) {
            if (eMax == nullptr) eMax = &p->info;
            if (p->info > *eMax) eMax = &p->info;
            p = p->next;
        }
    }
    if (this->empty()) throw ContDynArrayEmptyException();
    return *eMax;
}


template<typename E, size_t N>
size_t BaidiukSeparateChaining<E, N>::x() const {
    if (nAdd < 1 || nRem < 1) throw ContDynArrayEmptyException();
//    std::cerr << lAdd<< "    \n";
//    std::cerr << lRem<< "    \n";
//    std::cerr << nAdd<< "    \n";
//    std::cerr << nRem<< "    \n";
    size_t xxx = 0;
    Node *p;
    for (int i = 0; i < nmax; i++) {
        p = nArray[i];
        while (p) {

            if (lAdd > lRem) {
                if (p->info > lRem && lAdd > p->info)
                    xxx++;
            } else{
                if (lRem > p->info && p->info > lAdd )
                    xxx++;
            }


            p = p->next;
        }
    }


    return xxx;
}


template<typename E, size_t N>
size_t BaidiukSeparateChaining<E, N>::apply(std::function<void(const E &)> f,
                                            Order order) const {
    size_t rc = 0;
    Node *p;
    if (order == dontcare) {
        try {
            for (size_t i = 0; i < nmax; ++i) {
                p = nArray[i];
                while (p) {
                    f(p->info);
                    ++rc;
                    p = p->next;
                }
            }
        } catch (...) { }
    } else {
        E *values = new E[n];
        size_t x = 0;

        for (size_t i = 0; i < nmax; ++i) {
            p = nArray[i];
            while (p) {
                values[x++] = p->info;
                p = p->next;
            }
        }
        sort(values, n);
        try {
            if (order == descending) {
                for (size_t i = n; i--;) {
                    f(values[i]);
                    ++rc;
                }
            } else {
                for (size_t i = 0; i < n; ++i) {
                    f(values[i]);
                    ++rc;
                }
            }
        } catch (...) { }
        delete[] values;
    }
    return rc;
}


template<typename E, size_t N>
void BaidiukSeparateChaining<E, N>::sort(E *values, size_t n) const {

    int k = n / 2;
    int l, r, lh, rh;
    for (; k; k /= 2) {
        l = 0;
        r = k;
        for (; r < n; ++l, ++r) {
            if (values[l] > values[r]) {
                std::swap(values[l], values[r]);
                lh = l - k;
                rh = l;
                for (; lh >= 0 && values[lh] > values[rh]; lh -= k, rh -= k) {
                    std::swap(values[lh], values[rh]);
                }
            }
        }
    }

}

//erledigt
template<typename E, size_t N>
std::ostream &BaidiukSeparateChaining<E, N>::print(std::ostream &o) const {
    o << "BaidiukSeparateChaining nmax=" << nmax << " n= " << n << "\n";
    Node *p;
    for (size_t i = 0; i < nmax; ++i) {
        p = nArray[i];
        o << i << "--";
        while (p) {
            o << "->";
            o << "( " << p->info << " )";
            p = p->next;
        }
        o << '\n';
    }
    return o;
}


#endif //ContDynArray_H