#include "syntree.h"

#include <stdio.h>
#include <stdlib.h>


node_t*
getNewNode(syntree_t* self){
  static syntree_nid id = -1;
  nodeList_t* list = (nodeList_t*) malloc(sizeof(nodeList_t));
  node_t* node     = (node_t*) malloc(sizeof(node_t));
  if(!list || !node){
    fprintf(stderr, "memory allocation went wrong :(\n");
    return NULL;
  }
  list->elem = node;
  list->next = self->nodes;
  self->nodes = list;
  node->id = ++id;
  return node;
}


node_t*
getNode(const syntree_t* self, syntree_nid id){
  nodeList_t* current = self->nodes;
  while(current && current->elem->id > id){
    current = current->next;
  }
  if(!current){
    fprintf(stderr, "There is no node with ID %i :(\n", id);
    abort();
  }
  return current->elem;
}


extern int
syntreeInit(syntree_t* self){
  if(!self) return 1;
  self->nodes = NULL;
  return 0;
}

void
nodeRelease(nodeList_t* self){
  if(!self) return;
  nodeRelease(self->next);
  free(self->elem);
  free(self);
}


extern void
syntreeRelease(syntree_t* self){
  if(!self){
    return;
  }
  nodeRelease(self->nodes);
}

extern syntree_nid
syntreeNodeNumber(syntree_t* self, int number){
  node_t* newNode      = getNewNode(self);
  newNode->nodeType       = LEAF;
  newNode->content.number = number;

  //syntreePrint(self, newNode->id);
  //fprintf(stdout, " Number\n");
  return newNode->id;
}

extern syntree_nid
syntreeNodeTag(syntree_t* self, syntree_nid id){
  node_t* capsule = getNewNode(self);
  nodeList_t* encapsulated = (nodeList_t*) malloc(sizeof(nodeList_t));
  encapsulated->elem = getNode(self, id);
  encapsulated->next = NULL;

  capsule->nodeType = LIST;
  capsule->content.children = encapsulated;

  //syntreePrint(self, capsule->id);
  //fprintf(stdout, " Tag\n");
  return capsule->id;
}


extern syntree_nid
syntreeNodePair(syntree_t* self, syntree_nid id1, syntree_nid id2){
  node_t* capsule = getNewNode(self);
  capsule->nodeType         = LIST;
  capsule->content.children = NULL;

  nodeList_t* encapsulated1 = (nodeList_t*) malloc(sizeof(nodeList_t));
  nodeList_t* encapsulated2 = (nodeList_t*) malloc(sizeof(nodeList_t));
  encapsulated1->elem = getNode(self, id1);
  encapsulated2->elem = getNode(self, id2);

  capsule->content.children = encapsulated1;
  encapsulated1->next       = encapsulated2;
  encapsulated2->next       = NULL;

  //syntreePrint(self, capsule->id);
  //fprintf(stdout, " Pair\n");
  return capsule->id;
}


extern syntree_nid
syntreeNodeAppend(syntree_t* self, syntree_nid list, syntree_nid elem){
  node_t* listNode = getNode(self, list);
  nodeList_t* elemChild = (nodeList_t*) malloc(sizeof(nodeList_t));
  elemChild->elem = getNode(self, elem);
  elemChild->next = NULL;

  if(listNode->nodeType == LIST){
    nodeList_t* current = listNode->content.children;
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
  //syntreePrint(self, list);
  //fprintf(stdout, " Append\n");
  return list;
}


extern syntree_nid
syntreeNodePrepend(syntree_t* self, syntree_nid elem, syntree_nid list){
  node_t* listNode = getNode(self, list);
  nodeList_t* elemChild = (nodeList_t*) malloc(sizeof(nodeList_t));
  elemChild->elem = getNode(self, elem);
  elemChild->next = NULL;

  if(listNode->nodeType == LIST){
    nodeList_t* children = listNode->content.children;
    listNode->content.children = elemChild;
    elemChild->next = children;
  }
  else{
    fprintf(stderr, "Node with ID %i is no list node :(\n",list);
    abort();
  }
  //syntreePrint(self, list);
  //fprintf(stdout, " Prepend\n");
  return list;
}


extern void
syntreePrint(const syntree_t* self, syntree_nid root){
  nodeList_t* current;
  node_t* rootNode = getNode(self, root);
  if(rootNode->nodeType == LEAF){
    fprintf(stdout,"(%i)",rootNode->content.number);
  }
  else{
    fprintf(stdout, "{");
    current = rootNode->content.children;
    while(current){
      syntreePrint(self, current->elem->id);
      current = current->next;
    }
    fprintf(stdout, "}");
  }
}
