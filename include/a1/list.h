#ifndef __A1_LIST_H_
#define __A1_LIST_H_

#include <stdlib.h>
#include <stdint.h>

/**
 * A function that is able to compare between two void pointers.
 * 
 * Comparator functions can be called multiple times
 * so they should be decently fast.
 */
typedef uint8_t (*comparator)(void*, void*);


/**
 * A list which supports any type. It's safe to use integers despite 
 * the "making pointer from integer" warning
 * so long as care is taken to ensure they are not used as pointers.
 * 
 * Data in the list is only stored and never read,
 * comparator access the data but the list itself will never.
 * 
 * Pointers stored in the list can
 * still be freed from outside.
 */
struct list {
  struct node* head;
  struct node* tail;
  size_t size;
};

struct node;

/// Creates a new empty list
struct list* list_new ();
/// Adds an item to the list
size_t list_add (struct list*, void*);

/**
 * @brief removes an item from the list in O(n)
 * @param data the first argument passed to the comparator,
 *  may be NULL depending on the comparator
 * @param cmp a comparator function that can compare
 *  the data stored in the list
 */
void* list_rem (struct list*, void* data, comparator cmp);
/**
 * @brief returns non-zero if the list contains data in O(n)
 * @param data the data to look for in the list
 * @param cmp a comparator function used to determine if any 
 *  of the items in the list match the passed data value
 */
uint8_t list_contains (struct list*, void* data, comparator);

#endif
