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
using LevelInfos=std::vector<LevelInfo>;

class OrderBookLevelInfo
{
    public:
    OrderBookLevelInfo(const LevelInfos& bids,const LevelInfos& asks)
    :bids_{bids}, asks_{asks}
    {}
    const LevelInfos& getBids() const{
    return bids_;
}

const LevelInfos& getAsks() const{
    return asks_;
}

private:
    LevelInfos bids_;
    LevelInfos asks_;

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
    OrderId GetOrderId() const{
        return orderid_;
    }
    Side GetSide() const{
        return side_;
    }
    Price GetPrice() const{
        return price_;
    }
    OrderType GetOrderType() const{
        return ordertype_;
    }
    Quantity GetInitialQuantity() const{
        return initialquantity_;
    }
    Quantity GetRemainingQuantity()const
    {
        return remainingquantity_;
    }
    Quantity Getfilledquantity() const{
        return GetInitialQuantity()-GetRemainingQuantity();
    }
    bool IsFilled()const
    {
        return GetRemainingQuantity()==0;
    }

    void Fill(Quantity quantity)
    {
        if(quantity>GetRemainingQuantity())
        {
            throw std::logic_error("Order cannot be filled: quantity exceeds remaining quantity");
        }
        remainingquantity_-=quantity;
    }
    private:
    OrderType ordertype_;
    OrderId orderid_;
    Side side_;
    Price price_;
    Quantity initialquantity_;
    Quantity remainingquantity_;



};
using OrderPointer=std::shared_ptr<Order>;
using OrderPointers=std::list<OrderPointer>;
class OrderModify{
public:
        OrderModify(OrderId orderid,Price price,Side side,Quantity quantity)
        :orderid_{orderid}
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
        OrderPointer ToOrderPointer(OrderType type) const{
            return std::make_shared<Order>(type,GetOrderId(),GetSide(),GetPrice(),GetQuantity());
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

class Orderbook
{
    private:
    struct OrderEntry{
        OrderPointer order_{nullptr};
        OrderPointers::iterator location_;

    };
    std::map<Price,OrderPointers,std::greater<Price>>bids_;
    std::map<Price,OrderPointers,std::less<Price>>asks_;
    std::unordered_map<OrderId,OrderEntry>orders_;
    
    bool CanMatch(Side side,Price price)
    {
        if(side== Side ::Buy)
        {
            if(asks_.empty())
            {
                return false;
                const auto&[bestAsk, _]=*asks_.begin();
                return price>=bestAsk;

            }
            else{
                if(bids_.empty())
                     return false;

                    const auto&[bestBid,_]=*bids_.begin();
                    return price<=bestBid;
            }
        
    }
}
    Trades matchOrders()
    {
        Trades trades;
        trades.reserve(orders_.size());
        while(true)
        {
            if(bids_.empty()||asks_.empty())
            {
               break; 
            }
            auto&[bidPrice,bids]=*bids_.begin();
            auto[askPrice,asks]=*asks_.begin();
             
            if(bidPrice<askPrice)
            break;

            while(bids.size()&&asks.size())
            {
                auto& bid=bids.front();
                auto& ask=asks.front();
                Quantity quantity=std::min(bid->GetRemainingQuantity(),ask->GetRemainingQuantity());
                bid->Fill(quantity);
                ask->Fill(quantity);

                if(bid->IsFilled())
                {
                    bids.pop_front();
                    orders_.erase(bid->GetOrderId());
                }
                if(ask->IsFilled())
                {
                    asks.pop_front();
                    orders_.erase(ask->GetOrderId());
                }
                if(bids.empty())
                {
                    bids_.erase(bidPrice);
                }
                if(asks.empty())
                {
                    asks_.erase(askPrice);
                }
                trades.push_back(Trade{
                    TradeInfo{bid->GetOrderId(),bid->GetPrice(),quantity},
                    TradeInfo{ask->GetOrderId(),ask->GetPrice(),quantity}
                });
            }
        }
        
        if(!bids_.empty())
        {
            auto& [_,bids]=*bids_.begin();
            auto& order =bids.front();
            if(order->GetOrderType()==OrderType::FindAndKill)
                CancelOrder(order->GetOrderId());
        }
        if(!asks_.empty())
        {
            auto& [_,asks]=*asks_.begin();
            auto& order=asks.front();
            if(order->GetOrderType()==OrderType::FindAndKill)
                CancelOrder(order->GetOrderId());
        }
        return trades;
    }
    public:
    Trades AddOrder(OrderPointer order)
    {
        if(orders_.contains(order->GetOrderId()))
        return {};

        if(order->GetOrderType()==OrderType::FindAndKill&&CanMatch(order->GetSide(),order->GetPrice()))
            return {};
        OrderPointers::iterator iterator;
        if(order->GetSide()==Side::Buy){
            auto& orders=bids_[order->GetPrice()];
        orders.push_back(order);
        iterator=std::next(orders.begin(),orders.size()-1);

        }
        else{
            auto& orders=asks_[order->GetPrice()];
            orders.push_back(order);
            iterator=std::next(orders.begin(),orders.size()-1);
        }
        orders_.insert({order->GetOrderId(),OrderEntry{order,iterator}});
         
        return matchOrders();
    
    }
    void CancelOrder(OrderId orderId)
    {
        if(!orders_.contains(orderId))
        {
            return;
        }
        const auto& [order,iterator]=orders_.at(orderId);
        orders_.erase(orderId);
        if(order->GetSide()==Side::Sell)
        {
            auto price=order->GetPrice();
            auto& orders=asks_.at(price);
            orders.erase(iterator);
            if(orders.empty())
            asks_.erase(price);
        }
        else{
            auto price=order->GetPrice();
            auto& orders=bids_.at(price);
            orders.erase(iterator);
            if(orders.empty())
            {
                bids_.erase(price);
            }
        }
        
    }
    Trades MatchOrder(OrderModify order)
    {
        if(!orders_.contains(order.GetOrderId()))
        {
            return {};
        }
        const auto& [existingOrder,iterator]=orders_.at(order.GetOrderId());
        CancelOrder(order.GetOrderId());
        return AddOrder(order.ToOrderPointer(existingOrder->GetOrderType()));
    }
    std::size_t Size() const{
        return orders_.size();
    }
    
    OrderBookLevelInfo GetOrderInfo() const
    {
        LevelInfos bidinfo, askinfo;
        bidinfo.reserve(orders_.size());
        askinfo.reserve(orders_.size());
        
        auto CreateLevelInfo = [](Price price, const OrderPointers& orders)
        {
            return LevelInfo{price, std::accumulate(orders.begin(), orders.end(), (Quantity)0,
                [](Quantity runningsum, const OrderPointer& order){
                    return runningsum + order->GetRemainingQuantity();
                })};
        };
        
        for(const auto& [price, orders] : bids_)
            bidinfo.push_back(CreateLevelInfo(price, orders));
        
        for(const auto& [price, orders] : asks_)
            askinfo.push_back(CreateLevelInfo(price, orders));
        
        return OrderBookLevelInfo{bidinfo, askinfo};
    }
};
    




    
int main()
{
    Orderbook orderbook;
    const OrderId orderId=1;
    OrderPointer order1=std::make_shared<Order>(OrderType::GoodTillCancel,orderId,Side::Buy,100,10);
    std::cout<<"Adding Order 1\n";
    orderbook.AddOrder(order1);
    std::cout<<orderbook.Size()<< " orders in the orderbook\n";
    orderbook.CancelOrder(orderId);
    std::cout<<"Cancelled Order 1\n";
    std::cout<<orderbook.Size()<<" orders in the orderbook\n";
    return 0;
}