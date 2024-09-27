// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 07 de agosto de 2024

#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <opencv2/videoio.hpp>
#include <string>
#include <vector>

std::vector<std::string> get_args_vector(int argc, char **argv);

std::string ui_select_video();

cv::VideoCapture abrir_video(const std::string &filename);

void agregar_texto(cv::Mat &imagen, int center_x, int center_y,
                   const std::string &texto);

void mostrar_imagen(const std::string &window_name, const cv::Mat &imagen,
                    bool valorAbsoluto, bool escalarMin0Max255);

// Añadidos para tarea 1

std::vector<std::string>
listar_archivos_en_carpeta(const std::string &imagenes_dir);

cv::Mat calcular_descriptores(
    std::function<cv::Mat(const std::string &, const std::string &)>
        funcion_descriptor,
    const std::vector<std::string> &nombres_imagenes,
    const std::string &imagenes_dir);

template <typename T>
void guardar_objeto(const T &objeto, const std::string &carpeta,
                    const std::string &nombre_archivo) {
    // Ensure the directory exists
    std::filesystem::create_directories(carpeta);

    // Construct the full file path
    std::string archivo = carpeta + "/" + nombre_archivo;

    // Open the file stream
    std::ofstream ofs(archivo, std::ios::binary);
    if (!ofs) {
        std::cerr << "Error opening file for writing: " << archivo << std::endl;
        return;
    }

    // Write the object to the binary file
    ofs.write(reinterpret_cast<const char *>(&objeto), sizeof(T));
}

template <typename T>
void leer_objeto(const std::string &carpeta, const std::string &nombre_archivo, T &objeto) {
    // Construct the full file path
    std::string archivo = carpeta + "/" + nombre_archivo;

    // Check if the file exists
    if (!std::filesystem::exists(archivo)) {
        std::cerr << "File does not exist: " << archivo << std::endl;
        exit(1);
    }

    // Open the file stream
    std::ifstream ifs(archivo, std::ios::binary);
    if (!ifs) {
        std::cerr << "Error opening file for reading: " << archivo << std::endl;
        exit(1);
    }

    // Read the object from the binary file
    ifs.read(reinterpret_cast<char *>(&objeto), sizeof(T));
}