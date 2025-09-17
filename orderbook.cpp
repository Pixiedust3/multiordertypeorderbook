#include<iostream>
#include<map>
#include<set>
#include<cmath>
#include<ctime>
#include<deque>
#include<queue>
#include<stack>
#include<limits>
#include<string>
#include<vector>
#include<numeric>
#include<numeric>
#include<unordered_map>
#include<unordered_set>
#include<algorithm>
#include<functional>
#include<random>
#include<memory>
#include<variant>
#include<optional>
#include<tuple>
#include<list>
//#include<format>
enum class OrderType
{
    GoodTillCancel,
    FindAndKill
};
enum class Side
{
    Buy,
    Sell
};
using Price =std::int32_t;
using Quantity=std::uint32_t;
using OrderId=std::uint64_t;
struct LevelInfo
{
    Price price1;
    Quantity quantity1;

};
using LevelInfolist=std::vector<LevelInfo>;

class OrderBookLevelInfo
{
    public:
    OrderBookLevelInfo(const LevelInfo& bids,const LevelInfo& asks)
    :bids_{bids}, asks_{asks}
    {}
    const LevelInfo& getBids() const{
    return bids_;
}

const LevelInfo& getAsks() const{
    return asks_;
}

private:
    LevelInfo bids_;
    LevelInfo asks_;

};
class Order
{
    public:
    Order(OrderType ordertype,OrderId orderid,Side side,Price price,Quantity quantity )
    :ordertype_{ordertype}
    ,orderid_{orderid}
    ,side_{side}
    ,price_{price}
    ,initialquantity_{quantity}
    ,remainingquantity_{quantity}
    {}
    OrderId getOrderId() const{
        return orderid_;
    }
    Side getSide() const{
        return side_;
    }
    Price getPrice() const{
        return price_;
    }
    OrderType getOrderType() const{
        return ordertype_;
    }
    Quantity getInitialQuantity() const{
        return initialquantity_;
    }
    Quantity getRemainingQuantity() const{
        return remainingquantity_;
    }
    Quantity getfilledquantity() const{
        return initialquantity_-remainingquantity_;
    }
    void fill(Quantity quantity)
    {
        if(quantity>getRemainingQuantity())
        {
            throw std::logic_error(std::format("Order({}) cannot be filled with quantity({}) greater than remaining quantity({})",getOrderId(),quantity,getRemainingQuantity()));
        remainingquantity_-=quantity;
        }

    }
    private:
    OrderType ordertype_;
    OrderId orderid_;
    Side side_;
    Price price_;
    Quantity initialquantity_;
    Quantity remainingquantity_;

using OrderPointer=std::shared_ptr<Order>;
using OrderPointers=std::list<OrderPointer>;
};
class OrderModify{
public:
        OrderModify(OrderId orderid,Price price,Side side,Quantity quantity)
        :orderid_{OrderId}
        ,price_{price}
        ,side_{side}
        ,quantity_{quantity}
        {}
        OrderId GetOrderId() const{
            return orderid_;
        }

        Price GetPrice() const{
            return price_;
        }
        Side GetSide() const{
            return side_;
        }
        Quantity GetQuantity() const{
            return quantity_;
        }
private:
OrderId orderid_;
Price price_;
Side side_;
Quantity quantity_;


};
struct TradeInfo
{
    OrderId orderid_;
    Price price_;
    Quantity quantity;
};
class Trade
{
public:
Trade(const TradeInfo& bidTrade,const TradeInfo& askTrade)
:bidTrade_{bidTrade}
,askTrade_{askTrade}
{}
const TradeInfo& GetBidTrade() const{
    return bidTrade_;
}
const TradeInfo& GetAskTrade() const{
    return askTrade_;
}
private:
TradeInfo bidTrade_;
TradeInfo askTrade_;
};
using Trades=std::vector<Trade>;



    
int main()
{
    return 0;
}