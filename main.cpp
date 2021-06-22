#include <stdio.h>

using namespace std;

#define POOLSIZE 32

// Abstraction layer for a memory manager
class IMemoryManager 
  {
  public:
    virtual void∗ allocate(size_t) = 0;
    virtual void free(void∗) = 0;
  };

class MemoryManager : public IMemoryManager
  {
    struct FreeStore {
      FreeStore* next;
    };

  private:
    FreeStore* head;

    void expandPoolSide();
    void cleanup();

  public: 
    // Constructor
    MemoryManager() {
      head = 0;
      expandPoolSide();
    }

    // Destructor
    virtual ~MemoryManager() {
      cleanup();
    }

    virtual void* allocate(size_t);
    virtual void free(void∗);
  };

  MemoryManager gMemoryManager;

  class Complex {
      private:
      double r;
      double c;

      public:
        Complex(double _a, double _b): r(_a), c(_b) {}
        inline void* operator new(size_t);
        inline void operator delete(void*);
  };

void* MemoryManager::allocate(size_t size) {
    if(0 == head) {
        expandPoolSide();
    }

    FreeStore* _head = head;
    head = _head->next;

    return _head;
}

void MemoryManager::free(void* deleted) {
    FreeStore* _del = static_cast<FreeStore*>(deleted);
    _del->next = head;
    head = _del;
}

void MemoryManager::expandPoolSide() {
    size_t size = (sizeof(Complex) > sizeof(FreeStore*)) ? sizeoff(Complex) : sizeof(FreeStore*);
    FreeStore* _head = reinterpret_cast<FreeStore*>(new char[size]);
    head = _head;

    for(unsigned int i = 0; i < POOLSIZE; ++i) {
        _head->next = reinterpret_cast<FreeStore*>(new char[size]);
        _head = _head->next;
    }

    _head->next = 0;
}

void MemoryManager::cleanup() {
    FreeStore* nextPtr = head;
    for(; nextPtr; nextPtr = head) {
        head = head->next;
        delete[] nextPtr;
    }
}

inline void* Complex::operator new(size_t size) {
    return gMemoryManager.allocate(size);
}

inline void* Complex::operator delete(void* ptrToDelete) {
    return gMemoryManager.free(ptrToDelete);
}

int main(int argc, char* argv[]) {
    cout << "Welcome to Memory Management" << endl;

    return 0;
}