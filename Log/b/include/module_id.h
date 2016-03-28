#ifndef APP_INCLUDE_MODULE_ID_H_
#define APP_INCLUDE_MODULE_ID_H_
/*---------------------------------------------------------------------------------------------------------------------
 * Copyright (C) 2000-2015 by Ericsson AB
 *
 * Address:
 * Torshamnsgatan 21
 * 164 83 Kista
 * SWEDEN
 *
 * Telephone: +46 10 719 00 00
 *
 * All Rights Reserved. No part of this software may be reproduced in
 * any form without the written permission of the copyright owner.
 *-------------------------------------------------------------------------------------------------------------------*/

/* Module Ids */
/* ---------- */
enum App_ModuleId_t
{
    APP_ID=0,                 /* -- APP_ID MUST BE FIRST ONE -- */
    CORE_ID,
    GCCP_ID,
    GTP_ID,
    GTPB_ID,
    GTPCD_ID,
    GUCP_ID,
    SAC_ID,
    GSC_ID,
    OAM_ID,
    SRVLIB_ID,
    SEC_ID,
    XMLPARSE_ID,
    CHG_ID,
    RADIUS_ID,
    DHCP_ID,
    PLATFORM_ID,
    NCT_ID,
    NMBPROXY_ID,
    TDM_ID,
    APNMGT_ID,
    LIH_ID,
    RPD_ID,
    SM_ID,
    LI_ID,
    SACC_ID,
    DBP_ID,
    DAPP_ID,
    BOARDH_ID,
    ETFCTRL_ID,
    DEBUG_ID,
    GTCP_ID,
    PPB_ID,
    DATA_ID,
    PDP_ID,
    PDN_ID,
    GPCP_ID,
    GPSP_ID,
    GNSP_ID,
    GTPV2_ID,
    PIAF_ID,
    UNITTEST_ID,
    URM_ID,
    EBM_ID,
    S6B_ID,
    PMIP_ID,
    CONTENTFILTERING_ID,
    LIM_ID,
    APR_ID,
    ICR_ID,
    QOS_ID,
    UETRACE_ID,
    SMGR_ID,
    ACS_ID,
    QCT_ID,
    HA_ID,
    SE_ID,
    SERVICE_ID,
    LAST_ID                   /* -- LAST_ID MUST BE LAST ONE -- */
};

/*-------------------------------------------------------------------------------------------------------------------*/
#endif
