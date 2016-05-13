#include "syntree.h"

#include <stdio.h>
#include <stdlib.h>


syntree_nid
getNewNID(){
  static syntree_nid id = -1;
  syntree_nid i = ++id;
  fprintf(stdout, "gave out id: %i\n", i);
  return i;
}


syntree_t*
getNode(const syntree_t* self, syntree_nid id){
  const syntree_t* current = self;
  while(current && current->id != id){
    current = current->next;
  }
  if(!current){
    fprintf(stderr, "There is no node with ID %i :(\n", id);
    abort();
  }
  return (syntree_t*) current;
}


extern int
syntreeInit(syntree_t* self){
  if(!self){
    fprintf(stderr, "Initialisation went wrong :(\n");
    return 1;
  }
  self->id            = getNewNID();
  self->nodeType      = LIST;
  self->elem.children = NULL;
  self->next          = NULL;
  return 0;
}

extern void
syntreeRelease(syntree_t* self){
  if(self->nodeType == LIST){
    syntreeRelease(self->elem.children);
  }
  syntreeRelease(self->next);
  free(self);
}

extern syntree_nid
syntreeNodeNumber(syntree_t* self, int number){
  syntree_t* newNode = (syntree_t*) malloc(sizeof(syntree_t));
  syntree_t* first = self->next;

  self->next = newNode;
  newNode->id          = getNewNID();
  newNode->nodeType    = LEAF;
  newNode->elem.number = number;
  newNode->next        = first;

  syntreePrint(self, self->id);
  return newNode->id;
}

extern syntree_nid
syntreeNodeTag(syntree_t* self, syntree_nid id){
  syntree_t* encapsulated = getNode(self, id);
  syntree_t* capsule = (syntree_t*) malloc(sizeof(syntree_t));
  capsule->id            = getNewNID();
  capsule->nodeType      = LIST;
  capsule->elem.children = encapsulated;
  capsule->next          = NULL;

  syntreePrint(self, self->id);
  return capsule->id;
}


extern syntree_nid
syntreeNodePair(syntree_t* self, syntree_nid id1, syntree_nid id2){
  syntree_t* capsule = (syntree_t*) malloc(sizeof(syntree_t));
  capsule->id            = getNewNID();
  capsule->nodeType      = LIST;
  capsule->elem.children = NULL;
  capsule->next          = NULL;

  syntree_t* encapsulated1 = getNode(self, id1);
  syntree_t* encapsulated2 = getNode(self, id2);
  capsule->elem.children   = encapsulated1;
  encapsulated1->next      = encapsulated2;

  return capsule->id;
}


extern syntree_nid
syntreeNodeAppend(syntree_t* self, syntree_nid list, syntree_nid elem){
  syntree_t* listNode = getNode(self, list);
  syntree_t* elemNode = getNode(self, elem);
  if(listNode->nodeType == LIST){
    syntree_t* current = listNode->elem.children;
    if(!current){
      listNode->elem.children = elemNode;
      return list;
    }
    while(current->next){
      current = current->next;
    }
    current->next = elemNode;
    return list;
  }
  else{
    fprintf(stderr, "Node with ID %i is no list node :(\n", list);
    abort();
  }
}


extern syntree_nid
syntreeNodePrepend(syntree_t* self, syntree_nid elem, syntree_nid list){
  syntree_t* elemNode = getNode(self, elem);
  syntree_t* listNode = getNode(self, list);
  if(listNode->nodeType == LIST){
    if(elemNode->next){
      fprintf(stderr, "Elem node with ID %i is already a child :(\n", elem);
      abort();
    }
    syntree_t* firstChild = listNode->elem.children;
    listNode->elem.children = elemNode;
    elemNode->next = firstChild;
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
  syntree_t* rootNode = getNode(self, root);
  if(rootNode->nodeType == LEAF){
    fprintf(stdout,"(%i)",rootNode->id);
  }
  else{
    fprintf(stdout, "{%i}\n", rootNode->id);
    current = rootNode->elem.children;
    while(current){
      syntreePrint(current, current->id);
      current = current->next;
    }
  }
}
