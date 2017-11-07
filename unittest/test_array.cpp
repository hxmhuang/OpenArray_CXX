
#include <armadillo>
#include "../Array.hpp"
#include "../Function.hpp"
#include "../NodePool.hpp"
#include "../Operator.hpp"
#include "gtest/gtest.h"


namespace gt=::testing;

#define MRange gt::Range(3, 4)
#define NRange gt::Range(3, 4)
#define PRange gt::Range(3, 4)

class MPITest : public gt::TestWithParam
<std::tr1::tuple<MPI_Comm, int, int, int> > {
protected:
  virtual void SetUp() {

    comm = std::tr1::get<0>(GetParam());

    m  = std::tr1::get<1>(GetParam());
    n  = std::tr1::get<2>(GetParam());
    p  = std::tr1::get<3>(GetParam());

    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);
  }

  virtual void TearDown() {
    
  }

  MPI_Comm comm;
  int size;
  int rank;

  int m;
  int n;
  int p;
};

template<class T>
arma::Cube<T> make_seqs(int m, int n, int p){
  arma::Cube<T> v1(m, n, p);

  int cnt = 0;
  for(int k = 0; k < p; ++k){
    for(int j = 0; j < n; ++j){
      for(int i = 0; i < m; ++i){
	v1(i, j, k) = cnt;
	cnt++;
      }
    }
  }

  return v1;
}

///:set dtypes = ['int', 'float', 'double']


namespace{

  TEST_P(MPITest, ArrayCreation){

    ///:for t in dtypes
    {
      ArrayPtr A1 = oa::funcs::to_rank0(oa::funcs::seqs(comm, {m, n, p}, 1,
							oa::utils::dtype<${t}$>::type));
      arma::Cube<${t}$> B1 = make_seqs<${t}$>(m, n, p);

      ArrayPtr A2 =
	oa::funcs::to_rank0(oa::funcs::consts<${t}$>(comm, {m, n, p},
						     ${t}$(2),
						     oa::utils::dtype<${t}$>::type));
      arma::Cube<${t}$> B2(m, n, p);
      B2.fill(${t}$(2));
      
      if(rank == 0){
	EXPECT_TRUE(oa::funcs::is_equal(A1, B1));
	EXPECT_TRUE(oa::funcs::is_equal(A2, B2));
      }
    }
    ///:endfor
  }


  TEST_P(MPITest, BasicMath){

    ///:for t1 in dtypes
    ///:for t2 in dtypes
    {
      DataType dt1  = oa::utils::dtype<${t1}$>::type;
      DataType dt2  = oa::utils::dtype<${t2}$>::type;
      
      NodePtr N1 = oa::ops::new_node(oa::funcs::seqs(comm, {m, n, p}, 1, dt1));
      NodePtr N2 = oa::ops::new_node(oa::funcs::consts(comm, {m, n, p}, ${t2}$(2.0), 1));

      arma::Cube<${t1}$> C1 = make_seqs<${t1}$>(m, n, p);
      arma::Cube<${t2}$> C2(m,n,p);  C2.fill(${t2}$(2));
      arma::Cube<otype<${t1}$, ${t2}$>::value> C3;
      
      ///:for o in [['+','PLUS'], ['-', 'MINUS'], ['%','MULT'], ['/', 'DIVD']]
      {
      	NodePtr N3 = oa::ops::new_node(TYPE_${o[1]}$, N1, N2);
	ArrayPtr A3 = oa::funcs::to_rank0(oa::ops::eval(N3));

	C3 = C1 ${o[0]}$ C2;

	if(rank == 0){
	  EXPECT_TRUE(oa::funcs::is_equal(A3, C3));
	}
      }
      ///:endfor
		
      // // ((A+B)-(C*D))/E
      // NodePtr A = oa::ops::new_node(ap1);
      // NodePtr B = oa::ops::new_node(ap2);
      // NodePtr C = oa::ops::new_node(ap3);

      // NodePtr F = oa::ops::new_node(TYPE_PLUS, A, B);
      // NodePtr G = oa::ops::new_node(TYPE_PLUS, F, C);
      // ArrayPtr ans = oa::funcs::to_rank0(oa::ops::eval(G));
    }
    ///:endfor
    ///:endfor
  }

  INSTANTIATE_TEST_CASE_P(OpenArray, MPITest,
			  gt::Combine(gt::Values(MPI_COMM_WORLD),
				      MRange, NRange, PRange));

  
}
