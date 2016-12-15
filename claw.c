/*
   claw - receives data from the third interface of the asus strix claw gaming mouse.
   Copyright (C) 2016 Tristan Šneider

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
   */
#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
uint8_t DATA_SIZE=8;
uint16_t VENDOR=0x195d;
uint16_t CLAW=0x1016;
uint16_t SAGITTA=0x1017;

void transferComplete(struct libusb_transfer *transfer);
struct libusb_device_handle *getHandle(struct libusb_context *ctx,
				       struct libusb_device **list);
int main (int argc, char *argv[]){

	struct libusb_context *ctx;
	struct libusb_device **list;

	libusb_init(&ctx);

	if(libusb_get_device_list(ctx,&list)<0){
		fprintf(stderr,"Couldn't get device list.");
		return -1;
	}
	struct libusb_device_handle *handle;

	handle=getHandle(ctx,list);
	if(handle==NULL){
		fprintf(stderr,"No device or no handle.");
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
	dataR=(unsigned char*)malloc(size);
	int completed=0;
	libusb_fill_interrupt_transfer(transfer,handle,0x83,dataR,
				       size,transferComplete,NULL,10000);


	while (!completed) {
		if(libusb_submit_transfer(transfer)<0){
			fprintf(stderr,"Error while submitting transfer.");
			libusb_free_transfer(transfer);
			completed=1;
		}
		if (libusb_handle_events_completed(ctx, NULL) < 0){   // negative values are errors
			fprintf(stderr,"handle events error");
			completed=1;
		}
	}

	libusb_free_device_list(list,1);
	libusb_close(handle);
	libusb_exit(ctx);
	printf("completed!");

	return 0;
}

struct libusb_device_handle *getHandle(struct libusb_context *ctx,
				       struct libusb_device **list){
	struct libusb_device *dev;
	int i=0;
	while ((dev = list[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "getHandle() descriptor");
			return NULL;
		}
		if(desc.idVendor==VENDOR){
			if(desc.idProduct==CLAW || desc.idProduct==SAGITTA){
				return libusb_open_device_with_vid_pid(ctx,
								       VENDOR,
								       desc.idProduct);
			}
		}
	}
	return NULL;
}
void transferComplete(struct libusb_transfer *transfer)
{
	switch(transfer->status){
	case LIBUSB_TRANSFER_COMPLETED:
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
/*
   fprintf(stdout,"Actual length %d \n",transfer->actual_length);
   for(int i=0;i<transfer->actual_length;i++){
   fprintf(stdout,"%02x ",transfer->buffer[i]);
   }
   fprintf(stdout,"\n");
   fflush(stdout);
   */
