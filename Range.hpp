#ifndef __RANGE_HPP__
#define __RANGE_HPP__

#include <iostream>


/*
 * Range [lower, upper)
 * */

class Range {
    private:
        int m_lower;
        int m_upper;

    public:
        Range();
        Range(int st, int ed);
        int get_lower();
        int get_upper();
        bool equal(int st, int ed);
        bool equal(const Range &rg);
        int size() const;
        void display(char const *prefix = "");

        // check if Range is inside the Range u
        bool is_inside(const Range &u);

        // [lower, upper) + num = [lower + num, upper + num)
        void shift(int num);

        // check if Range has intersection with Range u
        bool intersection(const Range &u); 

        // get intersection Range with Range u
        Range get_intersection(const Range &u);
};

#endif
