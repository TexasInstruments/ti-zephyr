#ifndef MOTORVISION_HASH_H_
#define MOTORVISION_HASH_H_

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// HASH TABLE CONFIGURATION
// ============================================================================

// Hash table selection (for compatibility with F28 code)
#define HASH_TABLE_FourSineWaves   0
#define MVHASH_HASH_TABLE_SELECTED HASH_TABLE_FourSineWaves

// Number of variables in hash table
#define MVHASH_NUMBER_OF_VARIABLES 45

// Number of variable types supported
#define MVHASH_AMOUNT_OF_VARIABLE_TYPES 6

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

// Hash table array - stores pointers to variables
extern volatile void* MVHASH_hash_table[MVHASH_NUMBER_OF_VARIABLES];

// Type indices - indicate where each type starts in hash table
// -1 means type not present in table
extern int16_t MVHASH_uint16_t_startIndex;
extern int16_t MVHASH_uint32_t_startIndex;
extern int16_t MVHASH_int16_t_startIndex;
extern int16_t MVHASH_int32_t_startIndex;
extern int16_t MVHASH_bool_startIndex;
extern int16_t MVHASH_float_startIndex;

#endif /* MOTORVISION_HASH_H_ */
