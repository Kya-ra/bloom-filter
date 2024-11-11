#include "bloom.h"

// include the standard library header files that we use in this
// program, which are denoted by angle brackets < >
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// define constant dummy values to avoid compiler warnings
static const int dummy_integer_value = -99;
static struct bloom * dummy_pointer_value = NULL;

const int BLOOM_HASH1 = 17;
const int BLOOM_HASH2 = 29;

// compute a hash of a string using a seed value, where the result
// falls between zero and range-1
int hash_string(char * string, int seed, int range)
{
  int i;
  int hash = 0;

  // simple loop for mixing the input string
  for ( i = 0; string[i] != '\0'; i++ ) {
    hash = hash * seed + string[i];
  }
  // check for unlikely case that hash is negative
  if ( hash < 0 ) {
    hash = -hash;
  }
  // bring the hash within the range 0..range-1
  hash = hash % range;

  return hash;
}


// create a new, empty Bloom filter of 'size' items
struct bloom * bloom_new(int size) {
  struct bloom * set;
  set = malloc(sizeof(struct bloom));
  set->size = size;
  set->bitset = bitset_new(size);
  return set;
}

// check to see if a string is in the set
// convert it to the hashed values, then check both against the bitset, and return true only if both are in the bitset
int bloom_lookup(struct bloom * this, char * item) {
  int hashresult1 = hash_string(item, BLOOM_HASH1, this->size);
  int hashresult2 = hash_string(item, BLOOM_HASH2, this->size);
  hashresult1 = bitset_lookup(this->bitset, hashresult1);
  hashresult2 = bitset_lookup(this->bitset, hashresult2);
  return hashresult1 && hashresult2;
}

// add a string to the set
// has no effect if the item is already in the set
// hash the string and add both to the bitset
void bloom_add(struct bloom * this, char * item) {
    int hashresult1 = hash_string(item, BLOOM_HASH1, this->size);
    int hashresult2 = hash_string(item, BLOOM_HASH2, this->size);
    bitset_add(this->bitset, hashresult1);
    bitset_add(this->bitset, hashresult2);
}

// place the union of src1 and src2 into dest
void bloom_union(struct bloom * dest, struct bloom * src1,
    struct bloom * src2) {
	bitset_union(dest->bitset, src1->bitset, src2->bitset);
}

// place the intersection of src1 and src2 into dest
void bloom_intersect(struct bloom * dest, struct bloom * src1,
    struct bloom * src2) {
	bitset_intersect(dest->bitset, src1->bitset, src2->bitset);
}

//Bloom is freed in my local main