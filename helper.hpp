// CC5213 - Recuperación de Información Multimedia
// Profesor: Juan Manuel Barrios
// Fecha: 07 de agosto de 2024

#pragma once

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
