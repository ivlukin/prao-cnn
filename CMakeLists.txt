cmake_minimum_required(VERSION 3.17)
project(prao_cnn)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCL REQUIRED)
find_package(clFFT CONFIG REQUIRED)
add_executable(prao_cnn main.cpp
        Time/TimeCoordinate.cpp Time/TimeCoordinate.h
        Time/TimeCoordinate.cpp Time/TimeCoordinate.h
        Time/TimeUtils.cpp Time/TimeUtils.h
        Time/TimeCoordinateHandler.cpp Time/TimeCoordinateHandler.h
        Config/Config.cpp Config/Config.h
        FileHandler/FileHandler.cpp FileHandler/FileHandler.h
        Reader/DataHeader.cpp Reader/DataHeader.h
        Reader/FilesListItem.h Reader/FilesListItem.cpp
        Calibration/CalibrationData.cpp Calibration/CalibrationData.h
        Calibration/CalibrationDataInput.cpp Calibration/CalibrationDataInput.h
        Calibration/CalibrationDataStorage.cpp Calibration/CalibrationDataStorage.h
        FourierHandler/FourierHandler.cpp FourierHandler/FourierHandler.h
        Context/OpenCLContext.h Context/OpenCLContext.cpp
        FourierHandler/FourierTransformer.cpp FourierHandler/FourierHandler.h
        FourierHandler/Processor.cpp FourierHandler/Processor.h
        Reader/DataSeeker.h Reader/DataSeeker.cpp
        Celestial/Ray.cpp Celestial/Ray.h
        Celestial/Timestamp.h
        Summarizing/SummationHandler.cpp
        Summarizing/SummationHandler.h Writing/SummationWriteHandler.cpp
        Writing/SummationWriteHandler.h Writing/SimpleWriteHandler.cpp
        Writing/SimpleWriteHandler.h Utils/Utils.cpp Utils/Utils.h)
target_link_libraries(prao_cnn OpenCL::OpenCL clFFT)