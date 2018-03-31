#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"
#include <string.h>
#include "Coordinator.h"
#include "DebugTrace.h"

#if !defined( WIN32 )
  #include "OnBoard.h"
#endif

#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"


#include "Sensor.h"

#define SEND_DATA_EVENT 0x01
#define DISABLE_ALL_INTERRUPTS() (IEN0 = IEN1 = IEN2 = 0x00)

const cId_t GenericApp_ClusterList[GENERICAPP_MAX_CLUSTERS] =
{
  GENERICAPP_CLUSTERID
};

const SimpleDescriptionFormat_t GenericApp_SimpleDesc =
{
  GENERICAPP_ENDPOINT,              
  GENERICAPP_PROFID,                
  GENERICAPP_DEVICEID,              
  GENERICAPP_DEVICE_VERSION,        
  GENERICAPP_FLAGS,    
  0,
  (cId_t *)NULL,
  GENERICAPP_MAX_CLUSTERS,          
  (cId_t *)GenericApp_ClusterList      
};


endPointDesc_t GenericApp_epDesc;
byte GenericApp_TaskID;   
byte GenericApp_TransID;
devStates_t GenericApp_NwkState;
 
void SendInfo(void);
void To_string(uint8 *dest,char *src,uint8 length);
void sendpH(void);

void GenericApp_Init( byte task_id )
{
  GenericApp_TaskID = task_id;
  GenericApp_NwkState  = DEV_INIT;
  GenericApp_TransID = 0;
  GenericApp_epDesc.endPoint = GENERICAPP_ENDPOINT;
  GenericApp_epDesc.task_id = &GenericApp_TaskID;
  GenericApp_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&GenericApp_SimpleDesc;
  GenericApp_epDesc.latencyReq = noLatencyReqs;
  afRegister( &GenericApp_epDesc );
  
  
  UTX0IF = 1; //UART0 TX中断标志初始置位1 
        U0CSR |= 0X40; //允许接收
        IEN0 |= 0x84; //开总中断，接收中断
 
}       //任务初始化

void initTempSensor(void){
 HAL_DISABLE_INTERRUPTS();  //关闭所有中断
   CLKCONCMD = 0x28; //时器计数时钟设定为1M Hz, 系统时钟设定为32 MHz 
   while(CLKCONSTA & 0x40); //等晶振稳定 
  // P0DIR &= 0x3f;  //p0.6 & p0.7设为输入
   APCFG=0x81; //将P0.0和P0.7口设置为模拟输入
}



uint16 GenericApp_ProcessEvent( byte task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
  if ( events & SYS_EVENT_MSG )
  {
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    while ( MSGpkt )
    {
      switch ( MSGpkt->hdr.event )
      {
        
        case ZDO_STATE_CHANGE:
          GenericApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
          if  ((GenericApp_NwkState == DEV_END_DEVICE)||(GenericApp_NwkState == DEV_ROUTER))
          {
            
            osal_set_event( GenericApp_TaskID,SEND_DATA_EVENT);
          }
          break;
        default:
          break;
      }

      osal_msg_deallocate( (uint8 *)MSGpkt );
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( GenericApp_TaskID );
    }
    return (events ^ SYS_EVENT_MSG);
  }  
  if ( events & SEND_DATA_EVENT )
  {
    
   sendpH();
    osal_start_timerEx( GenericApp_TaskID,
                        SEND_DATA_EVENT,
                      6000 );
    return (events ^ SEND_DATA_EVENT);
  }
  
  return 0;
  
}    //事件处理


void sendpH(void)
{
  PH ph;
 float tvalue;  int i;
  uint16 nwk;
  float tempH=getpH();
  for(i=0;i<64;i++)
  {
    tempH+=getpH();
    tempH/=2;
  }
  
 
  tvalue=(int)tempH;
  /*
  ph.BUF.value[0]=(unsigned char)tvalue/10+'0';
  ph.BUF.value[1]=(unsigned char)tvalue%10+'0';
                    ph.BUF.value[2]='.'; 
  ph.BUF.value[3]=(unsigned char)(10*tvalue)%10+'0';
  */
  tvalue= (tempH*(-1)*5.6347+36.386);        //27.786
  ph.BUF.value[0]=(unsigned char)tvalue/10+'0';
  ph.BUF.value[1]=(unsigned char)tvalue%10+'0';
  ph.BUF.value[2]='.'; 
    ph.BUF.value[3]=(unsigned char)(tvalue*10)%10+'0'; 
  osal_memcpy(ph.BUF.head,"pH",2);
  ph.BUF.left='(';
  if(GenericApp_NwkState==DEV_ROUTER)
  {
    osal_memcpy(ph.BUF.type,"ROU",3);
  }
  if(GenericApp_NwkState==DEV_END_DEVICE)
  {
    osal_memcpy(ph.BUF.type,"END",3);
  }
  nwk=NLME_GetShortAddr();
  To_string(ph.BUF.myNWK,(uint8 *)& nwk,2);
  
  nwk=NLME_GetCoordShortAddr();
  To_string(ph.BUF.pNWK,(uint8 *)& nwk,2);
  
  ph.BUF.right=')';
  ph.BUF.equal='=';
  
  afAddrType_t my_DstAddr;
  my_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;
   my_DstAddr.endPoint = GENERICAPP_ENDPOINT;
    my_DstAddr.addr.shortAddr=0x0000;
    AF_DataRequest( &my_DstAddr, &GenericApp_epDesc,GENERICAPP_CLUSTERID,20,
                   (uint8*)&ph,&GenericApp_TransID,AF_DISCV_ROUTE,AF_DEFAULT_RADIUS);

  
}


void To_string(uint8 *dest, char *src,uint8 length)
{
  uint8 *xad;
  uint8 i=0;
  uint8 ch;
  xad = src + length - 1;
  for(i=0;i<length;i++,xad--)
  {
    ch=(*xad>>4) & 0x0F;
         dest[i<<1]=ch+((ch<10)?'0':'7');
         ch= *xad & 0x0F;
         dest[(i<<1)+1]=ch+((ch<10)?'0':'7');
  }
       
 
}