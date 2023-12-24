#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct TCriminal
{
  struct TCriminal  * m_Next;
  char              * m_Name;
  struct TCriminal ** m_Contacts;
  size_t              m_Cnt;
  size_t              m_Capacity;
} TCRIMINAL;

#endif /* __PROGTEST__ */

TCRIMINAL        * createRecord ( const char      * name,
                                  TCRIMINAL       * next )
{

  TCRIMINAL * new_criminal = (TCRIMINAL*)malloc(sizeof(TCRIMINAL));
  new_criminal->m_Next = next;
  new_criminal->m_Name = (char*)malloc(strlen(name)+1);
  strcpy(new_criminal->m_Name, name);
  new_criminal->m_Contacts = NULL;
  new_criminal->m_Cnt = 0;
  new_criminal->m_Capacity = 0;
  
  return new_criminal;
}

void               addContact   ( TCRIMINAL       * dst,
                                  TCRIMINAL       * contact )
{
  if(dst->m_Capacity == 0){
    dst->m_Capacity = 1;
    dst->m_Contacts = (TCRIMINAL**)malloc(sizeof(TCRIMINAL*));
  }
  else if(dst->m_Capacity == dst->m_Cnt){
    dst->m_Capacity *= 2;
    dst->m_Contacts = (TCRIMINAL**)realloc(dst->m_Contacts, dst->m_Capacity*sizeof(TCRIMINAL*));
  }
  dst->m_Contacts[dst->m_Cnt] = contact;
  dst->m_Cnt++;
}


TCRIMINAL        * cloneList    ( TCRIMINAL       * src )
{
    if (src == NULL) {
        return NULL;  // Return NULL for an empty list
    }

    // Copy the first criminal in the list with only the name
    TCRIMINAL *new_head = createRecord(src->m_Name, NULL);

    TCRIMINAL *current = new_head;
    TCRIMINAL *src_current = src->m_Next;

    // Copy the rest of the list with only the names
    while (src_current != NULL) {
        current->m_Next = createRecord(src_current->m_Name, NULL);
        current = current->m_Next;
        src_current = src_current->m_Next;
    }
    // Copy the contacts
    current = new_head;
    src_current = src;
    while (src_current != NULL){
        for (size_t i = 0; i < src_current->m_Cnt; ++i) {
            TCRIMINAL *contact = src_current->m_Contacts[i];
            TCRIMINAL *newContact = new_head;
            // Find the contact in the new list by name
            while (newContact != NULL) {
                if (strcmp(contact->m_Name, newContact->m_Name) == 0) {
                    addContact(current, newContact);
                    break;
                }
                newContact = newContact->m_Next;
            }
        }
        current = current->m_Next;
        src_current = src_current->m_Next;
    }
    return new_head;
}

void               freeList     ( TCRIMINAL       * src )
{
  while(src != NULL){
    TCRIMINAL * next = src->m_Next;
    free(src->m_Name);
    free(src->m_Contacts);
    free(src);
    src = next;
  }
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  TCRIMINAL * a, *b;
  char tmp[100];

  assert ( sizeof ( TCRIMINAL ) == 3 * sizeof ( void * ) + 2 * sizeof ( size_t ) );
  a = nullptr;
  a = createRecord ( "Peter", a );
  a = createRecord ( "John", a );
  a = createRecord ( "Joe", a );
  a = createRecord ( "Maria", a );
  addContact ( a, a -> m_Next );
  addContact ( a -> m_Next -> m_Next, a -> m_Next -> m_Next -> m_Next );
  addContact ( a -> m_Next -> m_Next -> m_Next, a -> m_Next );

  assert ( a
           && ! strcmp ( a -> m_Name, "Maria" )
           && a -> m_Cnt == 1
           && a -> m_Contacts[0] == a -> m_Next );
  assert ( a -> m_Next
           && ! strcmp ( a -> m_Next -> m_Name, "Joe" )
           && a -> m_Next -> m_Cnt == 0 );
  assert ( a -> m_Next -> m_Next
           && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "John" )
           && a -> m_Next -> m_Next -> m_Cnt == 1
           && a -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next -> m_Next -> m_Next );
  assert ( a -> m_Next -> m_Next -> m_Next
           && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
           && a -> m_Next -> m_Next -> m_Next -> m_Cnt == 1
           && a -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next );
  assert ( a -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );

  b = cloneList ( a );

  strcpy ( tmp, "Moe" );
  a = createRecord ( tmp, a );
  strcpy ( tmp, "Victoria" );
  a = createRecord ( tmp, a );
  strcpy ( tmp, "Peter" );
  a = createRecord ( tmp, a );
  addContact ( b -> m_Next -> m_Next -> m_Next, b -> m_Next -> m_Next );
  assert ( a
          && ! strcmp ( a -> m_Name, "Peter" )
          && a -> m_Cnt == 0 );
  assert ( a -> m_Next
          && ! strcmp ( a -> m_Next -> m_Name, "Victoria" )
          && a -> m_Next -> m_Cnt == 0 );
  assert ( a -> m_Next -> m_Next
          && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "Moe" )
          && a -> m_Next -> m_Next -> m_Cnt == 0 );
  assert ( a -> m_Next -> m_Next -> m_Next
          && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Maria" )
          && a -> m_Next -> m_Next -> m_Next -> m_Cnt == 1
          && a -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( a -> m_Next -> m_Next -> m_Next -> m_Next
          && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Joe" )
          && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cnt == 0 );
  assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
          && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "John" )
          && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cnt == 1
          && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
          && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
          && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Cnt == 1
          && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );

  assert (b -> m_Contacts[0] == b -> m_Next);
  assert ( b
           && ! strcmp ( b -> m_Name, "Maria" )
           && b -> m_Cnt == 1
           && b -> m_Contacts[0] == b -> m_Next );
  assert ( b -> m_Next
           && ! strcmp ( b -> m_Next -> m_Name, "Joe" )
           && b -> m_Next -> m_Cnt == 0 );
  assert ( b -> m_Next -> m_Next
           && ! strcmp ( b -> m_Next -> m_Next -> m_Name, "John" )
           && b -> m_Next -> m_Next -> m_Cnt == 1
           && b -> m_Next -> m_Next -> m_Contacts[0] == b -> m_Next -> m_Next -> m_Next );
  assert ( b -> m_Next -> m_Next -> m_Next
           && ! strcmp ( b -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
           && b -> m_Next -> m_Next -> m_Next -> m_Cnt == 2
           && b -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == b -> m_Next
           && b -> m_Next -> m_Next -> m_Next -> m_Contacts[1] == b -> m_Next -> m_Next );
  assert ( b -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
  freeList ( a );
  addContact ( b -> m_Next, b -> m_Next );
  a = cloneList ( b );
  assert ( a
           && ! strcmp ( a -> m_Name, "Maria" )
           && a -> m_Cnt == 1
           && a -> m_Contacts[0] == a -> m_Next );
  assert ( a -> m_Next
           && ! strcmp ( a -> m_Next -> m_Name, "Joe" )
           && a -> m_Next -> m_Cnt == 1
           && a -> m_Next -> m_Contacts[0] == a -> m_Next );
  assert ( a -> m_Next -> m_Next
           && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "John" )
           && a -> m_Next -> m_Next -> m_Cnt == 1
           && a -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next -> m_Next -> m_Next );
  assert ( a -> m_Next -> m_Next -> m_Next
           && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
           && a -> m_Next -> m_Next -> m_Next -> m_Cnt == 2
           && a -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == a -> m_Next
           && a -> m_Next -> m_Next -> m_Next -> m_Contacts[1] == a -> m_Next -> m_Next );
  assert ( a -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
  assert ( b
           && ! strcmp ( b -> m_Name, "Maria" )
           && b -> m_Cnt == 1
           && b -> m_Contacts[0] == b -> m_Next );
  assert ( b -> m_Next
           && ! strcmp ( b -> m_Next -> m_Name, "Joe" )
           && b -> m_Next -> m_Cnt == 1
           && b -> m_Next -> m_Contacts[0] == b -> m_Next );
  assert ( b -> m_Next -> m_Next
           && ! strcmp ( b -> m_Next -> m_Next -> m_Name, "John" )
           && b -> m_Next -> m_Next -> m_Cnt == 1
           && b -> m_Next -> m_Next -> m_Contacts[0] == b -> m_Next -> m_Next -> m_Next );
  assert ( b -> m_Next -> m_Next -> m_Next
           && ! strcmp ( b -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
           && b -> m_Next -> m_Next -> m_Next -> m_Cnt == 2
           && b -> m_Next -> m_Next -> m_Next -> m_Contacts[0] == b -> m_Next
           && b -> m_Next -> m_Next -> m_Next -> m_Contacts[1] == b -> m_Next -> m_Next );
  assert ( b -> m_Next -> m_Next -> m_Next -> m_Next == nullptr );
  freeList ( b );
  freeList ( a );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
