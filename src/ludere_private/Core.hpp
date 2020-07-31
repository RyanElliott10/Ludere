//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_CORE_HPP
#define LUDERE_CORE_HPP


#include <queue>

#include <ludere/Event.hpp>

// TODO: Reconsider this design pattern. Perhaps EventQueue should be a class all on its own
#define EventQueue std::queue<Event>


#endif //LUDERE_CORE_HPP
