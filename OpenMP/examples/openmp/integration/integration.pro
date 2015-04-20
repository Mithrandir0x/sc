TARGET = integration
TEMPLATE = app

SOURCES += \
    integration.cpp

# Use -openmp in case of msvc201x
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS   += -fopenmp
