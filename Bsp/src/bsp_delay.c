#include "bsp_delay.h"

static uint32_t g_fac_us = 0;       /* usÑÓÊ±±¶³ËÊý */



/**
 * @brief     ³õÊ¼»¯ÑÓ³Ùº¯Êý
 * @param     sysclk: ÏµÍ³Ê±ÖÓÆµÂÊ, ¼´CPUÆµÂÊ(rcc_c_ck), 168MHz
 * @retval    ÎÞ
 */  
void delay_init(uint16_t sysclk)
{
#if SYS_SUPPORT_OS                                      /* Èç¹ûÐèÒªÖ§³ÖOS */
    uint32_t reload;
#endif
    g_fac_us = sysclk;                                  /* ÓÉÓÚÔÚHAL_InitÖÐÒÑ¶Ôsystick×öÁËÅäÖÃ£¬ËùÒÔÕâÀïÎÞÐèÖØÐÂÅäÖÃ */
#if SYS_SUPPORT_OS                                      /* Èç¹ûÐèÒªÖ§³ÖOS. */
    reload = sysclk;                                    /* Ã¿ÃëÖÓµÄ¼ÆÊý´ÎÊý µ¥Î»ÎªM */
    reload *= 1000000 / delay_ostickspersec;            /* ¸ù¾Ýdelay_ostickspersecÉè¶¨Òç³öÊ±¼ä,reloadÎª24Î»
                                                         * ¼Ä´æÆ÷,×î´óÖµ:16777216,ÔÚ168MÏÂ,Ô¼ºÏ0.09986s×óÓÒ
                                                         */
    g_fac_ms = 1000 / delay_ostickspersec;              /* ´ú±íOS¿ÉÒÔÑÓÊ±µÄ×îÉÙµ¥Î» */
    SysTick->CTRL |= 1 << 1;                            /* ¿ªÆôSYSTICKÖÐ¶Ï */
    SysTick->LOAD = reload;                             /* Ã¿1/delay_ostickspersecÃëÖÐ¶ÏÒ»´Î */
    SysTick->CTRL |= 1 << 0;                            /* ¿ªÆôSYSTICK */
#endif 
}							    


/**
 * @brief     ÑÓÊ±nus
 * @note      ÎÞÂÛÊÇ·ñÊ¹ÓÃOS, ¶¼ÊÇÓÃÊ±ÖÓÕªÈ¡·¨À´×öusÑÓÊ±
 * @param     nus: ÒªÑÓÊ±µÄusÊý
 * @note      nusÈ¡Öµ·¶Î§: 0 ~ (2^32 / fac_us) (fac_usÒ»°ãµÈÓÚÏµÍ³Ö÷Æµ, ×ÔÐÐÌ×Èë¼ÆËã)
 * @retval    ÎÞ
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOADµÄÖµ */
    ticks = nus * g_fac_us;                 /* ÐèÒªµÄ½ÚÅÄÊý */
    
#if SYS_SUPPORT_OS                          /* Èç¹ûÐèÒªÖ§³ÖOS */
    delay_osschedlock();                    /* Ëø¶¨ OS µÄÈÎÎñµ÷¶ÈÆ÷ */
#endif

    told = SysTick->VAL;                    /* ¸Õ½øÈëÊ±µÄ¼ÆÊýÆ÷Öµ */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* ÕâÀï×¢ÒâÒ»ÏÂSYSTICKÊÇÒ»¸öµÝ¼õµÄ¼ÆÊýÆ÷¾Í¿ÉÒÔÁË */
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) 
            {
                break;                      /* Ê±¼ä³¬¹ý/µÈÓÚÒªÑÓ³ÙµÄÊ±¼ä,ÔòÍË³ö */
            }
        }
    }

#if SYS_SUPPORT_OS                          /* Èç¹ûÐèÒªÖ§³ÖOS */
    delay_osschedunlock();                  /* »Ö¸´ OS µÄÈÎÎñµ÷¶ÈÆ÷ */
#endif 

}

/**
 * @brief     ÑÓÊ±nms
 * @param     nms: ÒªÑÓÊ±µÄmsÊý (0< nms <= (2^32 / fac_us / 1000))(fac_usÒ»°ãµÈÓÚÏµÍ³Ö÷Æµ, ×ÔÐÐÌ×Èë¼ÆËã)
 * @retval    ÎÞ
 */
void delay_ms(uint16_t nms)
{
    
#if SYS_SUPPORT_OS  /* Èç¹ûÐèÒªÖ§³ÖOS, Ôò¸ù¾ÝÇé¿öµ÷ÓÃosÑÓÊ±ÒÔÊÍ·ÅCPU */
    if (delay_osrunning && delay_osintnesting == 0)     /* Èç¹ûOSÒÑ¾­ÔÚÅÜÁË,²¢ÇÒ²»ÊÇÔÚÖÐ¶ÏÀïÃæ(ÖÐ¶ÏÀïÃæ²»ÄÜÈÎÎñµ÷¶È) */
    {
        if (nms >= g_fac_ms)                            /* ÑÓÊ±µÄÊ±¼ä´óÓÚOSµÄ×îÉÙÊ±¼äÖÜÆÚ */
        {
            delay_ostimedly(nms / g_fac_ms);            /* OSÑÓÊ± */
        }

        nms %= g_fac_ms;                                /* OSÒÑ¾­ÎÞ·¨Ìá¹©ÕâÃ´Ð¡µÄÑÓÊ±ÁË,²ÉÓÃÆÕÍ¨·½Ê½ÑÓÊ± */
    }
#endif

    delay_us((uint32_t)(nms * 1000));                   /* ÆÕÍ¨·½Ê½ÑÓÊ± */
}



