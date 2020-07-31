cmake_minimum_required(VERSION 3.17)
project(Ludere VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(Ludere src/core/Engine.cpp include/ludere/Strategy.hpp src/core/Exchange.cpp include/ludere/Exchange.hpp src/ludere_private/Log.hpp include/ludere/CandlestickData.hpp include/ludere/Event.hpp include/ludere/Core.hpp src/core/Portfolio.cpp include/ludere/Portfolio.hpp include/ludere/Order.hpp src/events/MarketEvent.cpp include/ludere/MarketEvent.hpp src/events/FillEvent.cpp include/ludere/FillEvent.hpp include/ludere/Events.hpp include/ludere/MarketEventSubscriber.hpp include/ludere/UUID.hpp)

target_include_directories(Ludere
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ludere_private
)

target_compile_options(Ludere PRIVATE -Werror)
target_compile_features(Ludere PRIVATE cxx_std_20)