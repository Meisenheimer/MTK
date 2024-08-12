#include <type_traits>

int main()
{
    typename std::common_type_t<float, long > a;
}