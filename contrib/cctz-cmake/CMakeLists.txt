include(${ClickHouse_SOURCE_DIR}/cmake/embed_binary.cmake)
set(LIBRARY_DIR "${ClickHouse_SOURCE_DIR}/contrib/cctz")

set (SRCS
    "${LIBRARY_DIR}/src/civil_time_detail.cc"
    "${LIBRARY_DIR}/src/time_zone_fixed.cc"
    "${LIBRARY_DIR}/src/time_zone_format.cc"
    "${LIBRARY_DIR}/src/time_zone_if.cc"
    "${LIBRARY_DIR}/src/time_zone_impl.cc"
    "${LIBRARY_DIR}/src/time_zone_info.cc"
    "${LIBRARY_DIR}/src/time_zone_libc.cc"
    "${LIBRARY_DIR}/src/time_zone_lookup.cc"
    "${LIBRARY_DIR}/src/time_zone_posix.cc"
    "${LIBRARY_DIR}/src/zone_info_source.cc"
)

add_library (_cctz ${SRCS})
target_include_directories (_cctz PUBLIC "${LIBRARY_DIR}/include")

if (OS_FREEBSD)
    # yes, need linux, because bsd check inside linux in time_zone_libc.cc:24
    target_compile_definitions (_cctz PRIVATE __USE_BSD linux _XOPEN_SOURCE=600)
endif ()

# Related to time_zones table:
# StorageSystemTimeZones.generated.cpp is autogenerated each time during a build
# data in this file will be used to populate the system.time_zones table, this is specific to OS_LINUX
# as the library that's built using embedded tzdata is also specific to OS_LINUX
set(SYSTEM_STORAGE_TZ_FILE "${PROJECT_BINARY_DIR}/src/Storages/System/StorageSystemTimeZones.generated.cpp")
# remove existing copies so that its generated fresh on each build.
file(REMOVE ${SYSTEM_STORAGE_TZ_FILE})

# get the list of timezones from tzdata shipped with cctz
set(TZDIR "${LIBRARY_DIR}/testdata/zoneinfo")
file(STRINGS "${LIBRARY_DIR}/testdata/version" TZDATA_VERSION)
set_property(GLOBAL PROPERTY TZDATA_VERSION_PROP "${TZDATA_VERSION}")
message(STATUS "Packaging with tzdata version: ${TZDATA_VERSION}")

set(TIMEZONE_RESOURCE_FILES)

# each file in that dir (except of tab and localtime) store the info about timezone
execute_process(COMMAND
    bash -c "cd ${TZDIR} && find * -type f -and ! -name '*.tab' -and ! -name 'localtime' | LC_ALL=C sort | paste -sd ';' -"
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE TIMEZONES)

file(APPEND ${SYSTEM_STORAGE_TZ_FILE} "// autogenerated by ClickHouse/contrib/cctz-cmake/CMakeLists.txt\n")
file(APPEND ${SYSTEM_STORAGE_TZ_FILE} "const char * auto_time_zones[] {\n" )

foreach(TIMEZONE ${TIMEZONES})
    file(APPEND ${SYSTEM_STORAGE_TZ_FILE} "    \"${TIMEZONE}\",\n")
    list(APPEND TIMEZONE_RESOURCE_FILES "${TIMEZONE}")
endforeach(TIMEZONE)
file(APPEND ${SYSTEM_STORAGE_TZ_FILE} "    nullptr};\n")
clickhouse_embed_binaries(
    TARGET tzdata
    RESOURCE_DIR "${TZDIR}"
    RESOURCES ${TIMEZONE_RESOURCE_FILES}
)
add_dependencies(_cctz tzdata)
target_link_libraries(_cctz INTERFACE "-Wl,${WHOLE_ARCHIVE} $<TARGET_FILE:tzdata> -Wl,${NO_WHOLE_ARCHIVE}")

add_library(ch_contrib::cctz ALIAS _cctz)
