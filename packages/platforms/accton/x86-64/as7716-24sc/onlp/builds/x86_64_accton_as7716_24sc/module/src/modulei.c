/************************************************************
 * <bsn.cl fy=2014 v=onl>
 *
 *           Copyright 2014 Big Switch Networks, Inc.
 *
 * Licensed under the Eclipse Public License, Version 1.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *        http://www.eclipse.org/legal/epl-v10.html
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the
 * License.
 *
 * </bsn.cl>
 ************************************************************
 *
 *
 *
 ***********************************************************/
#include <onlp/platformi/modulei.h>
#include <onlp/module.h>
#include <onlplib/file.h>

#define MODULE1_ID         1
#define MODULE2_ID         2
#define MODULE3_ID         3
#define MODULE4_ID         4
#define MODULE5_ID         5
#define MODULE6_ID         6
#define MODULE7_ID         7
#define MODULE8_ID         8

static onlp_module_info_t minfo[] = {
    {}, /* not used */
    {
      {ONLP_MODULE_ID_CREATE(MODULE1_ID), "SLOT-1 : PIU + CFP2", 0},
      0,
    },
    {
      {ONLP_MODULE_ID_CREATE(MODULE2_ID), "SLOT-2 : PIU + CFP2", 0},
      0,
    },
    {
      {ONLP_MODULE_ID_CREATE(MODULE3_ID), "SLOT-3 : PIU + CFP2", 0},
      0,
    },
    {
      {ONLP_MODULE_ID_CREATE(MODULE4_ID), "SLOT-4 : PIU + CFP2", 0},
      0,
    },
    {
      {ONLP_MODULE_ID_CREATE(MODULE5_ID), "SLOT-5 : PIU + CFP2", 0},
      0,
    },
    {
      {ONLP_MODULE_ID_CREATE(MODULE6_ID), "SLOT-6 : PIU + CFP2", 0},
      0,
    },
    {
      {ONLP_MODULE_ID_CREATE(MODULE7_ID), "SLOT-7 : PIU + CFP2", 0},
      0,
    },
    {
      {ONLP_MODULE_ID_CREATE(MODULE8_ID), "SLOT-8 : PIU + CFP2", 0},
      0,
    }
};

int onlp_modulei_info_get(onlp_oid_t id, onlp_module_info_t *info) {
    uint32_t status = 0;
    *info = minfo[ONLP_OID_ID_GET(id)];
    onlp_modulei_status_get(id, &status);
    info->status = status;
    return ONLP_STATUS_OK;
}

int onlp_modulei_status_get(onlp_oid_t id, uint32_t* status) {
    /* Here ID can be used to identify the slotno */
    int slot = ONLP_OID_ID_GET(id);
    char *string = NULL;
    int len = onlp_file_read_str(&string, "/sys/class/piu/piu%d/piu_type", slot);
    if (string && len) {
      if (strncmp(string, "ACO", 3) == 0) {
        *status = ONLP_MODULE_STATUS_PIU_ACO_PRESENT;
      } else if (strncmp(string, "DCO", 3) == 0) {
        *status = ONLP_MODULE_STATUS_PIU_DCO_PRESENT;
      } else if (strncmp(string, "QSFP", 4) == 0) {
        *status = ONLP_MODULE_STATUS_PIU_QSFP28_PRESENT;
      }
      int cfp2 = 0;
      onlp_file_read_int(&cfp2, "/sys/class/piu/piu%d/cfp2_exists", slot);
      if (cfp2) {
        *status |= ONLP_MODULE_STATUS_PIU_CFP2_PRESENT;
      }
    } else {
      *status = ONLP_MODULE_STATUS_UNPLUGGED;
    }

    return ONLP_STATUS_OK;
}
