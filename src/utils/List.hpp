#ifndef CIRCULAR_LINKED_LIST_HOME
#define CIRCULAR_LINKED_LIST_HOME

namespace ArduinoManager
{
  namespace Utils
  {
    class List
    {
    private:
      struct _List
      {
        void *value;
        _List *next;
      };

      _List *l; // Puntero al último nodo
      int size;

      _List *_get_pointer(int index);

    public:
      List();
      ~List();

      bool add(void *value);       // Agrega al final
      bool add_first(void *value); // Agrega al inicio
      void *pop();                 // Elimina y devuelve el primero
      void *get(int index);        // Devuelve el valor en un índice
      void clear();                // Limpia toda la lista
      int get_size();              // Devuelve el tamaño
    };
  }
}

#endif
