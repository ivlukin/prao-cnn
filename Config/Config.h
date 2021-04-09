

#ifndef PRAO_FOURIER_CONFIG_H
#define PRAO_FOURIER_CONFIG_H


#include <string>
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/filereadstream.h"
#include <iostream>

class Config {
public:
    explicit Config(char* fileName);
private:
    /**
     * Дата начала наблюдений (СОЛНЕЧНОЕ ВРЕМЯ, не ЗВЕЗДНОЕ)
     * Из этой даты начинаются высчитываться имена файлов, из которых нужно считывать данные
     * @see Utils::getFileNameFromDate
     * Формат: YYYY-MM-DD HH:MM:SS (например 2014-10-01 00:00:00)
     * ВАЖНО: часовой пояс MSK (строго говоря, в том часовом поясе, в котором названы файлы)
     * (это время затем автоматически конвертируется в звездное и расчеты идут в нем)
     */
    std::string startDate;
    /**
     * Длина наблюдений (в звездных сутках)
     * целесообразно ставить >=1 :)
     */
    int observationLength;

    /**
     * Северная или южная стойка
     * (N1 или S1 и тд)
     * Нужно опять же для корректного формирования списка файлов
     * TimeCoordinateHandler::getFileNameFromDate
     */
    std::string range;

    /**
     * Шаг (в звездных секундах) с которым нужно снимать отрезок данных для преобразования Фурье
     * Целесообразно согласно теореме Котельникова задавать значения
     * <= durationStarSeconds /2
     * @see durationStarSeconds
     */
    int step;

    /**
     * Директория, в которую нужно складывать выходные результаты
     */
    std::string outputPath;

    /**
     * В каком режиме велись наблюдения.
     * Здесь указывается расширение файла - .pnt или .pnthr
     */
    std::string mode;
    /**
     * Путь к файлу, где указываются фактические местоположения файлов с данными
     */
    std::string fileListPath;
    /**
     * Путь к файлу, где указываются калибровочные данные
     */
    std::string calibrationListPath;

    /**
     * Длина отрезка данных для преобразования Фурье (в звездных секундах)
     */
    double durationStarSeconds;

    /**
     * Режим суммирования включен/отключен
     */
     bool summationEnabled;

     /**
      * Суммировать фурье-спектры отдельных частот или считать среднее
      */
      bool frequencyAverage;
public:
    const std::string &getStartDate() const;

    const std::string &getRange() const;

    int getStep() const;

    const std::string &getOutputPath() const;

    const std::string &getMode() const;

    const std::string &getFileListPath() const;

    const std::string &getCalibrationListPath() const;

    double getDurationStarSeconds() const;

    int getObservationLength() const;

    bool isSummationEnabled() const;

    bool isFrequencyAverage() const;
};


#endif //PRAO_FOURIER_CONFIG_H
