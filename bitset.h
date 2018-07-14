#ifndef BITSET_H_
#define BITSET_H_

#include <limits.h>
#include <stddef.h>

/* The integer type representing a single chunk of a set. A set always consists
 * of a whole number of chunks. */
typedef unsigned long long bitset_chunk_t;

/* The number of bits in a single chunk. */
#define BITSET_CHUNK_BIT (sizeof(bitset_chunk_t) * CHAR_BIT)

/* Get the number of chunks in a set.
 * Arguments:
 *   num_or_set: Either a number of bits or a set of bits. providing a set is
 *    equivalent to calling this macro with the number of bits in the set.
 * Return value: The number of chunks. */
#define BITSET_CHUNKS(num_or_set)                                              \
	( (size_t)((num_or_set) + 1) - (size_t)(num_or_set) == 1               \
	? ((size_t)(num_or_set) + BITSET_CHUNK_BIT - 1) / BITSET_CHUNK_BIT     \
	: sizeof((num_or_set)) / sizeof(*(bitset_chunk_t *)(num_or_set))       )

/* Get the number of bits in a set.
 * Arguments:
 *   num_or_set: Either a number of chunks or a set. Providing a set is
 *    equivalent to calling this macro with the number of chunks in the set.
 * Return value: The number of bits. */
#define BITSET_SIZE(num_or_set)                                                \
	( (size_t)((num_or_set) + 1) - (size_t)(num_or_set) == 1               \
	? ((size_t)(num_or_set)) * BITSET_CHUNK_BIT                            \
	: sizeof((num_or_set)) * CHAR_BIT                                      )

/* Defines a fixed-size bit set.
 * Arguments:
 *   name: The name of the variable.
 *   size: The minimum number of bits in the set. The set might have more, but
 *    never less.*/
#define BITSET_DEFINE(name, size) bitset_chunk_t name[BITSET_CHUNKS(size)]

/* Compute the index of a chunk from a bit index.
 * Arguments:
 *   i: The bit index.
 * Return value: The index of the chunk. */
#define BITSET_IDX_MAJOR(i) ((i) / BITSET_CHUNK_BIT)
/* Compute the index of a bit within a chunk from a bit index.
 * Arguments:
 *   i: The bit index.
 * Return value: The bit index fit to the size of the chunk. */
#define BITSET_IDX_MINOR(i) ((i) % BITSET_CHUNK_BIT)

/* The following three macros manipulate a set a certain index. The bitwise
 *  operations corresponding to the macro actions are noted above each macro.
 * Arguments:
 *   set: The bit set to mutate.
 *   idx: The index where the set should be mutated.
 * Return value: None. */
/* Bitwise equivalent: | bit */
#define BITSET_PUT(set, idx) \
	((void)((set)[BITSET_IDX_MAJOR(idx)] |= 1l << BITSET_IDX_MINOR(idx)))
/* Bitwise equivalent: & ~bit */
#define BITSET_REMOVE(set, idx) \
	((void)((set)[BITSET_IDX_MAJOR(idx)] &= ~(1l << BITSET_IDX_MINOR(idx))))
/* Bitwise equivalent: ^ bit */
#define BITSET_FLIP(set, idx) \
	((void)((set)[BITSET_IDX_MAJOR(idx)] ^= 1l << BITSET_IDX_MINOR(idx)))

/* Check whether a certain bit in a set is flipped on.
 * Arguments:
 *   set: The bit set to look in.
 *   idx: The index to look at.
 * Return value: 1 if the bit at idx is on, 0 otherwise. */
#define BITSET_HAS(set, idx) \
	(((set)[BITSET_IDX_MAJOR(idx)] & (1l << BITSET_IDX_MINOR(idx))) != 0)

#endif /* Header guard */
