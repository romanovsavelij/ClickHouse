SET (USE_INTERNAL_LIBBSC_LIBRARY 1)
SET (LIBRARY_DIR "${ClickHouse_SOURCE_DIR}/contrib/libbsc")

# cd contrib/libbsc
# find . -name '*.c*' | sort | sed 's/^\./        ${LIBRARY_DIR}/'
SET (Sources
        ${LIBRARY_DIR}/bsc.cpp
        ${LIBRARY_DIR}/libbsc/adler32/adler32.cpp
        ${LIBRARY_DIR}/libbsc/bwt/bwt.cpp
        ${LIBRARY_DIR}/libbsc/bwt/libsais/libsais.c
        ${LIBRARY_DIR}/libbsc/coder/coder.cpp
        ${LIBRARY_DIR}/libbsc/coder/qlfc/qlfc.cpp
        ${LIBRARY_DIR}/libbsc/coder/qlfc/qlfc_model.cpp
        ${LIBRARY_DIR}/libbsc/filters/detectors.cpp
        ${LIBRARY_DIR}/libbsc/filters/preprocessing.cpp
        ${LIBRARY_DIR}/libbsc/libbsc/libbsc.cpp
        ${LIBRARY_DIR}/libbsc/lzp/lzp.cpp
        ${LIBRARY_DIR}/libbsc/platform/platform.cpp
        ${LIBRARY_DIR}/libbsc/st/st.cpp
        ${LIBRARY_DIR}/libbsc/st/st.cu
        ${LIBRARY_DIR}/libbsc/st/st.cuh
        )

# cd contrib/libbsc
# find . -name '*.c*' | sort | sed 's/^\./        ${LIBRARY_DIR}/'
SET (Headers
        ${LIBRARY_DIR}/libbsc/adler32/adler32.h
        ${LIBRARY_DIR}/libbsc/bwt/bwt.h
        ${LIBRARY_DIR}/libbsc/bwt/libsais/libsais.h
        ${LIBRARY_DIR}/libbsc/coder/coder.h
        ${LIBRARY_DIR}/libbsc/coder/common/predictor.h
        ${LIBRARY_DIR}/libbsc/coder/common/rangecoder.h
        ${LIBRARY_DIR}/libbsc/coder/common/tables.h
        ${LIBRARY_DIR}/libbsc/coder/qlfc/qlfc.h
        ${LIBRARY_DIR}/libbsc/coder/qlfc/qlfc_model.h
        ${LIBRARY_DIR}/libbsc/filters.h
        ${LIBRARY_DIR}/libbsc/filters/tables.h
        ${LIBRARY_DIR}/libbsc/libbsc.h
        ${LIBRARY_DIR}/libbsc/lzp/lzp.h
        ${LIBRARY_DIR}/libbsc/platform/platform.h
        ${LIBRARY_DIR}/libbsc/st/st.h
        )

ADD_LIBRARY(libbsc ${Sources} ${Headers})

target_include_directories (libbsc PUBLIC ${LIBRARY_DIR})