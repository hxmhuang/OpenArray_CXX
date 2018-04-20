/*
 * Partition.hpp
 * OpenArray uses partition information to identify each array
 *
=======================================================*/

#ifndef __PARTITION_HPP__
#define __PARTITION_HPP__

#include <list>
#include <mpi.h>
#include <vector>
#include <memory>
#include "Box.hpp"
#include "common.hpp"

using namespace std;

class Partition;
typedef std::shared_ptr<Partition> PartitionPtr;

class Partition {
  private:
  static Shape m_default_procs_shape;   // static default process shape
  static int m_default_stencil_width;   // static default stencil width
  static int m_default_stencil_type;    // static default stencil type
  
  public:
  int m_stencil_width = 1;              // default stencil width as 1   
  int m_stencil_type = STENCIL_BOX;     // default stencil type as stencil box
  MPI_Comm m_comm = MPI_COMM_SELF;      // MPI_Comm, default as MPI_COMM_SELF

  // array global shape [gx, gy, gz], default as [1, 1, 1]     
  Shape m_global_shape = { {1, 1, 1} }; 

  // array processes shape [px, py, pz], default as [1, 1, 1]
  Shape m_procs_shape = {{1, 1, 1}};

  //Array boundary type [bx, by, bz], default as [0, 0, 0]
  Shape m_bound_type = {{0, 0, 0}};

  size_t m_hash;    // each partition has hash value, used in Partition Pool

  // Partition information in each process of every dimension
  vector<int> m_lx = {1};
  vector<int> m_ly = {1};
  vector<int> m_lz = {1};

  // accumulate information in each dimension
  vector<int> m_clx = {0, 1};
  vector<int> m_cly = {0, 1};
  vector<int> m_clz = {0, 1};
  
  public:
  static const PartitionPtr Scalar;
  
  public:
  // Constructor
  Partition();

  // give global_shape, and total process number
  // find the approriate partition
  Partition(MPI_Comm comm, int size,
          const Shape &gs, int stencil_width = 1);
  
  // get partition based on user's given lx, ly, lz information
  Partition(MPI_Comm comm, const vector<int> &x,
          const vector<int> &y, 
    const vector<int> &z, int stencil_width = 1);

  // check if two Partition is equal or not
  bool equal(const PartitionPtr &par_ptr);
  bool equal(const Partition &par);

  // check if two Partition distribution is equal or not
  bool equal_distr(const PartitionPtr &par_ptr);

  // return global shape of Partition
  Shape shape();

  // return processes shape of partition
  Shape procs_shape() const;

  // return processes total size
  int procs_size() const;

  // return global size of Partition
  int size();

  // return mpi rank 
  int rank();

  // update accumulate distribution
  void update_acc_distr();

  // set stencil type & width
  void set_stencil(int type, int width);

  // get the box info of each process
  Box get_local_box();
  
  // get the box info based on process's coord [px, py, pz]
  Box get_local_box(const vector<int> &coord);

  // get the box info based on process's rank
  Box get_local_box(int rank);

  // coord = [x, y, z], rank = x + y * px + z * px * py
  int get_procs_rank(int x, int y, int z);

  // coord = [x, y, z], rank = x + y * px + z * px * py
  int get_procs_rank(const vector<int> &coord);

  // given rank, calculate coord[x, y, z]
  vector<int> get_procs_3d(int rank);

  // display Partition information, default display all information
  void display(const char *prefix = NULL, bool all = true); 

  // display distribution information
  void display_distr(const char *prefix = NULL);

  // get & set partition hash value, used in Partition Pool
  void set_hash(const size_t &hash);
  size_t get_hash() const;

  // get mpi_comm
  MPI_Comm get_comm() const;

  // get stencil width
  int get_stencil_width() const;

  // get & set stencil type
  void set_stencil_type(int st);
  int get_stencil_type() const;

  // set boundary type
  Shape get_bound_type() const;

  // splic box in each procs
  // rsx[procs_x * 3] box have procs_x processes in x dimension
  // rsy[procs_y * 3]
  // rsz[procs_z * 3]
  void split_box_procs(const Box& b,
        vector<int> &rsx,
        vector<int> &rsy,
        vector<int> &rsz) const;

  // get accumulate vector based on split_box_procs
  void get_acc_box_procs(
    vector<int> &rsx, 
    vector<int> &rsy, 
    vector<int> &rsz,
    vector<int> &acc_rsx, 
    vector<int> &acc_rsy, 
    vector<int> &acc_rsz) const;
  
  // get sub partition based on box b
  PartitionPtr sub(const Box& b) const;

  // static function, gen hash based on [comm, gs(x, y, z), stencil_width]
  static size_t gen_hash(MPI_Comm comm, const Shape& gs, int stencil_width = 1);

  // static function, gen hash based on [comm, x, y, z, stencil_width]
  static size_t gen_hash(MPI_Comm comm, const vector<int> &x, 
    const vector<int> &y, const vector<int> &z, int stencil_width = 1);

  // get & set default process
  static Shape get_default_procs_shape();
  static void set_auto_procs_shape();  

  // set default process shape as s
  static void set_default_procs_shape(const Shape& s);

  // get & set default stencil width
  static int  get_default_stencil_width();
  static void set_default_stencil_width(int);

  // get & set default stencil type
  static int  get_default_stencil_type();
  static void set_default_stencil_type(int);  
};


#endif
