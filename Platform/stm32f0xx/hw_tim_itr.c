/**
  ******************************************************************************
  * @file    tim_itr.c
  * @author  杜公子寒枫
  * @version V3.0 寄存器版本
  * @date    2016.08.10
  * @brief   定时器x中断功能配置
  ******************************************************************************
  * @attention
  * 修改描述: 修复初始化阶段执行未开启定时器便执行中断函数的错误
  * 错误描述: 在定时器初始化时,在配置时序参数后便给个更新事件,希望借此刷新影子寄
  *           存器中的PSC与ARR值,但是这导致中断挂起位置1,一旦开启核级中断(这时定
  *           时器还没开启),便会立即执行一次中断服务函数.
  * 解决方法: 删除初始化阶段中的软件更新的代码
  * 修改作者: 杜公子寒枫
  * 当前版本: V1.1
  * 修改日期: 2015.12.30
  * 
  * V2.0------------
  * 修改描述: 优化框架,删除冗余函数,整合定时器的使能/禁止函数,
  * 修改作者: 杜公子寒枫
  * 当前版本: V2.0
  * 修改日期: 2016.05.16
  * 
  ******************************************************************************
  */

/***********************************<INCLUDES>**********************************/
#include "hw_tim_itr.h"
#include "SourceLib.h"

/*----------------------------------------------------------------------------
    模块功能自匹配
 *----------------------------------------------------------------------------*/

#define ITR_TIM   TIMx_6        //修改此宏, 即可匹配成对应的寄存器
#define TIMx  TIM[ITR_TIM]     



/**
  * @brief  TIMx初始化
  * @param  nms 中断间隔时间,单位:ms,范围: 1-3276
  * @retval None
  */
void TIMx_ItrInit(uint16_t nms)
{
  /* 开时钟 */
  TIMx_EnableClock(ITR_TIM);
  
  /* 配置时序参数 */
  TIMx->PSC = TIM_GET_PSC_BY_CNT_FRE(20000);   //计数频率为20KHz
  TIMx->ARR = 20 * nms;    //计数器每记20个数为1ms
  
  /* 配置工作模式 */
  TIMx->CR1 |=  (0X1<<7);  //开影子
  TIMx->CR1 |=  (0X1<<3);  //单脉冲
  TIMx->CR1 &= ~(0X1<<2);  //配置更新源:允许软件更新
  TIMx->CR1 &= ~(0X1<<1);  //使能更新
  
  /* 配置事件/中断 */
  TIMx->DIER &= ~(0X1<<8); //禁止更新DMA请求
  TIMx->DIER |=  (0X1<<0); //使能更新中断
  TIMx->SR   &= ~(0X1<<0); //清标志位
  
  /* 配置核级中断 */
//  NVIC_Enable(TIMx_ITR_IQRn,2,2);
  
  /* 关闭定时器 */
  TIMx->CR1 |=  (0X1<<0);

}


/**
  * @brief  基本定时器TIM2中断函数
  * @param  无
  * @retval 无
  */
void TIMx_ITR_IRQHandler(void)
{
  TIMx->SR   &= ~(0X1<<0); //清标志位
  
}


