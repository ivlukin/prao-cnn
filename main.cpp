#include <iostream>
#include <vector>
#include <sys/stat.h>
#include "Time/TimeCoordinateHandler.h"
#include "FileHandler//FileHandler.h"
#include "FourierHandler/FourierHandler.h"
#include "Summarizing/SummationHandler.h"
#include "Writing/SummationWriteHandler.h"
#include "Writing/SimpleWriteHandler.h"

CalibrationDataStorage *readCalibrationDataStorage(const string &basicString);

std::string tm_tostring(tm *datetime);

int main(int argc, char **argv) {
    std::vector<std::string> args;
    if (argc != 3) {
        std::cout << "provide correct number of args!" << std::endl;
        exit(-1);
    }
    for (int i = 0; i < argc; ++i) {
        std::string arg(argv[i]);
        args.push_back(arg);
    }
    if (args[1] != "-config") {
        std::cout << "provide --c or -config argument" << std::endl;
        std::cout << args[1] << std::endl;
        exit(-1);
    }

    Config config = Config(args[2].data());
#ifdef _WIN32
    CreateDirectory(outputDirectoryForExactFile, nullptr);
#else
    mkdir(config.getOutputPath().c_str(), 0777);
#endif
    Utils::createDirectory(config.getOutputPath());
    OpenCLContext context = OpenCLContext(config.getDeviceNumber());
    context.initContext();
    std::cout << "reading calibration storage..." << std::endl;
    CalibrationDataStorage *storage = readCalibrationDataStorage(config.getCalibrationListPath());
    clock_t tStart = clock();
    TimeCoordinateHandler handler = TimeCoordinateHandler(config);
    std::cout << "generating time coordinates..." << std::endl;
    handler.generateTimeCoordinates();
    for (const TimeCoordinate &coordinate: handler.getTimeCoordinateSet()) {
        time_t sunTime = to_SunTime(coordinate.getBeginDateTime());
        std::cout << "processing msk_SUN_time: " << tm_tostring(localtime(&sunTime)) << std::endl;
        const std::vector<double> &coordinatesWithSameStarTime = coordinate.getTimeCoordinatesWithSameStarTime();
        FileHandler fileHandler = FileHandler(coordinatesWithSameStarTime, config);
        fileHandler.calculateRelatedFiles();
        FourierHandler fourierHandler = FourierHandler(config, fileHandler.getFileNameToTimestampsMap(), context);
        fourierHandler.setStorage(storage);
        fourierHandler.run();

        if (config.isSummationEnabled()) {
            SummationHandler summarizeHandler = SummationHandler(fourierHandler.getCalculatedData());
            SummationWriteHandler writeHandler = SummationWriteHandler(config,
                                                                       summarizeHandler.getSummaryForEveryRayInTime(),coordinate);
            writeHandler.write();
            if (config.isWriteRawData()) {
                SummationWriteHandler rawWriteHandler = SummationWriteHandler(config, summarizeHandler.getSummaryOfRawDataForEveryRayInTime(), coordinate);
                rawWriteHandler.write();
            }
        } else {
            SimpleWriteHandler simpleWriteHandler = SimpleWriteHandler(config, fourierHandler.getCalculatedData(), coordinate);
            simpleWriteHandler.write();
        }
    }
    clReleaseCommandQueue(context.getClCommandQueue());
    clReleaseContext(context.getContext());
    clfftTeardown();
    std::cout << "elapsed time: " << (float) (clock() - tStart) / CLOCKS_PER_SEC << "s" << std::endl;
    std::cout << "Finish!" << std::endl;
}

CalibrationDataStorage *readCalibrationDataStorage(const string &path_calibration) {
    float start = 0, diff = 0;
    auto *storage = new CalibrationDataStorage();

    start = clock();
    std::string path(path_calibration);
    storage->add_items_from_file(path);
    diff = (clock() - start) / CLOCKS_PER_SEC;
    cout << "reading calibration file took " << diff << " sec" << endl;
    return storage;
}

std::string tm_tostring(tm *datetime) {
    return std::to_string(datetime->tm_hour) + ":" + std::to_string(datetime->tm_min) + ":" +
           std::to_string(datetime->tm_sec);
}
