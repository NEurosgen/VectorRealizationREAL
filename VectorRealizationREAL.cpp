// VectorRealizationREAL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstddef>
#include <algorithm>
#include <iterator>
#include <vector>
#include <memory>
#include <chrono>
#include <string>
//Vector norm but not good
template <typename T>
class Vector1 {
private:
    int sz;
    int cap;
    T* ptr=nullptr;
   
    void destroy() {
        for (int i = 0; i < sz; ++i) {
            (ptr + i)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(ptr);
    }
    struct Iterator {
    private:
        T* data = nullptr;
    public:
        void begin( T* ptr)  {
            data = ptr;
            
        }
        void  end( T* ptr)  {
            
            data=ptr ;
        }
        T* operator++() {
            return ++data;
        }
        T* operator--() {
            --data;
            return data;
        }
        T* operator+=(size_t i) {
            data += i;
            return data;
        }
        T& operator *() {
            return *data;
        }
        bool operator==(const Iterator&  a) const{
            return data == a.data;
        }
        bool operator!=(const Iterator& a)const {
            return data != a.data;
        }
    };
public:
    //comstructor
    Vector1():sz(0), cap(0){}
    Vector1(int i) :sz(i), cap(i), ptr(reinterpret_cast<T*> (new int8_t[i * sizeof(T)])){}
    Vector1(size_t i, T val) :Vector1(i) {
        for (size_t i = 0; i < sz; ++i) {
            new(ptr + i) T(val);
        }
    }
    Vector1(std::initializer_list<size_t> array)  :Vector1(array.size()) {
        std::copy(array.begin(), array.end(), ptr);
    }
    Vector1(const Vector1& a)  {
        *this = a;
    }
    Iterator begin() {
        Iterator it;
        it.begin(ptr);
        return it;
    }
    Iterator end() {
        Iterator it;
        it.end(ptr+sz);
        return it ;
    }

    //iterators







    //methods
    void clear() {
        destroy();
        sz = 0;
        cap = 0;
    }
    void reserve(int n) {
        if (n <= cap) return;
        
        T* newarr = reinterpret_cast<T*>(new int8_t[sizeof(T) * n]);
     
        try {
            std::uninitialized_copy(ptr, ptr + sz, newarr);
        }
        catch (...) {
           
            delete[] reinterpret_cast<int8_t*>(newarr);
            throw;
        }
        destroy();
        ptr = newarr;
        cap = n;
    }
    void push_back(const T& val) {
        if (!cap){
            ptr = new T(val);
            ++cap;
            ++sz;
            return;
        }
        if (cap == sz) {
            reserve(2 * sz);
        }
        new(ptr+sz) T(val);
        ++sz;
    }
    void pop_back() {
        (ptr + sz - 1)->~T();
        --sz;
    }
    void print()const  {
        for (int i = 0; i < sz; ++i) {
            std::cout << ptr[i]<<" ";
        }
        std::cout << "\n";
    }
    size_t size()const  {
        return sz;
    }
    size_t cp()const  {
        return cap;
    }
    const T& operator[](int index) const{
        return *(ptr + index);
    }
    T& operator[](int index) {
        return *(ptr + index);
    }

    Vector1& operator =(const Vector1& a) {
        cap = a.cap;
        sz = a.sz;
        destroy();
        ptr = reinterpret_cast<T*>(new int8_t[sizeof(T) * cap]);
        for (int i = 0; i < sz; ++i) {
            new(ptr + i) T(a.ptr[i]);
        }
        return *this;
        
        
    }

    bool empty() const {
        return sz == 0;
    }
    
    
    //destructor
    ~Vector1() {
        if (cap != 0) {
            destroy();
        }
    }
};

//Vector good but not the best;
template<typename T,typename Alloc=std::allocator<T>>
class Vector2 {
private:
    size_t size;
    size_t cap;
 
    Alloc alloc;
    T* arr;
    Vector2(const size_t& count, Alloc alloc = Alloc()) :size(count),cap(count),alloc(alloc) {
        arr = alloc.allocate(size);
    }
    void destroy() {
        for (int i = 0; i < size; ++i) {
            alloc.destroy(arr + i);
        }
        
        size = 0;
        
    }
    void swap(Vector2& v) {
        std::swap(v.size, size);
        std::swap(v.cap, cap);
        std::swap(v.alloc, alloc);
        std::swap(v.arr, arr);


    }

public:
    struct iterator {
        T* data;
        T& operator*() {
            return *data;
        }
        T* operator->() {
            return data;
        }
        iterator& operator+=(size_t n) {
            data += n;
            return *this;
        }
        bool operator==(const iterator& it) {
            return it.data == data;
        }
        bool operator!=(const iterator& it) const {
            return !(it == *this);
        }
        bool operator<(const iterator& it) const {
            return data < it.data;
        }
    };
    Vector2(const size_t& count, const T& value, Alloc alloc = Alloc()) :Vector2(count,alloc) {
        
        for (int i = 0; i < count; ++i) {
            alloc.construct(arr + i, value);
        }
    }
    Vector2(Alloc alloc=Alloc()):size(0),cap(0),alloc(alloc)  {}
    
    Vector2(std::initializer_list<T> array) :Vector2(array.size()) {
        std::copy(array.begin(), array.end(), arr);
    }
    Vector2(const Vector2& v) {
        cap = v.cap;
        size = v.size;
        alloc = v.alloc;
        arr = alloc.allocate(size);
        for (int i = 0; i < size; ++i) {
            new(arr + i)  T(v.arr[i]);
        }



    }
    Vector2& operator=(const Vector2& v) {
        Vector2 copy = v;
        swap(copy);
        return *this;

    }
    T& operator[](size_t i) {
        return arr[i];
    }
    const T& operator[](size_t i)const {
        return arr[i];
    }
    void clear() {
        destroy();
        alloc.deallocate(arr, cap);
        arr = nullptr;
        cap = 0;
    }
    void pop_back() {
        alloc.destroy(arr + size-1);
        --size;
   }
    void reserve(size_t count) {
        if (count <= cap) { return; }
        T* newarr = alloc.allocate(count);
        
        try {
            std::uninitialized_copy(arr, arr + size, newarr);
        }
        catch (...) {
            
           
                alloc.deallocate(newarr , count);
            
            throw;
        }
        for (int i = 0; i < size; ++i) {
            alloc.destroy(arr+i);
        }
        alloc.deallocate(arr,cap);

        arr = newarr;
        cap = count;
    }
    
    
    void push_back(const T& val) {
        if (cap == 0) { cap = 1; arr = alloc.allocate(1); }
        if (cap == size) {
            reserve(2 * cap);
        }
        alloc.construct(arr + size,val);
        ++size;
    }
    int sz() {
        return size;
    }
    int capacity() {
        return cap;
    }
    iterator begin() {
        iterator it;
        it.data = arr;
        return it;
    }
    iterator end() {
        iterator it;
        it.data = arr + size;
        return it;
    }

    void print()const {
        for (int i = 0; i < size; ++i) {
            std::cout << *(arr + i) << " ";
        }
        std::cout << "\n";
    }
    ~Vector2() {
        clear();
       
    }

};

//My the best realization use move semantic
template<typename T,typename Alloc=std::allocator<T>>
class Vector3 {
private:
    size_t sz;
    size_t cap;
    Alloc alloc;
    T* arr;
    using AllocTraits = std::allocator_traits<Alloc>;
    Vector3(size_t i,  Alloc alloc = Alloc()) :sz(i), cap(i), alloc(alloc) {
        arr = AllocTraits::allocate(alloc, i);
        //arr = alloc.allocate(i);
    }
    void destroy() {
        for (int i = 0; i < sz; ++i) {
            AllocTraits::destroy(alloc, arr + i);
            //alloc.destroy(arr + i);
        }
        sz = 0;
        
    }
    void swap(Vector3& v) {
        T t = std::move(*this);
        *this = std::move(v);
        v = std::move(t);


    }
public:
    Vector3(std::initializer_list<T> array) :Vector3(array.size()) {
        std::copy(array.begin(), array.end(), arr);
    }
    Vector3(size_t i, T val,Alloc alloc=Alloc()) :Vector3(i) {
        for (int i = 0; i < sz; ++i) {
            AllocTraits::construct(alloc, arr + i, val);
            //alloc.construct(arr + i, val);
        }
    }
    Vector3() :Vector3(0){}
  
    Vector3(const Vector3& v) {
        cap = v.cap;
        sz = v.sz;
        alloc = v.alloc;
        arr = alloc.allocate(cap);
        std::uninitialized_copy(v.arr, v.arr + sz, arr);
        
    }
    Vector3(Vector3&& v) :sz(v.sz), cap(v.cap), alloc(v.alloc), arr(v.arr) {
        v.sz = 0;
        v.cap = 0;
        v.arr = nullptr;
    }

    Vector3& operator=(const Vector3& v) {
        Vector3 copy = v;
        swap(copy);
        return *this;
    }
    
    T& operator[](size_t i) {
        return arr[i];
    }
    const T& operator[](size_t i) const {
        return arr[i];
    }
    ~Vector3() {
        
            clear();
        
    }
    void clear() {
        destroy();
        AllocTraits::deallocate(alloc, arr, cap);
        //alloc.deallocate(arr, cap);
        arr = nullptr;
        cap = 0;
    }
    size_t size() { return sz; }
    size_t capas() { return cap; }
    void reserve(size_t n) {
        if (n <= cap) { return; }
        T* newarr = AllocTraits::allocate(alloc, 2 * n);
        //T* newarr = alloc.allocate(2 * n);
        size_t i = 0;
        try {
            for (; i < sz; ++i) {
                AllocTraits::construct(alloc, newarr + i, std::move_if_noexcept(arr[i]));
                //alloc.construct(newarr + i, std::move_if_noexcept(arr[i]));
            }
        }
        catch (...) {
            for (int j = 0; j < i; ++j) {
                AllocTraits::destroy(alloc, newarr + j);
                //alloc.destroy(newarr + j);
            }
            AllocTraits::deallocate(alloc, newarr, n);
            //alloc.deallocate(newarr, n);
            throw;
        }
        for (int i = 0; i < sz; ++i) {
            AllocTraits::destroy(alloc, arr + i);
            //alloc.destroy(arr + i);
        }
        AllocTraits::deallocate(alloc, arr, cap);
        //alloc.deallocate(arr,cap);
        arr = newarr;
        cap = 2 * n;
    }
    template<typename ...Arg>
    void emplace_back(Arg&&... arg) {
        if (cap == 0) { cap = 1; arr = AllocTraits::allocate(alloc,1); }
        else {
            if (cap == sz) {
                reserve(2 * cap);
            }
        }
        AllocTraits::construct(alloc, arr + sz, std::forward<Arg>(arg)...);
        //alloc.construct(arr + sz,std::forward<Arg>(arg)...);
        ++sz;
    }
    template<typename U=T>
    void push_back(U&& val) {
        emplace_back(val);
    }
 
    void pop_back() {
        alloc.destroy(arr + sz - 1);
        --sz;
    }
    void print() {
        for (int i = 0; i < sz; ++i) {
            std::cout << arr[i] << " ";
        }
    }
};
template<typename T>
void test(T& a) {
     
    
    std::string s = "fweflwlksdklfmlsmdfslsdflsdflsmflslksdklfmlsmdfslsdflsdflsmflslksdklfmlsmdfslsdflsdflsmflsflksdklfmlsmdfslsdflsdflsmflskg";
    for (int i = 0; i < 10; ++i) {
        auto begin = std::chrono::steady_clock::now();

        for (int i = 100000; i >= 0; --i) {
            a.push_back("sdfsfnsdlknslkdnfsdsfnsdlknslkdsfnsdlknslkdsfnsdlknslkdsfnsdlknslkdfsslkdnfsdsfnsdlknslkdsfnsdlknslkdsfnsdlknslkdsfnsdlknslkdfsslkdnfsdsfnsdlknslkdsfnsdlknslkdsfnsdlknslkdsfnsdlknslkdfsslkdnfsdsfnsdlknslkdsfnsdlknslkdsfnsdlknslkdsfnsdlknslkdfsfnfsldnfslsl");
            a.push_back(s);
        }
        for (int i = 50000; i >= 0; --i) {
            a.pop_back();
        }
        for (int i = 50000; i >= 0; --i) {
            a[i] = "sdsvsvsvsvsvsvsvsvsvsvsvbf";
        }

        a.clear();

        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        std::cout << i + 1 << " the time: " << elapsed_ms.count() << " ms\n";


    }
  

    

}

template<typename T>
void f(T&& x) {
    std::cout << 1;
}
int main()
{
    //temporari materialization
   

    Vector3<std::string>a;
    Vector2<std::string> b;
    std::vector<std::string>c;
    test(a);
    test(b);
    test(c);
   

    

  

   

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
