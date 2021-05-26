#include <cstddef>
#include <iostream>
#include <string>

template <typename T>
class SmartPointer {
public:
    explicit SmartPointer(T* memory);
    SmartPointer(const SmartPointer& other);
    SmartPointer& operator=(const SmartPointer& other);
    ~SmartPointer();

    T& operator*() const;
    T* operator->() const;

    T* get() const;
    size_t getShareCount() const;
    void reset(T* newRes);

private:
    struct Intermediary {
        T* resource;
        size_t refCount;
    };
    Intermediary* data;

    void detach();
    void attach(Intermediary* to);
};

template <typename T>
SmartPointer<T>::SmartPointer(T* memory) {
    data = new Intermediary;
    data->resource = memory;
    data->refCount = 1;
}

template <typename T>
SmartPointer<T>::~SmartPointer() {
    detach();
}

template <typename T>
T& SmartPointer<T>::operator*() const {
    return *data->resource;
}

template <typename T>
T* SmartPointer<T>::operator->() const {
    return data->resource;
}

template <typename T>
void SmartPointer<T>::detach() {
    --data->refCount;
    if (data->refCount == 0) {
        delete data->resource;
        delete data;
    }
}

template <typename T>
void SmartPointer<T>::attach(Intermediary* to) {
    data = to;
    ++data->refCount;
}

template <typename T>
SmartPointer<T>::SmartPointer(const SmartPointer& other) {
    attach(other.data);
}

template <typename T>
SmartPointer<T>& SmartPointer<T>::operator=(const SmartPointer& other) {
    if (this != &other) {
        detach();
        attach(other.data);
    }
    return *this;
}

template <typename T>
T* SmartPointer<T>::get() const {
    return data->resource;
}

template <typename T>
size_t SmartPointer<T>::getShareCount() const {
    return data->refCount;
}

template <typename T>
void SmartPointer<T>::reset(T* newRes) {
    detach();
    data = new Intermediary;
    data->resource = newRes;
    data->refCount = 1;
}

int main() {
    SmartPointer<std::string> myPtr(new std::string);

    *myPtr = "stay awhile, you are so beautiful";
    std::cout << *myPtr << std::endl;

    SmartPointer<std::string> other = myPtr;
    std::cout << *other << std::endl;
    std::cout << other->length() << std::endl;

    return 0;
}
