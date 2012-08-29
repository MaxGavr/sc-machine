#include "sc_config.h"
#include "sc_types.h"
#include "sc_element.h"


//! sc-iterator types
typedef enum
{
  sc_iterator3_f_a_a = 0, // output arcs
  sc_iterator3_a_a_f,     // input arcs
  sc_iterator3_f_a_f      // arcs between two elements
} sc_iterator_type;

/*! Iterator parameter
 */
struct _sc_iterator_param
{
  gboolean is_type;
  union
  {
    sc_addr addr;
    sc_type type;
  };
};

/*! Structure to store iterator information
 */
struct _sc_iterator3
{
  sc_iterator_type type; // iterator type (search template)
  sc_iterator_param params[3]; // parameters array
  sc_addr results[3]; // results array (same size as params)
  guint32 time_stamp; // iterator creation time stamp
};

/*! Create iterator to find output arcs for specified element
 * @param el sc-addr of element to iterate output arcs
 * @param arc_type Type of output arc to iterate (0 - all types)
 * @param end_type Type of end element for output arcs, to iterate
 * @return If iterator created, then return pointer to it; otherwise return null
 */
sc_iterator3* sc_iterator3_f_a_a_new(sc_addr el, sc_type arc_type, sc_type end_type);

/*! Create iterator to find input arcs for specified element
 * @param beg_type Type of begin element for input arcs, to iterate
 * @param arc_type Type of input arc to iterate (0 - all types)
 * @param el sc-addr of element to iterate input arcs
 * @return If iterator created, then return pointer to it; otherwise return null
 */
sc_iterator3* sc_iterator3_a_a_f_new(sc_type beg_type, sc_type arc_type, sc_addr el);

/*! Create iterator to find arcs between two specified elements
 * @param el_beg sc-addr of begin element
 * @param arc_type Type of arcs to iterate (0 - all types)
 * @param el_end sc-addr of end element
 * @return If iterator created, then return pointer to it; otherwise return null
 */
sc_iterator3* sc_iterator3_f_a_f_new(sc_addr el_beg, sc_type arc_type, sc_addr el_end);

/*! Create new sc-iterator-3
 * @param type Iterator type (search template)
 * @param p1 First iterator parameter
 * @param p2 Second iterator parameter
 * @param p3 Third iterator parameter
 * @return Pointer to created iterator. If parameters invalid for specified iterator type, or type is not a sc-iterator-3, then return 0 
 */
sc_iterator3* sc_iterator3_new(sc_iterator_type type, sc_iterator_param p1, sc_iterator_param p2, sc_iterator_param p3);

/*! Destroy iterator and free allocated memory
 * @param it Pointer to sc-iterator that need to be destroyed
 */
void sc_iterator3_free(sc_iterator3 *it);

/*! Go to next iterator result
 * @param it Pointer to iterator that we need to go next result
 * @return Return TRUE, if iterator moved to new results; otherwise return FALSE.
 * example: while(sc_iterator_next(it)) { <your code> }
 */
gboolean sc_iterator3_next(sc_iterator3 *it);

/*! Get iterator value
 * @param it Pointer to iterator for getting value
 * @param vid Value id (can't be more that 3 for sc-iterator3)
 * @return Return sc-addr of search result value
 */
sc_addr sc_iterator3_value(sc_iterator3 *it, guint vid);