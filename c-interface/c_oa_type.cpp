#include "c_oa_type.hpp"
#include "../Function.hpp"
#include "../Operator.hpp"

extern "C" {
  void destroy_array(void* A) {
    //cout<<"destroy_array called"<<endl;
    delete((ArrayPtr*) A);
  }

  void destroy_node(void* A) {
    delete((NodePtr*) A);
  }

  void* ones(int m, int n, int k, int stencil_width, 
    int data_type, MPI_Comm comm) {
    Shape s = {m, n, k};
    ArrayPtr ap = oa::funcs::ones(comm, s, stencil_width, data_type);
    ArrayPtr* A = new ArrayPtr();
    *A = ap;
    return A;
  }

  void* zeros(int m, int n, int k, int stencil_width, 
    int data_type, MPI_Comm comm) {
    Shape s = {m, n, k};
    ArrayPtr ap = oa::funcs::zeros(comm, s, stencil_width, data_type);
    ArrayPtr* A = new ArrayPtr();
    *A = ap;
    return A; 
  }

  void* rands(int m, int n, int k, int stencil_width, 
    int data_type, MPI_Comm comm) {
    Shape s = {m, n, k};
    ArrayPtr ap = oa::funcs::rand(comm, s, stencil_width, data_type);
    ArrayPtr* A = new ArrayPtr();
    *A = ap;
    return A;
  }

  void* seqs(int m, int n, int k, int stencil_width, 
    int data_type, MPI_Comm comm) {
    Shape s = {m, n, k};
    ArrayPtr ap = oa::funcs::seqs(comm, s, stencil_width, data_type);
    ArrayPtr* A = new ArrayPtr();
    *A = ap;
    return A;
  }

  #:mute
  #:set TYPE = [['int'], &
                ['float'], &
                ['double']]
  #:endmute

  #:for t in TYPE
  void* consts_${t[0]}$(int m, int n, int k, ${t[0]}$ val, 
    int stencil_width, MPI_Comm comm) {
    Shape s = {m, n, k};
    ArrayPtr ap = oa::funcs::consts(comm, s, val, stencil_width);
    ArrayPtr* A = new ArrayPtr();
    *A = ap;
    return A;
  }

  #:endfor

  #:for t in TYPE
  void* new_seqs_scalar_node_${t[0]}$(${t[0]}$ val, 
    MPI_Comm comm) {
    NodePtr np = oa::ops::new_seqs_scalar_node(comm, val);
    NodePtr* A = new NodePtr();
    *A = np;
    return A;
  }
    
  #:endfor

  void* new_node_array(void* ap) {
    NodePtr np = oa::ops::new_node(*(ArrayPtr*)ap);
    NodePtr* A = new NodePtr();
    *A = np;
    return A;
  }

  void* new_node_op2(int nodetype, void* u, void* v) {
    NodePtr np = oa::ops::new_node((NodeType)nodetype, *(NodePtr*)u, *(NodePtr*)v);
    NodePtr* A = new NodePtr();
    *A = np;
    return A;
  }

  void* new_node_op1(int nodetype, void* u) {
    NodePtr np = oa::ops::new_node((NodeType)nodetype, *(NodePtr*)u);
    NodePtr* A = new NodePtr();
    *A = np;
    return A;
  }
}
