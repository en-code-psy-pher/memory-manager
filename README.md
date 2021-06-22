# Memory Allocator

## Design Goals
- Speed, must be faster than the compiler provided allocators
- Robustness must return all memory requested to the sys before termination
- User shouldn't have to change much code when integrating this allocator
- Portability between systems nust exist

<hr>

## Strategy deployed
This allocator must try to achieve the above by following the strategy depicted here,
 - Optimize for speed by reducing the number of system calls and imporinhg efficiency by reducing time requied to allocated memory. Thus is done by requesting the system at start up and intermitently during execution for big chunks for memory. Data structure can then be carved out of memory from these chunks. 
 - Optimize for common request sizes.
 - Pool deleted memory in containers so that it can be reused by the application. If a call happens to fail, the request is redirected to a chunk of memory that was issued by the system during its life cycle with the applciation. *This is the **BIGGEST** reason to reason to write a custom memory allocator. It **CHANGES** the game.*
 - new, new[], delete and delete[] will be operator overloaded, and it will return either an *allocate* or *deallocate* function. 
 - Will take the the total size to be allocated or freed.
 - All four methods will serve as static global functions and will not be class specific *(must be able to easily integrate with any objecy/data structure)*
 - Memory **Manager** will be a ***Singleton***
 
 ## Allocate | De-allocate
 ### Allocate
 1. If the free-store has not been created yet, create the free-store and then go to step 3
 2. If the free-store has been exhausted, create a new free-store
 3. Return the first element of the free-store and mark the next element of the free-store as the free-store head.

 ### De-Allocate
 1. Make the next field in the deleted pointer point to the current free-store head.
 2. Mark the deleted pointer as the free-store head.

 Expandning the free-store is not trivial, it is done by understanding the same free-store pointer is used as a **Complex** object. The size requested for the individual free-store pointers must be the size of whichever is greater, the free-store or Complex object.