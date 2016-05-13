#include "syntree.h"

#include <stdio.h>
#include <stdlib.h>


typedef struct listNode {
  syntree_t* elem;
  struct listNode* prev;
} listNode_t;


listNode_t* syntreeNodes = NULL;


syntree_t*
getNewNode(){
  static syntree_nid id = -1;
  listNode_t* list = (listNode_t*) malloc(sizeof(listNode_t));
  syntree_t* tree  = (syntree_t*) malloc(sizeof(syntree_t));
  if(!list || !tree){
    fprintf(stderr, "memory allocation went wrong :(\n");
    return NULL;
  }
  list->elem = tree;
  list->prev = syntreeNodes;
  syntreeNodes = list;
  tree->id = ++id;
  return tree;
}


syntree_t*
getNode(syntree_nid id){
  listNode_t* current = syntreeNodes;
  syntree_nid lastID = (current) ? current->elem->id : 0;
  int i;
  for(i = 0; i < lastID-id; ++i){
    current = current->prev;
  }

  if(current->elem->id != id){
    fprintf(stderr, "There is no node with ID %i :(\n", id);
    abort();
  }
  return current->elem;
}


extern int
syntreeInit(syntree_t* self){
  listNode_t* list = (listNode_t*) malloc(sizeof(listNode_t));
  if(!list || !self){
    fprintf(stderr, "memory allocation went wrong :(\n");
    return 1;
  }
  list->elem   = self;
  list->prev   = syntreeNodes;
  syntreeNodes = list;
  self->id               = 0;
  self->nodeType         = LIST;
  self->content.children = NULL;
  return 0;
}

void
childrenRelease(children_t* self){
  if(!self){
    return;
  }
  if(self->next){
    childrenRelease(self->next);
  }
  syntreeRelease(self->elem);
}

extern void
syntreeRelease(syntree_t* self){
  if(!self){
    return;
  }
  printf("\nReleasing %u\n", self->id);
  if(self->nodeType == LIST){
    childrenRelease((children_t*) self->content.children);
  }
  free(self);
}

extern syntree_nid
syntreeNodeNumber(syntree_t* self, int number){
  syntree_t* newNode = getNewNode();
  newNode->nodeType    = LEAF;
  newNode->content.number = number;

  syntreePrint(self, newNode->id);
  fprintf(stdout, " Number\n");
  return newNode->id;
}

extern syntree_nid
syntreeNodeTag(syntree_t* self, syntree_nid id){
  syntree_t* capsule = getNewNode();
  children_t* encapsulated = (children_t*) malloc(sizeof(children_t));
  encapsulated->elem = getNode(id);
  encapsulated->next = NULL;

  capsule->nodeType = LIST;
  capsule->content.children = encapsulated;

  syntreePrint(self, capsule->id);
  fprintf(stdout, " Tag\n");
  return capsule->id;
}


extern syntree_nid
syntreeNodePair(syntree_t* self, syntree_nid id1, syntree_nid id2){
  syntree_t* capsule        = getNewNode();
  capsule->nodeType         = LIST;
  capsule->content.children = NULL;

  children_t* encapsulated1 = (children_t*) malloc(sizeof(children_t));
  children_t* encapsulated2 = (children_t*) malloc(sizeof(children_t));
  encapsulated1->elem = getNode(id1);
  encapsulated2->elem = getNode(id2);

  capsule->content.children = encapsulated1;
  encapsulated1->next       = encapsulated2;
  encapsulated2->next       = NULL;

  syntreePrint(self, capsule->id);
  fprintf(stdout, " Pair\n");
  return capsule->id;
}


extern syntree_nid
syntreeNodeAppend(syntree_t* self, syntree_nid list, syntree_nid elem){
  syntree_t* listNode   = getNode(list);
  children_t* elemChild = (children_t*) malloc(sizeof(children_t));
  elemChild->elem = getNode(elem);
  elemChild->next = NULL;

  if(listNode->nodeType == LIST){
    children_t* current = listNode->content.children;
    if(!current){
      listNode->content.children = elemChild;
    }
    else{
      while(current->next){
        current = current->next;
      }
      current->next = elemChild;
    }
  }
  else{
    fprintf(stderr, "Node with ID %i is no list node :(\n", list);
    abort();
  }
  syntreePrint(self, list);
  fprintf(stdout, " Append\n");
  return list;
}


extern syntree_nid
syntreeNodePrepend(syntree_t* self, syntree_nid elem, syntree_nid list){
  syntree_t* listNode = getNode(list);
  children_t* elemChild = (children_t*) malloc(sizeof(children_t));
  elemChild->elem = getNode(elem);
  elemChild->next = NULL;

  if(listNode->nodeType == LIST){
    children_t* children = listNode->content.children;
    listNode->content.children = elemChild;
    elemChild->next = children;
  }
  else{
    fprintf(stderr, "Node with ID %i is no list node :(\n",list);
    abort();
  }
  syntreePrint(self, list);
  fprintf(stdout, " Prepend\n");
  return list;
}


extern void
syntreePrint(const syntree_t* self, syntree_nid root){
  children_t* current;
  syntree_t* rootNode = getNode(root);
  if(rootNode->nodeType == LEAF){
    fprintf(stdout,"(%i)",rootNode->content.number);
  }
  else{
    fprintf(stdout, "{");
    current = rootNode->content.children;
    while(current){
      syntreePrint(current->elem, current->elem->id);
      current = current->next;
    }
    fprintf(stdout, "}");
  }
}
