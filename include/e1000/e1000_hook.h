#include "depend.h"


#ifndef E1000_HOOK_FUNCS
#define E1000_HOOK_FUNCS

#include "e1000.h" 

struct e1000_adapter* get_e1000_adapter(struct net_device* net_dev);

bool e1000_clean_rx_irq(struct e1000_adapter *adapter, struct e1000_rx_ring *rx_ring, int *work_done, int work_to_do);


#endif