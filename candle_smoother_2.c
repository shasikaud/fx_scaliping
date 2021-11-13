//+------------------------------------------------------------------+
//|                                                  MACD Sample.mq4 |
//|                   Copyright 2005-2014, MetaQuotes Software Corp. |
//|                                              http://www.mql4.com |
//+------------------------------------------------------------------+
#property copyright   "2005-2014, MetaQuotes Software Corp."
#property link        "http://www.mql4.com"

input double TakeProfit    =40;
input double Lots          =0.1;
input double TrailingStop  =10;
//input double MACDCloseLevel=2;
//input int    MATrendPeriod =26;

double h_open, h_close, h_high, h_low;
double MacdCurrent,MacdPrevious;
double SignalCurrent,SignalPrevious;
double MaCurrent,MaPrevious,MaPrev2;
double Ma20, Ma50, Ma200, Ma2000;

double long_pos_tp,long_pos_sl;
double short_pos_tp,short_pos_sl;

string status_long, status_short;


//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void OnTick(void)
  {
   //double MacdCurrent,MacdPrevious;
   //double SignalCurrent,SignalPrevious;
   //double MaCurrent,MaPrevious;
   int    cnt,ticket,total;
   
//---
// initial data checks
// it is important to make sure that the expert works with a normal
// chart and the user did not make any mistakes setting external 
// variables (Lots, StopLoss, TakeProfit, 
// TrailingStop) in our case, we check TakeProfit
// on a chart of less than 100 bars
//---
   if(Bars<100)
     {
      Print("bars less than 100");
      return;
     }
   if(TakeProfit<10)
     {
      Print("TakeProfit less than 10");
      return;
     }
//--- to simplify the coding and speed up access data are put into internal variables
   MacdCurrent=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_MAIN,0);
   MacdPrevious=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_MAIN,1);
   SignalCurrent=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_SIGNAL,0);
   SignalPrevious=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_SIGNAL,1);
   //MaCurrent=iMA(NULL,0,MATrendPeriod,0,MODE_EMA,PRICE_CLOSE,0);
   //MaPrevious=iMA(NULL,0,MATrendPeriod,0,MODE_EMA,PRICE_CLOSE,1);

   total=OrdersTotal();
   if(total<1)
     {
      //--- no opened orders identified
      if(AccountFreeMargin()<(1000*Lots))
        {
         Print("We have no money. Free Margin = ",AccountFreeMargin());
         return;
        }
      //--- check for long position (BUY) possibility
      //if(MacdCurrent<0 && MacdCurrent>SignalCurrent && MacdPrevious<SignalPrevious && 
         //MathAbs(MacdCurrent)>(MACDOpenLevel*Point) && MaCurrent>MaPrevious)
      if (calc_heikin_ashi("LONG"))
        {
         ticket=OrderSend(Symbol(),OP_BUY,Lots,Ask,3,Bid-TrailingStop,Ask+TakeProfit*Point,"macd sample",16384,0,Green);
         if(ticket>0)
           {
            if(OrderSelect(ticket,SELECT_BY_TICKET,MODE_TRADES))
               Print("BUY order opened : ",OrderOpenPrice());
           }
         else
            Print("Error opening BUY order : ",GetLastError());
         return;
        }
        
      if (status_long == "BOUGHT" && ((Ask > long_pos_tp) || (Bid < long_pos_sl))){
         status_long = "CLEAR";     
      }
        
      //--- check for short position (SELL) possibility
      //if(MacdCurrent>0 && MacdCurrent<SignalCurrent && MacdPrevious>SignalPrevious && 
      //   MacdCurrent>(MACDOpenLevel*Point) && MaCurrent<MaPrevious)
      if (calc_heikin_ashi("SHORT"))
        {
         ticket=OrderSend(Symbol(),OP_SELL,Lots,Bid,3,Ask+TrailingStop,Bid-TakeProfit*Point,"macd sample",16384,0,Red);
         if(ticket>0)
           {
            if(OrderSelect(ticket,SELECT_BY_TICKET,MODE_TRADES))
               Print("SELL order opened : ",OrderOpenPrice());
           }
         else
            Print("Error opening SELL order : ",GetLastError());
        }
      //--- exit from the "no opened orders" block
         return;
     }
     
     if (status_short == "BOUGHT" && ((Ask < short_pos_tp) || (Bid > short_pos_sl)))
     {
      //Alert("SHORT >> CLEAR");
         status_short = "CLEAR"; 
     }

//---
  }
  
bool calc_heikin_ashi(string pos){
   //Alert("FLAG01");
   h_high = NormalizeDouble(High[1],5); //prev high
   h_low = NormalizeDouble(Low[1],5); //prev low
   h_open = NormalizeDouble((Open[2]+Close[2])/2,5);
   h_close = NormalizeDouble((Open[1]+Low[1]+Close[1]+High[1])/4,5);
   
   
   //Alert(h_high,"-",h_low,"-",h_open,"-",h_close);
   //Alert(MathAbs(h_high-h_open)*10000);
   //Alert(MathAbs(h_low-h_close)*10000);
   //Alert(h_high);
   //enter long
   if (calcMA("LONG") && pos=="LONG" && (h_open<h_close) && (MathAbs(h_low-h_close)*10000<1)){
   //if (pos=="LONG" && (h_open<h_close) && (MathAbs(h_low-h_close)*10000<1)){
      return true;      
   };
   
   if (calcMA("SHORT") && pos=="SHORT" && (h_open>h_close) && (MathAbs(h_high-h_open)*10000<1)){
   //if (pos=="SHORT" && (h_open>h_close) && (MathAbs(h_high-h_open)*10000<1)){
      return true;      
   };
   
   return false;
   
};

bool calcMA(string signal){
   MacdCurrent=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_MAIN,0);
   //MacdPrevious=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_MAIN,1);
   SignalCurrent=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_SIGNAL,0);
   //SignalPrevious=iMACD(NULL,0,12,26,9,PRICE_CLOSE,MODE_SIGNAL,1);
   //MaCurrent=iMA(NULL,0,14,0,MODE_EMA,PRICE_CLOSE,0);
   //MaPrevious=iMA(NULL,0,14,0,MODE_EMA,PRICE_CLOSE,1);
   //MaPrev2=iMA(NULL,0,14,0,MODE_EMA,PRICE_CLOSE,2);
   //close_price =iMA(NULL,0,1,0,MODE_EMA,PRICE_CLOSE,0);
   //Ma20=iMA(NULL,0,5,0,MODE_EMA,PRICE_CLOSE,0);
   Ma50=iMA(NULL,0,50,0,MODE_EMA,PRICE_CLOSE,0);
   Ma200=iMA(NULL,0,200,0,MODE_EMA,PRICE_CLOSE,0);
   //Ma2000=iMA(NULL,0,20,0,MODE_EMA,PRICE_CLOSE,0);

   
   //Alert(Close[1]);
   //if (signal == "LONG" && Ma20>Ma50 && Ma50>Ma200 && (Close[1]>Ma20)){
   if (signal == "LONG" && Ma50>Ma200){   
      return true;
      //return false;
   }
   
   if (signal == "SHORT" && Ma50<Ma200){  //check ths price below MAs
      return true;
      //return false;
   }
   
   if (signal == "LONG-HOLD" && MacdCurrent<0 && SignalCurrent<0){
      return true;
      //return false;
   }
   
    if (signal == "SHORT-HOLD" && MacdCurrent>0 && SignalCurrent>0){
      return true;
      //return false;
   }
         
   return false;
};
//+------------------------------------------------------------------+
