#ifndef SYNTREE_H_INCLUDED
#define SYNTREE_H_INCLUDED

/* *** structures *********************************************************** */

typedef unsigned int syntree_nid;

/**@brief Struktur des abstrakten Syntaxbaumes.
 */
typedef struct syntree{
  syntree_nid id;
  enum {LIST,LEAF} nodeType;
  union{
    int number;
    struct syntree* children;
  } elem;
  struct syntree* next;
} syntree_t;


/* *** interface ************************************************************ */

/**@brief Initialisiert einen neuen Syntaxbaum.
 * @param self  der zu initialisierende Syntaxbaum
 * @return 0, falls keine Fehler bei der Initialisierung aufgetreten sind\n
 *      != 0 ansonsten
 */
extern int
syntreeInit(syntree_t* self);

/**@brief Gibt den Syntaxbaum und alle assoziierten Strukturen frei.
 * @param self  der freizugebende Syntaxbaum
 */
extern void
syntreeRelease(syntree_t* self);

/**@brief Erstellt einen neuen Knoten mit einem Zahlenwert als Inhalt.
 * @param self    der Syntaxbaum
 * @param number  die Zahl
 * @param ID des neu erstellten Knoten
 */
extern syntree_nid
syntreeNodeNumber(syntree_t* self, int number);

/**@brief Kapselt einen Knoten innerhalb eines anderen Knotens.
 * @param self  der Syntaxbaum
 * @param id    der zu kapselnde Knoten
 * @param ID des neu erstellten Knoten
 */
extern syntree_nid
syntreeNodeTag(syntree_t* self, syntree_nid id);

/**@brief Kapselt zwei Knoten innerhalb eines Knoten.
 * @param self  der Syntaxbaum
 * @param id1   erster Knoten
 * @param id2   zweiter Knoten
 * @param ID des neu erstellten Knoten
 */
extern syntree_nid
syntreeNodePair(syntree_t* self, syntree_nid id1, syntree_nid id2);

/**@brief Hängt einen Knoten an das Ende eines Listenknotens.
 * @param self  der Syntaxbaum
 * @param list  Listenknoten
 * @param elem  anzuhängender Knoten
 * @param ID des Listenknoten
 */
extern syntree_nid
syntreeNodeAppend(syntree_t* self, syntree_nid list, syntree_nid elem);

/**@brief Hängt einen Knoten an den Anfang eines Listenknotens.
 * @param self  der Syntaxbaum
 * @param elem  anzuhängender Knoten
 * @param list  Listenknoten
 * @param ID des Listenknoten
 */
extern syntree_nid
syntreeNodePrepend(syntree_t* self, syntree_nid elem, syntree_nid list);

/**@brief Gibt alle Zahlenknoten rekursiv (depth-first) aus.
 * @param self  der Syntaxbaum
 * @param root  der Wurzelknoten
 */
extern void
syntreePrint(const syntree_t* self, syntree_nid root);

#endif /* SYNTREE_H_INCLUDED */
