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
    class Converter 
> inline void visualCorrectnessTest(Converter & converter , typename Converter::IndexT testSize = 16)
{

    typedef typename Converter::IndexT IndexT ; 
    typedef SimpleCoordinate< IndexT > Coordinate ; 


    std::cout << "Original matrix coordinates :" << std::endl << std::endl ; 

    std::vector< Coordinate > coords ; 
    coords.reserve(testSize*testSize) ; 

    for (IndexT i = 0 , k = 0 ; i < testSize ; ++i)
    {
        for (IndexT j = 0 ; j < testSize ; ++j , ++k)
        {
            coords.emplace_back(i,j) ; 
            std::cout << "{" << coords[k].row << "," << coords[k].col << "}" << "\t" ; 
        }
        std::cout << std::endl ; 
    }

    std::cout << std::endl ; 


    std::cout << "Converting matrix coordinates to Hilbert coordinates" << std::endl << std::endl ; 

    for (IndexT i = 0 ; i < (testSize * testSize) ; ++i) 
    { 
        converter.matrixToHilbert(coords[i].row,coords[i].col,coords[i].hilbert0,coords[i].hilbert1) ; 
    }


    std::cout << "Hilbert coordinates :" << std::endl << std::endl ; 

    for (IndexT i = 0 , k = 0 ; i < testSize ; ++i)
    {
        for (IndexT j = 0 ; j < testSize ; ++j , ++k)
        {
            std::cout << "{" << coords[k].hilbert0 << "," << coords[k].hilbert1 << "}" << "\t" ;
        }
        std::cout << std::endl ; 
    }
    std::cout << std::endl ; 


    std::cout << "Converting Hilbert coordinates back to matrix coordinates" << std::endl << std::endl ; 

    for (IndexT i = 0 ; i < (testSize * testSize) ; ++i) 
    { 
        converter.hilbertToMatrix(coords[i].hilbert0,coords[i].hilbert1,coords[i].row,coords[i].col) ; 
    }


    std::cout << "Re-computed matrix coordinates :" << std::endl << std::endl ; 

    for (IndexT i = 0 , k = 0 ; i < testSize ; ++i)
    {
        for (IndexT j = 0 ; j < testSize ; ++j , ++k)
        {
            std::cout << "{" << coords[k].row << "," << coords[k].col << "}" << "\t" ;
        }
        std::cout << std::endl ; 
    }
    std::cout << std::endl ; 


    std::cout << "Visual test complete!" << std::endl << std::endl ; 

}





int main(int argc , char * argv [])
{

    int whichConv = std::atoi(argv[1]) ; 
    int indexSize = std::atoi(argv[2]) ; 

    switch (indexSize)
    {

        case 1 : 
        {

            typedef std::uint8_t IndexT ; 

            IndexT testSize = std::atoi(argv[3]) ; 

            switch (whichConv)
            {

                case 0 : 
                {

                    typedef typename CZ::HilbertConverter< IndexT > Converter ; 

                    int algMtoH = std::atoi(argv[4]) ; 
                    int algHtoM = std::atoi(argv[5]) ; 
                    IndexT prefixSize = std::atoi(argv[6]) ; 
                    IndexT suffixSize = std::atoi(argv[7]) ; 
                    bool packPrefix = ((prefixSize > 0) ? true : false) ; 
                    bool packSuffix = ((suffixSize > 0) ? true : false) ; 
                    bool rowMajor = true ; 

                    Converter conv(algMtoH,algHtoM,prefixSize,suffixSize,packPrefix,packSuffix,rowMajor) ;             

                    std::cout << "Visual correctness test of CZ::HilbertConverter :" << std::endl ; 
                    if (algMtoH == 0) std::cout << "matrixToHilbert algorithm = Direct" << std::endl ; 
                    if (algMtoH == 1) std::cout << "matrixToHilbert algorithm = Branching" << std::endl ; 
                    if (algMtoH == 2) std::cout << "matrixToHilbert algorithm = Reverse" << std::endl ; 
                    if (algHtoM == 0) std::cout << "hilbertToMatrix algorithm = Direct" << std::endl ; 
                    if (algHtoM == 1) std::cout << "hilbertToMatrix algorithm = Branching" << std::endl ; 
                    std::cout << "prefixSize = " << std::to_string(prefixSize) << " , " ; 
                    std::cout << "suffixSize = " << std::to_string(suffixSize) << " , "  ; 
                    std::cout << "packPrefix = " << std::boolalpha << packPrefix << " , "  ; 
                    std::cout << "packSuffix = " << packSuffix << " , and "  ; 
                    std::cout << "rowMajor = " << rowMajor << std::endl ; 
                    
                    std::cout << "testSize = " << std::to_string(testSize) << std::endl << std::endl ; 

                    visualCorrectnessTest(conv,testSize) ; 

                    break ; 

                }
#ifdef CZ_TEST_YZELMAN

                case 2 : 
                {

                    typedef typename Yzelman::HilbertConverter< IndexT > Converter ; 

                    Converter conv ; 

                    std::cout << "Visual correctness test of Yzelman::HilbertConverter : " << std::endl ; 
                    std::cout << "testSize = " << std::to_string(testSize) << std::endl << std::endl ; 

                    visualCorrectnessTest(conv,testSize) ; 

                    break ; 

                }
#endif
            }

            break ; 

        }

        case 2 : 
        {

            typedef std::uint16_t IndexT ; 

            IndexT testSize = std::atoi(argv[3]) ; 

            switch (whichConv)
            {

                case 0 : 
                {

                    typedef typename CZ::HilbertConverter< IndexT > Converter ; 

                    int algMtoH = std::atoi(argv[4]) ; 
                    int algHtoM = std::atoi(argv[5]) ; 
                    IndexT prefixSize = std::atoi(argv[6]) ; 
                    IndexT suffixSize = std::atoi(argv[7]) ; 
                    bool packPrefix = ((prefixSize > 0) ? true : false) ; 
                    bool packSuffix = ((suffixSize > 0) ? true : false) ; 
                    bool rowMajor = true ; 

                    Converter conv(algMtoH,algHtoM,prefixSize,suffixSize,packPrefix,packSuffix,rowMajor) ; 

                    std::cout << "Visual correctness test of CZ::HilbertConverter :" << std::endl ; 
                    if (algMtoH == 0) std::cout << "matrixToHilbert algorithm = Direct" << std::endl ; 
                    if (algMtoH == 1) std::cout << "matrixToHilbert algorithm = Branching" << std::endl ; 
                    if (algMtoH == 2) std::cout << "matrixToHilbert algorithm = Reverse" << std::endl ; 
                    if (algHtoM == 0) std::cout << "hilbertToMatrix algorithm = Direct" << std::endl ; 
                    if (algHtoM == 1) std::cout << "hilbertToMatrix algorithm = Branching" << std::endl ; 
                    std::cout << "prefixSize = " << std::to_string(prefixSize) << " , " ; 
                    std::cout << "suffixSize = " << std::to_string(suffixSize) << " , "  ; 
                    std::cout << "packPrefix = " << std::boolalpha << packPrefix << " , "  ; 
                    std::cout << "packSuffix = " << packSuffix << " , and "  ; 
                    std::cout << "rowMajor = " << rowMajor << std::endl ; 
                    
                    std::cout << "testSize = " << std::to_string(testSize) << std::endl << std::endl ; 

                    visualCorrectnessTest(conv,testSize) ; 

                    break ; 

                }
#ifdef CZ_TEST_YZELMAN

                case 2 : 
                {

                    typedef typename Yzelman::HilbertConverter< IndexT > Converter ; 

                    Converter conv ; 

                    std::cout << "Visual correctness test of Yzelman::HilbertConverter : " << std::endl ; 
                    std::cout << "testSize = " << std::to_string(testSize) << std::endl << std::endl ; 

                    visualCorrectnessTest(conv,testSize) ; 

                    break ; 

                }
#endif
            }

            break ; 

        }

        case 4 : 
        {

            typedef std::uint32_t IndexT ; 

            IndexT testSize = std::atoi(argv[3]) ; 

            switch (whichConv)
            {

                case 0 : 
                {

                    typedef typename CZ::HilbertConverter< IndexT > Converter ; 

                    int algMtoH = std::atoi(argv[4]) ; 
                    int algHtoM = std::atoi(argv[5]) ; 
                    IndexT prefixSize = std::atoi(argv[6]) ; 
                    IndexT suffixSize = std::atoi(argv[7]) ; 
                    bool packPrefix = ((prefixSize > 0) ? true : false) ; 
                    bool packSuffix = ((suffixSize > 0) ? true : false) ; 
                    bool rowMajor = true ; 

                    Converter conv(algMtoH,algHtoM,prefixSize,suffixSize,packPrefix,packSuffix,rowMajor) ; 

                    std::cout << "Visual correctness test of CZ::HilbertConverter :" << std::endl ; 
                    if (algMtoH == 0) std::cout << "matrixToHilbert algorithm = Direct" << std::endl ; 
                    if (algMtoH == 1) std::cout << "matrixToHilbert algorithm = Branching" << std::endl ; 
                    if (algMtoH == 2) std::cout << "matrixToHilbert algorithm = Reverse" << std::endl ; 
                    if (algHtoM == 0) std::cout << "hilbertToMatrix algorithm = Direct" << std::endl ; 
                    if (algHtoM == 1) std::cout << "hilbertToMatrix algorithm = Branching" << std::endl ; 
                    std::cout << "prefixSize = " << std::to_string(prefixSize) << " , " ; 
                    std::cout << "suffixSize = " << std::to_string(suffixSize) << " , "  ; 
                    std::cout << "packPrefix = " << std::boolalpha << packPrefix << " , "  ; 
                    std::cout << "packSuffix = " << packSuffix << " , and "  ; 
                    std::cout << "rowMajor = " << rowMajor << std::endl ; 
                    
                    std::cout << "testSize = " << std::to_string(testSize) << std::endl << std::endl ; 

                    visualCorrectnessTest(conv,testSize) ; 

                    break ; 

                }
#ifdef CZ_TEST_YZELMAN

                case 2 : 
                {

                    typedef typename Yzelman::HilbertConverter< IndexT > Converter ; 

                    Converter conv ; 

                    std::cout << "Visual correctness test of Yzelman::HilbertConverter : " << std::endl ; 
                    std::cout << "testSize = " << std::to_string(testSize) << std::endl << std::endl ; 

                    visualCorrectnessTest(conv,testSize) ; 

                    break ; 

                }
#endif
            }

            break ; 

        }

        case 8 : 
        {

            typedef std::uint64_t IndexT ; 

            IndexT testSize = std::atoi(argv[3]) ; 

            switch (whichConv)
            {

                case 0 : 
                {

                    typedef typename CZ::HilbertConverter< IndexT > Converter ; 

                    int algMtoH = std::atoi(argv[4]) ; 
                    int algHtoM = std::atoi(argv[5]) ; 
                    IndexT prefixSize = std::atoi(argv[6]) ; 
                    IndexT suffixSize = std::atoi(argv[7]) ; 
                    bool packPrefix = ((prefixSize > 0) ? true : false) ; 
                    bool packSuffix = ((suffixSize > 0) ? true : false) ; 
                    bool rowMajor = true ; 

                    Converter conv(algMtoH,algHtoM,prefixSize,suffixSize,packPrefix,packSuffix,rowMajor) ; 

                    std::cout << "Visual correctness test of CZ::HilbertConverter :" << std::endl ; 
                    if (algMtoH == 0) std::cout << "matrixToHilbert algorithm = Direct" << std::endl ; 
                    if (algMtoH == 1) std::cout << "matrixToHilbert algorithm = Branching" << std::endl ; 
                    if (algMtoH == 2) std::cout << "matrixToHilbert algorithm = Reverse" << std::endl ; 
                    if (algHtoM == 0) std::cout << "hilbertToMatrix algorithm = Direct" << std::endl ; 
                    if (algHtoM == 1) std::cout << "hilbertToMatrix algorithm = Branching" << std::endl ; 
                    std::cout << "prefixSize = " << std::to_string(prefixSize) << " , " ; 
                    std::cout << "suffixSize = " << std::to_string(suffixSize) << " , "  ; 
                    std::cout << "packPrefix = " << std::boolalpha << packPrefix << " , "  ; 
                    std::cout << "packSuffix = " << packSuffix << " , and "  ; 
                    std::cout << "rowMajor = " << rowMajor << std::endl ; 
                    
                    std::cout << "testSize = " << std::to_string(testSize) << std::endl << std::endl ; 

                    visualCorrectnessTest(conv,testSize) ; 

                    break ; 

                }

#ifdef CZ_TEST_YZELMAN

                case 2 : 
                {

                    typedef typename Yzelman::HilbertConverter< IndexT > Converter ; 

                    Converter conv ; 

                    std::cout << "Visual correctness test of Yzelman::HilbertConverter : " << std::endl ; 
                    std::cout << "testSize = " << std::to_string(testSize) << std::endl << std::endl ; 

                    visualCorrectnessTest(conv,testSize) ; 

                    break ; 

                }
#endif
            }

            break ; 

        }

    }

    return 0 ; 

} ; 

