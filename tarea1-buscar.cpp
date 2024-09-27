#include "helper.hpp"

int main(int argc, char const *argv[]) {
    cv::Mat descriptores;
    leer_objeto("test", "descriptores.bin", descriptores);
    std::cout << "Descriptores: " << std::endl << descriptores << std::endl;
    return 0;
}
