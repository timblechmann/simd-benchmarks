#include <boost/config.hpp>
#include <boost/align/assume_aligned.hpp>

#include <boost/simd/include/pack.hpp>
#include <boost/simd/include/functions/multiplies.hpp>
#include <boost/simd/include/functions/aligned_load.hpp>
#include <boost/simd/include/functions/aligned_store.hpp>
#include <boost/simd/memory/is_aligned.hpp>

BOOST_NOINLINE void multiply( float * out, const float * in0, const float * in1, size_t count)
{
    for( size_t i = 0 ; i != count ; ++i ) {
        out[ i ] = in0[ i ] * in1[ i ];
    }
}


// avoid multiplication, stalling the frontend
BOOST_NOINLINE void multiply_1( float * out, const float * in0, const float * in1, size_t count)
{
    for( size_t i = 0 ; i != count ; ++i ) {
        float factor = in1[ i ];

        out[ i ] = factor ? in0[ i ] * factor
                          : 0.f;
    }
}

// help the compiler
BOOST_NOINLINE void multiply_2( float * __restrict__ out, const float *  __restrict__ in0,
                                const float *  __restrict__ in1, size_t count)
{
    BOOST_ALIGN_ASSUME_ALIGNED( in0, 64 );
    BOOST_ALIGN_ASSUME_ALIGNED( in1, 64 );
    BOOST_ALIGN_ASSUME_ALIGNED( out, 64 );

    for( size_t i = 0 ; i != count ; ++i ) {
        out[ i ] = in0[ i ] * in1[ i ];
    }
}

// vectorize by hand
BOOST_NOINLINE void multiply_3( float * out, const float * in0, const float * in1, size_t count)
{
    assert( boost::simd::is_aligned( count, 8 ) );

    size_t loops = count / 8;

    for( size_t i = 0 ; i != loops ; ++i ) {
        typedef boost::simd::pack<float, 8> pack_t;

        pack_t result = boost::simd::aligned_load<pack_t>( in0 + i * 8 ) * boost::simd::aligned_load<pack_t>( in1 + i * 8 );

        boost::simd::aligned_store( result, out + i * 8 );
    }
}


// reduce read-after-write hazards
BOOST_NOINLINE void multiply_4( float * out, const float * in0, const float * in1, size_t count)
{
    assert( boost::simd::is_aligned( count, 32 ) );

    size_t loops = count / 32;

    for( size_t i = 0 ; i != loops ; ++i ) {
        typedef boost::simd::pack<float, 8> pack_t;

        pack_t result0 = boost::simd::aligned_load<pack_t>( in0 + i * 32 + 0 * 8 ) * boost::simd::aligned_load<pack_t>( in1 + i * 32 + 0 * 8 );
        pack_t result1 = boost::simd::aligned_load<pack_t>( in0 + i * 32 + 1 * 8 ) * boost::simd::aligned_load<pack_t>( in1 + i * 32 + 1 * 8 );
        pack_t result2 = boost::simd::aligned_load<pack_t>( in0 + i * 32 + 2 * 8 ) * boost::simd::aligned_load<pack_t>( in1 + i * 32 + 2 * 8 );
        pack_t result3 = boost::simd::aligned_load<pack_t>( in0 + i * 32 + 3 * 8 ) * boost::simd::aligned_load<pack_t>( in1 + i * 32 + 3 * 8 );

        boost::simd::aligned_store( result0, out + i * 32 + 0 * 8 );
        boost::simd::aligned_store( result1, out + i * 32 + 1 * 8 );
        boost::simd::aligned_store( result2, out + i * 32 + 2 * 8 );
        boost::simd::aligned_store( result3, out + i * 32 + 3 * 8 );
    }
}
