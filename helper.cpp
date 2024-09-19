// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 07 de agosto de 2024

#include "helper.hpp"
#include <QApplication>
#include <QFileDialog>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sys/stat.h>

// un namespace anónimo mantiene funciones visibles solo dentro de este archivo
namespace {

bool is_int(const std::string &string) {
    if (string.length() == 0)
        return false;
    for (char c : string) {
        if (!std::isdigit(c))
            return false;
    }
    return true;
}
int parse_int(const std::string &string) {
    std::size_t next_position = 0;
    int n;
    try {
        n = std::stoi(string, &next_position, 10);
    } catch (...) {
        throw std::logic_error("invalid number \"" + string + "\"");
    }
    if (next_position != string.length())
        throw std::logic_error("invalid number \"" + string + "\"");
    return n;
}

} // namespace

std::vector<std::string> get_args_vector(int argc, char **argv) {
    std::vector<std::string> args;
    for (int i = 0; i < argc; ++i) {
        args.push_back(std::string(argv[i]));
    }
    return args;
}

std::string ui_select_video() {
    int argc = 0;
    char **argv = 0;
    QApplication app(argc, argv);
    QFileDialog::Options options;
    QString selectedFilter;
    QString filename = QFileDialog::getOpenFileName(
        nullptr, QString("Elegir Video"), ".",
        QString("Videos (*.mpg *.mp4 *.avi)"), &selectedFilter, options);
    return filename.toStdString();
}

cv::VideoCapture abrir_video(const std::string &filename) {
    cv::VideoCapture capture;
    if (is_int(filename)) {
        int id_webcam = parse_int(filename);
        std::cout << "abriendo camara " << id_webcam << std::endl;
        if (capture.open(id_webcam)) {
            // leer el primer frame
            cv::Mat frame;
            capture.read(frame);
            std::cout << "video " << frame.size << std::endl;
        }
    } else {
        std::cout << "abriendo archivo " << filename << std::endl;
        capture.open(filename);
    }
    if (!capture.isOpened()) {
        std::cout << "no puedo abrir video " << filename << std::endl;
        throw std::runtime_error("no puedo abrir video");
    }
    std::cout << filename << ": " << capture.get(cv::CAP_PROP_FRAME_WIDTH)
              << "x" << capture.get(cv::CAP_PROP_FRAME_HEIGHT)
              << " fps=" << capture.get(cv::CAP_PROP_FPS) << std::endl;
    return capture;
}

void agregar_texto(cv::Mat &imagen, int center_x, int center_y,
                   const std::string &texto) {
    static int fontFace = cv::FONT_HERSHEY_PLAIN;
    static double fontScale = 1;
    static int thickness = 1;
    int baseline = 0;
    cv::Size textSize =
        cv::getTextSize(texto, fontFace, fontScale, thickness, &baseline);
    cv::Point p(center_x - textSize.width / 2, center_y + textSize.height);
    cv::Rect rect(p.x - 1, p.y - textSize.height - 1, textSize.width + 2,
                  textSize.height + 2);
    cv::rectangle(imagen, rect, cv::Scalar::all(0), -1);
    cv::putText(imagen, texto, p, fontFace, fontScale, cv::Scalar::all(255),
                thickness);
}

void mostrar_imagen(const std::string &window_name, const cv::Mat &imagen,
                    bool valorAbsoluto, bool escalarMin0Max255) {
    cv::Mat imagen_abs;
    if (valorAbsoluto) {
        imagen_abs = cv::abs(imagen);
    } else {
        imagen_abs = imagen;
    }
    cv::Mat imagen_norm;
    if (escalarMin0Max255) {
        cv::normalize(imagen_abs, imagen_norm, 0, 255, cv::NORM_MINMAX, CV_8U);
    } else {
        imagen_norm = imagen_abs;
    }
    cv::imshow(window_name, imagen_norm);
}

// Añadidos para tarea 1

std::vector<std::string>
listar_archivos_en_carpeta(const std::string &imagenes_dir) {
    std::vector<std::string> lista;

    for (const auto &entry :
         std::filesystem::directory_iterator(imagenes_dir)) {
        if (entry.path().extension() == ".jpg") {
            lista.push_back(entry.path().filename().string());
        }
    }

    sort(lista.begin(), lista.end());
    return lista;
}

cv::Mat calcular_descriptores(
    std::function<cv::Mat(const std::string &, const std::string &)>
        funcion_descriptor,
    const std::vector<std::string> &nombres_imagenes,
    const std::string &imagenes_dir) {
    cv::Mat matriz_descriptores;
    int num_fila = 0;
    auto t0 = std::chrono::high_resolution_clock::now();

    for (const auto &nombre_imagen : nombres_imagenes) {
        cv::Mat descriptor_imagen =
            funcion_descriptor(nombre_imagen, imagenes_dir);
        // Crear la matriz de descriptores (numero imagenes x largo_descriptor)
        if (matriz_descriptores.empty()) {
            matriz_descriptores = cv::Mat::zeros(
                nombres_imagenes.size(), descriptor_imagen.cols, CV_32F);
        }
        // Copiar descriptor a una fila de la matriz de descriptores
        descriptor_imagen.copyTo(matriz_descriptores.row(num_fila));
        num_fila++;
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tiempo = t1 - t0;
    std::cout << "Tiempo calcular descriptores = " << tiempo.count()
              << " segundos" << std::endl;

    return matriz_descriptores;
}