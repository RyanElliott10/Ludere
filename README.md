# Ludere
An easy to use, event-driven backtesting library.

## Purpose
This project began as a means for myself (and others) to learn about algorithmic trading. Of course, backtesting is a key principle in developing a profitable trading strategy -- hence the conception of Ludere.

From the beginning, the system has always been designed to be easy-to-use with a clean, intuitive API (pull requests to rectify areas of awry API conventions are more than welcome!) built atop an effective, event-driven engine. Event-driven being key here; among many reasons, vectorized implementations' inability to implement slippage, brokerage fees, etc. in an intuitive manner developed a hard case against that route.

## Architecture
As noted above, Ludere is an event-driven backtesting engine. Bearing that in mind, we can cover the three main classes that power the system, in addition to any fringe data structures in use.

### lud::Engine
As the name suggests, this class is in control of the entire system, controlling the beginning and ending of a backtest. At its core, it's strictly composed of `lud::Exchange` and `std::set<lud::Strategy>` instances. Once a `lud::Engine` has been constructed with these two parameters, calling `engine.trade()` will begin a backtest.

```cpp
...

std::unordered_set<std::shared_ptr<lud::AbstractStrategy>> strategies;
lud::Exchange exchange(dataStream, false);
lud::Engine engine(exchange, strategies);

engine.trade();
```

### lud::Exchange
An instance of `lud::Exchange` controls the actual market: the event-loop, order handling, data delegation, etc. all take place in `lud::Exchange`. Taking a look at the source code is _strongly_ encouraged to understand what is happening under the hood and how to properly use a `lud::Exchange` instance.

`lud::Exchange` instances are, similarly to `lud::Engine`, very easy to instantiate and begin using. A `lud::Exchange`, upon instantiation, only cares about how it should delegate candlestick data. To allow for easy extensability, you only need to pass a class that implements the `lud::IDataStreamer` interface.

```cpp
std::shared_ptr<lud::IDataStreamer> dataStream = std::make_shared<DataStreamer>(TSLA_CSV);
lud::Exchange exchange(dataStream, false);
```

#### Event Queue
`lud::Exchange` is in control of the event queue. To add an event to the event queue, simply utilize `lud::Exchange::addEvent(std::shared_ptr<Event> &)`, where it will handle processing the event and passing information to the appropriate data structures.

```cpp
std::shared_ptr<OrderEvent> orderEvent = std::shared_ptr<MarketEvent>(MarketEvent::MarketEventType::kMarketOpen);
exchange.addEvent(orderEvent);
```

#### Order Queue
The order queue within `lud::Exchange` operates on the same API as the event queue. In fact, orders are directly added to the event queue. However, the typical flow is slightly different.

A `lud::AbstractStrategy`-derived class will place an order using `lud::Strategy::placeLimitOrder(...)`, which is a non-virtual function within the parent class. `lud::AbstractStrategy` will create the actual order with the parameters, passing the order to the associated `lud::Portfolio`. The portfolio will add the appropriate callbacks to ensure the order is valid and can be executed prior to placing it on the event queue.

From a basic usage perspective, the API can be depicted as follows (within a `lud::AbstractStrategy`-derived class):

```cpp
void MovingAverageStrategy::handleMarketData(const std::unordered_map<std::string, lud::CandlestickData> &data)
    if (shouldPlaceOrder(data)) {
        placeLimitOrder(security, numShares, lud::Order::PositionType::kLongPosition, desiredPrice);
    }
}
```

### lud::Portfolio
The portfolio will, among other things, track value and send orders from strategies to the exchange. Instantiation simply requires a `std::shared_ptr<lud::Exchange>` and beginning capital.

All portfolios should be subscribed to a datastream to ensure it's able to update the portfolio value at an appropriate interval. The syntax and logic for creating and subscribing to a datastream are covered down below.

```cpp
std::unordered_set<std::string> securities = { "TSLA" };
std::shared_ptr<lud::Portfolio> portfolio = std::make_shared<lud::Portfolio>(exchange, 1000.0f);

exchange.subscribeToDataStream(std::make_shared<lud::DataEventSubscription>(portfolio, securities));
```

### lud::AbstractStrategy

## How-To

### lud::AbstractStrategy

### lud::IDataEventSubscriber

### lud::IMarketEventSubscriber

### lud::Order

### IDataStreamer

## Roadmap
- [x] Custom data streaming extension
- [ ] Slippage
- [ ] Volume simulation
- [ ] Charting
- [ ] Python interface
