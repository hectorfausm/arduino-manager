#include "List.hpp"
#include <stdlib.h>
#include <stdio.h>

namespace ArduinoManager
{
  namespace Utils
  {
    List::List()
    {
      this->l = NULL;
      this->size = 0;
    }

    List::~List()
    {
      this->clear();
    }

    bool List::add(void *value)
    {
      _List *q = (_List *)malloc(sizeof(_List));
      if (!q)
        return false;

      q->value = value;

      if (this->l == NULL)
      {
        q->next = q;
        this->l = q;
      }
      else
      {
        q->next = this->l->next;
        this->l->next = q;
        this->l = q;
      }

      this->size++;
      return true;
    }

    bool List::add_first(void *value)
    {
      _List *q = (_List *)malloc(sizeof(_List));
      if (!q)
        return false;

      q->value = value;

      if (this->l == NULL)
      {
        q->next = q;
        this->l = q;
      }
      else
      {
        q->next = this->l->next;
        this->l->next = q;
      }

      this->size++;
      return true;
    }

    void *List::pop()
    {
      if (this->l == NULL)
        return NULL;

      _List *first = this->l->next;

      if (first == this->l)
      {
        this->l = NULL;
      }
      else
      {
        this->l->next = first->next;
      }

      void *result = first->value;
      free(first);
      this->size--;

      return result;
    }

    void *List::get(int index)
    {
      _List *q = this->_get_pointer(index);
      return q ? q->value : NULL;
    }

    void List::clear()
    {
      while (this->size > 0)
      {
        this->pop();
      }
    }

    int List::get_size()
    {
      return this->size;
    }

    List::_List *List::_get_pointer(int index)
    {
      if (this->l == NULL || index < 0 || index >= this->size)
        return NULL;

      _List *q = this->l->next;
      for (int i = 0; i < index; i++)
      {
        q = q->next;
      }
      return q;
    }
  }
}