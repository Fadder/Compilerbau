#include "syntree.h"

#include <stdio.h>
#include <stdlib.h>


typedef struct list_node {
  syntree_t* elem;
  struct list_node* prev;
} node;


node* syntreeNodes = NULL;


syntree_t*
getNewNode(){
  static syntree_nid id = -1;
  node* newNode      = (node*) malloc(sizeof(node));
  syntree_t* newTree = (syntree_t*) malloc(sizeof(syntree_t));
  if(!newNode || !newTree){
    fprintf(stderr, "memory allocation went wrong :(\n");
    return NULL;
  }
  newNode->elem = newTree;
  newNode->prev = syntreeNodes;
  syntreeNodes = newNode;
  newTree->id = ++id;
  return newTree;
}


syntree_t*
getNode(syntree_nid id){
  node* current = syntreeNodes;
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
  node* newNode      = (node*) malloc(sizeof(node));
  if(!newNode || !self){
    fprintf(stderr, "memory allocation went wrong :(\n");
    return 1;
  }
  newNode->elem = self;
  newNode->prev = syntreeNodes;
  syntreeNodes = newNode;
  self->id = 0;
  self->nodeType      = LIST;
  self->elem.children = NULL;
  self->next          = NULL;
  return 0;
}

extern void
syntreeRelease(syntree_t* self){
  if(!self){
    return;
  }
  printf("\nReleasing %u\n", self->id);
  if(self->nodeType == LIST){
    syntreeRelease(self->elem.children);
  }
  syntreeRelease(self->next);
  self = getNode(self->id);
  free(self);
}

extern syntree_nid
syntreeNodeNumber(syntree_t* self, int number){
  syntree_t* newNode = getNewNode();
  newNode->nodeType    = LEAF;
  newNode->elem.number = number;
  newNode->next        = NULL;

  syntreePrint(self, newNode->id);
  fprintf(stdout, " Number\n");
  return newNode->id;
}

extern syntree_nid
syntreeNodeTag(syntree_t* self, syntree_nid id){
  syntree_t* encapsulated = getNode(id);
  syntree_t* capsule = getNewNode();

  capsule->nodeType      = LIST;
  capsule->elem.children = encapsulated;
  capsule->next          = NULL;

  syntreePrint(self, capsule->id);
  fprintf(stdout, " Tag\n");
  return capsule->id;
}


extern syntree_nid
syntreeNodePair(syntree_t* self, syntree_nid id1, syntree_nid id2){
  syntree_t* capsule = getNewNode();
  capsule->nodeType      = LIST;
  capsule->elem.children = NULL;
  capsule->next          = NULL;

  syntree_t* encapsulated1 = getNode(id1);
  syntree_t* encapsulated2 = getNode(id2);
  capsule->elem.children   = encapsulated1;
  encapsulated1->next      = encapsulated2;

  syntreePrint(self, capsule->id);
  fprintf(stdout, " Pair\n");
  return capsule->id;
}


extern syntree_nid
syntreeNodeAppend(syntree_t* self, syntree_nid list, syntree_nid elem){
  syntree_t* listNode = getNode(list);
  syntree_t* elemNode = getNode(elem);

  if(listNode->nodeType == LIST){
    syntree_t* current = listNode->elem.children;
    if(!current){
      listNode->elem.children = elemNode;
    }
    else{
      while(current->next){
        current = current->next;
      }
      current->next = elemNode;
      elemNode->next = NULL;
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
  syntree_t* elemNode = getNode(elem);
  syntree_t* listNode = getNode(list);
  if(listNode->nodeType == LIST){
    if(elemNode->next){
      fprintf(stderr, "Elem node with ID %i is already a child :(\n", elem);
      abort();
    }
    syntree_t* firstChild = listNode->elem.children;
    listNode->elem.children = elemNode;
    elemNode->next = firstChild;
    syntreePrint(self, list);
    fprintf(stdout, " Prepend\n");
    return list;
  }
  else{
    fprintf(stderr, "Node with ID %i is no list node :(\n",list);
    abort();
  }
}


extern void
syntreePrint(const syntree_t* self, syntree_nid root){
  syntree_t* current;
  syntree_t* rootNode = getNode(root);
  if(rootNode->nodeType == LEAF){
    fprintf(stdout,"(%i)",rootNode->elem.number);
  }
  else{
    fprintf(stdout, "{");
    current = rootNode->elem.children;
    while(current){
      syntreePrint(current, current->id);
      current = current->next;
    }
    fprintf(stdout, "}");
  }
}
