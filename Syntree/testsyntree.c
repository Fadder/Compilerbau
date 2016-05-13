#include <stdio.h>
#include "syntree.h"

int main()
{
	syntree_t tree;
	syntree_nid id;

	syntreeInit(&tree);

	id = syntreeNodeTag(&tree, syntreeNodeNumber(&tree, 0));
	for (unsigned int i = 1; i < 10; ++i)
	{
		switch (i % 3)
		{
		case 0:
			id = syntreeNodePair(&tree, syntreeNodeNumber(&tree, i), id);
			break;

		case 1:
			id = syntreeNodeAppend(&tree, id, syntreeNodeNumber(&tree, i));
			break;

		case 2:
			id = syntreeNodePrepend(&tree, syntreeNodeNumber(&tree, i), id);
			break;
		}
	}

  syntree_t tree2;
  syntree_nid id2;

  syntreeInit(&tree2);

  id2 = syntreeNodeTag(&tree2, syntreeNodeNumber(&tree2, 10));
  for (unsigned int i = 31; i < 40; ++i)
  {
    switch (i % 3)
    {
    case 0:
      id2 = syntreeNodePair(&tree2, syntreeNodeNumber(&tree2, i), id2);
      break;

    case 1:
      id2 = syntreeNodeAppend(&tree2, id2, syntreeNodeNumber(&tree2, i));
      break;

    case 2:
      id2 = syntreeNodePrepend(&tree2, syntreeNodeNumber(&tree2, i), id2);
      break;
    }
  }

	syntreePrint(&tree, id);
  printf("\n");
	syntreePrint(&tree2, id2);
  printf("\n");
  syntreeRelease(&tree);
	syntreeRelease(&tree2);

	return 0;
}

/* Erwartete Ausgabe nach Entfernen aller Leerzeichen:
{(9){(8)(6){(5)(3){(2)(0)(1)}(4)}(7)}}

Mit Leerzeichen:
{
    (9)
    {
        (8)
        (6)
        {
            (5)
            (3)
            {
                (2)
                (0)
                (1)
            }
            (4)
        }
        (7)
    }
}
*/
