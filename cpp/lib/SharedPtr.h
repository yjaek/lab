#pragma once

#include <utility>

template <typename T>
class SharedPtr
{
    T* ptr;
    int* refCount;

    void reset()
    {
        if (refCount && --(*refCount) == 0)
        {
            delete ptr;
            delete refCount;
        }
    }

public:
    SharedPtr() : ptr(nullptr), refCount(nullptr) {}

    SharedPtr(T* const ptr) : ptr(ptr), refCount(new int(1)) {}

    SharedPtr(const SharedPtr& other) : ptr(other.ptr), refCount(other.refCount) { (*refCount)++; }

    SharedPtr(SharedPtr<T>&& other) : ptr(other.ptr), refCount(other.refCount)
    {
        other.ptr = nullptr;
        other.refCount = nullptr;
    }

    ~SharedPtr() { reset(); }

    SharedPtr& operator=(const SharedPtr& other)
    {
        reset();
        (*other.refCount)++;
        ptr = other.ptr;
        refCount = other.refCount;
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other)
    {
        reset();
        ptr = other.ptr;
        refCount = other.refCount;
        other.ptr = nullptr;
        refCount = nullptr;
        return *this;
    }

    T& operator*() const { return *ptr; }

    T* operator->() const { return ptr; }

    T* get() const { return ptr; }
};

template <typename T, typename... Args>
SharedPtr<T> makeShared(Args&&... args)
{
    return SharedPtr(new T(std::forward<Args>(args)...));
}
