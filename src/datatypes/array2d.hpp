// Trying out implementation from 'Disch':
// http://www.cplusplus.com/forum/articles/17108/
#pragma once 

template <typename T>
class Array2D
{
    public:
        // constructor
        Array2D() {}
        Array2D(unsigned wd,unsigned ht)
            : nWd(wd), nHt(ht), pAr(0)
        {
            if(wd > 0 && ht > 0)
                pAr = new T[wd * ht];
        }

        // destructor
        ~Array2D()
        {
            delete[] pAr;
        }

        // indexing (parenthesis operator)
        //  two of them (for const correctness)

        const T& operator () (unsigned x,unsigned y) const
        {  return pAr[ y*nWd + x ];   }

        T& operator () (unsigned x,unsigned y)
        {  return pAr[ y*nWd + x ];   }

        // get dims
        unsigned GetWd() const { return nWd; }
        unsigned GetHt() const { return nHt; }


        // private data members
    private:
        unsigned nWd;
        unsigned nHt;
        T*       pAr;

        // to prevent unwanted copying:
        Array2D(const Array2D<T>&);
        Array2D& operator = (const Array2D<T>&);
};
