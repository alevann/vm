#include "list.h"
#include <string.h>
#include <stdio.h>

struct list* list_new ()
{
  struct list* list = malloc(sizeof(struct list));
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;
  return list;
}

size_t list_add (struct list* list, void* data)
{
  struct node* node = malloc(sizeof(struct node));
  node->next = NULL;
  node->prev = NULL;
  node->data = data;

  // If tail is not null append the node at the end of the list
  if (list->tail) {
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
  } else {
    list->head = node;
    list->tail = node;
  }

  return ++(list->size);
}

void* list_del (struct list* list, void* data, comparator cmp)
{
  if (list->size == 0) {
    return NULL;
  }

  struct node* cursor = list->head;
  do {
    if (!cmp(data, cursor->data)) {
      continue;
    }
    printf("Found a match while deleting from list!\n");
    void* value = cursor->data;
    printf("Returning %p\n", value);

    // Update previous and next nodes to remove the cursor from the list 
    if (cursor->next) cursor->next->prev = cursor->prev;
    if (cursor->prev) cursor->prev->next = cursor->next;

    // If the cursor is the head or the tail of the list
    // update those values to avoid a SIGSEV on the next access
    if (cursor == list->tail) list->tail = cursor->prev;
    if (cursor == list->head) list->head = cursor->next;
    
    free(cursor);
    list->size--;
    return value;
  } while ((cursor = cursor->next));

  return NULL;
}

uint8_t list_contains (struct list* list, void* data, uint8_t (*cmp)(void*, void*))
{
  printf("Checking if list contains!\n");
  if (list->size == 0) {
    printf("List is empty\n");
    return 0;
  }

  struct node* cursor = list->head;
  do {
    printf("checking next...\n");
    if (cmp(data, cursor->data)) {
      printf("FOund a match!\n");
      return 1;
    }
  } while ((cursor = cursor->next));

  return 0;
}

uint8_t identity (void* v1, void* v2)
{
  return v1 == v2;
}

uint8_t streq (void* v1, void* v2)
{
  return strcmp(v1, v2) == 0;
}
