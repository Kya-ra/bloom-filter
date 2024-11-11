#include "bitset.h"

// include the standard library header files that we use in this
// program, which are denoted by angle brackets < >
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>


// define constant dummy values to avoid compiler warnings
static const int dummy_integer_value = -99;
static struct bitset * dummy_pointer_value = NULL;

// create a new, empty bit vector set with a universe of 'size' items
struct bitset * bitset_new(int size) {
  fprintf(stderr, "Bitset Created, Size: %i\n", size);
  struct bitset * set;
  set = malloc(sizeof(struct bitset));
  set->size_in_words = ceil(size/64.0);
  set->universe_size = 64*set->size_in_words;
  set->bits = malloc(sizeof(uint64_t)*set->size_in_words);
  //memset to 0 because submitty has junk data
  memset(set->bits, 0, sizeof(uint64_t)*set->size_in_words);
  return set;
}

// get the size of the universe of items that could be stored in the set
int bitset_size(struct bitset * this) {
  return this->universe_size;
}

// get the number of items that are stored in the set
// pass through each word, right shifting after every bit, and adding 1 if the LSB is 1 each time
int bitset_cardinality(struct bitset * this){
  int setCount = 0;
  for (int i = 0; i < this->size_in_words; i++) {
    uint64_t temp = this->bits[i];
    for (int j = 0; j < 64; j++) {
         setCount += temp & 1;
         temp >>= 1;
	}
  }
  return setCount;
}

// check to see if an item is in the set
// load the word into temp, rightshift to the relevant bit, return the LSB
int bitset_lookup(struct bitset * this, int item){
  uint64_t temp = this->bits[item/64];
  temp >>= item%64;
  temp &= 1;
  return temp;
}

// add an item, with number 'item' to the set
// has no effect if the item is already in the set
// create a temp value of 1, left shift to relvant position, bitwise or with relevant word
void bitset_add(struct bitset * this, int item) {
  //fprintf(stderr, "Adding %i [%i.%i]\n", item, item/64, item%64);
  uint64_t temp = 1ULL;
  temp <<= item%64;
  this->bits[item/64] |= temp;
}

// remove an item with number 'item' from the set
// create a temp value of 1, left shift to relevant position, then invert and do bitwise and with relevant word
void bitset_remove(struct bitset * this, int item) {
  uint64_t temp = 1ULL;
  temp <<= item%64;
  this->bits[item/64] &= ~temp;
}

// place the union of src1 and src2 into dest;
// all of src1, src2, and dest must have the same size universe
// for each word, put bitwise or of src1 and src2 in dest
void bitset_union(struct bitset * dest, struct bitset * src1,
    struct bitset * src2) {
	for (int i = 0; i < src1->size_in_words; i++) {
            dest->bits[i] = src1->bits[i] | src2->bits[i];
  }
}

// place the intersection of src1 and src2 into dest
// all of src1, src2, and dest must have the same size universe
// for each word, put bitwise and of src1 and src2 in dest
void bitset_intersect(struct bitset * dest, struct bitset * src1,
    struct bitset * src2) {
	for (int i = 0; i < src1->size_in_words; i++) {
            dest->bits[i] = src1->bits[i] & src2->bits[i];
    }
}

//Bitset is freed in my local main