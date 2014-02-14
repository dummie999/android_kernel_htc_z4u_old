/*
 * u_serial.h - interface to USB gadget "serial port"/TTY utilities
 *
 * Copyright (C) 2008 David Brownell
 * Copyright (C) 2008 by Nokia Corporation
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef __U_SERIAL_H
#define __U_SERIAL_H

#include <linux/usb/composite.h>
#include <linux/usb/cdc.h>

enum fserial_func_type {
	USB_FSER_FUNC_NONE,
	USB_FSER_FUNC_SERIAL,
	USB_FSER_FUNC_MODEM,
	USB_FSER_FUNC_MODEM_MDM,
	USB_FSER_FUNC_ACM,
	USB_FSER_FUNC_AUTOBOT,
};

struct gserial {
	struct usb_function		func;

	
	struct gs_port			*ioport;

	struct usb_ep			*in;
	struct usb_ep			*out;

	
	struct usb_cdc_line_coding port_line_coding;	
	u16				serial_state;

	
	unsigned int (*get_dtr)(struct gserial *p);
	unsigned int (*get_rts)(struct gserial *p);

	
	void (*connect)(struct gserial *p);
	void (*disconnect)(struct gserial *p);
	int (*send_break)(struct gserial *p, int duration);
	unsigned int (*send_carrier_detect)(struct gserial *p, unsigned int);
	unsigned int (*send_ring_indicator)(struct gserial *p, unsigned int);
	int (*send_modem_ctrl_bits)(struct gserial *p, int ctrl_bits);

	
	void (*notify_modem)(void *gser, u8 portno, int ctrl_bits);
};

struct gsmd_Info{
	unsigned count;
	struct work_struct gsmd_setup_wq;

}the_gsmd_Info;


struct usb_request *gs_alloc_req(struct usb_ep *ep, unsigned len, gfp_t flags);
void gs_free_req(struct usb_ep *, struct usb_request *req);

int gserial_setup(struct usb_gadget *g, unsigned n_ports);
void gserial_cleanup(void);

int gserial_connect(struct gserial *, u8 port_num);
void gserial_disconnect(struct gserial *);

int gsdio_setup(struct usb_gadget *g, unsigned n_ports);
int gsdio_connect(struct gserial *, u8 port_num);
void gsdio_disconnect(struct gserial *, u8 portno);

int gsmd_setup(struct usb_gadget *g, unsigned n_ports);
int gsmd_connect(struct gserial *, u8 port_num);
void gsmd_disconnect(struct gserial *, u8 portno);

int acm_bind_config(struct usb_configuration *c, u8 port_num);
int gser_bind_config(struct usb_configuration *c, u8 port_num);
int obex_bind_config(struct usb_configuration *c, u8 port_num);

#endif 