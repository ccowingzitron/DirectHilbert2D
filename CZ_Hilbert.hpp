#pragma once
#ifndef CZ_Hilbert_17_5_11_856am
#define CZ_Hilbert_17_5_11_856am


// basic (hopefully minimal) std includes

#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <type_traits>
#include <iostream> 
#include <string>


namespace CZ
{


template< 
    class IndexT_ , 
    std::size_t Size = sizeof(IndexT_) , 
    typename std::enable_if< std::is_unsigned< IndexT_ >::value , int >::type = 0 
> struct BitTwidles ; 


template<
    class IndexT_ 
> struct BitTwidles< IndexT_ , 8 , 0 >
{

    typedef IndexT_                                         IndexT ; 

    static constexpr IndexT Size =                          IndexT(sizeof(IndexT)) ;
    static constexpr IndexT HalfBits =                      IndexT(4 * Size) ;
    static constexpr IndexT FullBits =                      IndexT(8 * Size) ;

    static constexpr IndexT Zero =                          IndexT(0) ; 
    static constexpr IndexT One =                           IndexT(1) ; 
    static constexpr IndexT Two =                           IndexT(2) ; 
    static constexpr IndexT Four =                          IndexT(4) ; 
    static constexpr IndexT Eight =                         IndexT(8) ; 
    static constexpr IndexT Sixteen =                       IndexT(16) ; 
    static constexpr IndexT ThirtyTwo =                     IndexT(32) ; 

    static constexpr IndexT Mask_1of2 =                     IndexT(0x5555555555555555) ; 
    static constexpr IndexT Mask_2of4 =                     IndexT(0x3333333333333333) ;
    static constexpr IndexT Mask_4of8 =                     IndexT(0x0F0F0F0F0F0F0F0F) ;
    static constexpr IndexT Mask_8of16 =                    IndexT(0x00FF00FF00FF00FF) ;
    static constexpr IndexT Mask_16of32 =                   IndexT(0x0000FFFF0000FFFF) ; 
    static constexpr IndexT Mask_32of64 =                   IndexT(0x00000000FFFFFFFF) ; 


    inline IndexT reverse(IndexT x) const
    {

        // 6 * Log2(sizeof(IndexT) * 8) operations

        IndexT y ;

        y = x ; 
        y >>= One ; 
        y &= Mask_1of2 ; 
        x &= Mask_1of2 ; 
        x <<= One ; 
        x |= y ; 

        y = x ; 
        y >>= Two ; 
        y &= Mask_2of4 ; 
        x &= Mask_2of4 ; 
        x <<= Two ; 
        x |= y ; 

        y = x ; 
        y >>= Four ; 
        y &= Mask_4of8 ; 
        x &= Mask_4of8 ; 
        x <<= Four ; 
        x |= y ; 

        y = x ; 
        y >>= Eight ; 
        y &= Mask_8of16 ; 
        x &= Mask_8of16 ; 
        x <<= Eight ; 
        x |= y ; 

        y = x ; 
        y >>= Sixteen ; 
        y &= Mask_16of32 ; 
        x &= Mask_16of32 ; 
        x <<= Sixteen ; 
        x |= y ; 

        y = x ; 
        y >>= ThirtyTwo ; 
        y &= Mask_32of64 ; 
        x &= Mask_32of64 ; 
        x <<= ThirtyTwo ; 
        x |= y ; 

        return x ;

    }

    inline IndexT interleave(IndexT x) const 
    {

        // 6 * Log2(sizeof(IndexT) * 8) operations

        IndexT y = (x >> ThirtyTwo) & Mask_32of64 ; 
        x &= Mask_32of64 ; 


        x |= (x << Sixteen) ; 
        x &= Mask_16of32 ; 

        x |= (x << Eight) ; 
        x &= Mask_8of16 ; 

        x |= (x << Four) ; 
        x &= Mask_4of8 ; 

        x |= (x << Two) ; 
        x &= Mask_2of4 ; 

        x |= (x << One) ; 
        x &= Mask_1of2 ; 


        y |= (y << Sixteen) ; 
        y &= Mask_16of32 ; 

        y |= (y << Eight) ; 
        y &= Mask_8of16 ; 

        y |= (y << Four) ; 
        y &= Mask_4of8 ; 

        y |= (y << Two) ; 
        y &= Mask_2of4 ; 

        y |= (y << One) ; 
        y &= Mask_1of2 ; 


        y <<= One ; 
        y |= x ; 


        return y ; 
        
    }

    inline IndexT defoliate(IndexT x) const 
    {

        // 6 * Log2(sizeof(IndexT) * 8) operations

        IndexT y = (x >> One) & Mask_1of2 ; 
        x &= Mask_1of2 ; 


        x |= x >> One ; 
        x &= Mask_2of4 ; 

        x |= x >> Two ; 
        x &= Mask_4of8 ; 

        x |= x >> Four ; 
        x &= Mask_8of16 ; 

        x |= x >> Eight ; 
        x &= Mask_16of32 ; 

        x |= x >> Sixteen ; 
        x &= Mask_32of64 ; 


        y |= y >> One ; 
        y &= Mask_2of4 ; 

        y |= y >> Two ; 
        y &= Mask_4of8 ; 

        y |= y >> Four ; 
        y &= Mask_8of16 ; 

        y |= y >> Eight ; 
        y &= Mask_16of32 ; 

        y |= y >> Sixteen ; 
        y &= Mask_32of64 ; 


        y <<= ThirtyTwo ; 
        y |= x ; 
        

        return y ; 
        
    }

} ; 


template<
    class IndexT_ 
> struct BitTwidles< IndexT_ , 4 , 0 >
{

    typedef IndexT_                                         IndexT ; 

    static constexpr IndexT Size =                          IndexT(sizeof(IndexT)) ;
    static constexpr IndexT HalfBits =                      IndexT(4 * Size) ;
    static constexpr IndexT FullBits =                      IndexT(8 * Size) ;

    static constexpr IndexT Zero =                          IndexT(0) ; 
    static constexpr IndexT One =                           IndexT(1) ; 
    static constexpr IndexT Two =                           IndexT(2) ; 
    static constexpr IndexT Four =                          IndexT(4) ; 
    static constexpr IndexT Eight =                         IndexT(8) ; 
    static constexpr IndexT Sixteen =                       IndexT(16) ; 

    static constexpr IndexT Mask_1of2 =                     IndexT(0x55555555) ;
    static constexpr IndexT Mask_2of4 =                     IndexT(0x33333333) ;
    static constexpr IndexT Mask_4of8 =                     IndexT(0x0F0F0F0F) ;
    static constexpr IndexT Mask_8of16 =                    IndexT(0x00FF00FF) ;
    static constexpr IndexT Mask_16of32 =                   IndexT(0x0000FFFF) ;


    inline IndexT reverse(IndexT x) const
    {

        // 6 * Log2(sizeof(IndexT) * 8) operations

        IndexT y ;

        y = x ; 
        y >>= One ; 
        y &= Mask_1of2 ; 
        x &= Mask_1of2 ; 
        x <<= One ; 
        x |= y ; 

        y = x ; 
        y >>= Two ; 
        y &= Mask_2of4 ; 
        x &= Mask_2of4 ; 
        x <<= Two ; 
        x |= y ; 

        y = x ; 
        y >>= Four ; 
        y &= Mask_4of8 ; 
        x &= Mask_4of8 ; 
        x <<= Four ; 
        x |= y ; 

        y = x ; 
        y >>= Eight ; 
        y &= Mask_8of16 ; 
        x &= Mask_8of16 ; 
        x <<= Eight ; 
        x |= y ; 

        y = x ; 
        y >>= Sixteen ; 
        y &= Mask_16of32 ; 
        x &= Mask_16of32 ; 
        x <<= Sixteen ; 
        x |= y ; 

        return x ;

    }

    inline IndexT interleave(IndexT x) const 
    {

        // 6 * Log2(sizeof(IndexT) * 8) operations

        IndexT y = (x >> Sixteen) & Mask_16of32 ; 
        x &= Mask_16of32 ; 


        x |= (x << Eight) ; 
        x &= Mask_8of16 ; 

        x |= (x << Four) ; 
        x &= Mask_4of8 ; 

        x |= (x << Two) ; 
        x &= Mask_2of4 ; 

        x |= (x << One) ; 
        x &= Mask_1of2 ; 


        y |= (y << Eight) ; 
        y &= Mask_8of16 ; 

        y |= (y << Four) ; 
        y &= Mask_4of8 ; 

        y |= (y << Two) ; 
        y &= Mask_2of4 ; 

        y |= (y << One) ; 
        y &= Mask_1of2 ; 


        y <<= One ; 
        y |= x ; 


        return y ; 
        
    }

    inline IndexT defoliate(IndexT x) const 
    {

        // 6 * Log2(sizeof(IndexT) * 8) operations

        IndexT y = (x >> One) & Mask_1of2 ; 
        x &= Mask_1of2 ; 


        x |= x >> One ; 
        x &= Mask_2of4 ; 

        x |= x >> Two ; 
        x &= Mask_4of8 ; 

        x |= x >> Four ; 
        x &= Mask_8of16 ; 

        x |= x >> Eight ; 
        x &= Mask_16of32 ; 


        y |= y >> One ; 
        y &= Mask_2of4 ; 

        y |= y >> Two ; 
        y &= Mask_4of8 ; 

        y |= y >> Four ; 
        y &= Mask_8of16 ; 

        y |= y >> Eight ; 
        y &= Mask_16of32 ; 


        y <<= Sixteen ; 
        y |= x ; 
        

        return y ; 
        
    }

} ; 


template<
    class IndexT_ 
> struct BitTwidles< IndexT_ , 2 , 0 >
{

    typedef IndexT_                                         IndexT ; 

    static constexpr IndexT Size =                          IndexT(sizeof(IndexT)) ;
    static constexpr IndexT HalfBits =                      IndexT(4 * Size) ;
    static constexpr IndexT FullBits =                      IndexT(8 * Size) ;

    static constexpr IndexT Zero =                          IndexT(0) ; 
    static constexpr IndexT One =                           IndexT(1) ; 
    static constexpr IndexT Two =                           IndexT(2) ; 
    static constexpr IndexT Four =                          IndexT(4) ; 
    static constexpr IndexT Eight =                         IndexT(8) ; 

    static constexpr IndexT Mask_1of2 =                     IndexT(0x5555) ;
    static constexpr IndexT Mask_2of4 =                     IndexT(0x3333) ;
    static constexpr IndexT Mask_4of8 =                     IndexT(0x0F0F) ;
    static constexpr IndexT Mask_8of16 =                    IndexT(0x00FF) ;


    inline IndexT reverse(IndexT x) const
    {

        // 6 * Log2(sizeof(IndexT) * 8) operations

        IndexT y ;

        y = x ; 
        y >>= One ; 
        y &= Mask_1of2 ; 
        x &= Mask_1of2 ; 
        x <<= One ; 
        x |= y ; 

        y = x ; 
        y >>= Two ; 
        y &= Mask_2of4 ; 
        x &= Mask_2of4 ; 
        x <<= Two ; 
        x |= y ; 

        y = x ; 
        y >>= Four ; 
        y &= Mask_4of8 ; 
        x &= Mask_4of8 ; 
        x <<= Four ; 
        x |= y ; 

        y = x ; 
        y >>= Eight ; 
        y &= Mask_8of16 ; 
        x &= Mask_8of16 ; 
        x <<= Eight ; 
        x |= y ; 

        return x ;

    }

    inline IndexT interleave(IndexT x) const 
    {

        // 6 * Log2(sizeof(IndexT) * 8) operations

        IndexT y = (x >> Eight) & Mask_8of16 ; 
        x &= Mask_8of16 ; 


        x |= (x << Four) ; 
        x &= Mask_4of8 ; 

        x |= (x << Two) ; 
        x &= Mask_2of4 ; 

        x |= (x << One) ; 
        x &= Mask_1of2 ; 


        y |= (y << Four) ; 
        y &= Mask_4of8 ; 

        y |= (y << Two) ; 
        y &= Mask_2of4 ; 

        y |= (y << One) ; 
        y &= Mask_1of2 ; 


        y <<= One ; 
        y |= x ; 


        return y ; 
        
    }

    inline IndexT defoliate(IndexT x) const 
    {

        // 6 * Log2(sizeof(IndexT) * 8) operations

        IndexT y = (x >> One) & Mask_1of2 ; 
        x &= Mask_1of2 ; 


        x |= x >> One ; 
        x &= Mask_2of4 ; 

        x |= x >> Two ; 
        x &= Mask_4of8 ; 

        x |= x >> Four ; 
        x &= Mask_8of16 ; 


        y |= y >> One ; 
        y &= Mask_2of4 ; 

        y |= y >> Two ; 
        y &= Mask_4of8 ; 

        y |= y >> Four ; 
        y &= Mask_8of16 ; 


        y <<= Eight ; 
        y |= x ; 
        

        return y ; 
        
    }

} ; 


template<
    class IndexT_ 
> struct BitTwidles< IndexT_ , 1 , 0 >
{

    typedef IndexT_                                         IndexT ; 

    static constexpr IndexT Size =                          IndexT(sizeof(IndexT)) ;
    static constexpr IndexT HalfBits =                      IndexT(4 * Size) ;
    static constexpr IndexT FullBits =                      IndexT(8 * Size) ;

    static constexpr IndexT Zero =                          IndexT(0) ; 
    static constexpr IndexT One =                           IndexT(1) ; 
    static constexpr IndexT Two =                           IndexT(2) ; 
    static constexpr IndexT Four =                          IndexT(4) ; 

    static constexpr IndexT Mask_1of2 =                     IndexT(0x55) ;
    static constexpr IndexT Mask_2of4 =                     IndexT(0x33) ;
    static constexpr IndexT Mask_4of8 =                     IndexT(0x0F) ;


    inline IndexT reverse(IndexT x) const
    {

        // (6 * Log2(sizeof(IndexT) * 8)) operations

        IndexT y ;

        y = x ; 
        y >>= One ; 
        y &= Mask_1of2 ; 
        x &= Mask_1of2 ; 
        x <<= One ; 
        x |= y ; 

        y = x ; 
        y >>= Two ; 
        y &= Mask_2of4 ; 
        x &= Mask_2of4 ; 
        x <<= Two ; 
        x |= y ; 

        y = x ; 
        y >>= Four ; 
        y &= Mask_4of8 ; 
        x &= Mask_4of8 ; 
        x <<= Four ; 
        x |= y ; 

        return x ;

    }

    inline IndexT interleave(IndexT x) const 
    {

        // (6 * (Log2(sizeof(IndexT) * 8)) - 1) + 5 operations

        IndexT y = (x >> Four) & Mask_4of8 ; 
        x &= Mask_4of8 ; 


        x |= (x << Two) ; 
        x &= Mask_2of4 ; 

        x |= (x << One) ; 
        x &= Mask_1of2 ; 


        y |= (y << Two) ; 
        y &= Mask_2of4 ; 

        y |= (y << One) ; 
        y &= Mask_1of2 ; 


        y <<= One ; 
        y |= x ; 


        return y ; 
        
    }

    inline IndexT defoliate(IndexT x) const 
    {

        // (6 * (Log2(sizeof(IndexT) * 8) - 1)) + 6 operations

        IndexT y = (x >> One) & Mask_1of2 ; 
        x &= Mask_1of2 ; 


        x |= x >> One ; 
        x &= Mask_2of4 ; 

        x |= x >> Two ; 
        x &= Mask_4of8 ; 


        y |= y >> One ; 
        y &= Mask_2of4 ; 

        y |= y >> Two ; 
        y &= Mask_4of8 ; 


        y <<= Four ; 
        y |= x ; 
        

        return y ; 
        
    }

} ; 



template<
    class IndexT_
> struct HilbertConverter : public BitTwidles< IndexT_ >
{


    typedef IndexT_                                         IndexT ; 
    typedef BitTwidles< IndexT >                            BaseT ; 
    typedef HilbertConverter< IndexT >                      SelfT ; 


    using BaseT::Size ; 
    using BaseT::HalfBits ; 
    using BaseT::FullBits ; 

    using BaseT::Zero ; 
    using BaseT::One ; 
    using BaseT::Two ; 

    using BaseT::reverse ; 
    using BaseT::interleave ; 
    using BaseT::defoliate ; 


    enum Algorithm : int
    {
        Direct = 0 , 
        Branching = 1 , 
        Reverse = 2
    } ; 


    int                                                     algMtoH ; 
    int                                                     algHtoM ; 
    IndexT                                                  prefixSize ; 
    IndexT                                                  suffixSize ; 
    bool                                                    packPrefix ; 
    bool                                                    packSuffix ; 
    bool                                                    rowMajor ; 


    HilbertConverter(int algMtoH = 0 , int algHtoM = 0 , 
                     IndexT prefixSize = 0 , IndexT suffixSize = 0 , 
                     bool packPrefix = false , bool packSuffix = false , 
					 bool rowMajor = true) : 

        algMtoH(algMtoH) , 
        algHtoM(algHtoM) , 
        prefixSize(prefixSize) , 
        suffixSize(suffixSize) , 
        packPrefix(packPrefix) , 
        packSuffix(packSuffix) , 
        rowMajor(rowMajor) 

    {}


    inline void matrixToHilbert(IndexT row , IndexT col , IndexT & hilbert0 , IndexT & hilbert1) const
    {

        switch (algMtoH)
        {

            case Direct : 
            {

                matrixToHilbert_direct(row,col,hilbert0,hilbert1) ; 
                return ; 

            }

            case Branching : 
            {

                matrixToHilbert_branching(row,col,hilbert0,hilbert1) ; 
                return ; 

            }

            case Reverse : 
            {

                matrixToHilbert_reverse(row,col,hilbert0,hilbert1) ; 
                return ; 

            }

        }

    }


    inline void hilbertToMatrix(IndexT hilbert0 , IndexT hilbert1 , IndexT & row , IndexT & col) const
    {

        switch (algHtoM)
        {

            case Direct : 
            {

                hilbertToMatrix_direct(hilbert0,hilbert1,row,col) ; 
                return ; 

            }

            case Branching : 
            {

                hilbertToMatrix_branching(hilbert0,hilbert1,row,col) ; 
                return ; 

            }

        }

    }


    inline void packEnds(const IndexT & row , const IndexT & col , IndexT & first , IndexT & second) const
    {

        if ((prefixSize + suffixSize) == 0) return ; 
        
        IndexT mask = 0 ; 

        if ((prefixSize > 0) && packPrefix)
        {
            mask = -(IndexT(One) << (FullBits - prefixSize)) ;

            if (rowMajor)
            {
                first |= row & mask ; 
                first |= (col & mask) >> prefixSize ; 
            }
            else
            {
                first |= col & mask ; 
                first |= (row & mask) >> prefixSize ; 
            }

        }

        if ((suffixSize > 0) && packSuffix)
        {
            mask = (IndexT(One) << suffixSize) - IndexT(One) ;

            if (rowMajor)
            {
                second |= (row & mask) << suffixSize ; 
                second |= col & mask ; 
            }
            else
            {
                second |= (col & mask) << suffixSize ; 
                second |= row & mask ; 
            }

        }

    }

    inline void unpackEnds(const IndexT & first , const IndexT & second , IndexT & row , IndexT & col) const
    {

        if ((prefixSize + suffixSize) == 0) return ; 
        
        IndexT mask = 0 ; 

        if ((prefixSize > 0) && packPrefix)
        {
            mask = -(IndexT(One) << (FullBits - prefixSize)) ;

            if (rowMajor)
            {
                row |= first & mask ; 
                col |= (second & mask) << prefixSize ; 
            }
            else
            {
                col |= first & mask ; 
                row |= (first & mask) << prefixSize ; 
            }

        }

        if ((suffixSize > 0) && packSuffix)
        {
            mask = (IndexT(One) << suffixSize) - IndexT(One) ;

            if (rowMajor)
            {
                row |= (second & mask) >> suffixSize ; 
                col |= second & mask ; 
            }
            else
            {
                col |= (second & mask) >> suffixSize ; 
                row |= second & mask ; 
            }

        }

    }


    inline void splitInterleave(IndexT & higher , IndexT & lower) const 
    {

        IndexT split = (higher >> HalfBits) | (lower << HalfBits) ; // split == {(original second half of lower) , (original first half of higher)}

        higher <<= HalfBits ; // higher == {(original second half of higher) , 0}
        lower >>= HalfBits ; // lower == {0 , (original first half of lower)}

        std::swap(higher,lower) ; // higher == {0 , (original first half of lower)} and lower == {(original second half of higher) , 0} 

        higher |= split << HalfBits ; // higher == {(original first half of higher) , (original first half of lower)}
        lower |= split >> HalfBits ; // lower == {(original second half of higher) , (original second half of lower)}

        higher = interleave(higher) ; // first == {(prefix segment of higher) , (interleaved first halves of higher and lower)}
        lower = interleave(lower) ; // second == {(interleaved second halves of higher and lower) , (suffix segment of lower)}

    }


    inline void splitDefoliate(IndexT & higher , IndexT & lower) const 
    {

        higher = defoliate(higher) ; // higher == {(original first half of higher) , (original first half of lower)}
        lower = defoliate(lower) ; // lower == {(original second half of higher) , (original second half of lower)}

        IndexT split = (higher >> HalfBits) | (lower << HalfBits) ; // split == {(original second half of lower) , (original first half of higher)}

        higher <<= HalfBits ; // higher == {(original first half of lower) , 0}
        higher |= split >> HalfBits ; // higher == {(original first half of lower) , (original second half of lower)}

        lower >>= HalfBits ; // lower == {0 , (original second half of higher)}
        lower |= split << HalfBits ; // lower == {(original first half of higher) , (original second half of higher)}

        std::swap(higher,lower) ; // higher == (original higher) and lower == (original lower)

    }


    inline void matrixToHilbert_direct(IndexT row , IndexT col , IndexT & hilbert0 , IndexT & hilbert1) const
    {

        IndexT higher = row , lower = col , prefix = 0 , suffix = 0 ; 

        packEnds(higher,lower,prefix,suffix) ; 

        IndexT mask = ~IndexT(Zero) >> (prefixSize + suffixSize) ;

        higher >>= suffixSize ;
        higher &= mask ;
        lower >>= suffixSize ;
        lower &= mask ;

        std::swap(higher,lower) ;
        lower ^= higher ; 
        higher ^= mask ; 

        mask = (IndexT(One) << (FullBits - One)) >> (prefixSize + suffixSize) ;

        IndexT side = 0 , pivot = 0 ;

        while (mask)
        {

            // (ops / bit) == 11 + while-test

            higher ^= side & lower ; 
            side ^= higher ; 
            side &= mask ; 
            side >>= One ; 

            pivot ^= higher & lower ; 
            pivot &= mask ; 
            pivot >>= One ; 
            higher ^= pivot ; 

            mask >>= One ; 

        }

        mask = ~IndexT(Zero) >> (prefixSize + suffixSize) ;
        higher ^= mask ; 
        higher ^= lower ; 
        higher <<= suffixSize ; 
        lower <<= suffixSize ; 

        splitInterleave(higher,lower) ; 

        hilbert0 = prefix | higher ; 
        hilbert1 = suffix | lower ; 

    }



    inline void matrixToHilbert_branching(IndexT row , IndexT col , IndexT & hilbert0 , IndexT & hilbert1) const
    {

        IndexT higher = row , lower = col , prefix = 0 , suffix = 0 ; 

        packEnds(higher,lower,prefix,suffix) ; 

        IndexT mask = (IndexT(One) << (FullBits - One)) >> (prefixSize + suffixSize) , flip = mask - One ;

        higher <<= prefixSize ; 
        higher >>= prefixSize + suffixSize ;
        lower <<= prefixSize ; 
        lower >>= prefixSize + suffixSize ;

        std::swap(lower,higher) ; 
        lower ^= higher ; 

        while (mask != One)
        {

            if ((higher & mask) == 0)
            {

                higher ^= (lower & flip) ;

                if (lower & mask)
                {

                    higher ^= flip ;

                }

            }

            flip >>= One ; 
            mask >>= One ; 
         
        }

        higher ^= lower ; 
        lower <<= suffixSize ; 
        higher <<= suffixSize ; 

        splitInterleave(higher,lower) ; 

        hilbert0 = prefix | higher ; 
        hilbert1 = suffix | lower ; 

    }


    inline void matrixToHilbert_reverse(IndexT row , IndexT col , IndexT & hilbert0 , IndexT & hilbert1) const
    {

        IndexT higher = row , lower = col , prefix = 0 , suffix = 0 ; 

        packEnds(higher,lower,prefix,suffix) ; 

        IndexT mask = ~IndexT(Zero) >> (prefixSize + suffixSize) ;

        higher >>= suffixSize ;
        higher &= mask ;
        lower >>= suffixSize ;
        lower &= mask ;

        higher ^= lower ; 

        IndexT outer = reverse(higher) , inner = reverse(lower ^ mask) , flip = 0 ;

        lower = higher ; 
        higher ^= mask ; 

        mask = IndexT(One) << (prefixSize + suffixSize) ;

        while (mask)
        {

            // (ops / bit) == 9 + while-test

            flip = mask & inner ;           // (flip[k] == inner_k ^ pivot_k ^ 1) => ((flip[k] == 1) <=> ((hilbert_k == {0,0}) || (hilbert_k == {1,1})))
            inner ^= -flip & outer ;        // ((inner_k == pivot_k) <=> (-flip == -mask) <=> (side is updated for bits k+1:)) ; N.B. an unnecessary update of hilbert1[k] may occur here
            flip &= outer ;                 // (flip[k] == (inner_k ^ pivot_k ^ 1) & (inner_k ^ outer_k)) => ((flip[k] == 1) <=> (hilbert_k == {1,1}))
            inner ^= -flip ;                // ((hilbert_k == {1,1}) <=> (hilbert1[k:] ^= 1) <=> (pivot is updated for bits k+1:)) ; N.B. this un-does the unnecessary update of hilbert1[k]
            mask <<= One ;                  // move mask into place for next iteration
         
        }

        higher ^= reverse(inner) ;
        higher <<= suffixSize ; 
        lower <<= suffixSize ; 
    
        splitInterleave(higher,lower) ; 

        hilbert0 = prefix | higher ; 
        hilbert1 = suffix | lower ; 

    }


    inline void hilbertToMatrix_direct(IndexT hilbert0 , IndexT hilbert1 , IndexT & row , IndexT & col) const
    {

        IndexT h0 = hilbert0 , h1 = hilbert1 , m0 = 0 , m1 = 0 , mask = ~IndexT(Zero) >> (prefixSize + suffixSize + One) ; 

        unpackEnds(h0,h1,m0,m1) ;
        splitDefoliate(h0,h1) ; 

        h0 <<= prefixSize ; 
        h0 >>= (prefixSize + suffixSize) ; 
        h0 <<= suffixSize ; 
        
        h1 <<= prefixSize ; 
        h1 >>= (prefixSize + suffixSize) ; 
        h1 <<= suffixSize ; 
        
        IndexT side = h0 , pivot = h1 ; 

        side >>= suffixSize + One ; 
        side ^= mask ; 
        pivot >>= suffixSize + One ; 
        side ^= pivot ; 
        pivot &= side ; 

        mask = (IndexT(One) << (FullBits - One)) >> (prefixSize + suffixSize + One) ;

        IndexT alpha = Zero , beta = Zero ;

        while (mask)
        {

            // (ops/bit) == 9 + while-test

            // preconditions : alpha == side_k-1 , beta == pivot_k-1 , side[k] == inner_k-1 ^ pivot_k-1 ^ 1 , pivot[k] == (inner_k-1 ^ pivot_k-1 ^ 1) & (outer_k-1 ^ inner_k-1)

            side ^= alpha ;                 // side[k] == side_k-1 ^ (inner_k-1 ^ pivot_k-1 ^ 1) == side_k
            alpha = side ;                  // alpha[k] == side_k
            alpha &= mask ;                 // trim alpha
            alpha >>= One ;                 // move alpha into place for next iteration

            pivot ^= beta ;                 // pivot[k] == pivot_k-1 ^ ((inner_k-1 ^ pivot_k-1 ^ 1) & (outer_k-1 ^ inner_k-1)) == pivot_k
            beta = pivot ;                  // beta[k] == pivot_k
            beta &= mask ;                  // trim beta
            beta >>= One;                   // move beta into place for next iteration

            mask >>= One ;                  // move mask into place for next iteration

        }

        side <<= suffixSize ; 
        pivot <<= suffixSize ; 

        side &= h1 ;                        // side[...] == side_... & (outer_... ^ inner_...)
        h0 ^= side ;                        // h0[...] == (outer_... ^ pivot_...) ^ (side_... & (outer_... ^ inner_...)) == row_... ^ pivot_...
        h0 ^= pivot ;                       // h0[...] == (row_... ^ pivot_...) ^ (pivot_...) == row_...
        h1 ^= h0 ;                          // h1[...] == (row_... ^ column_...) ^ row_... == column_...

        row = m0 | h0 ;
        col = m1 | h1 ;

    }


    inline void hilbertToMatrix_branching(IndexT hilbert0 , IndexT hilbert1 , IndexT & row , IndexT & col) const
    {

        IndexT h0 = hilbert0 , h1 = hilbert1 , m0 = 0 , m1 = 0 ; 
        IndexT mask = (IndexT(One) << (FullBits - One)) >> (prefixSize + suffixSize) , flip = (~IndexT(Zero) >> (prefixSize + suffixSize)) ; 

        unpackEnds(h0,h1,m0,m1) ;
        splitDefoliate(h0,h1) ; 

        h0 <<= prefixSize ; 
        h0 >>= (prefixSize + suffixSize) ; 
        
        h1 <<= prefixSize ; 
        h1 >>= (prefixSize + suffixSize) ; 
                
        h0 ^= h1 ; // h0 == inner_... ^ pivot_...
        h0 ^= flip ; // h0 == inner_... ^ pivot_... ^ 1...
        flip >>= One ; 
        
        while (flip)
        {

            if (h0 & mask)
            {
                h0 ^= (h1 & flip) ;
                if (h1 & mask)
                {
                    h0 ^= flip ;
                }
            }

            flip >>= One ; 
            mask >>= One ; 
         
        }

        flip = (~IndexT(Zero) >> (prefixSize + suffixSize)) ; 
        h0 ^= flip ; 
        h1 ^= h0 ; 
        std::swap(h0,h1) ; 
        h0 <<= suffixSize ; 
        h1 <<= suffixSize ; 

        row = m0 | h0 ;
        col = m1 | h1 ;

    }


} ; // end struct HilbertConverter



} // end namespace CZ


#endif // end include guard ifndef CZ_Hilbert_17_5_11_856am

