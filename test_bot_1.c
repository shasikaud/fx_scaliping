//+------------------------------------------------------------------+
//|                                                   test_bot01.mq4 |
//|                                                        shasikaud |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "shasikaud"
#property link      "https://www.mql5.com"
#property version   "1.00"
#property strict

extern int    MagicNumber = 1234567;//Magic number
extern int    Slippage    = 3;//Slippage
extern double TakeProfit  = 20;//Take proft in pips
extern double StopLoss    = 30;//Stop loss in pips
input int     OB          = 70;//RSi overbought level
input int     OS          = 30;//RSi oversold level

double PT,SL,TP,Rsi,stops,SL1,TP1;
int    Ticket = 0;



double movingAverage,rsi;


//long
int max_long;
int count_long;
string status_long;   // CLEAR = 0 | BOUGHT = 1 | HOLD
bool trigger_long;  //exit trade 

//short
int max_short;
int count_short;
string status_short;   // CLEAR = 0 | BOUGHT = 1 | HOLD
double trigger_short;  //exit trade 


double MacdCurrent,MacdPrevious;
double SignalCurrent,SignalPrevious;
double MaCurrent,MaPrevious,MaPrev2;
double Ma20, Ma50, Ma200;
double close_price;

uint tick = 0;

double long_pos_tp,long_pos_sl;
double short_pos_tp,short_pos_sl;

//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
  {
//---
   if((Digits==5)||(Digits==3))
      PT = Point*10;
   else
      PT = Point;
   stops=MarketInfo(Symbol(),MODE_STOPLEVEL)*PT;
   
   status_long = "CLEAR";
   status_short = "CLEAR";

//---
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
//---
   
  }
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
{
   
   //if (status_long == "CLEAR" && (calculate_rsi()<25) && funcTA()){
   if (status_long == "CLEAR" && takePosition("LONG")){
      //Alert(Symbol()," :: RSI TRIGGERED ", calculate_rsi());
      BuyOrder();
      //tick = TimeMinute(TimeCurrent());
      //Alert(tick);
      //SellOrder();
      long_pos_tp = Ask + 4 * PT;
      long_pos_sl = Bid - 2 * PT;
      status_long = "BOUGHT"; 
   }
   
   if (status_long == "BOUGHT" && ((Ask > long_pos_tp) || (Bid < long_pos_sl))){
      status_long = "HOLD";      
   }
   
   if (status_long == "HOLD" && funcTA("LONG-F1")){
   //if (status_long == "BOUGHT" && funcTA("SHORT")){
   //if (status_long == "BOUGHT" && (TimeMinute(TimeCurrent()) > tick + 10)){
      Alert(Symbol(),":: LONG CLEAR");
      status_long = "CLEAR"; 
   }
   
   //if (status_short == "CLEAR" && funcTA("SHORT")){
      //Alert(Symbol()," :: RSI TRIGGERED ", calculate_rsi());
   //   SellOrder();
      //BuyOrder();
   //   short_pos_tp = Ask - 3 * PT;
   //   short_pos_sl = Bid + 3 * PT;
   //   status_short = "BOUGHT"; 
   //}

   
   //if (status_short == "BOUGHT" && ((Ask < short_pos_tp) || (Bid > short_pos_sl))){
    //  Alert(Symbol(),":: SELL CLEAR");
    //  status_short = "CLEAR"; 
   //}
   //funcTA("x");
   
   

}


//+------------------------------------------------------------------+

double calculate_rsi(){
   rsi = iRSI(Symbol(),PERIOD_CURRENT,14,PRICE_CLOSE,0);
   return rsi;
}


bool fractals(string signal){
   if (signal == "SHORT"){
      if ((High[3]>High[5]) && (High[3]>High[4]) && (High[3]>High[2]) && (High[3]>High[1])){
         return true;
      }
   }
   else if (signal == "LONG"){
      if ((Low[3]<Low[5]) && (Low[3]<Low[4]) && (Low[3]<Low[2]) && (Low[3]<Low[1])){
         return true;
      }  
   }
   return false;
}

bool calcMA(string signal){
   //MacdCurrent=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_MAIN,0);
   //MacdPrevious=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_MAIN,1);
   //SignalCurrent=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_SIGNAL,0);
   //SignalPrevious=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_SIGNAL,1);
   //MaCurrent=iMA(NULL,0,14,0,MODE_EMA,PRICE_CLOSE,0);
   //MaPrevious=iMA(NULL,0,14,0,MODE_EMA,PRICE_CLOSE,1);
   //MaPrev2=iMA(NULL,0,14,0,MODE_EMA,PRICE_CLOSE,2);
   //close_price =iMA(NULL,0,1,0,MODE_EMA,PRICE_CLOSE,0);
   Ma20=iMA(NULL,0,20,0,MODE_EMA,PRICE_CLOSE,0);
   Ma50=iMA(NULL,0,50,0,MODE_EMA,PRICE_CLOSE,0);
   Ma200=iMA(NULL,0,200,0,MODE_EMA,PRICE_CLOSE,0);

   
   //Alert(Close[1]);
   if (signal == "LONG" && Ma20>Ma50 && Ma50>Ma200){
      return true;
      //return false;
   }
   
   if (signal == "SHORT" && Ma20<Ma50 && Ma50<Ma200){
      return true;
      //return false;
   }
   
   //if (position == "LONG-F1" && MacdCurrent<0 && SignalCurrent<0){
   //   return true;
      //return false;
   //}
         
   return false;
}

bool takePosition(string position){
   if (position == "LONG" && (calculate_rsi()>50) && fractals("LONG") && calcMA("LONG")){
      return true;
   }
   if (position == "SHORT" && (calculate_rsi()<50) && fractals("SHORT") && calcMA("SHORT")){
      return true;
   }
   return false;
}

bool funcTA(string position){
   MacdCurrent=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_MAIN,0);
   //MacdPrevious=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_MAIN,1);
   SignalCurrent=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_SIGNAL,0);
   //SignalPrevious=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_SIGNAL,1);
   MaCurrent=iMA(NULL,0,14,0,MODE_EMA,PRICE_CLOSE,0);
   //MaPrevious=iMA(NULL,0,14,0,MODE_EMA,PRICE_CLOSE,1);
   //MaPrev2=iMA(NULL,0,14,0,MODE_EMA,PRICE_CLOSE,2);
   //close_price =iMA(NULL,0,1,0,MODE_EMA,PRICE_CLOSE,0);
   Ma20=iMA(NULL,0,20,0,MODE_EMA,PRICE_CLOSE,0);
   Ma50=iMA(NULL,0,50,0,MODE_EMA,PRICE_CLOSE,0);
   Ma200=iMA(NULL,0,200,0,MODE_EMA,PRICE_CLOSE,0);

   
   //Alert(Close[1]);
   if (position == "LONG" && Ma20>Ma50 && MacdCurrent>0 && SignalCurrent>0){
      return true;
      //return false;
   }
   
   if (position == "LONG-F1" && MacdCurrent<0 && SignalCurrent<0){
      return true;
      //return false;
   }
      
   if (position == "SHORT" && Ask < MaCurrent){
      //return true;
      return false;
   }
      
   return false;
}

//Buy order function (ECN style -  stripping out the StopLoss and
//TakeProfit. Next, it modifies the newly opened market order by adding the desired SL and TP)
bool BuyOrder()
{
   Alert(Symbol()," :: PLACING BUY ORDER");
   double Contract=CheckVolumeValue(0.01);
   if(CheckMoneyForTrade(Symbol(),OP_BUY,Contract)){   //OP_BUYLIMIT
      Ticket = OrderSend(Symbol(), OP_BUY, Contract, 0, Slippage, Bid - 2 * PT, Ask + 4 * PT, "", MagicNumber, 0, Blue);
      //(Symbol(), OP_BUYLIMIT,NR(Lots), Ask-Distance*Point, 3, Ask-Distance*Point-SL*Point,Ask-Distance*Point+TP*Point, "", Magic, 0, Blue)
      //Alert(Symbol()," :: BUY ORDER :: EXECUTED");
      //return true; //override
   }
//---
   if(Ticket<1){
      //Alert(Symbol()," :: Order send error BUY order - errcode : ",GetLastError());
      return false;
   }
   else{
      //Alert(Symbol()," :: BUY order, Ticket : ",DoubleToStr(Ticket,0),", executed successfully!");
      return true;
 
//---
      //if(OrderSelect(Ticket, SELECT_BY_TICKET, MODE_TRADES))
      //{
         //SL = Bid - sc(stop) * PT;
         //TP = Ask + sc(take) * PT;
         //SL = Bid - 20 * PT;
         //TP = Ask + 30 * PT;
         //if(!OrderModify(OrderTicket(), OrderOpenPrice(), SL, TP, 0))
          // {
           //// Alert("Failed setting SL/TP BUY order, Ticket : ",DoubleToStr(Ticket,0));
          // return false;
          // }
        // else{
           // Alert("Successfully setting SL/TP BUY order, Ticket : ",DoubleToStr(Ticket,0));
            //return true;
        // }
     //  }
       //else{
         //return false;
   }
}

  
  

//////////////////////////////////////////////////////////////////////
//Sell order function (ECN style -  stripping out the StopLoss and
//TakeProfit. Next, it modifies the newly opened market order by adding the desired SL and TP)
void SellOrder()
  {
   Alert(Symbol()," :: PLACING SELL ORDER");
   double Contract=CheckVolumeValue(0.01);
   if(CheckMoneyForTrade(Symbol(),OP_SELL,Contract))
      Ticket = OrderSend(Symbol(), OP_SELL, Contract, 0, Slippage, Bid + 4 * PT, Ask - 6 * PT, "", MagicNumber, 0, Blue);
//---
   if(Ticket<1)
     {
      //Alert("Order send error SELL order - errcode : ",GetLastError());
      return;
     }
   
      //Alert("SELL order, Ticket : ",DoubleToStr(Ticket,0),", executed successfully!");
//---
   //if(OrderSelect(Ticket, SELECT_BY_TICKET, MODE_TRADES))
   //  {
      //SL = Ask + sc(stop) * PT;
      //TP = Bid - sc(take) * PT;
   //   SL1 = Ask + 3 * PT;
    ///  TP1 = Bid - 3 * PT;
    //  if(!OrderModify(OrderTicket(), OrderOpenPrice(), SL1, TP1, 0))
    //    {
     //    Alert("Failed setting SL/TP SELL order, Ticket : ",DoubleToStr(Ticket,0));
     //    return;
      //  }
    //  else
      //   Alert("Successfully setting SL/TP SELL order, Ticket : ",DoubleToStr(Ticket,0));
     //}
  }
//////////////////////////////////////////////////////////////////////
//Position selector function
int PosSelect()
  {
   Alert("POSSELECT");
   int posi=0;
   for(int k = OrdersTotal() - 1; k >= 0; k--)
     {
      if(!OrderSelect(k, SELECT_BY_POS))
         break;
      if(OrderSymbol()!=Symbol()&&OrderMagicNumber()!= MagicNumber)
         continue;
      if(OrderCloseTime() == 0 && OrderSymbol()==Symbol() && OrderMagicNumber()==MagicNumber)
        {
         if(OrderType() == OP_BUY||OrderType() == OP_SELL)
            posi = 1;
         if(!(OrderType() == OP_SELL||OrderType() == OP_BUY))
            posi = -1;
        }
     }
   return(posi);
  }
double sc(double _param)
  {
   if(_param < stops)
      _param=stops;
   return(_param);
  }
////////////////////////////////////////////////////////////////
//Money check
bool CheckMoneyForTrade(string symb,int type,double lots)
  {
   double free_margin=AccountFreeMarginCheck(symb,type,lots);
   if(free_margin<0)
     {
      string oper=(type==OP_BUY)? "Buy":"Sell";
      Alert("Not enough money for ",oper," ",lots," ",symb," Error code=",GetLastError());
      return(false);
     }
//--- checking successful
   return(true);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
double CheckVolumeValue(double checkedvol)
  {
//--- minimal allowed volume for trade operations
   double min_volume=SymbolInfoDouble(Symbol(),SYMBOL_VOLUME_MIN);
   if(checkedvol<min_volume)
      return(min_volume);

//--- maximal allowed volume of trade operations
   double max_volume=SymbolInfoDouble(Symbol(),SYMBOL_VOLUME_MAX);
   if(checkedvol>max_volume)
      return(max_volume);

//--- get minimal step of volume changing
   double volume_step=SymbolInfoDouble(Symbol(),SYMBOL_VOLUME_STEP);
   int ratio=(int)MathRound(checkedvol/volume_step);
   if(MathAbs(ratio*volume_step-checkedvol)>0.0000001)
      return(ratio*volume_step);
   return(checkedvol);
  }
//+------------------------------------------------------------------+

  
