#include <algorithm>
#include <vector>

#include <boost/config.hpp>
#include <boost/align/aligned_allocator.hpp>


void multiply  ( float * out, const float * in0, const float * in1, size_t count );
void multiply_1( float * out, const float * in0, const float * in1, size_t count );
void multiply_2( float * out, const float * in0, const float * in1, size_t count );
void multiply_3( float * out, const float * in0, const float * in1, size_t count );
void multiply_4( float * out, const float * in0, const float * in1, size_t count );


int main(int argc, char *argv[])
{
    std::vector<float, boost::alignment::aligned_allocator<float, 64>> in0(1024), in1(1024), out(1024);

    std::generate_n( in0.begin(), 1024, std::rand );
    std::generate_n( in1.begin(), 1024, std::rand );
    std::generate_n( out.begin(), 1024, std::rand );

    for( int i = 0; i != 16384 * 128; ++i) {
        multiply  ( out.data(), in0.data(), in1.data(), 1024 );
        multiply_1( out.data(), in0.data(), in1.data(), 1024 );
        multiply_2( out.data(), in0.data(), in1.data(), 1024 );
        multiply_3( out.data(), in0.data(), in1.data(), 1024 );
        multiply_4( out.data(), in0.data(), in1.data(), 1024 );
    }

    return 0;
}
