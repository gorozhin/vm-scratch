#include <iostream>
#include <string.h>

class Memory {
  typedef struct M {
    struct M* l;
    struct M* r;
    int data;
  } M;
private:
  M* root;

public:
  Memory(){
    M* nm = new M;

    nm->data = 0;
	
    nm->l = NULL;
    nm->r = NULL;
    root = nm;
  }

  void inc(){
    if (root != NULL){
      root->data++;
    }
  }

  void dec(){
    if (root != NULL){
      root->data--;
    }
  }
  
  void set(int d){
    if (root != NULL){
      root->data = d;
    }
  }

  int get(){
    if (root != NULL){
      return root->data;
    }
    throw std::out_of_range("GET");
  }
  
  void shiftL(){
    if (root == NULL){
      M* nm = new M;

      nm->data = 0;
	
      nm->l = NULL;
      nm->r = NULL;
      root = nm;
      return;
    }
    
    if (root->l != NULL){
      root = root->l;
    } else {
      M* nm = new M;

      nm->data = 0;
	
      nm->r = root;
      nm->l = NULL;
      root->l = nm;
      
      root = root->l;
    }
  }

  void shiftR(){
    if (root == NULL){
      M* nm = new M;

      nm->data = 0;
	
      nm->l = NULL;
      nm->r = NULL;
      root = nm;
      return;
    }
    
    if (root->r != NULL){
      root = root->r;
    } else {
      M* nm = new M;

      nm->data = 0;
	
      nm->l = root;
      nm->r = NULL;
      root->r = nm;
      
      root = root->r;
    }
  }
};

class State{
  typedef struct s {
    int data;
    struct s* next;
  } S;


  private:
public:

  int pos;
  S* marks;
  
  State(){
    pos = 0;
    marks = NULL;
  }

  void inc(){
    pos++;
  }
  
  void dec(){
    pos++;
  }

  void push(int i){
    S* ns = new S;
    ns->data = i;
    ns->next = marks;
    
    marks = ns;
  }

  int pop(){
    if (marks == NULL) throw std::out_of_range("POP");
    else {
      S* ns = marks;
      int data = ns->data;
      marks = ns->next;
      delete ns;
      return data;
    }
  }

  void save(){
    push(pos);
  }

  void recover(){
    //std::cout << "HERE;";
    pos = pop();
  }
};

class Interpreter{
private:
  Memory* memory;
  char* program;
  State* state;

public:
  Interpreter(Memory* m, State* s, char* prog){
    memory = m;
    program = prog;
    state = s;
  }

  void interpretCommand(char c){
    switch(c){
    case '>' :
      memory->shiftR();
      state->inc();

      break;
    case '<' :
      memory->shiftL();
      state->inc();

      break;
    case '.' : 
      std::cout << (char)memory->get();
      state->inc();    
      break;
    case ',' : {
      char cc;
      std::cin >> cc;
      memory->set(cc);
      state->inc();
	    
    }
      break;
    case '+' :
      memory->inc();
      state->inc();

      break;
    case '-' :
      memory->dec();
      state->inc();
	    
      break;
    case '[' :
      if (memory->get()){
	state->save();
      } else {
	int check = 0;
	state->inc();
	while(program[state->pos] != ']' && !check) {
	  if (program[state->pos] == '[') check++;
	  if (program[state->pos] == ']') check--;
	  state->inc();
	}
	//state->pop();
      }
      state->inc();

      break;
    case ']' :
      state->recover();
      break;
    default:
      state->inc();	
    }

  }
  
  void run(int steps){
    int l = strlen(program);
    for(int i = 0; i < steps; i++){
      if (state->pos > l) return;
      interpretCommand(program[state->pos]);
    }
  }

  
};

int main(){
  Memory m;  
  State s;
  char p[] = "+++++++++[>++++++++<-]>.<+++[>+++++++++++<-]>.[-]<+++[>+++++++++++<-]>.";
  
  Interpreter i(&m,&s,p);
  i.run(1000);
  
}


