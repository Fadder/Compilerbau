##include "syntree.h"

syntree_nid
getNewNID(){
  static syntree_nid id = 0;
  return ++id;
}

extern int
syntreeInit(syntree_t* self){
  if(self = (syntree_t*) malloc(sizeof(syntree_t)) == NULL){
    return 1;
  }
  self->elem = NULL;
  self->next = NULL;
  return 0;
}

extern void
syntreeRelease(syntree_t* self){
  syntreeRelease(self.next);
  free(self);
}

extern syntree_nid
syntreeNodeNumber(syntree_t* self, int number){
  syntree_t* newNode = (syntree_t*) malloc(sizeof(syntree_t));
  newNode.id = getNewNID();
  newNode.number = number;
  newNode.next = NULL;

  if(!self->elem){
    self.elem = newNode;
  }
  else{
    syntree_nid last = self.next;
    while(next){
      last = last.next;
    }
    last.next = newNode.id;
  }

  return newNode.id;
}

extern syntree_nid
syntreeNodeTag(syntree_t* self, syntree_nid id){
  syntree_t* node = self;
  while(node.elem.id != id && node.next){
    node = node.next;
  }
  if(node.elem.id == id){
    if(node.elem.childrenList){ // TODO ??? geht das so

    }
  }
  else{
    // TODO ERR
  }

}
