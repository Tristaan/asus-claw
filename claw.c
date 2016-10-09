#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

uint16_t VENDOR=0x195d;
uint16_t PRODUCT=0x1016;
libusb_device_handle* findClaw(struct libusb_device  **list){
  int i=0;
  struct libusb_device *dev;
  struct libusb_device_handle *handle=NULL;

  while((dev=list[i++])!=NULL){
    struct libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0) {
        fprintf(stderr, "failed to get device descriptor");
        return NULL;
    }

    if(desc.idVendor==VENDOR && desc.idProduct==PRODUCT){
      if(libusb_open(dev,&handle)){
        fprintf(stderr,"Device couldn't be opened.");
        return NULL;
      }
      return handle;
    }
  }
  return NULL;
}

void transferComplete(struct libusb_transfer *transfer){
  switch(transfer->status){
    case LIBUSB_TRANSFER_COMPLETED:
      fprintf(stdout,"Actual length %d \n",transfer->actual_length);
      for(int i=0;i<transfer->actual_length;i++){
        fprintf(stdout,"%02x ",transfer->buffer[i]);
      }
      fprintf(stdout,"\n");
      fflush(stdout);
    break;
    case LIBUSB_TRANSFER_CANCELLED:
      fprintf(stderr,"Canceled");
      break;
    case LIBUSB_TRANSFER_NO_DEVICE:
      fprintf(stderr,"NO_DEV");
      break;
    case LIBUSB_TRANSFER_TIMED_OUT:
      //fprintf(stderr,"TIMEOUT");
      break;
    case LIBUSB_TRANSFER_ERROR:
      fprintf(stderr,"ERROR");
      break;
    case LIBUSB_TRANSFER_STALL:
      fprintf(stderr,"STALL");
      break;
    case LIBUSB_TRANSFER_OVERFLOW:
      fprintf(stderr,"OVERFLOW");// Various type of errors here
      break;
  }
  fflush(stderr);
}

int main (){
  struct libusb_context *ctx;
	struct libusb_device **list;

  libusb_init(&ctx);
  if(libusb_get_device_list(ctx,&list)<0){
    fprintf(stderr,"Couldn't get device list.");
    return 1;
  }
  struct libusb_device_handle *handle=findClaw(list);
  if(handle==NULL){
    libusb_free_device_list(list,1);
    libusb_exit(ctx);
    return -1;
  }
  //Handle DPI BUTTON INTERFACE -> Move to function?
  libusb_detach_kernel_driver(handle,2); //Third interface are DPI buttons.EP 0x83
  if(libusb_claim_interface(handle,2)){
    fprintf(stderr,"Couldn't claim interface.");
    return 1;
  }

  struct libusb_transfer *transfer=libusb_alloc_transfer(0);
  unsigned char *dataR;
  int size=8;
  dataR=malloc(size);
  int completed=0;

  libusb_fill_interrupt_transfer(transfer,handle,0x83,dataR,size,transferComplete,NULL,5000);


  while (!completed) {
    if(libusb_submit_transfer(transfer)<0){
      fprintf(stderr,"Error while submitting transfer.");
      libusb_free_transfer(transfer);
    }
    if (libusb_handle_events_completed(ctx, NULL) < 0){   // negative values are errors
     fprintf(stderr,"handle events error"); 
      break;
    }
  }
  libusb_free_device_list(list,1);
  libusb_exit(ctx);
  printf("completed!");
  
  return 0;
}
