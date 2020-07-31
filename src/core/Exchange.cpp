//
// Created by Ryan Elliott on 7/30/20.
//

#include <ludere/Exchange.hpp>
#include <ludere/Events.hpp>

namespace lud {

Exchange::Exchange()
{
    m_isTrading = false;
}

void Exchange::beginTrading()
{
    m_isTrading = true;

    while (m_isTrading) {
        while (m_eventQueue.size() > 0) {
            Event &event = m_eventQueue.front();
            switch (event.type) {
            case EventType::kMarketEvent: {
                MarketEvent &strict_event = (MarketEvent &) event;
                break;
            }
            case EventType::kFillEvent: {
                FillEvent &strict_event = (FillEvent &) event;
                break;
            }
            }

            m_eventQueue.pop();
        }
    }
}

}