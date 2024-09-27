#include "helper.hpp"
#include <filesystem>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;
using namespace std::filesystem;

Mat vector_de_intensidades(string nombre_imagen, string imagenes_dir) {
    string path = imagenes_dir + "/" + nombre_imagen;
    Mat imagen = imread(path, IMREAD_GRAYSCALE);
    if (imagen.empty()) {
        cerr << "No se pudo cargar la imagen " << path << endl;
        exit(1);
    }
    Mat imagen_reducida;
    resize(imagen, imagen_reducida, Size(5, 5), 0, 0, INTER_AREA);
    Mat vector_intensidades = imagen_reducida.reshape(1, 1);
    // mostrar_imagen("Imagen", imagen, false, false);
    // Mat imagen_reducida_grande;
    // resize(imagen_reducida, imagen_reducida_grande, Size(imagen.cols,
    // imagen.rows), 0, 0, INTER_NEAREST); mostrar_imagen("Imagen reducida",
    // imagen_reducida_grande, false, false); cout << "Vector de intensidades: "
    // << vector_intensidades << endl; waitKey(0);
    return vector_intensidades;
}

void tarea1_indexar(const char *dir_input_imagenes_R,
                    const char *dir_output_descriptores_R) {
    if (!exists(dir_input_imagenes_R)) {
        cerr << dir_input_imagenes_R << ": directory does not exist" << endl;
        return;
    } else if (!is_directory(dir_input_imagenes_R)) {
        cerr << dir_input_imagenes_R << ": is not a directory" << endl;
        return;
    }

    if (exists(dir_output_descriptores_R)) {
        cerr << dir_output_descriptores_R << " already exists" << endl;
        return;
    }

    vector<std::string> imagenes =
        listar_archivos_en_carpeta(dir_input_imagenes_R);
    Mat descriptores = calcular_descriptores(vector_de_intensidades, imagenes,
                                             dir_input_imagenes_R);
    cout << "Descriptores: " << endl << descriptores << endl;
    cout << "Filas: " << descriptores.rows << " Columnas: " << descriptores.cols
         << endl;
    guardar_objeto(descriptores, dir_output_descriptores_R, "descriptores.bin");
}

int main(int argc, char **argv) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0]
             << " <dir_input_imagenes_R> <dir_output_descriptores_R>" << endl;
        return 1;
    }

    char *dir_input_imagenes_R = argv[1];
    char *dir_output_descriptores_R = argv[2];

    tarea1_indexar(dir_input_imagenes_R, dir_output_descriptores_R);
    return 0;
}