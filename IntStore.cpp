// FILE: IntStore.cpp
//       Implementation file for the IntStore class
//       (See IntStore.h for documentation.)
// INVARIANT for the IntStore class:
// (1) Distinct values of the IntStore are stored in a 1-D,
//     dynamic array whose size is store in member variable
//     capacity; member variable data references the array.
// (2) The # of occurrences associated with each distinct value
//     (stored in data) is stored in the corresponding element
//     of a parallel 1-D, dynamic array whose size is also
//     stored in member variable capacity (i.e., data and freq
//     are of the same size); member variable freq references the
//     array.
// (3) The distinct value added the earliest is stored in data[0],
//     the distinct value that is added the 2nd-earliest is stored
//     in data[1], and so on.
// (4) The # of distinct values the IntStore currently contains is
//     stored in the member variable used.
// (5) Except when the IntStore is empty (used == 0), ALL elements
//     of data from data[0] until data[used - 1] contain relevant
//     distinct-value data and ALL elements of freq from freq[0]
//     until freq[used - 1] contain relevant occurrence data;
//     the # of occurrences of data[i] is stored in freq[i] (i.e.,
//     data and freq are parallel arrays).
// (6) We DON'T care what is stored in any of the array elements
//     from data[used] through data[capacity - 1], and
//     from freq[used] through freq[capacity - 1].
//
// DOCUMENTATION for private member (helper) function:
//   void resize(int new_capacity)
//     Pre:  (none)
//           Note: Recall that one of the things a constructor
//                 has to do is to make sure that the object
//                 created BEGINS to be consistent with the
//                 class invariant. Thus, resize() should not
//                 be used within constructors unless it is at
//                 a point where the class invariant has already
//                 been made to hold true.
//     Post: The capacity (sizes of the dynamic arrays) of the
//           invoking IntStore is changed to new_capacity...
//           ...EXCEPT when new_capacity would not allow the
//           invoking IntStore to preserve current contents (i.e.,
//           value for new_capacity is invalid or too low for the
//           IntStore to represent the existing collection),...
//           ...IN WHICH CASE the capacity of the invoking IntStore
//           is set to "the minimum that is needed" (which is the
//           same as "exactly what is needed") to preserve current
//           contents...
//           ...BUT if "exactly what is needed" is 0 (i.e. existing
//           collection is empty) then the capacity should be
//           further adjusted to 1 or DEFAULT_CAPACITY (since we
//           don't want to request dynamic arrays of size 0).
//           The collection represented by the invoking IntStore
//           remains unchanged.
//           If reallocation of dynamic array is unsuccessful, an
//           error message to the effect is displayed and the
//           program unconditionally terminated.

#include "IntStore.h"
#include <iostream>
#include <cstdlib>
#include <cassert>
using namespace std;

void IntStore::resize(int new_capacity)
{
    if(new_capacity < used)
        new_capacity = used;
    if(new_capacity < 1)
        new_capacity = 1;
    capacity = new_capacity;
    int* newData = new int[capacity];
    int* newFreq = new int[capacity];
    for(int x = 0; x < used; x++)
    {
        newData[x] = data[x];
        newFreq[x] = freq[x];
    }
    delete [] data;
    delete [] freq;
    data = newData;
    freq = newFreq;
}

IntStore::IntStore(int init_capacity) : capacity(init_capacity)
{
    if(capacity < 1)
        capacity = DEFAULT_CAPACITY;
    data = new int[capacity];
    freq = new int[capacity];
}

IntStore::IntStore(const IntStore& src) : capacity(src.capacity), used(src.used)
{
    data = new int[capacity];
    freq = new int[capacity];
    for(int x = 0; x < used; x++)
    {
        data[x] = src.data[x];
        freq[x] = src.freq[x];
    }
}

IntStore::~IntStore()
{
    delete [] data;
    delete [] freq;
}

IntStore& IntStore::operator=(const IntStore& rhs)
{
    if(this != &rhs)
    {
        int* newData = new int[rhs.capacity];
        int* newFreq = new int[rhs.capacity];
        for(int x = 0; x < rhs.used; x++)
        {
            newData[x] = rhs.data[x];
            newFreq[x] = rhs.freq[x];
        }
        delete [] data;
        delete [] freq;
        data = newData;
        freq = newFreq;
        capacity = rhs.capacity;
        used = rhs.used;
    }
    return *this;
}

bool IntStore::empty() const
{
    if (used)
        return false;
    else
        return true;
}

int IntStore::countDistinct() const { return used; }


int IntStore::countAll() const
{
    int count = 0;
    for (int x = 0; x < used; x++){
            count += freq[x];
    }
    return count;
}

int IntStore::countTarget(int target) const
{
    int found = 0;
    for (int x = 0; x < used; x++){
        if(target == data[x])
            found = freq[x];
    }
    return found;
}

int IntStore::valAt(int pos) const
{
    assert(!empty());
    assert(pos >= 1);
    assert(!pos <= countDistinct());
    
    return data[pos-1];
}

int IntStore::freqAt(int pos) const
{
    assert(!empty());
    assert(pos >= 1);
    assert(pos <= countDistinct());
    
    return freq[pos-1];
}

double IntStore::avg() const
{
    assert(!empty());
    double dataTotal = 0,
    freqTotal = 0,
    avg;
    for (int x = 1; x <= used; x++){
        dataTotal = dataTotal + (valAt(x) * freqAt(x));
        freqTotal = freqTotal + freqAt(x);
    }
    avg = dataTotal/freqTotal;
    return avg;
}

void IntStore::reset() { used = 0; capacity = 0; }

bool IntStore::add(int item, int copies)
{
    assert(copies > 0);
    
    if(used >= capacity)
        resize( int(1.5*capacity) + 1 );
    
    for (int x = 0; x < used; x++){
        if(data[x] == item){
            freq[x] = freq[x] + copies;
            return true;
        }
    }
    data[used] = item;
    freq[used] = copies;
    used++;
    return true;
    
}

int IntStore::operator+=(const IntStore& addend)
{
    if (addend.empty()) return 0;
    
    int distinct = 0;
    
    for (int x = 1; x <= addend.used; x++){
        
        add(addend.valAt(x), addend.freqAt(x));
        distinct++;
        
    }
    return distinct;
}

bool IntStore::remOne(int target)
{
    for (int x = 0; x < used; x++){
        if(data[x] == target){
            freq[x] = freq[x] - 1;
            if(freq[x] == 0){
                for (int i = x; i < used; i++){
                    if(data[i]){
                        data[i] = data[i+1];
                        freq[i] = freq[i+1];
                    }
                }
                --used;
            }
            return true;
        }
    }
    return false;
}

int IntStore::remAll(int target)
{
    int total = 0;
    for (int x = 0; x < used; x++){
        if(data[x] == target){
            total = freq[x];
            data[x] = data[x+1];
            freq[x] = freq[x+1];
        }
    }
    --used;
    return total;
}

bool operator==(const IntStore& is1, const IntStore& is2)
{
    if (is1.empty() && is2.empty()) return true;
    if (is1.countDistinct() != is2.countDistinct()) return false;
    
    for (int x = 1; x <= is1.countDistinct(); x++){
        for (int i = 1; i <= (is2.countDistinct() + 1); i++){
            if (is1.valAt(i) == is2.valAt(x)) x++, i=0;
            else if (x > is2.countDistinct()) return true;
            else if (i > is1.countDistinct()) return false;
        }
    }
    return false;
}
