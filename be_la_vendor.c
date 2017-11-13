/*****************************************************************
* Copyright (C) 2017 Lenovo Co.,Ltd.*
******************************************************************
* 
* FILENAME:
*   be_la_vendor.c
*
* DESCRIPTION:
* 
* AUTHOR:
*   Feng Liu 626987
*
* CREATED DATE:
*   28/10/2017
* REVISION:
*   1.0
*
* MODIFICATION HISTORY
* --------------------
* $Log:$
*   Feng Liu 28/10/2017  Creation
*****************************************************************/

#include <memory.h>
#include <stdlib.h>

#include "be_la_vendor.h"
#include "be_la_log.h"


acc_cards_t g_acc_cards;

/******************************************************************************
* DESCRIPTION:
*       none
* INPUTS:
*       none
* OUTPUTS:
*       none
* RETURNS:
*       none
******************************************************************************/
int be_la_acc_card_reg(cmd_hdr_t * reginfo){
    int i = 0, retval = 0;
    acc_card_t * card = NULL;

    /* find a slot */
    // TODO: need update to bitmap, and find first bit
    for(i = 0; i < ACC_CARD_NUM; i++){
        if(NULL == g_acc_cards.acc_cards[i]){ 
            break;
        }
    }

    /* alloc memory on deamond */
    card = (acc_card_t * )malloc(sizeof(acc_cards_t));
    if(NULL == card){
        BE_LA_ERROR("acc card malloc failed\n");
        retval = -1;
    }

    /*register the new card into our database*/
    card->vendor = reginfo->vendor;
    REG_ACC_CARD_VENDOR(card, reginfo->vendor);
    REG_ACC_CARD_MODEL(card, reginfo->model);
    REG_ACC_CARD_ACCTYPE(card, reginfo->acc_type);
    REG_ACC_CARD_VMUUID(card, reginfo->vm_uuid);
    REG_ACC_CARD_VMPATH(card, reginfo->vmpath);    

    g_acc_cards.acc_cards[i] = card;
    g_acc_cards.reg_num++;
    
out:
    return retval;
}


/******************************************************************************
* DESCRIPTION:
*       none
* INPUTS:
*       none
* OUTPUTS:
*       none
* RETURNS:
*       none
******************************************************************************/
void be_la_acc_card_dump(void){
    int i = 0;
    acc_card_t * card = NULL;

    BE_LA_LOG("DUMP ACC Cards\n");
    for(i =0;  i < ACC_CARD_NUM; i++){
        if(g_acc_cards.acc_cards[i] != NULL){
            card = g_acc_cards.acc_cards[i];
            BE_LA_LOG("\tacc_cards[%d]: vendor = %u, model = %u, acc_type = 0x%x vmuuid = %s, vmpath = %s\n", 
                i, card->vendor, card->model, card->acc_type, card->vm_uuid, card->vmpath);
            card = NULL;
        }        
    }
    
    return;
}


/******************************************************************************
* DESCRIPTION:
*       none
* INPUTS:
*       none
* OUTPUTS:
*       none
* RETURNS:
*       none
******************************************************************************/
int be_la_vendor_init(void){
    int retval = 0;

    memset(&g_acc_cards, 0x0, sizeof(acc_cards_t));
    g_acc_cards.reg_num = 0;
    
    return retval;
}


/******************************************************************************
* DESCRIPTION:
*       none
* INPUTS:
*       none
* OUTPUTS:
*       none
* RETURNS:
*       none
******************************************************************************/
void be_la_vendor_exit(void){
    int i = 0;
    acc_card_t * card = NULL;

    for(i =0;  i < ACC_CARD_NUM; i++){
        if(NULL != g_acc_cards.acc_cards[i]){
            card = g_acc_cards.acc_cards[i];
            free(card);
            card = NULL;
        }        
    }

    memset(&g_acc_cards, 0x0, sizeof(acc_cards_t));
    g_acc_cards.reg_num = 0;
    
    return;
}


