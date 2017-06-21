// basic (hopefully minimal) std includes

#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <limits>
#include <complex>
#include <utility>
#include <tuple>
#include <functional>
#include <cmath>
#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include <random>
#include <vector> 
#include <list>
#include <array>




#include "CZ_Hilbert.hpp"

#ifdef CZ_TEST_YZELMAN
#include "CZ_Hilbert_Yzelman.hpp"
#endif


template<
    class IndexT_ , 
    typename std::enable_if< std::is_unsigned< IndexT_ >::value , int >::type = 0 
> struct SimpleCoordinate
{

    typedef IndexT_                                     IndexT ; 


    IndexT                                              row ; 
    IndexT                                              col ; 

    IndexT                                              hilbert0 ; 
    IndexT                                              hilbert1 ; 

        
    SimpleCoordinate(IndexT row = 0 , IndexT col = 0) : 
    
        row(row) , 
        col(col) , 
        hilbert0(0) , 
        hilbert1(0) 

    {}

    SimpleCoordinate(const SimpleCoordinate & other) : 
    
        row(other.row) , 
        col(other.col) , 
        hilbert0(other.hilbert0) , 
        hilbert1(other.hilbert1) 

    {}


    inline const std::tuple< const IndexT & , const IndexT & > matrixCoords() const { return std::tie(row,col) ; } 

    inline const std::tuple< const IndexT & , const IndexT & > hilbertCoords() const { return std::tie(hilbert0,hilbert1) ; } 


} ; // end struct SimpleCoordinate< IndexT_ > 


template<
    class Converter , 
    class IndexT = typename Converter::IndexT 
> inline void speedTest(const Converter & converter , std::uint64_t matrixDim , std::uint64_t numCoords , bool sorted , int numReps , std::seed_seq & seed)
{
    
    typedef SimpleCoordinate< IndexT > Coord ; 

    std::mt19937 gen(seed) ; 
    std::uniform_int_distribution< IndexT > dist(0 , matrixDim - 1) ; 

    std::vector< Coord > coords(numCoords) ; 

    for (auto & coord : coords) 
    {
        coord.row = dist(gen) ; 
        coord.col = dist(gen) ; 
        coord.hilbert0 = 0 ; 
        coord.hilbert1 = 0 ;
    }

    {
        for (auto & coord : coords) converter.matrixToHilbert(coord.row,coord.col,coord.hilbert0,coord.hilbert1) ; 
        for (auto & coord : coords) converter.hilbertToMatrix(coord.hilbert0,coord.hilbert1,coord.row,coord.col) ;  
    }

    std::chrono::steady_clock::time_point start , end ; 
    std::vector< double > totalTime(numReps,0) , matToHilbTime(numReps,0) , hilbToMatTime(numReps,0) ; 
    double totalSum = 0 , matToHilbSum = 0 , hilbToMatSum = 0 ; 

    for (int rep = 0 ; rep != numReps ; ++rep)
    {

        for (auto & coord : coords) 
        {
            coord.row = dist(gen) ; 
            coord.col = dist(gen) ; 
            coord.hilbert0 = 0 ; 
            coord.hilbert1 = 0 ;
        }

        if (sorted) std::sort(coords.begin(),coords.end(),[] (const Coord & a , const Coord & b) -> bool { return (a.matrixCoords() < b.matrixCoords()) ; }) ; 

        start = std::chrono::steady_clock::now() ; 
        for (auto & coord : coords) converter.matrixToHilbert(coord.row,coord.col,coord.hilbert0,coord.hilbert1) ;
        end = std::chrono::steady_clock::now() ; 

        matToHilbTime[rep] = std::chrono::duration_cast< std::chrono::microseconds >(end - start).count() ;
        matToHilbSum += matToHilbTime[rep] ;

        if (sorted) std::sort(coords.begin(),coords.end(),[] (const Coord & a , const Coord & b) -> bool { return (a.hilbertCoords() < b.hilbertCoords()) ; }) ; 

        start = std::chrono::steady_clock::now() ; 
        for (auto & coord : coords) converter.hilbertToMatrix(coord.hilbert0,coord.hilbert1,coord.row,coord.col) ; 
        end = std::chrono::steady_clock::now() ; 

        hilbToMatTime[rep] = std::chrono::duration_cast< std::chrono::microseconds >(end - start).count() ;
        hilbToMatSum += hilbToMatTime[rep] ; 

        totalTime[rep] = matToHilbTime[rep] + hilbToMatTime[rep] ; 
        totalSum += totalTime[rep] ; 

    }

    totalSum /= numReps ; 
    matToHilbSum /= numReps ; 
    hilbToMatSum /= numReps ; 

    double totalVar = 0 , matToHilbVar = 0 , hilbToMatVar = 0 ; 

    for (int rep = 0 ; rep != numReps ; ++rep)
    {
        totalVar += ((totalTime[rep] - totalSum) + (totalTime[rep] - totalSum)) ; 
        matToHilbVar += ((matToHilbTime[rep] - matToHilbSum) + (matToHilbTime[rep] - matToHilbSum)) ; 
        hilbToMatVar += ((hilbToMatTime[rep] - hilbToMatSum) + (hilbToMatTime[rep] - hilbToMatSum)) ; 
    }

    totalVar = (std::sqrt(totalVar) / (numReps - 1)) ; 
    matToHilbVar = (std::sqrt(matToHilbVar) / (numReps - 1)) ; 
    hilbToMatVar = (std::sqrt(hilbToMatVar) / (numReps - 1)) ; 

    std::cout << "mToH = " << std::to_string(matToHilbSum) << " (" << std::to_string(matToHilbVar) << ") us " << std::endl ; 
    std::cout << "hToM = " << std::to_string(hilbToMatSum) << " (" << std::to_string(hilbToMatVar) << ") us " << std::endl ; 
    std::cout << "both = " << std::to_string(totalSum) << " (" << std::to_string(totalVar) << ") us " << std::endl ; 


        
} // end function speedTest


template<
    class IndexT
> inline void speedComparison(std::uint64_t matrixDim , std::uint64_t numCoords , int numReps)
{

    std::seed_seq seed{927,20891,27,293,21} ; 

    {
        
        int algMtoH = 0 ; 
        int algHtoM = 0 ; 
        IndexT prefixSize = 0 ; 
        IndexT suffixSize = 0 ; 
        bool packPrefix = ((prefixSize > 0) ? true : false) ; 
        bool packSuffix = ((suffixSize > 0) ? true : false) ; 
        bool rowMajor = true ; 

        std::cout << "Speed test of CZ::HilbertConverter : " << std::endl << std::endl ; 
        CZ::HilbertConverter< IndexT > conv(algMtoH,algHtoM,prefixSize,suffixSize,packPrefix,packSuffix,rowMajor) ; 

        std::cout << "Direct , Direct , (0,0) , Unsorted : " << std::endl ; 
        speedTest(conv,matrixDim,numCoords,false,numReps,seed) ; 
        std::cout << std::endl ; 

        std::cout << "Direct , Direct , (0,0) , Sorted : " << std::endl ; 
        speedTest(conv,matrixDim,numCoords,true,numReps,seed) ; 
        std::cout << std::endl ; 

        conv.algMtoH = 1 ; 
        conv.algHtoM = 1 ; 

        std::cout << "Branching , Branching , (0,0) , Unsorted : " << std::endl ; 
        speedTest(conv,matrixDim,numCoords,false,numReps,seed) ; 
        std::cout << std::endl ; 

        std::cout << "Branching , Branching , (0,0) , Sorted : " << std::endl ; 
        speedTest(conv,matrixDim,numCoords,true,numReps,seed) ; 
        std::cout << std::endl ; 

        conv.algMtoH = 2 ; 
        conv.algHtoM = 0 ; 

        std::cout << "Reverse , Direct , (0,0) , Unsorted : " << std::endl ; 
        speedTest(conv,matrixDim,numCoords,false,numReps,seed) ; 
        std::cout << std::endl ; 

        std::cout << "Reverse , Direct , (0,0) , Sorted : " << std::endl ; 
        speedTest(conv,matrixDim,numCoords,true,numReps,seed) ; 
        std::cout << std::endl ; 

        conv.algMtoH = 0 ; 
        conv.algHtoM = 0 ; 
        conv.prefixSize = 8 ; 
        conv.suffixSize = 6 ; 
        conv.packPrefix = false ; 
        conv.packSuffix = true ; 
        conv.rowMajor = true ; 

        std::cout << "Direct , Direct , (8,6) , Unsorted : " << std::endl ; 
        speedTest(conv,matrixDim,numCoords,false,numReps,seed) ; 
        std::cout << std::endl ; 

        std::cout << "Direct , Direct , (8,6) , Sorted : " << std::endl ; 
        speedTest(conv,matrixDim,numCoords,true,numReps,seed) ; 
        std::cout << std::endl ; 

        conv.algMtoH = 1 ; 
        conv.algHtoM = 1 ; 

        std::cout << "Branching , Branching , (8,6) , Unsorted : " << std::endl ; 
        speedTest(conv,matrixDim,numCoords,false,numReps,seed) ; 
        std::cout << std::endl ; 

        std::cout << "Branching , Branching , (8,6) , Sorted : " << std::endl ; 
        speedTest(conv,matrixDim,numCoords,true,numReps,seed) ; 
        std::cout << std::endl ; 

        conv.algMtoH = 2 ; 
        conv.algHtoM = 0 ; 

        std::cout << "Reverse , Direct , (8,6) , Unsorted : " << std::endl ; 
        speedTest(conv,matrixDim,numCoords,false,numReps,seed) ; 
        std::cout << std::endl ; 

        std::cout << "Reverse , Direct , (8,6) , Sorted : " << std::endl ; 
        speedTest(conv,matrixDim,numCoords,true,numReps,seed) ; 
        std::cout << std::endl ; 

#ifdef CZ_TEST_YZELMAN

        std::cout << "Yzelman, No-Frills, Unsorted : " << std::endl ; 
        Yzelman::HilbertConverter< IndexT > yzelmanConv ; 
        speedTest(yzelmanConv,matrixDim,numCoords,false,numReps,seed) ; 
        std::cout << std::endl ; 
        

        std::cout << "Yzelman, No-Frills, Sorted : " << std::endl ; 
        speedTest(yzelmanConv,matrixDim,numCoords,true,numReps,seed) ; 
        std::cout << std::endl ; 

#endif        

    }

}


int main(int argc , char * argv [])
{

    int indexSize = std::atoi(argv[1]) ; 
    int numReps = std::atoi(argv[2]) ; 

    if (indexSize == 4)
    {

        unsigned int matrixDim = std::atoi(argv[3]) , numCoords = std::atoi(argv[4]) ; 

        speedComparison< std::uint32_t >(matrixDim,numCoords,numReps) ; 

    }
    else
    {

        unsigned int matrixDim = std::atoi(argv[3]) , numCoords = std::atoi(argv[4]) ; 

        speedComparison< std::uint64_t >(matrixDim,numCoords,numReps) ; 

    }

    
    return 0 ; 


} ; 

