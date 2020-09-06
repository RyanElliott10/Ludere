cmake_minimum_required(VERSION 3.16)
project(Ludere VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(Ludere src/core/engine.cpp include/ludere/abstract_strategy.hpp src/core/exchange.cpp include/ludere/exchange.hpp include/ludere/log.hpp include/ludere/candlestick_data.hpp include/ludere/event.hpp include/ludere/core.hpp src/core/portfolio.cpp include/ludere/portfolio.hpp include/ludere/order.hpp include/ludere/market_event.hpp include/ludere/order_event.hpp include/ludere/events.hpp include/ludere/market_event_subscribable.hpp include/ludere/uuid.hpp include/ludere/data_event_subscribable.hpp include/ludere/position.hpp include/ludere/filled_order.hpp include/ludere/datastreamable.hpp include/ludere/order_enums.hpp include/ludere/brokerage_fees.hpp src/core/abstract_strategy.cpp src/core/brokerage_fees.cpp src/core/candlestick_data.cpp)

target_include_directories(Ludere
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ludere_private
)

find_package(Boost 1.72.0)
include_directories(${Boost_INCLUDE_DIRS})

target_compile_options(Ludere PRIVATE -Werror)
target_compile_features(Ludere PRIVATE cxx_std_20)